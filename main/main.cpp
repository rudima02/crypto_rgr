#include "menu.h"
#include "locale"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
#ifdef _WIN32
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
#endif
    while (true) {
        std::cout << "\nГлавное меню\n"
                  << "1. Ввод текста в файл\n"
                  << "2. Шифрование\n"
                  << "3. Дешифрование\n"
                  << "4. Просмотр файла\n"
                  << "0. Выход\n"
                  << "Выберите пункт: ";

        int option;
        if (!(std::cin >> option)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Неверный ввод! Введите число.\n";
            continue;
        }

        switch (option) {
            case 1: write_file(); break;
            case 2: process_encrypt_decrypt(true); break;
            case 3: process_encrypt_decrypt(false); break;
            case 4: view_file(); break;
            case 0: return 0;
            default:
                std::cout << "Неверный пункт меню. Введите число от 0 до 4.\n";
        }
    }
}
