#pragma once

using namespace std;
namespace stack
{
    struct stack
    {
        long long* data;
        long long top;
        long long size;
        void push(stack* s, long long data);
        long long pop(stack* s);
    };

    struct list
    {
        double* data;
        long long index;
        long long size;
        list add(list* l, double data);
        double get(list* l, long long index);
    };

    stack make_stack(int size = 16);
    list make_list(int size = 16);
}
