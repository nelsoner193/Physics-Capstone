#ifndef ENTITY_H
#define ENTITY_H
#include <shape.h>
#include <circle.h>
#include <collection.h>

class Entity
{
    public:
        Entity();
        Entity(float a, float b);
        Entity(float a, float b, Shape* shape);
        virtual ~Entity();
        float GetX() { return x; }
        void SetX(float val) { x = val; }
        float GetY() { return y; }
        void SetY(float val) { y = val; }
        float GetAngle() { return angle; }
        void SetAngle(float val) { angle = val; }
        float Getdx() { return dx; }
        void Setdx(float val) { dx = val; }
        float Getdy() { return dy; }
        void Setdy(float val) { dy = val; }
        float Getddx() { return ddx; }
        void Setddx(float val) { ddx = val; }
        float Getddy() { return ddy; }
        void Setddy(float val) { ddy = val; }
        virtual void update(float dt);
        virtual void move();
        void setForce(float x, float y) { ddx = x; ddy = y; }
        void applyForce(float x, float y) { ddx += x; ddy += y; }
        void setGravity(float x, float y) { gravity_x = x; gravity_y = y; }
        void checkCollision(Entity* other, float start, float end);
        virtual void collide(Entity* other);
        unsigned int id;
        Shape* shape;
        static Collection<unsigned int> _idpool;
    protected:
        float x;
        float y;
        float angle;
        float dx;
        float dy;
        float ddx;
        float ddy;
        float gravity_x;
        float gravity_y;
    private:
};
#endif // ENTITY_H
