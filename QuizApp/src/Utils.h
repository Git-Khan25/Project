#pragma once
// ============================================================
//  Utils.h  –  ANSI colours, string helpers, screen utils
// ============================================================
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <limits>
#include <ctime>
#include <tuple>

namespace Utils {

    // ── ANSI colour codes ──────────────────────────────────
    namespace Color {
        const std::string RESET   = "\033[0m";
        const std::string BOLD    = "\033[1m";
        const std::string DIM     = "\033[2m";
        const std::string RED     = "\033[31m";
        const std::string GREEN   = "\033[32m";
        const std::string YELLOW  = "\033[33m";
        const std::string BLUE    = "\033[34m";
        const std::string MAGENTA = "\033[35m";
        const std::string CYAN    = "\033[36m";
        const std::string WHITE   = "\033[37m";
        const std::string BG_RED  = "\033[41m";
        const std::string BG_GRN  = "\033[42m";
    }

    inline void clearScreen() {
#ifdef _WIN32
        std::ignore = system("cls");
#else
        std::ignore = system("clear");
#endif
    }

    inline void printLine(char c = '=', int len = 62) {
        std::cout << std::string(len, c) << "\n";
    }

    inline void pressEnter() {
        std::cout << "\n  " << Color::DIM
                  << "[ Press ENTER to continue... ]" << Color::RESET;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();
    }

    inline std::string trim(const std::string& s) {
        size_t a = s.find_first_not_of(" \t\r\n");
        size_t b = s.find_last_not_of(" \t\r\n");
        return (a == std::string::npos) ? "" : s.substr(a, b - a + 1);
    }

    inline std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> toks;
        std::istringstream ss(s);
        std::string tok;
        while (std::getline(ss, tok, delim))
            toks.push_back(trim(tok));
        return toks;
    }

    inline std::string toUpper(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        return s;
    }

    inline std::string getCurrentDate() {
        time_t now = time(nullptr);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
        return std::string(buf);
    }

   inline void printBanner() {
    using namespace Color;
    clearScreen();
    printLine('*');
    std::cout << "\n";
    std::cout << BOLD << CYAN
        << "                  Q U I Z   A P P\n"
        << RESET;
    std::cout << "\n";
    printLine('*');
    std::cout << BOLD << MAGENTA
        << "    Race the Clock  |  Beat the Board  |  Prove Your Skills\n"
        << RESET;
    printLine('=');
    std::cout << DIM
        << "  Vardhaman College of Engineering\n"
        << RESET;
    printLine('-');
    std::cout << "\n";
 }
}
