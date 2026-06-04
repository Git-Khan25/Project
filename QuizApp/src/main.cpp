// ============================================================
//  main.cpp  –  Entry point
//  Quiz App with Timer and Leaderboard
//  Vardhaman College of Engineering  |  CSE Summer Project E-06
// ============================================================
#include "QuizEngine.h"
#include "Leaderboard.h"
#include "Utils.h"
#include <iostream>

// Paths to data files (relative to where the binary is run)
const std::string QUESTIONS_FILE  = "data/questions.txt";
const std::string LEADERBOARD_FILE= "data/leaderboard.txt";
const std::string PROGRESS_FILE   = "data/progress.txt";

void showMainMenu() {
    using namespace Utils::Color;
    Utils::printBanner();
    std::cout << "  " << BOLD << "MAIN MENU\n" << RESET << "\n";
    std::cout << "    " << CYAN << BOLD << "1)" << RESET << "  Start Quiz\n";
    std::cout << "    " << CYAN << BOLD << "2)" << RESET << "  View Leaderboard\n";
    std::cout << "    " << CYAN << BOLD << "3)" << RESET << "  How to Play\n";
    std::cout << "    " << CYAN << BOLD << "4)" << RESET << "  Exit\n\n";
    std::cout << "  " << BOLD << "Choice: " << RESET;
}

void showHowToPlay() {
    using namespace Utils::Color;
    Utils::clearScreen();
    Utils::printLine('=');
    std::cout << BOLD << CYAN << "  HOW TO PLAY\n" << RESET;
    Utils::printLine('=');
    std::cout << "\n"
        << "  1. Enter your name when prompted.\n\n"
        << "  2. Choose one or more topic categories, or press\n"
        << "     ENTER to include all topics.\n\n"
        << "  3. Each question has a " << YELLOW << BOLD << "15-second countdown"
        << RESET << ". Type A, B, C or D\n"
        << "     and press ENTER before time runs out.\n\n"
        << "  4. Scoring:\n"
        << "       " << GREEN  << BOLD << "+4 pts" << RESET << "  for every correct answer\n"
        << "       " << RED    << BOLD << "-1 pt"  << RESET << "  for every wrong answer\n"
        << "       " << MAGENTA<< BOLD << " 0 pts" << RESET << "  if time expires\n\n"
        << "  5. After the quiz you will see a " << BOLD << "result card"
        << RESET << " with\n"
        << "     topic-wise accuracy bars.\n\n"
        << "  6. If you make the " << YELLOW << BOLD << "Top-10"
        << RESET << ", your score is saved\n"
        << "     to the persistent leaderboard.\n\n"
        << "  7. " << BOLD << "Adaptive AI:" << RESET
        << " The engine tracks your per-topic\n"
        << "     accuracy across sessions and gives you more\n"
        << "     questions from topics where you are weak.\n\n";
    Utils::printLine('=');
    Utils::pressEnter();
}

int main() {
    // Load leaderboard once at startup
    Leaderboard board(LEADERBOARD_FILE);
    board.load();

    // Create engine (questions loaded lazily on first quiz)
    QuizEngine engine(QUESTIONS_FILE, PROGRESS_FILE, board);

    // Pre-load questions and warn if file missing
    if (!engine.loadQuestions()) {
        std::cerr << "\n  [Fatal] Could not load questions from '"
                  << QUESTIONS_FILE << "'.\n"
                  << "  Make sure the 'data/' folder is in the same\n"
                  << "  directory as the executable.\n\n";
        return 1;
    }

    // ── Main menu loop ─────────────────────────────────────
    int choice = 0;
    do {
        showMainMenu();
        std::cin >> choice;

        switch (choice) {
            case 1:
                engine.run();
                // Reload questions and engine state is reset per run
                break;
            case 2:
                Utils::clearScreen();
                board.display();
                Utils::pressEnter();
                break;
            case 3:
                showHowToPlay();
                break;
            case 4:
                Utils::clearScreen();
                std::cout << "\n  Thanks for playing!  Goodbye. 👋\n\n";
                break;
            default:
                std::cout << "\n  Invalid choice. Please enter 1-4.\n";
                Utils::pressEnter();
        }
    } while (choice != 4);

    return 0;
}
