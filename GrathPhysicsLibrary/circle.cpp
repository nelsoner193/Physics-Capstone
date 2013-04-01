#include "circle.h"

Circle::Circle(float radius)
{
     this->radius = radius;
     this->minSize = radius;
     this->maxSize = radius;
}

Circle::~Circle()
{
    //dtor
}
