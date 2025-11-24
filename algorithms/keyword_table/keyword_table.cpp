#include "keyword_table.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <vector>

// читаем ключ из файла
static std::string read_key(const std::string& filename) {
    std::ifstream f(filename);
    if (!f.is_open()) throw std::runtime_error("Ошибка открытия файла ключа");
    std::string key;
    f >> key; // ключ — одно слово
    return key;
}

// получение индексов перестановки из ключа
static std::vector<int> key_order(const std::string& key) {
    std::vector<std::pair<char, int>> tmp;
    for (int i = 0; i < key.size(); ++i)
        tmp.push_back({key[i], i});
    std::sort(tmp.begin(), tmp.end());

    std::vector<int> order(key.size());
    for (int i = 0; i < tmp.size(); ++i)
        order[tmp[i].second] = i;
    return order;
}

bool encrypt(const std::vector<unsigned char>& input,
             std::vector<unsigned char>& output,
             const std::string& key_file) {
    std::string key = read_key(key_file);
    std::vector<int> order = key_order(key);
    int cols = key.size();

    output = input;
    if (output.size() % cols != 0)
        output.insert(output.end(), cols - (output.size() % cols), ' ');

    for (size_t i = 0; i < output.size(); i += cols) {
        std::vector<unsigned char> block(cols);
        for (int j = 0; j < cols; ++j)
            block[j] = output[i + order[j]];
        std::copy(block.begin(), block.end(), output.begin() + i);
    }
    return true;
}

bool decrypt(const std::vector<unsigned char>& input,
             std::vector<unsigned char>& output,
             const std::string& key_file) {
    std::string key = read_key(key_file);
    std::vector<int> order = key_order(key);
    int cols = key.size();

    output = input;
    for (size_t i = 0; i < input.size(); i += cols) {
        std::vector<unsigned char> block(cols);
        for (int j = 0; j < cols; ++j)
            block[order[j]] = input[i + j];
        std::copy(block.begin(), block.end(), output.begin() + i);
    }
    return true;
}

const char* algorithm_name() {
    return "Табличная перестановка с ключом";
}
