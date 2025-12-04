#include "keyword_table.h"
#include <fstream>
#include <algorithm>
#include <stdexcept>

std::string read_key(const std::string& filename) {
    std::ifstream f(filename);
    if (!f.is_open()) throw std::runtime_error("Ошибка открытия файла ключа");

    std::string key;
    f >> key;
    if (key.empty())
        throw std::runtime_error("Ключ пустой!");
    return key;
}

std::vector<int> key_order(const std::string& key) {
    std::vector<std::pair<char,int>> v;
    v.reserve(key.size());
    for (int i = 0; i < (int)key.size(); ++i)
        v.push_back({key[i], i});

    std::sort(v.begin(), v.end(),
              [](auto& a, auto& b) {
                  if (a.first == b.first) return a.second < b.second;
                  return a.first < b.first;
              });

    std::vector<int> order(key.size());
    for (int new_pos = 0; new_pos < (int)v.size(); ++new_pos) {
        int old_pos = v[new_pos].second;
        order[old_pos] = new_pos;
    }

    return order;
}

bool encrypt(const std::vector<unsigned char>& input,
             std::vector<unsigned char>& output,
             const std::string& key_file)
{
    std::string key = read_key(key_file);
    auto order = key_order(key);

    size_t cols = key.size();
    if (cols == 0) return false;

    std::vector<unsigned char> padded = input;
    size_t rem = padded.size() % cols;
    if (rem != 0)
        padded.insert(padded.end(), cols - rem, ' '); 

    size_t rows = padded.size() / cols;
    std::vector<std::vector<unsigned char>> table(rows, std::vector<unsigned char>(cols));

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            table[i][j] = padded[i * cols + j];

    std::vector<std::vector<unsigned char>> perm(rows, std::vector<unsigned char>(cols));
    for (size_t old_j = 0; old_j < cols; ++old_j) {
        size_t new_j = order[old_j];
        for (size_t i = 0; i < rows; ++i)
            perm[i][new_j] = table[i][old_j];
    }

    output.clear();
    output.reserve(rows * cols);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            output.push_back(perm[i][j]);

    return true;
}

bool decrypt(const std::vector<unsigned char>& input,
             std::vector<unsigned char>& output,
             const std::string& key_file)
{
    std::string key = read_key(key_file);
    auto order = key_order(key);

    size_t cols = key.size();
    if (cols == 0) return false;

    size_t rows = input.size() / cols;
    std::vector<std::vector<unsigned char>> table(rows, std::vector<unsigned char>(cols));

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            table[i][j] = input[i * cols + j];

    std::vector<int> inverse(cols);
    for (size_t old_j = 0; old_j < cols; ++old_j)
        inverse[order[old_j]] = old_j;

    std::vector<std::vector<unsigned char>> restored(rows, std::vector<unsigned char>(cols));
    for (size_t new_j = 0; new_j < cols; ++new_j) {
        size_t old_j = inverse[new_j];
        for (size_t i = 0; i < rows; ++i)
            restored[i][old_j] = table[i][new_j];
    }

    output.clear();
    output.reserve(rows * cols);
    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            output.push_back(restored[i][j]);

    while (!output.empty() && output.back() == ' ')
        output.pop_back();

    return true;
}

const char* algorithm_name() {
    return "Табличная перестановка с ключом";
}
