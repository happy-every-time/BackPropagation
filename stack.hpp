#pragma once

using namespace std;

namespace stack
{
    struct stack
    {
        long long* data;
        long long top;
        void push(long long data);
    };

    struct list
    {
        double* data;
        double length;
        void add(double data);
    };
}
