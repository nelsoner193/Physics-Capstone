#ifndef SCENE_H
#define SCENE_H
#include <entity.h>
#include <map>
#include <vector>
#include <cstddef>

class Scene
{
    public:
        virtual ~Scene();
        static Scene& singleton();
        static void initSingleton();
        static void destroySingleton();
        void update(float dt);
        void addEntity(Entity* ent);
        void delEntity(Entity* ent);
        std::map<unsigned int, Entity*> _entmap;
    protected:
    private:
        Scene();
        static Scene* _singleton;
        std::vector<Entity*> _tobeadded;
        std::vector<Entity*> _toberemoved;
        void handleAdding();
        void handleRemoving();
};
#endif // SCENE_H
