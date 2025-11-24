#include "fixed_perm.h"
#include <iostream>

static std::vector<int> read_permutation(const std::string& filename) {
    std::ifstream f(filename);
    if (!f.is_open()) throw std::runtime_error("Ошибка открытия файла перестановки");
    std::vector<int> p;
    int val;
    while (f >> val) p.push_back(val);
    return p;
}

bool encrypt(const std::vector<unsigned char>& input,
             std::vector<unsigned char>& output,
             const std::string& key_file) {
    auto perm = read_permutation(key_file);
    int k = perm.size();
    output = input;
    if (output.size() % k != 0)
        output.insert(output.end(), k - (output.size() % k), ' ');

    for (size_t i = 0; i < output.size(); i += k) {
        std::vector<unsigned char> block(k);
        for (int j = 0; j < k; ++j)
            block[j] = output[i + perm[j] - 1];
        std::copy(block.begin(), block.end(), output.begin() + i);
    }
    return true;
}

bool decrypt(const std::vector<unsigned char>& input,
             std::vector<unsigned char>& output,
             const std::string& key_file) {
    auto perm = read_permutation(key_file);
    int k = perm.size();
    output = input;
    for (size_t i = 0; i < input.size(); i += k) {
        std::vector<unsigned char> block(k);
        for (int j = 0; j < k; ++j)
            block[perm[j] - 1] = input[i + j];
        std::copy(block.begin(), block.end(), output.begin() + i);
    }
    return true;
}

const char* algorithm_name() {
    return "Фиксированная перестановка";
}
