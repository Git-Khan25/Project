#pragma once
// ============================================================
//  FileParser.h  –  Reads questions.txt; manages progress.txt
//
//  questions.txt format (one question per line, pipe-delimited):
//    TOPIC|Question text|OptionA|OptionB|OptionC|OptionD|CORRECT(1-4)|DIFF(1-3)
//
//  progress.txt format (one player per line):
//    PlayerName|TOPIC1|correct|total|TOPIC2|correct|total|...
// ============================================================
#include "Question.h"
#include "Player.h"
#include <vector>
#include <string>
#include <map>

class FileParser {
public:
    // Load all questions from file; returns count loaded
    static int loadQuestions(const std::string& filePath,
                             std::vector<Question>& out);

    // Progress file: load historical topic accuracy for a player
    // Returns map<topic, TopicStats>
    static std::map<std::string, TopicStats>
    loadProgress(const std::string& filePath,
                 const std::string& playerName);

    // Save/merge player's session stats into progress file
    static void saveProgress(const std::string& filePath,
                             const Player& player);
};
