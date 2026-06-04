#pragma once
// ============================================================
//  QuizEngine.h  –  Runs the quiz: question selection,
//                   timed input, scoring, result card
// ============================================================
#include "Question.h"
#include "Player.h"
#include "Leaderboard.h"
#include <vector>
#include <string>
#include <map>

// Scoring constants
static const int POINTS_CORRECT =  4;
static const int POINTS_PENALTY =  1;   // deducted on wrong answer
static const int TIMER_SECONDS  = 15;   // seconds per question
static const int QUESTIONS_PER_QUIZ = 10;

class QuizEngine {
private:
    std::vector<Question> allQuestions;
    std::string           questionsPath;
    std::string           progressPath;
    Leaderboard&          leaderboard;

    // ── Internal helpers ────────────────────────────────────
    std::vector<std::string> getAvailableTopics() const;

    // Select questions using adaptive weighting from progress file
    std::vector<Question> selectQuestions(
        const std::vector<std::string>& chosenTopics,
        const std::string& playerName,
        int count) const;

    // Display one question, collect timed answer, return selected char (0 = timeout)
    char askQuestion(const Question& q, int qNum, int total,
                     bool& timedOut);

    void showFeedback(const Question& q, char answer, bool timedOut,
                      int scoreChange) const;

    void showResultCard(const Player& player) const;

public:
    QuizEngine(const std::string& questionsPath,
               const std::string& progressPath,
               Leaderboard& board);

    bool loadQuestions();

    // Main entry point — full quiz round for one player
    void run();
};
