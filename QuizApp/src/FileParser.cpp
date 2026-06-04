// ============================================================
//  FileParser.cpp
// ============================================================
#include "FileParser.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <sstream>

// ── Load questions ──────────────────────────────────────────
int FileParser::loadQuestions(const std::string& filePath,
                               std::vector<Question>& out) {
    std::ifstream fin(filePath);
    if (!fin.is_open()) {
        std::cerr << "\n  [Error] Cannot open questions file: " << filePath << "\n";
        return 0;
    }

    int count = 0;
    std::string line;
    int lineNo = 0;

    while (std::getline(fin, line)) {
        ++lineNo;
        line = Utils::trim(line);
        if (line.empty() || line[0] == '#') continue;  // skip comments/blanks

        auto parts = Utils::split(line, '|');
        if (parts.size() < 8) {
            std::cerr << "  [Warning] Line " << lineNo
                      << ": expected 8 fields, got " << parts.size()
                      << " — skipping.\n";
            continue;
        }

        std::string topic = parts[0];
        std::string text  = parts[1];
        std::vector<std::string> opts = {parts[2], parts[3],
                                         parts[4], parts[5]};
        int correct = std::stoi(parts[6]);   // 1-4
        int diff    = std::stoi(parts[7]);   // 1-3

        if (correct < 1 || correct > 4 || diff < 1 || diff > 3) {
            std::cerr << "  [Warning] Line " << lineNo
                      << ": invalid correct/diff value — skipping.\n";
            continue;
        }

        Difficulty d = (diff == 1) ? Difficulty::EASY :
                       (diff == 2) ? Difficulty::MEDIUM : Difficulty::HARD;

        out.emplace_back(topic, text, opts, correct, d);
        ++count;
    }
    return count;
}

// ── Load progress ───────────────────────────────────────────
std::map<std::string, TopicStats>
FileParser::loadProgress(const std::string& filePath,
                          const std::string& playerName) {
    std::map<std::string, TopicStats> result;
    std::ifstream fin(filePath);
    if (!fin.is_open()) return result;

    std::string line;
    while (std::getline(fin, line)) {
        line = Utils::trim(line);
        if (line.empty()) continue;
        auto parts = Utils::split(line, '|');
        if (parts.empty()) continue;
        if (parts[0] != playerName) continue;

        // Format: Name|Topic1|correct|total|Topic2|correct|total|...
        for (size_t i = 1; i + 2 < parts.size(); i += 3) {
            TopicStats ts;
            ts.correct = std::stoi(parts[i + 1]);
            ts.total   = std::stoi(parts[i + 2]);
            result[parts[i]] = ts;
        }
        break;
    }
    return result;
}

// ── Save progress ───────────────────────────────────────────
void FileParser::saveProgress(const std::string& filePath,
                               const Player& player) {
    // Read all existing lines
    std::vector<std::string> allLines;
    std::ifstream fin(filePath);
    if (fin.is_open()) {
        std::string line;
        while (std::getline(fin, line))
            if (!line.empty()) allLines.push_back(line);
        fin.close();
    }

    // Load existing progress for this player (to merge)
    auto existing = loadProgress(filePath, player.getName());

    // Merge session stats into existing
    for (const auto& kv : player.getTopicStats()) {
        existing[kv.first].correct += kv.second.correct;
        existing[kv.first].total   += kv.second.total;
    }

    // Build new line for this player
    std::ostringstream oss;
    oss << player.getName();
    for (const auto& kv : existing)
        oss << "|" << kv.first << "|" << kv.second.correct
            << "|" << kv.second.total;
    std::string newLine = oss.str();

    // Replace old line or append
    bool found = false;
    for (auto& l : allLines) {
        auto parts = Utils::split(l, '|');
        if (!parts.empty() && parts[0] == player.getName()) {
            l = newLine;
            found = true;
            break;
        }
    }
    if (!found) allLines.push_back(newLine);

    // Write back
    std::ofstream fout(filePath);
    if (!fout.is_open()) {
        std::cerr << "  [Warning] Could not save progress to " << filePath << "\n";
        return;
    }
    for (const auto& l : allLines)
        fout << l << "\n";
}
