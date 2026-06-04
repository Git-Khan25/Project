// ============================================================
//  Leaderboard.cpp
// ============================================================
#include "Leaderboard.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iomanip>

Leaderboard::Leaderboard(const std::string& filePath)
    : filePath(filePath) {}

void Leaderboard::sortEntries() {
    std::sort(entries.begin(), entries.end(),
              [](const LeaderboardEntry& a, const LeaderboardEntry& b) {
                  return a.score > b.score;
              });
}

void Leaderboard::load() {
    entries.clear();
    std::ifstream fin(filePath);
    if (!fin.is_open()) return;   // file doesn't exist yet — that's fine

    std::string line;
    while (std::getline(fin, line)) {
        auto parts = Utils::split(line, '|');
        if (parts.size() < 3) continue;
        LeaderboardEntry e;
        e.name  = parts[0];
        e.score = std::stoi(parts[1]);
        e.date  = parts[2];
        entries.push_back(e);
    }
    sortEntries();
}

void Leaderboard::save() const {
    std::ofstream fout(filePath);
    if (!fout.is_open()) {
        std::cerr << "  [Warning] Could not save leaderboard to " << filePath << "\n";
        return;
    }
    for (const auto& e : entries)
        fout << e.name << "|" << e.score << "|" << e.date << "\n";
}

bool Leaderboard::isTopScore(int score) const {
    if (static_cast<int>(entries.size()) < MAX_ENTRIES) return true;
    return score > entries.back().score;
}

int Leaderboard::addEntry(const std::string& name, int score,
                          const std::string& date) {
    if (!isTopScore(score)) return 0;

    LeaderboardEntry e{name, score, date};
    entries.push_back(e);
    sortEntries();
    if (static_cast<int>(entries.size()) > MAX_ENTRIES)
        entries.resize(MAX_ENTRIES);

    // Find rank
    for (int i = 0; i < static_cast<int>(entries.size()); ++i)
        if (entries[i].name == name && entries[i].score == score)
            return i + 1;
    return 0;
}

void Leaderboard::display() const {
    using namespace Utils::Color;
    std::cout << "\n";
    Utils::printLine('=');
    std::cout << BOLD << YELLOW
              << "              🏆  TOP-10 LEADERBOARD  🏆\n"
              << RESET;
    Utils::printLine('=');

    if (entries.empty()) {
        std::cout << "\n  " << DIM << "No scores yet. Be the first!\n" << RESET << "\n";
        Utils::printLine('=');
        return;
    }

    std::cout << BOLD
              << std::left
              << "  " << std::setw(5)  << "RANK"
              << std::setw(20) << "PLAYER"
              << std::setw(8)  << "SCORE"
              << std::setw(14) << "DATE"
              << "\n" << RESET;
    Utils::printLine('-');

    const std::string medals[] = {"🥇","🥈","🥉"};

    for (int i = 0; i < static_cast<int>(entries.size()); ++i) {
        std::string rankStr = (i < 3) ? medals[i]
                                      : ("  #" + std::to_string(i + 1));
        std::string col = (i == 0) ? YELLOW :
                          (i == 1) ? WHITE  :
                          (i == 2) ? YELLOW : RESET;

        std::cout << col
                  << "  " << std::setw(5)  << rankStr
                  << std::setw(20) << entries[i].name
                  << std::setw(8)  << entries[i].score
                  << std::setw(14) << entries[i].date
                  << RESET << "\n";
    }
    Utils::printLine('=');
    std::cout << "\n";
}

const std::vector<LeaderboardEntry>& Leaderboard::getEntries() const {
    return entries;
}
