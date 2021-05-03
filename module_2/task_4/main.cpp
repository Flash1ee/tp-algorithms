/*
 * 4_1. Солдаты. В одной военной части решили построить в одну шеренгу по росту.
 * Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя,
 * а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
 * Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие,
 * а в конце – самые низкие. За расстановку солдат отвечал прапорщик,
 * который заметил интересную особенность – все солдаты в части разного роста.
 * Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат,
 * а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится.
 */

#include <iostream>
#include <stack>
#include <cassert>

template<class T>
struct Node {
    T data;
    Node *left;
    Node *right;
    unsigned char height;
    int count;  //количество узлов кол-во дочерних + 1

    Node(T &_data) : data(_data), left(nullptr), right(nullptr), height(0), count(1) {}
};

template<class T, class Compare>
class AvlTree {
public:
    AvlTree() = delete;

    explicit AvlTree(Compare _cmp) : root(nullptr), cmp(_cmp) {}

    ~AvlTree() {
        PostOrder([](Node<T> *node) { delete node; });
    }

    size_t insert(T &_data);

    void remove(T &_data);

    void removeByPos(size_t pos);


private:
    Node<T> *root;
    Compare cmp;

    Node<T> *remove(T &_data, Node<T> *node);

    Node<T> *remove(size_t pos, Node<T> *node);


    Node<T> *insert(T &_data, size_t &pos, Node<T> *node);

    Node<T> *moveMin(Node<T> *node, Node<T> **minAddr);

    Node<T> *rotateLeft(Node<T> *node);

    Node<T> *rotateRight(Node<T> *node);

    Node<T> *balance(Node<T> *node);

    void PostOrder(void visitor(Node<T> *));


    unsigned char height(Node<T> *node) {
        if (!node) {
            return 0;
        }
        return node->height;
    }

    int count(Node<T> *node) {
        if (!node) {
            return 0;
        }
        return node->count;
    }

    int bFactor(Node<T> *node) {
        assert(node);
        return height(node->right) - height(node->left);
    }

    int countFactor(Node<T> *node) {
        assert(node);
        return count(node->right) - count(node->left);
    }

    void fixHeight(Node<T> *node) {
        assert(node);
        unsigned char hl = height(node->left);
        unsigned char hr = height(node->right);
        node->height = std::max(hl, hr) + 1;
    }

    void fixCount(Node<T> *node) {
        assert(node);
        unsigned char cl = count(node->left);
        unsigned char cr = count(node->right);
        node->count = cl + cr + 1;
    }
};

template<class T, class Compare>
size_t AvlTree<T, Compare>::insert(T &_data) {
    size_t pos = 0;
    this->root = this->insert(_data, pos, this->root);
    return pos;
}

template<class T, class Compare>
Node<T> *AvlTree<T, Compare>::rotateRight(Node<T> *node) {
    assert(node);

    Node<T> *new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;

    fixHeight(node);
    fixHeight(new_root);

    fixCount(node);
    fixCount(new_root);

    return new_root;
}

template<class T, class Compare>
Node<T> *AvlTree<T, Compare>::rotateLeft(Node<T> *node) {
    assert(node);

    Node<T> *new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;

    fixHeight(node);
    fixHeight(new_root);

    fixCount(node);
    fixCount(new_root);

    return new_root;
}

template<class T, class Compare>
Node<T> *AvlTree<T, Compare>::balance(Node<T> *node) {
    assert(node);

    fixHeight(node);
    fixCount(node);
    if (bFactor(node) == 2) {
        if (bFactor(node->right) < 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    } else if (bFactor(node) == -2) {
        if (bFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);

    }
    return node;
}

template<class T, class Compare>
Node<T> *AvlTree<T, Compare>::insert(T &_data, size_t &pos, Node<T> *node) {
    if (!node) {
        return new Node<T>(_data);
    }
    node->count++;
    if (cmp(_data, node->data) < 0) {
        pos += count(node->right) + 1;
        node->left = insert(_data, pos, node->left);
    } else {
        node->right = insert(_data, pos, node->right);
    }

    return balance(node);

}

template<class T, class Compare>
void AvlTree<T, Compare>::remove(T &_data) {
    this->root = remove(_data, this->root);
}

template<class T, class Compare>
void AvlTree<T, Compare>::removeByPos(size_t pos) {
    this->root = remove(pos, this->root);
}

template<class T, class Compare>
Node<T> *AvlTree<T, Compare>::remove(size_t pos, Node<T> *node) {
    if (!node) {
        return nullptr;
    }
    size_t cur = node->count;
    assert(cur > pos);
    cur = count(node->right);

    while (cur != pos) {
        if (pos < cur) {
            node = node->right;
            cur -= 1 + count(node->left);
        } else {
            node = node->left;
            cur += 1 + count(node->right);
        }
    }
    return this->remove(node->data, this->root);
}

template<class T, class Compare>
Node<T> *AvlTree<T, Compare>::remove(T &_data, Node<T> *node) {
    if (!node) {
        return nullptr;
    }
    if (cmp(_data, node->data) < 0) {
        node->left = remove(_data, node->left);
    } else if (cmp(_data, node->data) > 0) {
        node->right = remove(_data, node->right);
    } else {
        Node<T> *q = node->left;
        Node<T> *r = node->right;
        delete node;
        if (!r) {
            return q;
        }
        Node<T> *tmp_min = nullptr;
        Node<T> *tmp = moveMin(r, &tmp_min);
        tmp_min->left = q;
        tmp_min->right = tmp;

        return balance(tmp_min);
    }
    return balance(node);

}

template<class T, class Compare>
void AvlTree<T, Compare>::PostOrder(void visitor(Node<T> *)) {
    if (!root) {
        return;
    }
    std::stack<Node<T> *> nodes;
    nodes.push(root);
    while (!nodes.empty()) {
        if (root) {
            if (root->right) {
                nodes.push(root->right);
            }
            if (root->left) {
                nodes.push(root->left);
            }
            delete root;
        }
        root = nodes.top();
        nodes.pop();
    }
}


template<class T, class Compare>
Node<T> *AvlTree<T, Compare>::moveMin(Node<T> *node, Node<T> **minAddr) {
    if (!node->left) {
        *minAddr = node;
        return node->right;
    }
    node->left = moveMin(node->left, minAddr);

    return balance(node);
}

struct SolidersCmp {
    int operator()(const int a, const int b) {
        return a - b;
    }
};

int main() {
    int n = 0;
    int tmp = 0;
    int operation, val;
    SolidersCmp cmp;
    AvlTree<int, SolidersCmp> tree(cmp);

    std::cin >> n;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> operation >> val;
        switch (operation) {
            case 1:
                std::cout << tree.insert(val) << std::endl;
                break;
            case 2:
                tree.removeByPos(val);
                break;
            default:
                assert(false);
        }
    }


    return 0;
}