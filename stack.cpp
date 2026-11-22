#include "stack.hpp"
#include "copy_list.hpp"

using namespace std;

namespace stack
{
    void push(stack* s, long long data)
    {
        if (s->top > s->size)
        {
            long long* datas = new long long[s->size + 16];
            tools::copy(s->data, datas, s->size);
            delete s->data;
            s->data = datas;
            s->size += 16;
        }
        s->data[s->top] = data;
        s->top++;
    }

    long long pop(stack* s)
    {
        s->top--;
        long long data = s->data[s->top];
        if (s->size - s->top > 16)
        {
            long long* datas = new long long[s->size - 16];
            tools::copy(s->data, datas, s->size);
            delete s->data;
            s->data = datas;
            s->size -= 16;
        }
        return data;
    }

    void add(list* l, double data)
    {
        if (l->index > l->size)
        {
            double* datas = new double[l->size + 16];
            tools::copy(l->data, datas, l->size);
            delete l->data;
            l->data = datas;
            l->size += 16;
        }
        l->data[l->index] = data;
        l->index++;
    }

    double get(list* l, long long index)
    {
        return l->data[index];
    }
}
