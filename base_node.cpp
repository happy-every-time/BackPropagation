#include "base_node.hpp"

namespace base_node {
    Net* create_net(bool create_out = false) {
        Net* net = new Net;
        if (create_out) {
            OutputNode* node = new OutputNode;
            node->type = NODE_TYPE_OUTPUT;
            node->root = net;
            node->name = 0;
        }
        return net;
    }

    int get_a_new_number(Net* root) {
        if (root->old_number.size() < root->just_number) {
            int number = 0;
            while (true) {
                if (!(in(number, root->old_number))) {
                    root->old_number.push_back(number);
                    root->just_number = number;
                    return number;
                }
            }
        }
        else {
            int number = root->just_number + 1;
            root->just_number = number;
            root->old_number.push_back(number);
            return number;
        }
    }

    void del_a_number(Net* root, int number) {
        for (int i = 0; i < root->old_number.size(); i++) {
            if (root->old_number[i] == number) {
                root->old_number.erase(root->old_number.begin() + i);
            }
        }
    }

    void* create_node(short type, Net* root, int name) {
        MNode* n = new MNode;
        switch (type) {
        case NODE_TYPE_SUB:
        case NODE_TYPE_MUL:
        case NODE_TYPE_DIV:
            Node* node = new Node;
            node->type = type;
            node->root = root;
            node->name = name;
            n->type = type;
            n->node = node;
            return n;
        case NODE_TYPE_ADD:
            AddNode* node = new AddNode;
            node->type = NODE_TYPE_ADD;
            node->root = root;
            node->name = name;
            n->type = type;
            n->node = node;
            return n;
        case NODE_TYPE_OUTPUT:
            OutputNode* node = new OutputNode;
            node->type = NODE_TYPE_OUTPUT;
            node->root = root;
            node->name = name;
            n->type = type;
            n->node = node;
            return n;
        case NODE_TYPE_VAULE:
            ValueNode* node = new ValueNode;
            node->type = NODE_TYPE_VAULE;
            node->root = root;
            node->name = name;
            n->type = type;
            n->node = node;
            return n;
        case NODE_TYPE_PRINT:
            OutputNode* node = new OutputNode;
            node->type = NODE_TYPE_PRINT;
            node->root = root;
            node->name = name;
            n->type = type;
            n->node = node;
            return n;
        default:
            std::cerr << "[ERROR] " << "Wrong node type" << std::endl;
            throw std::runtime_error("[ERROR] Wrong node type");
        }
    }

    void add_node(Net* root, MNode* node) {
        root->nodes.push_back(node);
    }

    void link_node(Net* root, int aname, int bname, int bnumber) {
        for (int i = 0; i < root->nodes.size(); i++) {
            switch (root->nodes[i]->type) {
            case NODE_TYPE_SUB:
            case NODE_TYPE_MUL:
            case NODE_TYPE_DIV:
                Node* node1 = (Node*)(root->nodes[i]->node);
                int number = get_a_new_number(root);
                if (bnumber <= 0) {

                }
                break;
            case NODE_TYPE_ADD:
                AddNode* node2 = (AddNode*)(root->nodes[i]->node);
                break;
            case NODE_TYPE_OUTPUT:
            case NODE_TYPE_PRINT:
                OutputNode* node3 = (OutputNode*)(root->nodes[i]->node);
                break;
            case NODE_TYPE_VAULE:
                ValueNode* node3 = (ValueNode*)(root->nodes[i]->node);
                break;
            default:
                std::cerr << "[ERROR] " << "Wrong node type" << std::endl;
                throw std::runtime_error("[ERROR] Wrong node type");
            }
        }
    }

    void sort_node(Net* root) {
        for (int i = 0; i < root->nodes.size(); i++) {
            switch (root->nodes[i]->type) {
            case NODE_TYPE_SUB:
            case NODE_TYPE_MUL:
            case NODE_TYPE_DIV:
                Node* node1 = (Node*)(root->nodes[i]->node);
                node1->is_sort = false;
                break;
            case NODE_TYPE_ADD:
                AddNode* node2 = (AddNode*)(root->nodes[i]->node);
                node2->is_sort = false;
                break;
            case NODE_TYPE_OUTPUT:
            case NODE_TYPE_PRINT:
                OutputNode* node3 = (OutputNode*)(root->nodes[i]->node);
                node3->is_sort = false;
                break;
            default:
                std::cerr << "[ERROR] " << "Wrong node type" << std::endl;
                throw std::runtime_error("[ERROR] Wrong node type");
            }
        }

        std::vector<int> outputs = {};
        outputs.reserve(root->nodes.size());
        for (int i = 0; i < root->nodes.size(); i++) {
            if ((root->nodes[i]->type) == NODE_TYPE_VAULE) {
                ValueNode* node = (ValueNode*)(root->nodes[i]->node);
                outputs.push_back(node->output);
            }
        }
        #if DEBUG
        std::cout << "[DEBUG] ";
        for (int i : outputs) {
            std::cout << i << ' ';
        }
        std::cout << std::endl;
        #endif
        int higth = 0;
        while (true) {
            higth++;
            for (int i = 0; i < root->nodes.size(); i++) {
                switch (root->nodes[i]->type) {
                case NODE_TYPE_SUB:
                case NODE_TYPE_MUL:
                case NODE_TYPE_DIV:
                    Node* node1 = (Node*)(root->nodes[i]->node);
                    if (!(node1->is_sort)) {
                        if (in(node1->inputs[0], outputs) && in(node1->inputs[1], outputs)) {
                            node1->higth = higth;
                            node1->is_sort = true;
                        }
                    }
                    break;
                case NODE_TYPE_ADD:
                    AddNode* node2 = (AddNode*)(root->nodes[i]->node);
                    if (!(node2->is_sort)) {
                        bool in_it = true;
                        for (int i = 0; i < node2->width; i++) {
                            in_it = in_it && in(node2->inputs[i], outputs);
                        }
                        if (in_it) {
                            node2->higth = higth;
                            node2->is_sort = true;
                        }
                    }
                    break;
                case NODE_TYPE_OUTPUT:
                case NODE_TYPE_PRINT:
                    OutputNode* node3 = (OutputNode*)(root->nodes[i]->node);
                    if (!(node3->is_sort)) {
                        if (in(node3->input, outputs)) {
                            node3->higth = higth;
                            node3->is_sort = true;
                        }
                    }
                    break;
                default:
                    std::cerr << "[ERROR] " << "Wrong node type" << std::endl;
                    throw std::runtime_error("[ERROR] Wrong node type");
                }
            }
        };
    }

    bool in(int a, std::vector<int> b) {
        for (int i = 0; i < b.size(); i++) {
            if (a == b[i]) {
                return true;
            }
        }
        return false;
    }
};