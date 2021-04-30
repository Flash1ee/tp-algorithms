//
// Created by flashie on 28.04.2021.
//
//2_3. Выведите элементы в порядке post-order (снизу вверх).

#include <iostream>
#include <stack>
#include <cassert>

struct IsLessEqual {
    int operator()(const int a, const int b) {
        return a <= b;
    }
};

template<class T, class Compare>
class BinaryTree {
private:
    struct Node {
        T data;
        Node *left;
        Node *right;

        Node(const T &_data) : data(_data), left(nullptr), right(nullptr) {}
    };

    Node *root;
    Compare cmp;
public:
    BinaryTree() = delete;

    explicit BinaryTree(Compare _cmp);

    ~BinaryTree();

    void Add(const T _data);

    void PostOrder(void visitor(T &));

    BinaryTree(const BinaryTree<T, Compare> &) = delete;

    BinaryTree<T, Compare> &operator=(const BinaryTree<T, Compare>) = delete;

};

template<class T, class Compare>
BinaryTree<T, Compare>::BinaryTree(Compare _cmp) : root(nullptr), cmp(_cmp) {}

template<class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree() {
    if (!root) {
        return;
    }
    std::stack<Node *> nodes;
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
void BinaryTree<T, Compare>::Add(const T _data) {
    if (!root) {
        root = new Node(_data);
        return;
    }

    Node *cur = root;
    Node *parent = nullptr;
    while (cur) {
        parent = cur;
        if (cmp(cur->data, _data)) {
            cur = cur->right;
        } else {
            cur = cur->left;
        }
    }

    cur = new Node(_data);

    if (cmp(parent->data, cur->data)) {
        parent->right = cur;
    } else {
        parent->left = cur;
    }
}

template<class T, class Compare>
void BinaryTree<T, Compare>::PostOrder(void (*visitor)(T &)) {
    assert(visitor);
    if (root == nullptr) {
        return;
    }
    std::stack<Node *> nodes;
    Node *cur = root;
    nodes.push(nullptr);

    while (!nodes.empty()) {
        if (nodes.top() == nullptr) {
            nodes.pop();
        }
        while (cur) {
            if (cur->right) {
                nodes.push(cur->right);
            }
            nodes.push(cur);
            cur = cur->left;
        }
        cur = nodes.top();
        nodes.pop();

        auto top = nodes.size() ? nodes.top() : nullptr;
        if (cur->right && top == cur->right) {
            nodes.pop();
            nodes.push(cur);
            cur = cur->right;
        } else {
            visitor(cur->data);
            cur = nullptr;
        }
    }

}


int main() {
    IsLessEqual compare;
    BinaryTree<int, IsLessEqual> tree(compare);

    size_t n = 0;
    int tmp;
    std::cin >> n;
    for (size_t i = 0; i < n; ++i) {
        std::cin >> tmp;
        tree.Add(tmp);
    }
    tree.PostOrder([](int &_data) -> void { std::cout << _data << " "; });

    return 0;
}