#include <iostream>
#include <dlfcn.h>
#include "file_io.h"
#include "encrypt_interface.h"

typedef bool (*EncryptFunc)(const std::vector<unsigned char>&, std::vector<unsigned char>&, const std::string&);
typedef const char* (*NameFunc)();

int main() {
    std::cout << "Выберите алгоритм:\n"
              << "1. Фиксированная перестановка\n"
              << "2. Табличная перестановка с ключом\n"
              << "3. Магический квадрат\n";

    int choice;
    std::cin >> choice;

    std::string so_path;
    if (choice == 1) so_path = "./libfixed_perm.so";
    else if (choice == 2) so_path = "./libkeyword_table.so";
    else if (choice == 3) so_path = "./libmagic_square.so";
    else { std::cerr << "Неверный выбор.\n"; return 1; }

    void* handle = dlopen(so_path.c_str(), RTLD_LAZY);
    if (!handle) {
        std::cerr << "Ошибка загрузки библиотеки: " << dlerror() << '\n';
        return 1;
    }

    auto encrypt = (EncryptFunc)dlsym(handle, "encrypt");
    auto decrypt = (EncryptFunc)dlsym(handle, "decrypt");
    auto name = (NameFunc)dlsym(handle, "algorithm_name");

    std::string mode;
    std::cout << "Режим (e - шифр / d - дешифр): ";
    std::cin >> mode;

    std::string in_path, out_path, key;
    std::cout << "Файл ввода: "; std::cin >> in_path;
    std::cout << "Файл вывода: "; std::cin >> out_path;
    std::cout << "Ключ или путь к файлу данных (если нужен): ";
    std::cin >> key;

    try {
        auto input = read_binary(in_path);
        std::vector<unsigned char> output;

        bool ok = false;
        if (mode == "e") ok = encrypt(input, output, key);
        else if (mode == "d") ok = decrypt(input, output, key);
        else { std::cerr << "Неизвестный режим!\n"; return 1; }

        if (ok) {
            write_binary(out_path, output);
            std::cout << "Операция завершена успешно.\n";
        } else {
            std::cerr << "Ошибка при обработке данных.\n";
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    dlclose(handle);
    return 0;
}
