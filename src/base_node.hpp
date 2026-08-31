#pragma once
#define NODE_TYPE_ADD 0
#define NODE_TYPE_SUB 1
#define NODE_TYPE_MUL 2
#define NODE_TYPE_DIV 3
#define NODE_TYPE_OUTPUT 4
#define NODE_TYPE_VAULE 5
#define NODE_TYPE_PRINT 6
#define DEBUG 1
#include <iostream>
#include <vector>
#include <string>

namespace base_node {
    struct Net {
        std::vector<int> old_number = {0};
        std::vector<struct MNode> nodes;
        std::vector<struct Tuple> outputs;
        std::vector<struct Tuple> back_outputs;
        int just_number = 0;
    };

    struct MNode {
        short type;
        void* node;
    };

    struct Node {
        short type;
        bool is_sort;
        int output;
        Net* root;
        int hight;
        int name;
        int inputs[2];
        double value[2];
    };

    struct AddNode {
        const short type = NODE_TYPE_ADD;
        bool is_sort;
        int width;
        int output;
        Net* root;
        int hight;
        int name;
        int* inputs;
        double* value;
    };

    struct OutputNode {
        short type = -1;
        bool is_sort;
        int hight;
        Net* root;
        int name;
        int input;
        double value;
    };

    struct ValueNode {
        const short type = NODE_TYPE_VAULE;
        bool is_sort;
        int hight = 0;
        Net* root;
        int name;
        int output;
        double value;
    };

    struct Tuple {
        int key;
        double value;
    };

    Net* create_net(bool create_out);
    MNode create_node(short type, Net* root, int name);

    int get_a_new_number(Net* root);
    void del_a_number(Net* root, int number);
    
    void add_node(Net* root, MNode* node);
    void link_node(Net* root, int aname, int bname, int bnumber);
    void sort_node(Net* root);

    bool in(int a, std::vector<int> b);
    int get_node_hight(MNode node);
    double max(double a, double b);

    double node_forward(double a, double b, MNode* node); // Base node
    double node_forward(double* a, MNode* node); // Add node
    void node_forward(double a, MNode* node, Net* root); // Print and output node
    double node_forward(MNode* node); // Value node
    void net_forward(Net* root);

    void node_backward(double a, MNode* node, Net* root); // Value node
    void node_backward(double a, MNode* node, std::vector<Tuple*>* values); // Base and add node
    void net_backward(Net* root);
};