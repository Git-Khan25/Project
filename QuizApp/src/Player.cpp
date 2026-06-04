// ============================================================
//  Player.cpp
// ============================================================
#include "Player.h"

Player::Player(const std::string& name)
    : name(name), score(0), correctCount(0),
      wrongCount(0), timeoutCount(0) {}

void Player::addCorrect(const std::string& topic, int points) {
    score += points;
    ++correctCount;
    topicStats[topic].correct++;
    topicStats[topic].total++;
}

void Player::addWrong(const std::string& topic, int penalty) {
    score -= penalty;
    if (score < 0) score = 0;   // floor at zero
    ++wrongCount;
    topicStats[topic].total++;
}

void Player::addTimeout(const std::string& topic) {
    ++timeoutCount;
    topicStats[topic].total++;
}

const std::string& Player::getName()         const { return name;         }
int  Player::getScore()                      const { return score;        }
int  Player::getCorrectCount()               const { return correctCount; }
int  Player::getWrongCount()                 const { return wrongCount;   }
int  Player::getTimeoutCount()               const { return timeoutCount; }
int  Player::getTotalAttempted()             const {
    return correctCount + wrongCount + timeoutCount;
}
double Player::getOverallAccuracy() const {
    int att = getTotalAttempted();
    return (att == 0) ? 0.0
         : static_cast<double>(correctCount) / att * 100.0;
}
const std::map<std::string, TopicStats>& Player::getTopicStats() const {
    return topicStats;
}
