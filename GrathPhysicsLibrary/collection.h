#ifndef COLLECTION_H
#define COLLECTION_H

#include <queue>

template <typename T>
class Collection
{
    public:
        Collection()
        {
            _next = T();
        }
        virtual ~Collection()
        {
            // pass
        }

        T getNext()
        {
            if (_free.empty())
            {
                return _next++;
            }
            T buf = _free.front();
            _free.pop();
            return buf;
        }

        void free(T in)
        {
            _free.push(in);
        }
    protected:
    private:
        T _next;
        std::queue<T> _free;
};

#endif // COLLECTION_H
