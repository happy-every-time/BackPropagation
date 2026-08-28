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
        int number = get_a_new_number(root);
        for (int i = 0; i < root->nodes.size(); i++) {
            switch (root->nodes[i]->type) {
            case NODE_TYPE_SUB:
            case NODE_TYPE_MUL:
            case NODE_TYPE_DIV:
                Node* node1 = (Node*)(root->nodes[i]->node);
                if (node1->name == aname) {
                    node1->output = number;
                }
                else if (node1->name == bname) {
                    if (bnumber <= 0) {
                        node1->inputs[0] = number;
                    }
                    else if (bnumber >= 1) {
                        node1->inputs[1] = number;
                    }
                }
                break;
            case NODE_TYPE_ADD:
                AddNode* node2 = (AddNode*)(root->nodes[i]->node);
                if (node2->name == aname) {
                    node2->output = number;
                }
                else if (node2->name == bname) {
                    if (bnumber < 0) {
                        bnumber = 0;
                    }
                    if (bnumber > node2->width) {
                        bnumber = node2->width;
                    }
                    node2->inputs[bnumber] = number;
                }
                break;
            case NODE_TYPE_OUTPUT:
            case NODE_TYPE_PRINT:
                OutputNode* node3 = (OutputNode*)(root->nodes[i]->node);
                if (node3->name == bname) {
                    node3->input = number;
                }
                break;
            case NODE_TYPE_VAULE:
                ValueNode* node4 = (ValueNode*)(root->nodes[i]->node);
                if (node4->name == aname) {
                    node4->output = number;
                }
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

        std::vector<int>* outputs = new std::vector<int>;
        (*outputs).reserve(root->nodes.size());
        for (int i = 0; i < root->nodes.size(); i++) {
            if ((root->nodes[i]->type) == NODE_TYPE_VAULE) {
                ValueNode* node = (ValueNode*)(root->nodes[i]->node);
                (*outputs).push_back(node->output);
            }
        }
        #if DEBUG
        std::cout << "[DEBUG] ";
        for (int i : (*outputs)) {
            std::cout << i << ' ';
        }
        std::cout << std::endl;
        #endif
        int hight = 0;
        while (true) {
            hight++;
            for (int i = 0; i < root->nodes.size(); i++) {
                switch (root->nodes[i]->type) {
                case NODE_TYPE_SUB:
                case NODE_TYPE_MUL:
                case NODE_TYPE_DIV:
                    Node* node1 = (Node*)(root->nodes[i]->node);
                    if (!(node1->is_sort)) {
                        if (in(node1->inputs[0], (*outputs)) && in(node1->inputs[1], (*outputs))) {
                            node1->hight = hight;
                            node1->is_sort = true;
                        }
                    }
                    break;
                case NODE_TYPE_ADD:
                    AddNode* node2 = (AddNode*)(root->nodes[i]->node);
                    if (!(node2->is_sort)) {
                        bool in_it = true;
                        for (int i = 0; i < node2->width; i++) {
                            in_it = in_it && in(node2->inputs[i], (*outputs));
                        }
                        if (in_it) {
                            node2->hight = hight;
                            node2->is_sort = true;
                        }
                    }
                    break;
                case NODE_TYPE_OUTPUT:
                case NODE_TYPE_PRINT:
                    OutputNode* node3 = (OutputNode*)(root->nodes[i]->node);
                    if (!(node3->is_sort)) {
                        if (in(node3->input, (*outputs))) {
                            node3->hight = hight;
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
        delete outputs;
    }

    bool in(int a, std::vector<int> b) {
        for (int i = 0; i < b.size(); i++) {
            if (a == b[i]) {
                return true;
            }
        }
        return false;
    }

    int get_node_hight(MNode* node) {
        switch (node->type) {
        case NODE_TYPE_SUB:
        case NODE_TYPE_MUL:
        case NODE_TYPE_DIV:
            Node* node1 = (Node*)(node->type);
            return node1->hight;
            break;
        case NODE_TYPE_ADD:
            AddNode* node2 = (AddNode*)(node->type);
            return node2->hight;
            break;
        case NODE_TYPE_OUTPUT:
        case NODE_TYPE_PRINT:
            OutputNode* node3 = (OutputNode*)(node->type);
            return node3->hight;
            break;
        default:
            std::cerr << "[ERROR] " << "Wrong node type" << std::endl;
            throw std::runtime_error("[ERROR] Wrong node type");
        }
    }

    double max(double a, double b) {
        if (a > b) {
            return a;
        }
        else if (b > a) {
            return b;
        }
        else {
            return a;
        }
    }

    double node_forward(double a, double b, MNode* node) {
        Node* node1 = (Node*)(node->node);
        node1->inputs[0] = a;
        node1->inputs[1] = b;
        switch (node->type) {
        case NODE_TYPE_SUB:
            return a - b;
        case NODE_TYPE_MUL:
            return a * b;
        case NODE_TYPE_DIV:
            return a / b;
        }

    }

    double node_forward(double* a, MNode* node) {
        switch (node->type) {
        case NODE_TYPE_ADD:
            double value = 0.0;
            AddNode* node2 = (AddNode*)(node->node);
            for (int i = 0; i < node2->width; i++) {
                value += a[i];
            }
            return value;
        }
    }

    void node_forward(double a, MNode* node, Net* root) {
        switch (node->type) {
        case NODE_TYPE_OUTPUT:
        case NODE_TYPE_PRINT:
            OutputNode* node3 = (OutputNode*)(node->node);
            std::cout << a;
            Tuple* t = new Tuple;
            t->key = node3->name;
            t->value = a;
            root->outputs.push_back(t);
            break;
        default:
            std::cerr << "[ERROR] " << "Wrong node type" << std::endl;
            throw std::runtime_error("[ERROR] Wrong node type");
        }
    }

    double node_forward(MNode* node) {
        switch (node->type) {
        case NODE_TYPE_VAULE:
            ValueNode* node4 = (ValueNode*)(node->type);
            return node4->value;
            break;
        default:
            std::cerr << "[ERROR] " << "Wrong node type" << std::endl;
            throw std::runtime_error("[ERROR] Wrong node type");
        }
    }

    double* net_forward(Net* root) {
        int max_hight = 1;
        for (int i = 0; i < root->nodes.size(); i++) {
            max_hight = max(max_hight, get_node_hight(root->nodes[i]));
        }
        std::vector<Tuple*>* values = new std::vector<Tuple*>;
        (*values).reserve(root->nodes.size());
        for (int h = 0; h <= max_hight; h++) {
            for (int i = 0; i < root->nodes.size(); i++) {
                if (get_node_hight(root->nodes[i]) == h) {
                    if (h == 0) {
                        ValueNode* node = (ValueNode*)(root->nodes[i]->node);
                        Tuple* t = new Tuple;
                        t->key = node->output;
                        t->value = node->value;
                        (*values).push_back(t);
                    }
                    else {
                        switch (root->nodes[i]->type) {
                        case NODE_TYPE_SUB:
                        case NODE_TYPE_MUL:
                        case NODE_TYPE_DIV:
                            Node* node1 = (Node*)(root->nodes[i]->node);
                            double a = 0.0;
                            double b = 0.0;
                            for (int j; j < (*values).size(); j++) {
                                if ((*values)[j]->key == node1->inputs[0]) {
                                    a = (*values)[j]->value;
                                }
                                else if ((*values)[j]->key == node1->inputs[1]) {
                                    b = (*values)[j]->value;
                                }
                            }
                            double c = node_forward(a, b, root->nodes[i]);
                            Tuple* t = new Tuple;
                            t->key = node1->output;
                            t->value = c;
                            (*values).push_back(t);
                        case NODE_TYPE_ADD:
                            AddNode* node2 = (AddNode*)(root->nodes[i]->node);
                            double* a = new double[node2->width];

                        }
                    }
                }
            }
        }
    }
};