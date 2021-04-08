#include <iostream>
#include <cassert>

#define INIT_SIZE 2

template <class T>
class Stack {
public:
    Stack() : buffer(nullptr), bufSize(0), realSize(0), defaultInitSize(INIT_SIZE) {}

    ~Stack() { delete[] buffer; }

    void PushBack(const T& elem);

    bool IsEmpty();

    T& Pop();


private:
    T *buffer;
    int bufSize;
    int realSize;
    int defaultInitSize;

    void grow();
};

template<class T>
bool Stack<T>::IsEmpty() {
    return realSize == 0;
}

template<class T>
void Stack<T>::PushBack(const T &elem) {
    if (bufSize == realSize) {
        grow();
    }
    assert(realSize < bufSize && buffer != nullptr);
    buffer[realSize++] = elem;
}
template <class T>
T& Stack<T>::Pop() {
    assert(realSize > 0);
    return buffer[--realSize];
}

template<class T>
void Stack<T>::grow() {
    int newBufferSize = std::max(bufSize * 2, defaultInitSize);
    T *newBuf = new T[newBufferSize];
    for (int i = 0; i < realSize; ++i) {
        newBuf[i] = buffer[i];
    }
    delete[] buffer;

    buffer = newBuf;
    bufSize = newBufferSize;
}

bool process(std::string str) {
    Stack<char> stack{};

    for (auto &i : str) {
        if (i == '(') {
            stack.PushBack(i);
        } else if (i == ')'){
            if (stack.IsEmpty()) {
                return false;
            }
            char tmp = stack.Pop();
            if (tmp != '(') {
                return false;
            }
        }
    }
    if (!stack.IsEmpty()) {
        return false;
    }
    return true;
}

int main() {
    std::string str;

    std::cin >> str;

    std::cout << (process(str) ? "YES" : "NO");

    return 0;
}