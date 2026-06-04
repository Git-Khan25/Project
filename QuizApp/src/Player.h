#pragma once
// ============================================================
//  Player.h  –  Per-session score tracker + topic accuracy
// ============================================================
#include <string>
#include <map>

struct TopicStats {
    int correct = 0;
    int total   = 0;
    double accuracy() const {
        return (total == 0) ? 0.0
             : static_cast<double>(correct) / total * 100.0;
    }
};

class Player {
private:
    std::string                       name;
    int                               score;
    int                               correctCount;
    int                               wrongCount;
    int                               timeoutCount;
    std::map<std::string, TopicStats> topicStats;

public:
    explicit Player(const std::string& name);

    void addCorrect (const std::string& topic, int points);
    void addWrong   (const std::string& topic, int penalty);
    void addTimeout (const std::string& topic);

    const std::string& getName()          const;
    int    getScore()                     const;
    int    getCorrectCount()              const;
    int    getWrongCount()                const;
    int    getTimeoutCount()              const;
    int    getTotalAttempted()            const;
    double getOverallAccuracy()           const;

    const std::map<std::string, TopicStats>& getTopicStats() const;
};
