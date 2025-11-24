#pragma once
#include <vector>
#include <string>

extern "C" {

// функция шифрования
bool encrypt(const std::vector<unsigned char>& input,
             std::vector<unsigned char>& output,
             const std::string& key);

// функция дешифрования
bool decrypt(const std::vector<unsigned char>& input,
             std::vector<unsigned char>& output,
             const std::string& key);

// имя алгоритма (для вывода в меню)
const char* algorithm_name();

}
