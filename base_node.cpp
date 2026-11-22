#include "base_node.hpp"
#include <fstream>

using namespace std;
namespace base_node
{
    net load(string name)
    {
        ifstream file(name, ios::binary);
        net n;
        file.read(reinterpret_cast<char*>(&(n.inputs_length)), sizeof(long long));
        file.read(reinterpret_cast<char*>(&(n.outputs_length)), sizeof(long long));
        n.inputs = new double[n.inputs_length];
        n.outputs = new double[n.outputs_length];
        for (int i = 0; i < n.inputs_length; i++)
        {
            file.read(reinterpret_cast<char*>(&(n.inputs[i])), sizeof(double));
        }
        for (int i = 0; i < n.outputs_length; i++)
        {
            file.read(reinterpret_cast<char*>(&(n.outputs[i])), sizeof(double));
        }
        file.read(reinterpret_cast<char*>(&(n.size)), sizeof(long long));
        n.n = new node[n.size];
        for (int i = 0; i < n.size; i++)
        {
            node new_node;
            file.read(reinterpret_cast<char*>(&(new_node.position)), sizeof(long long));
            file.read(reinterpret_cast<char*>(&(new_node.inputs_length)), sizeof(long long));
            file.read(reinterpret_cast<char*>(&(new_node.outputs_length)), sizeof(long long));
            file.read(reinterpret_cast<char*>(&(new_node.type)), sizeof(short));
            new_node.inputs = new long long[n.inputs_length];
            new_node.outputs = new long long[n.outputs_length];
            new_node.forward_inputs = new double[n.inputs_length];
            new_node.forward_output = 0;
            for (int j = 0; j < new_node.inputs_length; j++)
            {
                file.read(reinterpret_cast<char*>(&(new_node.inputs[j])), sizeof(long long));
            }
            for (int j = 0; j < new_node.outputs_length; j++)
            {
                file.read(reinterpret_cast<char*>(&(new_node.outputs[j])), sizeof(long long));
            }
            n.n[i] = new_node;
        }
        return n;
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
