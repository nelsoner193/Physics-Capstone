#include "entity.h"
#include <cstddef>
#include <iostream>
#include <math.h>

Entity::Entity()
{
    x = 0;
    y = 0;
    angle = 0;
    dx = 0;
    dy = 0;
    ddx = 0;
    ddy = 0;
    gravity_x = 0;
    gravity_y = 0;
    id = _idpool.getNext();
    shape = NULL;
}

Entity::Entity(float a, float b)
{
    x = a;
    y = b;
    angle = 0;
    dx = 0;
    dy = 0;
    ddx = 0;
    ddy = 0;
    gravity_x = 0;
    gravity_y = 0;
    id = _idpool.getNext();
    shape = (Shape*)new Circle(10.0);
}

Entity::Entity(float a, float b, Shape* shape)
{
    x = a;
    y = b;
    angle = 0;
    dx = 0;
    dy = 0;
    ddx = 0;
    ddy = 0;
    gravity_x = 0;
    gravity_y = 0;
    id = _idpool.getNext();
    this->shape = shape;
}

Entity::~Entity()
{
    _idpool.free(id);
}

void Entity::update(float dt)
{
    dx += ddx*dt + gravity_x*dt;
    dy += ddy*dt + gravity_y*dt;
}

void Entity::move()
{
    x += dx;
    y += dy;
}

float sqr(float x)
{
    return x*x;
}

void Entity::checkCollision(Entity* other, float start, float end)
{
    float interval = end-start;
    float distance = sqrt(sqr((this->x+start*this->dx) - (other->x+start*other->dx))+sqr((this->y+start*this->dy) - (other->y)));
    float thisDistanceTraveled = sqrt(sqr(interval*this->dx)+sqr(interval*this->dy));
    float otherDistanceTraveled = sqrt(sqr(interval*other->dx)+sqr(interval*other->dy));
    std::cout << "Distance = " << distance << " and total distance traveled is " << thisDistanceTraveled + otherDistanceTraveled << std::endl;
    std::cout << "Distance is shorter than min size " << (distance < this->shape->minSize) << std::endl;
    if (/*distance < 2*(thisDistanceTraveled+otherDistanceTraveled) || */distance < 20)
    {
        std::cout << "They got close" << std::endl;
        /*if (thisDistanceTraveled > this->shape->minSize || otherDistanceTraveled > other->shape->minSize)
        {
            std::cout << "Too far apart." << std::endl;
            this->checkCollision(other, start, start+(interval/2));
            this->checkCollision(other, end-(interval/2), end);
        }
        else
        {*/
            std::cout << "Collided!" << std::endl;
            this->collide(other);
            other->collide(this);
        //}
    }
}

void Entity::collide(Entity* other)
{
    std::cout << "Entity " << this->id << " collided with " << other-> id << std::endl;
}

Collection<unsigned int> Entity::_idpool = Collection<unsigned int>();
