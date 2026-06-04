// ============================================================
//  QuizEngine.cpp
// ============================================================
#include "QuizEngine.h"
#include "FileParser.h"
#include "Timer.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <future>
#include <set>
#include <chrono>

// ── Constructor ─────────────────────────────────────────────
QuizEngine::QuizEngine(const std::string& qPath,
                       const std::string& pPath,
                       Leaderboard& board)
    : questionsPath(qPath), progressPath(pPath), leaderboard(board) {}

bool QuizEngine::loadQuestions() {
    int n = FileParser::loadQuestions(questionsPath, allQuestions);
    return n > 0;
}

// ── Get unique topic list ───────────────────────────────────
std::vector<std::string> QuizEngine::getAvailableTopics() const {
    std::set<std::string> seen;
    std::vector<std::string> topics;
    for (const auto& q : allQuestions) {
        if (seen.insert(q.getTopic()).second)
            topics.push_back(q.getTopic());
    }
    std::sort(topics.begin(), topics.end());
    return topics;
}

// ── Adaptive question selection ─────────────────────────────
//
//  AI FEATURE: Weighted random selection
//  weight(topic) = 1.0 - (historical accuracy / 100.0)
//  Topics with lower accuracy get higher probability.
// ───────────────────────────────────────────────────────────
std::vector<Question> QuizEngine::selectQuestions(
        const std::vector<std::string>& chosenTopics,
        const std::string& playerName,
        int count) const {

    // Load historical progress
    auto progress = FileParser::loadProgress(progressPath, playerName);

    // Build pool per topic
    std::map<std::string, std::vector<const Question*>> pool;
    for (const auto& q : allQuestions)
        if (std::find(chosenTopics.begin(), chosenTopics.end(),
                      q.getTopic()) != chosenTopics.end())
            pool[q.getTopic()].push_back(&q);

    // Compute weights (1 - accuracy)
    std::map<std::string, double> weights;
    for (const auto& t : chosenTopics) {
        double acc = 0.0;
        auto it = progress.find(t);
        if (it != progress.end()) acc = it->second.accuracy();
        weights[t] = 1.0 - (acc / 100.0) + 0.1; // +0.1 so no topic is zero
    }

    // Discrete weighted sampling
    std::mt19937 rng(std::chrono::steady_clock::now()
                         .time_since_epoch().count());

    // Build flat weighted list
    std::vector<std::pair<double, std::string>> wList;
    for (const auto& kv : weights)
        if (!pool[kv.first].empty())
            wList.push_back({kv.second, kv.first});

    if (wList.empty()) return {};

    // Shuffle each pool
    for (auto& kv : pool) {
        std::vector<const Question*>& v = kv.second;
        std::shuffle(v.begin(), v.end(), rng);
    }

    // Keep track of used indices per topic
    std::map<std::string, int> usedIdx;
    for (const auto& t : chosenTopics) usedIdx[t] = 0;

    std::vector<double> cumWeights;
    std::vector<std::string> wTopics;
    double total = 0;
    for (const auto& kv : wList) { total += kv.first; }
    double cum = 0;
    for (const auto& kv : wList) {
        cum += kv.first / total;
        cumWeights.push_back(cum);
        wTopics.push_back(kv.second);
    }

    std::uniform_real_distribution<double> dist(0.0, 1.0);
    std::vector<Question> selected;
    int attempts = 0;

    while (static_cast<int>(selected.size()) < count && attempts < count * 5) {
        ++attempts;
        double r = dist(rng);
        std::string chosenTopic;
        for (size_t i = 0; i < cumWeights.size(); ++i) {
            if (r <= cumWeights[i]) { chosenTopic = wTopics[i]; break; }
        }
        if (chosenTopic.empty()) chosenTopic = wTopics.back();

        auto& tPool = pool[chosenTopic];
        int& idx    = usedIdx[chosenTopic];
        if (idx < static_cast<int>(tPool.size())) {
            selected.push_back(*tPool[idx]);
            ++idx;
        }
    }

    // If still short, fill from any remaining
    for (const auto& t : chosenTopics) {
        auto& tPool = pool[t];
        int& idx    = usedIdx[t];
        while (static_cast<int>(selected.size()) < count
               && idx < static_cast<int>(tPool.size())) {
            selected.push_back(*tPool[idx++]);
        }
    }

    std::shuffle(selected.begin(), selected.end(), rng);
    return selected;
}

// ── Ask one question with countdown timer ──────────────────
//
//  Implementation note:
//  We run std::cin in a std::async thread and use wait_for()
//  to enforce the timeout.  std::cin cannot be interrupted
//  mid-block, so if the timer fires the async thread is
//  detached (it will unblock the next time ENTER is pressed).
// ───────────────────────────────────────────────────────────
char QuizEngine::askQuestion(const Question& q, int qNum, int total,
                              bool& timedOut) {
    q.display(qNum, total);

    // Start visual countdown
    Timer timer(TIMER_SECONDS);
    timer.start();

    // Async input
    auto inputFuture = std::async(std::launch::async, []() -> char {
        std::string s;
        std::cin >> s;
        return s.empty() ? '0' : static_cast<char>(toupper(s[0]));
    });

    // Wait for either input or timeout
    auto status = inputFuture.wait_for(
        std::chrono::seconds(TIMER_SECONDS + 1));

    timer.stop();

    if (status == std::future_status::ready && !timer.isExpired()) {
        timedOut = false;
        std::cout << "\n";
        return inputFuture.get();
    }

    timedOut = true;
    return 0;
}

// ── Show right/wrong feedback ───────────────────────────────
void QuizEngine::showFeedback(const Question& q, char answer,
                               bool timedOut, int scoreChange) const {
    using namespace Utils::Color;
    const char letters[] = {'A','B','C','D'};
    char correctChar = letters[q.getCorrectOption() - 1];

    Utils::printLine('-');
    if (timedOut) {
        std::cout << "  " << BG_RED << BOLD
                  << " ⏰  No answer — time expired! "
                  << RESET << "\n";
    } else if (answer == correctChar) {
        std::cout << "  " << BG_GRN << BOLD
                  << " ✔  Correct! +" << scoreChange << " points "
                  << RESET << "\n";
    } else {
        std::cout << "  " << BG_RED << BOLD
                  << " ✘  Wrong! -" << (-scoreChange) << " point"
                  << ((-scoreChange) != 1 ? "s" : "") << " "
                  << RESET << "\n";
    }

    std::cout << "\n  Correct answer: "
              << GREEN << BOLD << correctChar << ") "
              << q.getOptions()[q.getCorrectOption() - 1]
              << RESET << "\n";
    Utils::printLine('-');
}

// ── Result card ─────────────────────────────────────────────
void QuizEngine::showResultCard(const Player& player) const {
    using namespace Utils::Color;
    Utils::clearScreen();
    Utils::printLine('=');
    std::cout << BOLD << CYAN
              << "                  📊  RESULT CARD\n"
              << RESET;
    Utils::printLine('=');
    std::cout << "\n"
              << "  Player : " << BOLD << player.getName() << RESET << "\n"
              << "  Score  : " << BOLD << YELLOW
              << player.getScore() << " pts" << RESET << "\n"
              << "  Correct: " << GREEN  << player.getCorrectCount() << RESET << "\n"
              << "  Wrong  : " << RED    << player.getWrongCount()   << RESET << "\n"
              << "  Timeout: " << MAGENTA<< player.getTimeoutCount() << RESET << "\n"
              << "  Overall accuracy: " << BOLD
              << std::fixed << std::setprecision(1)
              << player.getOverallAccuracy() << "%" << RESET << "\n\n";

    Utils::printLine('-');
    std::cout << BOLD << "  Topic-wise Accuracy:\n" << RESET << "\n";
    for (const auto& kv : player.getTopicStats()) {
        double acc = kv.second.accuracy();
        std::string bar(static_cast<int>(acc / 5), '#');
        std::string fill(20 - static_cast<int>(acc / 5), '.');
        std::string col = (acc >= 70) ? GREEN : (acc >= 40) ? YELLOW : RED;
        std::cout << "  " << std::left << std::setw(18) << kv.first
                  << col << "[" << bar << fill << "] "
                  << std::right << std::setw(5) << std::fixed
                  << std::setprecision(1) << acc << "%"
                  << "  (" << kv.second.correct << "/"
                  << kv.second.total << ")"
                  << RESET << "\n";
    }
    Utils::printLine('=');
    std::cout << "\n";
}

// ── Main game loop ──────────────────────────────────────────
void QuizEngine::run() {
    using namespace Utils::Color;

    // ── Get player name
    Utils::printBanner();
    std::cout << "  " << BOLD << "Enter your name: " << RESET;
    std::string name;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, name);
    name = Utils::trim(name);
    if (name.empty()) name = "Player1";

    Player player(name);

    // ── Show topics
    Utils::clearScreen();
    auto topics = getAvailableTopics();

    std::cout << "\n  " << BOLD << CYAN
              << "Available Topics:\n" << RESET << "\n";
    for (int i = 0; i < static_cast<int>(topics.size()); ++i)
        std::cout << "    " << YELLOW << BOLD << (i + 1) << ")" << RESET
                  << "  " << topics[i] << "\n";

    std::cout << "\n  " << BOLD
              << "Select topics (e.g. 1 3) or press ENTER for all: "
              << RESET;
    std::string topicInput;
    std::getline(std::cin, topicInput);

    std::vector<std::string> chosenTopics;
    if (Utils::trim(topicInput).empty()) {
        chosenTopics = topics;
    } else {
        std::istringstream ss(topicInput);
        int idx;
        while (ss >> idx)
            if (idx >= 1 && idx <= static_cast<int>(topics.size()))
                chosenTopics.push_back(topics[idx - 1]);
        if (chosenTopics.empty()) chosenTopics = topics;
    }

    // ── Show scoring rules
    Utils::clearScreen();
    Utils::printLine('=');
    std::cout << BOLD << "  Quiz Rules\n" << RESET;
    Utils::printLine('-');
    std::cout << "  Questions  : " << BOLD << QUESTIONS_PER_QUIZ << RESET << "\n"
              << "  Timer      : " << BOLD << TIMER_SECONDS << "s per question\n" << RESET
              << "  Correct    : " << GREEN << BOLD << "+" << POINTS_CORRECT << " pts\n" << RESET
              << "  Wrong      : " << RED   << BOLD << "-" << POINTS_PENALTY << " pt\n" << RESET
              << "  Timeout    : " << MAGENTA << BOLD << "0 pts\n" << RESET
              << "\n  Topics     : ";
    for (const auto& t : chosenTopics) std::cout << t << "  ";
    std::cout << "\n";
    Utils::printLine('=');
    std::cout << "\n  " << DIM
              << "Adaptive AI will serve more questions from your weak topics.\n"
              << RESET;
    Utils::pressEnter();

    // ── Select questions (adaptive)
    auto selected = selectQuestions(chosenTopics, name, QUESTIONS_PER_QUIZ);
    if (selected.empty()) {
        std::cout << "\n  " << RED
                  << "[Error] No questions found for selected topics.\n"
                  << RESET;
        return;
    }

    // ── Quiz loop
    for (int i = 0; i < static_cast<int>(selected.size()); ++i) {
        Utils::clearScreen();
        const Question& q = selected[i];
        bool timedOut = false;

        char answer = askQuestion(q, i + 1,
                                   static_cast<int>(selected.size()),
                                   timedOut);

        const char letters[] = {'A','B','C','D'};
        char correctChar = letters[q.getCorrectOption() - 1];
        int  scoreChange = 0;

        if (timedOut) {
            player.addTimeout(q.getTopic());
        } else if (answer == correctChar) {
            scoreChange = POINTS_CORRECT;
            player.addCorrect(q.getTopic(), POINTS_CORRECT);
        } else {
            scoreChange = -POINTS_PENALTY;
            player.addWrong(q.getTopic(), POINTS_PENALTY);
        }

        showFeedback(q, answer, timedOut, scoreChange);
        std::cout << "  Running score: "
                  << BOLD << YELLOW << player.getScore() << " pts"
                  << RESET << "\n";
        Utils::pressEnter();
    }

    // ── Result card
    showResultCard(player);

    // ── Save progress & leaderboard
    FileParser::saveProgress(progressPath, player);

    int rank = leaderboard.addEntry(name, player.getScore(),
                                    Utils::getCurrentDate());
    leaderboard.save();

    if (rank > 0) {
        std::cout << "  " << BG_GRN << BOLD
                  << " 🎉  You entered the leaderboard at rank #" << rank
                  << "!  " << RESET << "\n\n";
    }

    leaderboard.display();
    Utils::pressEnter();
}
