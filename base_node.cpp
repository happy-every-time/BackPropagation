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
        if ((int)(root->old_number.size()) < root->just_number) {
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
        for (int i = 0; i < (int)(root->old_number.size()); i++) {
            if (root->old_number[i] == number) {
                root->old_number.erase(root->old_number.begin() + i);
            }
        }
    }

    MNode create_node(short type, Net* root, int name) {
        MNode n = {};
        if (type == NODE_TYPE_SUB || type == NODE_TYPE_MUL || type == NODE_TYPE_DIV) {
            Node* node = new Node;
            node->type = type;
            node->root = root;
            node->name = name;
            n.type = type;
            n.node = node;
            return n;
        }
        else if (type == NODE_TYPE_ADD) {
            AddNode* node = new AddNode;
            node->root = root;
            node->name = name;
            n.type = type;
            n.node = node;
            return n;
        }
        else if (type == NODE_TYPE_OUTPUT) {
            OutputNode* node = new OutputNode;
            node->type = NODE_TYPE_OUTPUT;
            node->root = root;
            node->name = name;
            n.type = type;
            n.node = node;
            return n;
        }
        else if (type == NODE_TYPE_VAULE) {
            ValueNode* node = new ValueNode;
            node->root = root;
            node->name = name;
            n.type = type;
            n.node = node;
            return n;
        }
        else if (type == NODE_TYPE_PRINT) {
            OutputNode* node = new OutputNode;
            node->type = NODE_TYPE_PRINT;
            node->root = root;
            node->name = name;
            n.type = type;
            n.node = node;
            return n;
        }
        else {
            std::cerr << "[ERROR] " << "Wrong node type" << std::endl;
            throw std::runtime_error("[ERROR] Wrong node type");
        }
    }

    void add_node(Net* root, MNode node) {
        root->nodes.push_back(node);
    }

    void link_node(Net* root, int aname, int bname, int bnumber) {
        int number = get_a_new_number(root);
        for (int i = 0; i < (int)(root->nodes.size()); i++) {
            if (root->nodes[i].type == NODE_TYPE_SUB || root->nodes[i].type == NODE_TYPE_MUL || root->nodes[i].type == NODE_TYPE_DIV) {
                Node* node = (Node*)(root->nodes[i].node);
                if (node->name == aname) {
                    node->output = number;
                }
                else if (node->name == bname) {
                    if (bnumber <= 0) {
                        node->inputs[0] = number;
                    }
                    else if (bnumber >= 1) {
                        node->inputs[1] = number;
                    }
                }
            }
            else if (root->nodes[i].type == NODE_TYPE_ADD) {
                AddNode* node = (AddNode*)(root->nodes[i].node);
                if (node->name == aname) {
                    node->output = number;
                }
                else if (node->name == bname) {
                    if (bnumber < 0) {
                        bnumber = 0;
                    }
                    if (bnumber > node->width) {
                        bnumber = node->width;
                    }
                    node->inputs[bnumber] = number;
                }
            }
            else if (root->nodes[i].type == NODE_TYPE_OUTPUT || root->nodes[i].type == NODE_TYPE_PRINT) {
                OutputNode* node = (OutputNode*)(root->nodes[i].node);
                if (node->name == bname) {
                    node->input = number;
                }
            }
            else if (root->nodes[i].type == NODE_TYPE_VAULE) {
                ValueNode* node = (ValueNode*)(root->nodes[i].node);
                if (node->name == aname) {
                    node->output = number;
                }
                break;
            }
            else {
                std::cerr << "[ERROR] " << "Wrong node type" << std::endl;
                throw std::runtime_error("[ERROR] Wrong node type");
            }
        }
    }

    void sort_node(Net* root) {
        for (int i = 0; i < (int)(root->nodes.size()); i++) {
            if (root->nodes[i].type == NODE_TYPE_SUB || root->nodes[i].type == NODE_TYPE_MUL || root->nodes[i].type == NODE_TYPE_DIV) {
                Node* node = (Node*)(root->nodes[i].node);
                node->is_sort = false;
            }
            else if (root->nodes[i].type == NODE_TYPE_ADD) {
                AddNode* node = (AddNode*)(root->nodes[i].node);
                node->is_sort = false;
            }
            else if (root->nodes[i].type == NODE_TYPE_OUTPUT || root->nodes[i].type == NODE_TYPE_PRINT) {
                OutputNode* node = (OutputNode*)(root->nodes[i].node);
                node->is_sort = false;
            }
            else {
                std::cerr << "[ERROR] " << "Wrong node type" << std::endl;
                throw std::runtime_error("[ERROR] Wrong node type");
            }
        }

        std::vector<int> outputs = {};
        outputs.reserve(root->nodes.size());
        for (int i = 0; i < (int)(root->nodes.size()); i++) {
            if ((root->nodes[i].type) == NODE_TYPE_VAULE) {
                ValueNode* node = (ValueNode*)(root->nodes[i].node);
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
        int hight = 0;
        while (true) {
            hight++;
            for (int i = 0; i < (int)(root->nodes.size()); i++) {
                if (root->nodes[i].type == NODE_TYPE_SUB || root->nodes[i].type == NODE_TYPE_MUL || root->nodes[i].type == NODE_TYPE_DIV) {
                    Node* node = (Node*)(root->nodes[i].node);
                    if (!(node->is_sort)) {
                        if (in(node->inputs[0], outputs) && in(node->inputs[1], outputs)) {
                            node->hight = hight;
                            node->is_sort = true;
                        }
                    }
                }
                else if (root->nodes[i].type == NODE_TYPE_ADD) {
                    AddNode* node = (AddNode*)(root->nodes[i].node);
                    if (!(node->is_sort)) {
                        bool in_it = true;
                        for (int i = 0; i < node->width; i++) {
                            in_it = in_it && in(node->inputs[i], outputs);
                        }
                        if (in_it) {
                            node->hight = hight;
                            node->is_sort = true;
                        }
                    }
                }
                else if (root->nodes[i].type == NODE_TYPE_OUTPUT || root->nodes[i].type == NODE_TYPE_PRINT) {
                    OutputNode* node = (OutputNode*)(root->nodes[i].node);
                    if (!(node->is_sort)) {
                        if (in(node->input, outputs)) {
                            node->hight = hight;
                            node->is_sort = true;
                        }
                    }
                }
                else {
                    std::cerr << "[ERROR] " << "Wrong node type" << std::endl;
                    throw std::runtime_error("[ERROR] Wrong node type");
                }
            }
        };
    }

    bool in(int a, std::vector<int> b) {
        for (int i = 0; i < (int)(b.size()); i++) {
            if (a == b[i]) {
                return true;
            }
        }
        return false;
    }

    int get_node_hight(MNode node) {
        if (node.type == NODE_TYPE_SUB || node.type == NODE_TYPE_MUL || node.type == NODE_TYPE_DIV) {
            Node* n = (Node*)(node.type);
            return n->hight;
        }
        else if (node.type == NODE_TYPE_ADD) {
            AddNode* n = (AddNode*)(node.type);
            return n->hight;
        }
        else if (node.type == NODE_TYPE_OUTPUT || node.type == NODE_TYPE_PRINT) {
            OutputNode* n = (OutputNode*)(node.type);
            return n->hight;
        }
        else {
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

    double node_forward(double a, double b, MNode node) {
        Node* n = (Node*)(node.node);
        n->value[0] = a;
        n->value[1] = b;
        if (node.type == NODE_TYPE_SUB) {
            return a - b;
        }
        else if (node.type == NODE_TYPE_MUL) {
            return a * b;
        }
        else if (node.type == NODE_TYPE_DIV) {
            return a / b;
        }
        else {
            std::cerr << "[ERROR] " << "Wrong node type" << std::endl;
            throw std::runtime_error("[ERROR] Wrong node type");
            return 0.0;
        }
    }

    double node_forward(double* a, MNode node) {
        if (node.type == NODE_TYPE_ADD) {
            double value = 0.0;
            AddNode* n = (AddNode*)(node.node);
            for (int i = 0; i < n->width; i++) {
                value += a[i];
            }
            return value;
        }
        else {
            std::cerr << "[ERROR] " << "Wrong node type" << std::endl;
            throw std::runtime_error("[ERROR] Wrong node type");
            return 0.0;
        }
    }

    void node_forward(double a, MNode node, Net* root) {
        if (node.type == NODE_TYPE_OUTPUT || node.type == NODE_TYPE_PRINT) {
            OutputNode* n = (OutputNode*)(node.node);
            std::cout << a;
            Tuple t = {};
            t.key = n->name;
            t.value = a;
            root->outputs.push_back(t);
        }
    }

    double node_forward(MNode node) {
        if (node.type == NODE_TYPE_VAULE) {
            ValueNode* n = (ValueNode*)(node.type);
            return n->value;
        }
        else {
            std::cerr << "[ERROR] " << "Wrong node type" << std::endl;
            throw std::runtime_error("[ERROR] Wrong node type");
            return 0.0;
        }
    }

    void net_forward(Net* root) {
        int max_hight = 1;
        for (int i = 0; i < (int)(root->nodes.size()); i++) {
            max_hight = max(max_hight, get_node_hight(root->nodes[i]));
        }
        std::vector<Tuple> values;
        values.reserve(root->nodes.size());
        for (int h = 0; h <= max_hight; h++) {
            for (int i = 0; i < (int)(root->nodes.size()); i++) {
                if (get_node_hight(root->nodes[i]) == h) {
                    if (h == 0) {
                        ValueNode* node = (ValueNode*)(root->nodes[i].node);
                        Tuple t = {};
                        t.key = node->output;
                        t.value = node_forward(root->nodes[i]);
                        values.push_back(t);
                    }
                    else {
                        if (root->nodes[i].type == NODE_TYPE_SUB || root->nodes[i].type == NODE_TYPE_MUL || root->nodes[i].type == NODE_TYPE_DIV) {
                            Node* n = (Node*)(root->nodes[i].node);
                            double a = 0.0;
                            double b = 0.0;
                            for (int j = 0; j < (int)(values.size()); j++) {
                                if (values[j].key == n->inputs[0]) {
                                    a = values[j].value;
                                }
                                else if (values[j].key == n->inputs[1]) {
                                    b = values[j].value;
                                }
                            }
                            double c = node_forward(a, b, root->nodes[i]);
                            Tuple t = {};
                            t.key = n->output;
                            t.value = c;
                            values.push_back(t);
                        }
                        else if (root->nodes[i].type == NODE_TYPE_ADD) {
                            AddNode* n = (AddNode*)(root->nodes[i].node);
                            int w = n->width;
                            double* aa = new double[w];
                            for (int j = 0; j < n->width; j++) {
                                for (int k = 0; k < (int)(values.size()); k++) {
                                    if (values[k].key == n->inputs[j]) {
                                        aa[j] = values[k].value;
                                    }
                                }
                            }
                            double c = node_forward(aa, root->nodes[i]);
                            delete aa;
                            Tuple t = {};
                            t.key = n->output;
                            t.value = c;
                            values.push_back(t);
                        }
                        else if (root->nodes[i].type == NODE_TYPE_OUTPUT || root->nodes[i].type == NODE_TYPE_PRINT) {
                            OutputNode* n = (OutputNode*)(root->nodes[i].node);
                            double a = 0.0;
                            for (int j = 0; j < (int)(values.size()); j++) {
                                if (values[j].key == n->input) {
                                    a = values[j].value;
                                }
                            }
                            node_forward(a, root->nodes[i], root);
                        }
                        else {
                            std::cerr << "[ERROR] " << "Wrong node type" << std::endl;
                            throw std::runtime_error("[ERROR] Wrong node type");
                        }
                    }
                }
            }
        }
    }

    void node_backward(double a, MNode node, Net* root) {
        if (node.type == NODE_TYPE_VAULE) {
            ValueNode* n = (ValueNode*)(node.node);
            Tuple t = {};
            t.key = n->name;
            t.value = a;
            root->outputs.push_back(t);
        }
    }

    void node_backward(double a, MNode node, std::vector<Tuple> values) {
        if (node.type == NODE_TYPE_ADD) {
            AddNode* n = (AddNode*)(node.node);
            for (int i = 0; i < n->width; i++) {
                Tuple t = {};
                t.key = n->inputs[i];
                t.value = a;
                values.push_back(t);
            }
        }
        else if (node.type == NODE_TYPE_SUB) {
            Node* n = (Node*)(node.node);
            Tuple t = {};
            t.key = n->inputs[0];
            t.value = a;
            values.push_back(t);
            Tuple t2 = {};
            t2.key = n->inputs[1];
            t2.value = a;
            values.push_back(t2);
        }
        else if (node.type == NODE_TYPE_MUL) {
            Node* n = (Node*)(node.node);
            Tuple t = {};
            t.key = n->inputs[0];
            t.value = a * n->value[1];
            values.push_back(t);
            Tuple t2 = {};
            t2.key = n->inputs[1];
            t2.value = a * n->value[0];
            values.push_back(t2);
        }
        else if (node.type == NODE_TYPE_DIV) {
            Node* n = (Node*)(node.node);
            Tuple t = {};
            t.key = n->inputs[0];
            t.value = a * (1 / n->value[1]);
            values.push_back(t);
            Tuple t2 = {};
            t2.key = n->inputs[1];
            t2.value = a * n->value[0];
            values.push_back(t2);
        }
    }

    void net_backward(Net* root) {
        int max_hight = 1;
        for (int i = 0; i < (int)(root->nodes.size()); i++) {
            max_hight = max(max_hight, get_node_hight(root->nodes[i]));
        }
        std::vector<Tuple> values;
        values.reserve(root->nodes.size());
        for (int h = max_hight; h >= 0; h--) {
            for (int i = 0; i < (int)(root->nodes.size()); i++) {
                if (get_node_hight(root->nodes[i]) == h) {
                    if (root->nodes[i].type == NODE_TYPE_SUB || root->nodes[i].type == NODE_TYPE_MUL || root->nodes[i].type == NODE_TYPE_DIV) {
                        Node* node = (Node*)(root->nodes[i].node);
                        double a = 0.0;
                        for (int j = 0; j < (int)(values.size()); j++) {
                            if (values[j].key == node->output) {
                                a = values[j].value;
                            }
                        }
                        node_backward(a, root->nodes[i], values);
                    }
                    else if (root->nodes[i].type == NODE_TYPE_ADD) {
                        AddNode* node = (AddNode*)(root->nodes[i].node);
                        double a = 0.0;
                        for (int j = 0; j < (int)(values.size()); j++) {
                            if (values[j].key == node->output) {
                                a = values[j].value;
                            }
                        }
                        node_backward(a, root->nodes[i], values);
                    }
                    else if (root->nodes[i].type == NODE_TYPE_VAULE) {
                        ValueNode* node = (ValueNode*)(root->nodes[i].node);
                        double a = 0.0;
                        for (int j = 0; j < (int)(values.size()); j++) {
                            if (values[j].key == node->output) {
                                a = values[j].value;
                            }
                        }
                        node_backward(a, root->nodes[i], root);
                    }
                    else {
                        std::cerr << "[ERROR] " << "Wrong node type" << std::endl;
                        throw std::runtime_error("[ERROR] Wrong node type");
                    }
                }
            }
        }
    }
};