#pragma once
#include <string>

using namespace std;

namespace base_node
{
    struct node
    {
        long long position;
        long long* inputs;
        long long* outputs;
        double* forward_inputs;
        double* forward_outputs;
        short type;
    };

    struct net
    {
        node** n;
        double* inputs;
        double* outputs;
        long long size;
        void save(net n, string name);
        void forward(net n);
        void backward(net n);
    };

    net load(string name);

    // functions about net
    void add_node(net net, node* node); // Add node to net
    long long make_node(short type, string name); // Make a new node in memory, then return its position
    long long search_node(string name); // Use its name to get its position
    long long remove_node(long long position); // Use position to remove node, them delete its memory
    long long remove_node(string name); // Search node name firstly,then use the position to remove it

    // functions about net link node
    

    // functions about node
    double node_forward(node* node, );
}
