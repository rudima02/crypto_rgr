#include "magic_square.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

std::vector<int> read_square(const std::string& filename, int& n) {
    std::ifstream f(filename);
    if (!f.is_open()) throw std::runtime_error("Ошибка открытия файла магического квадрата");

    std::vector<int> square;
    std::string line;
    n = 0;
    while (std::getline(f, line)) {
        std::stringstream ss(line);
        int val;
        int cnt = 0;
        while (ss >> val) {
            square.push_back(val);
            cnt++;
        }
        if (n == 0) n = cnt; 
    }
    if (square.size() != n * n) throw std::runtime_error("Неверный формат квадрата");
    return square;
}

bool encrypt(const std::vector<unsigned char>& input,
             std::vector<unsigned char>& output,
             const std::string& key_file) {
    int n;
    std::vector<int> square = read_square(key_file, n);

    output = input;
    if (output.size() % (n*n) != 0)
        output.insert(output.end(), n*n - (output.size() % (n*n)), ' ');

    for (size_t i = 0; i < output.size(); i += n*n) {
        std::vector<unsigned char> block(n*n);
        for (int j = 0; j < n*n; ++j)
            block[j] = output[i + square[j]-1]; 
        std::copy(block.begin(), block.end(), output.begin() + i);
    }
    return true;
}

bool decrypt(const std::vector<unsigned char>& input,
             std::vector<unsigned char>& output,
             const std::string& key_file) {
    int n;
    std::vector<int> square = read_square(key_file, n);

    output = input;
    for (size_t i = 0; i < input.size(); i += n*n) {
        std::vector<unsigned char> block(n*n);
        for (int j = 0; j < n*n; ++j)
            block[square[j]-1] = input[i+j]; 
        std::copy(block.begin(), block.end(), output.begin() + i);
    }

    while (!output.empty() && output.back() == ' ')
        output.pop_back();

    return true;
}

const char* algorithm_name() {
    return "Магический квадрат";
}
