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
    if (shape)
    {
        delete shape;
    }
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

void Entity::collide(Entity* other)
{
    std::cout << "Entity " << this->id << " collided with entity " << other->id << std::endl;
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
    float thisStartX = this->x + start*this->dx;
    float thisStartY = this->y + start*this->dy;
    float thisEndX = this->x + end*this->dx;
    float thisEndY = this->y + end*this->dy;
    float otherStartX = other->x + start*other->dx;
    float otherStartY = other->y + start*other->dy;
    float otherEndX = other->x + end*other->dx;
    float otherEndY = other->y + end*other->dy;
    if ((thisStartX + this->shape->minSize >= otherStartX - other->shape->minSize && thisEndX - this->shape->minSize <= otherEndX + other->shape->minSize) ||
            (thisStartX - this->shape->minSize <= otherStartX + other->shape->minSize && thisEndX + this->shape->minSize >= otherEndX - other->shape->minSize)) // If they could've crossed paths in X dimension
    {
        if ((thisStartY + this->shape->minSize >= otherStartY - other->shape->minSize && thisEndY - this->shape->minSize <= otherEndY + other->shape->minSize) ||
                (thisStartY - this->shape->minSize <= otherStartY + other->shape->minSize && thisEndY + this->shape->minSize >= otherEndY - other->shape->minSize)) // If they could've crossed paths in Y dimension
        {
            if (thisDistanceTraveled > this->shape->minSize || otherDistanceTraveled > other->shape->minSize)
            {
                std::cout << "Too far apart." << std::endl;
                this->checkCollision(other, start, start+(interval/2));
                this->checkCollision(other, end-(interval/2), end);
            }
            else
            {
                std::cout << "Collided" << std::endl;
                this->collide(other);
                other->collide(this);
            }
        }
    }

    /*if (distance < (this->shape->minSize + other->shape->minSize))
    {
        if (thisDistanceTraveled > this->shape->minSize || otherDistanceTraveled > other->shape->minSize)
        {
            std::cout << "Too far apart." << std::endl;
            this->checkCollision(other, start, start+(interval/2));
            this->checkCollision(other, end-(interval/2), end);
        }
        else
        {
            std::cout << "Collided" << std::endl;
            this->collide(other);
            other->collide(this);
        }
    }*/
}

Collection<unsigned int> Entity::_idpool = Collection<unsigned int>();
