//
// Created by flashie on 15.05.2021.
//
/*
 * Напишите две функции для создания архива из одного файла
 * и извлечения файла из архива.
 * Требования: коэффициент сжатия < 1.
 * В архиве сохраняйте дерево Хаффмана и код Хаффмана от исходных данных.
 * Дерево Хаффмана требуется хранить эффективно - не более 10 бит на каждый 8-битный символ.
 */
#include <iostream>
#include "Huffman.h"
#include <vector>
#include <map>
#include <set>
#include <algorithm>

class OutBitStream {
public:
    OutBitStream();

    void WriteBit(unsigned char bit);

    void WriteByte(unsigned char byte);

    const unsigned char *GetBuffer() const;

    int GetBitsCount() const;

private:
    std::vector<unsigned char> buffer;
    int bitsCount;
};

OutBitStream::OutBitStream() : bitsCount(0) {}

void OutBitStream::WriteBit(unsigned char bit) {
    if (bitsCount + 1 > buffer.size() * 8) {
        buffer.push_back(0);
    }
    if (bit != 0) {
        int bitPos = bitsCount % 8;
        buffer[bitsCount / 8] |= 1 << bitPos;
    }

    ++bitsCount;
}

void OutBitStream::WriteByte(unsigned char byte) {
    if (bitsCount % 8 == 0) {
        buffer.push_back(byte);
    } else {
        int bitPos = bitsCount % 8;
        buffer[bitsCount / 8] |= byte << bitPos;
        buffer.push_back(byte >> (8 - bitPos));
    }
    bitsCount += 8;
}

const unsigned char *OutBitStream::GetBuffer() const {
    return buffer.data();
}

int OutBitStream::GetBitsCount() const {
    return bitsCount;
}

class InBitStream {
public:
    InBitStream();

    void ReadBit(unsigned char bit);

    void ReadByte(unsigned char byte);

    const unsigned char *GetBuffer() const;

    int GetBitsCount() const;

private:
    std::vector<unsigned char> buffer;
    int bitsCount;
};

InBitStream::InBitStream() : bitsCount(0) {}

void InBitStream::ReadBit(unsigned char bit) {
    if (bitsCount + 1 > buffer.size() * 8) {
        buffer.push_back(0);
    }
    if (bit != 0) {
        int bitPos = bitsCount % 8;
        buffer[bitsCount / 8] |= 1 << bitPos;
    }

    ++bitsCount;
}

void InBitStream::ReadByte(unsigned char byte) {
    if (bitsCount % 8 == 0) {
        buffer.push_back(byte);
    } else {
        int bitPos = bitsCount % 8;
        buffer[bitsCount / 8] |= byte << bitPos;
        buffer.push_back(byte >> (8 - bitPos));
    }
}

const unsigned char *InBitStream::GetBuffer() const {
    return buffer.data();
}

int InBitStream::GetBitsCount() const {
    return bitsCount;
}

typedef unsigned char byte;
#define interface struct

interface IInputStream {
    // Возвращает false, если поток закончился
    virtual bool Read(byte &value) = 0;
};

interface IOutputStream {
    virtual void Write(byte value) = 0;
};

struct Node {
    unsigned char byte;
    Node *left = nullptr;
    Node *right = nullptr;
    int count;
};

struct Cmp {
    bool operator()(const Node *a, const Node *b) const {
        return a->count < b->count;
    }
};


void Encode(IInputStream &original, IOutputStream &compressed);

void Decode(IInputStream &compressed, IOutputStream &original);

static void copyStream(IInputStream &input, IOutputStream &output) {
    byte value;
    while (input.Read(value)) {
        output.Write(value);
    }
}

void get_table(Node *root, std::map<byte, int> &table, int code) {
    if (!root) {
        return;
    }
    if (!root->left && !root->right) {
        table[root->byte] = code;
        return;
    }
    int left_code = code << 1;
    int right_code = (code << 1) + 1;
    get_table(root->left, table, left_code);
    get_table(root->right, table, right_code);
}

void Encode(IInputStream &original, IOutputStream &compressed) {
    /*
     * Хранение:
     * 1. размер таблицы
     * 2. размер дерева
     * 3. Таблица
     * 4. Дерево
     */
    byte value; // cur
    std::map<byte, int> dict;
    while (original.Read(value)) {
        dict[value]++;
    }

    std::vector<Node *> nodes; // вектор нод - символ/количество
    for (const auto &iter : dict) {
        if (!iter.second) {
            continue;
        }
        Node *x = new Node;
        x->byte = iter.first;
        x->count = iter.second;
        nodes.push_back(x);
    }
    // сортировка по возрастанию
    std::sort(nodes.begin(), nodes.end(), Cmp());

    while (nodes.size() >= 2) {
        Node *parent = new Node;
        Node *left = *nodes.begin();
        Node *right = *(nodes.begin() + 1);
        nodes.erase(nodes.begin());
        nodes.erase(nodes.begin());

        parent->left = left;
        parent->right = right;
        parent->count = left->count + right->count;
//        parent->byte = left->byte;
        nodes.push_back(parent);
        std::sort(nodes.begin(), nodes.end(), Cmp());
    }

    Node *root = *nodes.begin();
    std::map<byte, int> table;
    int init = 1;
    get_table(root, table, init); // заполнили таблицу байт-код кодирования (начиная с 1, 10, 11 ...)

    int i = 0;
    byte s = 0;

    OutBitStream out;
    out.WriteByte(table.size());
    out.WriteByte(nodes.size());

    // запись таблицы
    for (auto &j : table) {
        out.WriteByte(table[j.first]);
        out.WriteByte(table[j.second]);
    }
    // Запись символ-код
    for (auto &j : )

    OutBitStream out;
    for (auto row : table) {
        table_data.push_back(row.first);
        table_data += to_bytes(row.second, 4);
    }

    compressed += to_bytes(table.size(), 4);
    compressed += to_bytes(encoded_bytes.size(), 4);
    compressed.push_back((_byte)(i % 8));
    compressed += table_data;
    compressed += encoded_bytes;
}

int compressed_pointer = 0;

int get_num(IInputStream &data, int _bytes_count) {
    int number = 0;
    for (int i = 0; i < _bytes_count; i++) {
        number <<= 8;
        number += (byte) data[compressed_pointer++];
    }
    return number;
}

_byte get_byte(chars &data) {
    return data[compressed_pointer++];
}

void Decode(IInputStream &compressed, IOutputStream &original) {
    OutBitStream input;
    byte value = 0;
    while (compressed.Read(value)) {
        input.WriteByte(value);
    }
    input.GetBuffer();
    int table_size = get_num(compressed, 4);
    int data_size = get_num(compressed, 4);
    byte remain = get_byte(compressed);
    std::map<int, byte> table;
    for (int i = 0; i < table_size; i++) {
        byte symbol = get_byte(compressed);
        bits code = get_num(compressed, 4);
        table[code] = symbol;
    }
    bits curr = 1;
    for (int i = 0; i < data_size; i++) {
        byte c = get_byte(compressed);
        for (int j = 0; j < 8; j++) {

            if (i == data_size - 1 && j == remain)
                break;
            curr <<= 1;
            curr += (c & (1 << (7 - j))) != 0;
            auto element = table.find(curr);
            if (element != table.end()) {
                original.push_back(element->second);
                curr = 1;
            }
        }
    }
}

void Encode(IInputStream &original, IOutputStream &compressed) {
    copyStream(original, compressed);
}

void Decode(IInputStream &compressed, IOutputStream &original) {
    copyStream(compressed, original);
}







