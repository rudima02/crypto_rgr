#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "file_io.h"

#ifdef _WIN32
    #include <windows.h>
    #define LIB_HANDLE HMODULE
    #define LOAD_LIB(name) LoadLibraryA(name)
    #define LOAD_FUNC GetProcAddress
    #define CLOSE_LIB FreeLibrary
    #define LIB_EXT ".dll"
#else
    #include <dlfcn.h>
    #define LIB_HANDLE void*
    #define LOAD_LIB(name) dlopen(name, RTLD_LAZY)
    #define LOAD_FUNC dlsym
    #define CLOSE_LIB dlclose
    #define LIB_EXT ".so"
#endif

typedef bool (*EncryptFunc)(const std::vector<unsigned char>&, std::vector<unsigned char>&, const std::string&);
typedef const char* (*NameFunc)();

void write_file() {
    std::string filename;
    std::cout << "Введите название файла для записи: ";
    std::cin >> filename;

    std::cin.ignore();
    std::string text;
    std::cout << "Введите текст для записи: ";
    std::getline(std::cin, text);

    try {
        std::ofstream out(filename, std::ios::binary);
        if (!out.is_open()) throw std::runtime_error("Не удалось открыть файл для записи");
        out.write(text.c_str(), text.size());
        std::cout << "Текст успешно записан в " << filename << "\n";
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }
}

void process_encrypt_decrypt(bool encrypt_mode) {
    std::cout << "Выберите алгоритм:\n"
              << "1. Фиксированная перестановка\n"
              << "2. Табличная перестановка с ключом\n"
              << "3. Магический квадрат\n";

    int choice;
    while (!(std::cin >> choice) || choice < 1 || choice > 3) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Неверный ввод! Введите число от 1 до 3: ";
    }

    std::string libname;

    if (choice == 1)
        libname = "libfixed_perm" LIB_EXT;
    else if (choice == 2)
        libname = "libkeyword_table" LIB_EXT;
    else
        libname = "libmagic_square" LIB_EXT;

    LIB_HANDLE handle = LOAD_LIB(libname.c_str());

    if (!handle) {
#ifdef _WIN32
        std::cerr << "Ошибка загрузки DLL: " << libname << "\n";
#else
        std::cerr << "Ошибка загрузки SO: " << libname << "\n";
        std::cerr << "Причина: " << dlerror() << "\n";
#endif
        return;
    }

    auto encrypt = (EncryptFunc)LOAD_FUNC(handle, "encrypt");
    auto decrypt = (EncryptFunc)LOAD_FUNC(handle, "decrypt");
    auto name = (NameFunc)LOAD_FUNC(handle, "algorithm_name");

    if (!encrypt || !decrypt || !name) {
        std::cerr << "Не найдены экспортированные функции.\n";
        CLOSE_LIB(handle);
        return;
    }

    std::string file_in, file_out, key_file;
    std::cout << "Файл для обработки: ";
    std::cin >> file_in;
    std::cout << "Файл для результата: ";
    std::cin >> file_out;

    std::cout << "Введите путь к файлу ключа: ";
    std::cin >> key_file;

    try {
        auto input = read_binary(file_in);
        std::vector<unsigned char> output;
        bool ok = encrypt_mode ? encrypt(input, output, key_file)
                               : decrypt(input, output, key_file);

        if (ok) {
            write_binary(file_out, output);
            std::cout << (encrypt_mode ? "Файл зашифрован: "
                                       : "Файл расшифрован: ")
                      << file_out << "\n";
        } else {
            std::cerr << "Ошибка обработки данных.\n";
        }

    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    CLOSE_LIB(handle);

    std::cout << "Нажмите Enter чтобы продолжить...";
    std::cin.ignore();
    std::cin.get();
}

void view_file() {
    std::cout << "Введите название файла для просмотра: ";
    std::string filename;
    std::cin >> filename;

    std::cout << "Выберите способ просмотра:\n1. Как текст\n2. В hex\n";
    int mode;
    while (!(std::cin >> mode) || (mode != 1 && mode != 2)) {
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Неверный ввод! Введите 1 или 2: ";
    }

    try {
        auto data = read_binary(filename);
        if (mode == 1) {
            for (auto c : data) std::cout << c;
            std::cout << "\n";
        } else {
            for (size_t i = 0; i < data.size(); ++i) {
                printf("%02X ", data[i]);
                if ((i + 1) % 16 == 0) std::cout << "\n";
            }
            std::cout << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
    }

    std::cout << "Нажмите Enter чтобы продолжить";
    std::cin.ignore();
    std::cin.get();
}
