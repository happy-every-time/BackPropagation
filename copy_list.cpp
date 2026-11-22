#include "copy_list.hpp"

using namespace std;

namespace tools
{
    void copy(double* a, double* b, long length)
    {
        for (long i = 0; i < length; i++)
        {
            a[i] = b[i];
        }
    }

    void copy(long long* a, long long* b, long length)
    {
        for (long i = 0; i < length; i++)
        {
            a[i] = b[i];
        }
    }
}
