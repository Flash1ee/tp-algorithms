/*
Реализовать дек с динамическим зацикленным буфером.
Требования: Дек должен быть реализован в виде класса.
 */
#include <iostream>
#include <sstream>
#include <cassert>

class Dec {
private:
    struct Node {
        int data;
        Node *next;
    };
    Node *head;
    Node *tail;
public:
    Dec() : head(nullptr), tail(nullptr) {};

    Dec(const Dec &) = delete;

    ~Dec();

    bool IsEmpty() const;

    Dec &operator=(const Dec &) = delete;

    void PushBack(int data);

    void PushFront(int data);

    int PopFront();

    int PopBack();

};

Dec::~Dec() {
    Node *prev = nullptr;
    while (!IsEmpty()) {
        prev = head->next;
        delete head;
        head = prev;
    }
}

bool Dec::IsEmpty() const {
    return head == nullptr;
}

void Dec::PushBack(int data) {
    Node *tmp = new Node;
    tmp->data = data;

    if (IsEmpty()) {
        head = tmp;
    } else {
        tail->next = tmp;
    }
    tail = tmp;
    tail->next = nullptr;
}

void Dec::PushFront(int data) {
    Node *tmp = new Node;
    tmp->data = data;
    if (IsEmpty()) {
        head = tmp;
        tail = tmp;
        head->next = nullptr;
    } else {
        tmp->next = head;
        head = tmp;
    }
}

int Dec::PopBack() {
    assert(tail != nullptr);
    Node *tmp = tail;

    Node *cur = head;
    Node *prev = cur;

    for (; cur != tail; cur = cur->next) {
        prev = cur;
    }
    tail = nullptr;
    if (prev == cur) {
        head = nullptr;
    } else {
        tail = prev;
    }

    int data = tmp->data;

    delete tmp;

    return data;
}

int Dec::PopFront() {
    assert(head != nullptr);
    Node *tmp = head;
    if (head == tail) {
        head = nullptr;
        tail = nullptr;
    } else {
        head = head->next;
    }
    int data = tmp->data;
    delete tmp;

    return data;
}

void run(std::istream &in, std::ostream &out) {
    int n = 0;
    bool answer = true;
    in >> n;
    Dec dequeue;
    for (int i = 0; i < n; i++) {
        int command = 0;
        int value = 0;
        in >> command >> value;
        switch (command) {
            case 1: {
                dequeue.PushFront(value);
                break;
            }
            case 2: {
                if (dequeue.IsEmpty()) {
                    answer = answer && value == -1;
                } else {
                    answer = answer && value == dequeue.PopFront();
                }
                break;
            }
            case 3: {
                dequeue.PushBack(value);
                break;
            }
            case 4: {
                if (dequeue.IsEmpty()) {
                    answer = answer && value == -1;
                } else {
                    answer = answer && value == dequeue.PopBack();
                }
                break;
            }
            default:
                assert(false);
        }
    }
    out << (answer ? "YES" : "NO");

}

void testDec() {
    {
        std::stringstream in;
        std::stringstream out;
        in << "3 1 44 3 50 2 44";
        run(in, out);
        assert(out.str() == "YES");
    }

    {
        std::stringstream in;
        std::stringstream out;
        in << "2 2 -1 1 10";
        run(in, out);
        assert(out.str() == "YES");
    }

    {
        std::stringstream in;
        std::stringstream out;
        in << "2 3 44 4 66";
        run(in, out);
        assert(out.str() == "NO");
    }


}

int main() {
    testDec();
//    run(std::cin, std::cout);

    return 0;
}