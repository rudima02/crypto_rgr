#pragma once
#include <vector>
#include <string>

extern "C" {
bool encrypt(const std::vector<unsigned char>& input,
             std::vector<unsigned char>& output,
             const std::string& key_file);

bool decrypt(const std::vector<unsigned char>& input,
             std::vector<unsigned char>& output,
             const std::string& key_file);

const char* algorithm_name();
}