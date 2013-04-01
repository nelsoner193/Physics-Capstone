#ifndef CIRCLE_H
#define CIRCLE_H

#include <shape.h>


class Circle : public Shape
{
    public:
        Circle() { radius = 0; }
        Circle(float radius);
        virtual ~Circle();
        float GetRadius() { return radius; }
        void SetRadius(float val) { radius = val; }
    protected:
    private:
        float radius;
};

#endif // CIRCLE_H
