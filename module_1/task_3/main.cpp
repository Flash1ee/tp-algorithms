/*
Реализовать дек с динамическим зацикленным буфером.
Требования: Дек должен быть реализован в виде класса.
 */
#include <iostream>
#include <sstream>
#include <cassert>

#define INIT_SIZE 2

#define PUSH_FRONT 1
#define POP_FRONT 2
#define PUSH_BACK 3
#define POP_BACK 4

class Dec {
private:
    int *buffer;
    int buffer_size;
    int real_size;
    int head;
    int tail;

    int defaultInitSize;

    void grow();

public:
    Dec()
            : buffer(nullptr), buffer_size(0), real_size(0), head(0), tail(0), defaultInitSize(INIT_SIZE) {};

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
    delete[] buffer;
}

bool Dec::IsEmpty() const {
    return real_size == 0;
}

void Dec::PushBack(int data) {
    if (buffer_size == real_size) {
        grow();
    }
    if (IsEmpty()) {
        buffer[tail] = data;
    } else if (tail == buffer_size - 1) {
        tail = 0; // 0
        buffer[tail] = data;
    } else {
        buffer[++tail] = data;
    }
    real_size++;

}

void Dec::PushFront(int data) {
    if (real_size == buffer_size) {
        grow();
    }
    if (IsEmpty()) {
        buffer[head] = data;
    } else if (head == 0) {
        head = buffer_size - 1;
        buffer[head] = data;
    } else {
        buffer[--head] = data;
    }
    real_size++;
}

int Dec::PopBack() {
    assert(!IsEmpty());

    int data = buffer[tail];
    real_size--;
    if (tail != head) {
        if (tail == 0) {
            tail = buffer_size - 1;
        } else {
            tail--;
        }
    }
    return data;
}

int Dec::PopFront() {
    assert(!IsEmpty());

    int data = buffer[head];
    real_size--;
    if (head != tail) {
        if (head == buffer_size - 1) {
            head = 0;
        } else {
            head++;
        }
    }

    return data;
}

void Dec::grow() {
    int size = std::max(defaultInitSize, buffer_size * 2);
    int *new_buf = new int[size];

    if (real_size) {
        if (head < tail) {
            for (int i = head; i < tail + 1; ++i) {
                new_buf[i] = buffer[i];
            }
        } else {
            int k = 0;
            for (int i = head; i < buffer_size; ++i, ++k) {
                new_buf[k] = buffer[i];
            }
            for (int i = 0; i < tail + 1; ++i, ++k) {
                new_buf[k] = buffer[i];
            }
            head = 0;
            tail = real_size - 1;
        }
    }
    delete[] buffer;
    buffer = new_buf;
    buffer_size = size;

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
            case PUSH_FRONT: {
                dequeue.PushFront(value);
                break;
            }
            case POP_FRONT: {
                if (dequeue.IsEmpty()) {
                    answer = answer && value == -1;
                } else {
                    answer = answer && value == dequeue.PopFront();
                }
                break;
            }
            case PUSH_BACK: {
                dequeue.PushBack(value);
                break;
            }
            case POP_BACK: {
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

    {
        std::stringstream in;
        std::stringstream out;
        in << "5 1 10 3 20 1 15 4 20 2 15";
        run(in, out);
        assert(out.str() == "YES");
    }
    {
        std::stringstream in;
        std::stringstream out;
        in << "5 1 10 3 20 1 15 4 20 4 10";
        run(in, out);
        assert(out.str() == "YES");
    }
    {
        std::stringstream in;
        std::stringstream out;
        in << "5 1 10 1 20 4 10 4 20 2 -1";
        run(in, out);
        assert(out.str() == "YES");
    }
    {
        std::stringstream in;
        std::stringstream out; // 10 20 30 100 200
        in << "13 1 10 1 20 1 30 3 100 3 200 2 30 4 200 4 100 4 20 4 10 4 -1 1 20 2 20";
        run(in, out);
        assert(out.str() == "NO");
    }
    {
        // Тест на проверку концов - ситуация, когда tail==head
        std::stringstream in;
        std::stringstream out; // 10 20 30 100 200
        in << "4 3 20 4 20 3 10 2 10";
        run(in, out);
        assert(out.str() == "YES");
    }


}

int main() {
//    testDec();
    run(std::cin, std::cout);

    return 0;
}