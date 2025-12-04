#include "fixed_perm.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>

std::vector<int> read_permutation(const std::string& filename) {
    std::ifstream f(filename);
    if (!f.is_open()) throw std::runtime_error("Ошибка открытия файла перестановки");

    std::vector<int> perm;
    int val;
    while (f >> val) perm.push_back(val);

    if (perm.empty()) throw std::runtime_error("Перестановка пуста!");

    std::vector<int> sorted_perm = perm;
    std::sort(sorted_perm.begin(), sorted_perm.end());
    for (size_t i = 0; i < sorted_perm.size(); ++i) {
        if (sorted_perm[i] != i + 1) throw std::runtime_error("Неверная перестановка: пропущено или повторено число");
    }

    return perm;
}

bool encrypt(const std::vector<unsigned char>& input,
             std::vector<unsigned char>& output,
             const std::string& key_file)
{
    auto perm = read_permutation(key_file);
    size_t k = perm.size();

    if (input.empty()) return false;
    if (k == 0) throw std::runtime_error("Перестановка пустая");

    output.clear();
    output.reserve(input.size() + k); 

    std::vector<unsigned char> tmp = input;
    size_t rest = tmp.size() % k;
    if (rest != 0) {
        tmp.insert(tmp.end(), k - rest, ' ');  
    }

    for (size_t i = 0; i < tmp.size(); i += k) {
        for (size_t j = 0; j < k; ++j) {
            output.push_back(tmp[i + perm[j] - 1]);
        }
    }

    return true;
}

bool decrypt(const std::vector<unsigned char>& input,
             std::vector<unsigned char>& output,
             const std::string& key_file)
{
    auto perm = read_permutation(key_file);
    size_t k = perm.size();

    if (input.empty()) return false;
    if (k == 0) throw std::runtime_error("Перестановка пустая");

    output.clear();
    output.reserve(input.size());

    for (size_t i = 0; i < input.size(); i += k) {
        std::vector<unsigned char> block(k);
        for (size_t j = 0; j < k; ++j) {
            block[perm[j] - 1] = input[i + j];
        }
        output.insert(output.end(), block.begin(), block.end());
    }

    return true;
}

const char* algorithm_name() {
    return "Фиксированная перестановка";
}
