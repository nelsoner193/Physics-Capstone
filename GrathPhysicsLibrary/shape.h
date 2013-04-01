#ifndef SHAPE_H
#define SHAPE_H


class Shape
{
    public:
        Shape();
        virtual ~Shape();
        float GetMinSize() { return minSize; }
        void SetMinSize(float val) { minSize = val; }
        float GetMaxSize() { return maxSize; }
        void SetMaxSize(float val) { maxSize = val; }
        float minSize;
        float maxSize;
    protected:
    private:
};

#endif // SHAPE_H
