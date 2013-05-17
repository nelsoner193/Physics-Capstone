#include "scene.h"
#include <cstddef>
#include <iostream>

// Standard singleton setup.

Scene::Scene()
{
    this->_singleton = NULL;
}

Scene::~Scene()
{
    for (std::map<unsigned int, Entity*>::iterator i = _entmap.begin(); i != _entmap.end(); i++)
    {
        Entity* value = (*i).second;
        delete value;
    }
    for (unsigned int n=0; n<_tobeadded.size(); n++)
    {
        delete _tobeadded[n];
    }
    for (unsigned int n=0; n<_toberemoved.size(); n++)
    {
        delete _toberemoved[n];
    }
}

void Scene::initSingleton()
{
    if (!_singleton)
    {
        _singleton = new Scene();
    }
}

void Scene::destroySingleton()
{
    if (_singleton)
    {
        delete _singleton;
        _singleton = NULL;
    }
}

Scene& Scene::singleton()
{
    return *_singleton;
}

// Queues adding of entity.
void Scene::addEntity(Entity* ent)
{
    _tobeadded.push_back(ent);
}

// Called every update - adds new entities that were spawned during the update, so that the entire update happens at once.
void Scene::handleAdding()
{
    for (unsigned int n=0; n<_tobeadded.size(); n++)
    {
        _entmap[_tobeadded[n]->id] = _tobeadded[n];
    }
    _tobeadded.clear();
}

// Queues deletion of entity.
void Scene::delEntity(Entity* ent)
{
    _toberemoved.push_back(ent);
}

// Called every update - deletes old entities, once we won't try to access them again.
void Scene::handleRemoving()
{
    for (unsigned int n=0; n<_toberemoved.size(); n++)
    {
        _entmap.erase(_toberemoved[n]->id);
        delete _toberemoved[n];
    }
    _toberemoved.clear();
}

// How things all work nicely
void Scene::update(float dt)
{
    // Update the velocity of every entity
    for (std::map<unsigned int, Entity*>::iterator i = _entmap.begin(); i != _entmap.end(); i++)
    {
        Entity* ent = (*i).second;
        ent->update(dt);
    }

    // Do a pairwise check for collision
    for (std::map<unsigned int, Entity*>::iterator i = _entmap.begin(); i != _entmap.end(); i++)
    {
        Entity* ent = (*i).second;
        std::map<unsigned int, Entity*>::iterator j = i;
        j++;
        for (; j != _entmap.end(); j++)
        {
            Entity* other = (*j).second;
            ent->checkCollision(other, 0, dt);
        }
    }

    // Move all of the entities
    for (std::map<unsigned int, Entity*>::iterator i = _entmap.begin(); i != _entmap.end(); i++)
    {
        Entity* ent = (*i).second;
        ent->move(dt);
    }

    // Add and remove any entities that were spawned or destroyed during the last update.
    handleAdding();
    handleRemoving();;
}

Scene* Scene::_singleton = NULL;
