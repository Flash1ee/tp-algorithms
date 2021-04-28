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
public:
    BinaryTree() = delete;

    explicit BinaryTree(Compare _cmp);

    ~BinaryTree();

    void Add(const T _data);

    void PostOrder(void visitor(T &));

    BinaryTree(const BinaryTree<T, Compare> &) = delete;

    BinaryTree<T, Compare> &operator=(const BinaryTree<T, Compare>) = delete;

private:
    struct Node {
        T data;
        Node *left;
        Node *right;

        Node(const T &_data) : data(_data), left(nullptr), right(nullptr) {}
    };

    Node *root;
    Compare cmp;
};

template<class T, class Compare>
BinaryTree<T, Compare>::BinaryTree(Compare _cmp) : root(nullptr), cmp(_cmp) {}

template<class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree() {

}

template<class T, class Compare>
void BinaryTree<T, Compare>::Add(const T _data) {
    if (!root) {
        root = new Node(_data);
        return;
    }

    Node *&cur = root;
    while (cur) {
        if (cmp(cur->data, _data)) {
            cur = cur->right;
        } else {
            cur = cur->left;
        }
    }
    cur = new Node(_data);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::PostOrder(void (*visitor)(T &)) {


}


int main() {

    return 0;
}