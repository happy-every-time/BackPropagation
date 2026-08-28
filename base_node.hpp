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
        std::vector<MNode*> nodes;
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
        int higth;
        int name;
        int inputs[2];
        int value[2];
    };

    struct AddNode {
        const short type = NODE_TYPE_ADD;
        bool is_sort;
        int width;
        int output;
        Net* root;
        int higth;
        int name;
        int* inputs;
        int* value;
    };

    struct OutputNode {
        short type = -1;
        bool is_sort;
        Net* root;
        int higth;
        int name;
        int input;
        int value;
    };

    struct ValueNode {
        const short type = NODE_TYPE_VAULE;
        bool is_sort;
        Net* root;
        int higth = 0;
        int name;
        int output;
        int value;
    };

    Net* create_net(bool create_out);
    int get_a_new_number(Net* root);
    void* create_node(short type, Net* root, int name);
    void add_node(Net* root, MNode* node);
    void link_node(Net* root, int aname, int bname, int bnumber);
    void sort_node(Net* net);
    bool in(int a, std::vector<int> b);
};