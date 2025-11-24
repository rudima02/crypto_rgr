#pragma once
#include <vector>
#include <string>

std::vector<unsigned char> read_binary(const std::string& path);
void write_binary(const std::string& path, const std::vector<unsigned char>& data);
