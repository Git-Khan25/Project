// ============================================================
//  Question.cpp
// ============================================================
#include "Question.h"
#include "Utils.h"
#include <iostream>

Question::Question(const std::string& topic,
                   const std::string& text,
                   const std::vector<std::string>& options,
                   int correct,
                   Difficulty diff)
    : topic(topic), text(text), options(options),
      correctOption(correct), difficulty(diff) {}

const std::string& Question::getTopic()        const { return topic; }
const std::string& Question::getText()         const { return text;  }
const std::vector<std::string>& Question::getOptions() const { return options; }
int        Question::getCorrectOption()        const { return correctOption; }
Difficulty Question::getDifficulty()           const { return difficulty; }

std::string Question::getDifficultyStr() const {
    switch (difficulty) {
        case Difficulty::EASY:   return "Easy";
        case Difficulty::MEDIUM: return "Medium";
        case Difficulty::HARD:   return "Hard";
        default:                 return "Unknown";
    }
}

void Question::display(int qNum, int total) const {
    using namespace Utils::Color;
    std::string diffCol =
        (difficulty == Difficulty::EASY)   ? GREEN  :
        (difficulty == Difficulty::MEDIUM) ? YELLOW : RED;

    std::cout << "\n";
    Utils::printLine('-');
    std::cout << BOLD << CYAN
              << "  Q" << qNum << " / " << total
              << "   [" << topic << "]"
              << "   Difficulty: " << diffCol << getDifficultyStr()
              << CYAN << RESET << "\n";
    Utils::printLine('-');
    std::cout << "\n  " << BOLD << text << RESET << "\n\n";

    const char letters[] = {'A','B','C','D'};
    for (int i = 0; i < static_cast<int>(options.size()); ++i) {
        std::cout << "    " << YELLOW << BOLD << letters[i] << ")" << RESET
                  << "  " << options[i] << "\n";
    }
    std::cout << "\n";
}
