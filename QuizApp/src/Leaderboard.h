#pragma once
// ============================================================
//  Leaderboard.h  –  Persistent top-10 high scores
//
//  File format (data/leaderboard.txt), one entry per line:
//      PlayerName|Score|Date
// ============================================================
#include <string>
#include <vector>

struct LeaderboardEntry {
    std::string name;
    int         score;
    std::string date;
};

class Leaderboard {
private:
    std::string                   filePath;
    std::vector<LeaderboardEntry> entries;   // sorted desc by score
    static const int MAX_ENTRIES = 10;

    void sortEntries();

public:
    explicit Leaderboard(const std::string& filePath);

    void load();
    void save() const;

    // Returns true if this score would enter the top-10
    bool isTopScore(int score) const;

    // Inserts entry if it qualifies. Returns 1-based rank (0 = not top-10)
    int addEntry(const std::string& name, int score, const std::string& date);

    void display() const;

    const std::vector<LeaderboardEntry>& getEntries() const;
};
