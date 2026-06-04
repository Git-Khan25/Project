#pragma once
// ============================================================
//  Question.h  –  One MCQ question with all its metadata
// ============================================================
#include <string>
#include <vector>

enum class Difficulty { EASY = 1, MEDIUM = 2, HARD = 3 };

class Question {
private:
    std::string              topic;
    std::string              text;
    std::vector<std::string> options;        // exactly 4 choices
    int                      correctOption;  // 1-based (1=A…4=D)
    Difficulty               difficulty;

public:
    Question(const std::string& topic,
             const std::string& text,
             const std::vector<std::string>& options,
             int correct,
             Difficulty diff);

    const std::string&              getTopic()         const;
    const std::string&              getText()          const;
    const std::vector<std::string>& getOptions()       const;
    int                             getCorrectOption() const;
    Difficulty                      getDifficulty()    const;
    std::string                     getDifficultyStr() const;

    void display(int qNum, int total) const;
};
