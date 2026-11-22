#include "base_node.hpp"
#include <fstream>

using namespace std;
namespace base_node
{
    net load(string name)
    {
        ifstream fin(name);
    }

    void save(net n, string name)
    {
        ofstream file(name, ios::trunc | ios::binary);
        for (int i = 0; i < n.size; i++)
        {
            node node = n.n[i];
            file.write(reinterpret_cast<const char*>(&p.x), sizeof(int));
        }
    }
}
