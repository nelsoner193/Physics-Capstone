#include "scene.h"
#include <cstddef>
#include <iostream>

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

void Scene::addEntity(Entity* ent)
{
    _tobeadded.push_back(ent);
}

void Scene::handleAdding()
{
    for (unsigned int n=0; n<_tobeadded.size(); n++)
    {
        _entmap[_tobeadded[n]->id] = _tobeadded[n];
    }
    _tobeadded.clear();
}

void Scene::delEntity(Entity* ent)
{
    _toberemoved.push_back(ent);
}

void Scene::handleRemoving()
{
    for (unsigned int n=0; n<_toberemoved.size(); n++)
    {
        _entmap.erase(_toberemoved[n]->id);
        delete _toberemoved[n];
    }
    _toberemoved.clear();
}

void Scene::update(float dt)
{
    for (std::map<unsigned int, Entity*>::iterator i = _entmap.begin(); i != _entmap.end(); i++)
    {
        Entity* ent = (*i).second;
        ent->update(dt);
    }
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
    for (std::map<unsigned int, Entity*>::iterator i = _entmap.begin(); i != _entmap.end(); i++)
    {
        Entity* ent = (*i).second;
        ent->move();
    }
    handleAdding();
    handleRemoving();;
}

Scene* Scene::_singleton = NULL;
