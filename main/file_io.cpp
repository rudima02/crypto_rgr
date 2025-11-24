#include "file_io.h"
#include <fstream>
#include <stdexcept>

std::vector<unsigned char> read_binary(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in.is_open())
        throw std::runtime_error("Ошибка открытия файла: " + path);
    return std::vector<unsigned char>(
        std::istreambuf_iterator<char>(in),
        std::istreambuf_iterator<char>());
}

void write_binary(const std::string& path, const std::vector<unsigned char>& data) {
    std::ofstream out(path, std::ios::binary);
    if (!out.is_open())
        throw std::runtime_error("Ошибка записи в файл: " + path);
    out.write(reinterpret_cast<const char*>(data.data()), data.size());
}
