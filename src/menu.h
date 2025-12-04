#pragma once
#include <string>
#include <vector>

void write_file();
void process_encrypt_decrypt(bool encrypt_mode);
void view_file();

extern "C" {

bool encrypt(const std::vector<unsigned char>& input,
             std::vector<unsigned char>& output,
             const std::string& key);

bool decrypt(const std::vector<unsigned char>& input,
             std::vector<unsigned char>& output,
             const std::string& key);

const char* algorithm_name();

}
