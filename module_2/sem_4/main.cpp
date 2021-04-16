#include <string>
#include <vector>
#include <cassert>
#include <iostream>
#include <utility>

// Хэш функция методом Горнера
struct Hasher {
    unsigned int operator()(const std::string &str) const {
        unsigned int hash = 0;
        for (unsigned int i = 0; i < str.size(); ++i) {
            hash = hash * 7 + str[i];
        }
        return hash;
    }
};

template<class T, class H>
class HashTable {
public:
    explicit HashTable(const H& hasher);

    HashTable(const HashTable &hasher) = delete;

    HashTable &operator=(const HashTable &hasher) = delete;

    ~HashTable();

    bool Add(const T &data);

    bool Has(const T &data) const;

    bool Delete(const T &data);

private:
    struct HashTableNode {
        T Data;
        unsigned int Hash;
        HashTableNode *Next;

        HashTableNode() : Next(nullptr) {};
        // инициализация в списке инициализации в порядке объявлении класса
        HashTableNode(const T& data, HashTableNode* next, unsigned int hash) : Data(data), Next(next), Hash(hash) {};
    };
    H hasher;
    std::vector<HashTableNode*> table;
    int keysCount;

    void growTable();
};
template<class T, class H>
HashTable<T, H>::HashTable(const H& hasher) : hasher(hasher), keysCount(0) {};

template<class T, class H>
HashTable<T, H>::~HashTable() {
    for (int i = 0; i < table.size(); ++i) {
        HashTableNode* node = table[i];
        while(node != nullptr) {
            HashTableNode* nextNode = node->Next;
            delete node;
            node = nextNode;
        }
    }
}
template<class T, class H>
bool HashTable<T, H>::Add(const T& data) {
    if (keysCount > 3 * table.size()) {
        growTable();
    }
    unsigned int absoluteHash = hasher(data);
    unsigned int hash = absoluteHash % table.size();
    HashTableNode* node = table[hash];
    while (node != nullptr && node->Data != data) {
        node = node->Next;
    }
    if (node != nullptr) {
        return false;
    }
    table[hash] = new HashTableNode(data, table[hash], hash);
    ++keysCount;

    return true;
}
template<class T, class H>
void HashTable<T, H>::growTable() {
    std::vector<HashTableNode*> newTable(table.size() * 2);
    for (int i = 0; i < table.size(); ++i) {
        HashTableNode* node = table[i];
        while (node != nullptr) {
            HashTableNode* nextNode = node->Next;
            unsigned int newHash = node->Hash;
            node->Next = newTable[newHash]; // если уже положили в newHash, то добавляем в начало
            newTable[newHash] = node;
            node = nextNode;
        }
    }
    table = std::move(newTable);
}

template<class T, class H>
bool HashTable<T, H>::Has(const T& data) const {
    unsigned int hash = hasher(data);
    HashTableNode* node = table[hash];
    while (node != nullptr && node->Data != data) {
        node = node->Next;
    }
    return node != nullptr;
}
template<class T, class H>
bool HashTable<T, H>::Delete(const T &data) {
    unsigned int hash = hasher(data);
    HashTableNode* node = table[hash];
    HashTableNode* prevNode = nullptr;

    while (node != nullptr && node->Data != data) {
        prevNode = node;
        node = node->Next;
    }
    if (node == nullptr) { // ключа нет в таблице
        return false;
    }
    if (prevNode == nullptr) {
        table[hash] = node->Next;
    } else {
        prevNode->Next = node->Next;
    }

    delete node;

    --keysCount;

    return true;
}

int main() {
    char operation = 0;
    std::string data;
    Hasher hasher;
    HashTable<std::string, Hasher> table(hasher);
    while(std::cin >> operation >> data) {
        switch(operation) {
            case '+':
                std::cout << table.Add(data) ? "OK" : "FAIL";
                break;
            case '-':
                std::cout << table.Delete(data) ? "OK" : "FAIL";
                break;
            case '?':
                std::cout << table.Has(data) ? "OK" : "FAIL";
                break;
            default:
                assert(false);
        }
    }

    return 0;
}
