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
        file.write(reinterpret_cast<const char*>(&(n.inputs_length)), sizeof(long long));
        file.write(reinterpret_cast<const char*>(&(n.outputs_length)), sizeof(long long));
        for (int i = 0; i < n.inputs_length; i++)
        {
            file.write(reinterpret_cast<const char*>(&(n.inputs[i])), sizeof(double));
        }
        for (int i = 0; i < n.outputs_length; i++)
        {
            file.write(reinterpret_cast<const char*>(&(n.outputs[i])), sizeof(double));
        }
        file.write(reinterpret_cast<const char*>(&(n.size)), sizeof(long long));
        for (int i = 0; i < n.size; i++)
        {
            node node = n.n[i];
            file.write(reinterpret_cast<const char*>(&(node.position)), sizeof(long long));
            file.write(reinterpret_cast<const char*>(&(node.inputs_length)), sizeof(long long));
            file.write(reinterpret_cast<const char*>(&(node.outputs_length)), sizeof(long long));
            file.write(reinterpret_cast<const char*>(&(node.type)), sizeof(short));
            for (int j = 0; j < node.inputs_length; j++)
            {
                file.write(reinterpret_cast<const char*>(&(node.inputs[j])), sizeof(long long));
            }
            for (int j = 0; j < node.outputs_length; j++)
            {
                file.write(reinterpret_cast<const char*>(&(node.outputs[j])), sizeof(long long));
            }
        }
        file.close();
    }
}
