# 🕹️ Quiz App with Timer and Leaderboard



---

## 📋 About

A console-based C++ MCQ quiz engine that loads questions from a structured text file, lets players race against a per-question countdown timer, applies positive/negative marking, and maintains a persistent top-10 leaderboard.  
An **adaptive AI** feature tracks per-topic accuracy across sessions and automatically serves more questions from topics where you are weak.

---

## ✨ Features

| Feature | Details |
|---|---|
| Question loader | Reads from `data/questions.txt` (pipe-delimited, easy to extend) |
| Topic selection | Choose one or more topics, or play all |
| Countdown timer | 15 s per question using `std::chrono` + background thread |
| Scoring | +4 correct · −1 wrong · 0 timeout |
| Instant feedback | Shows correct answer after every question |
| Random order | Questions shuffled with `std::shuffle` / `mt19937` |
| Result card | Topic-wise accuracy bars at the end of each round |
| Persistent leaderboard | Top-10 saved to `data/leaderboard.txt` via sorted insert |
| Adaptive AI | Weighted question selection from `data/progress.txt` |

---

## 🛠️ Tech Stack

- **Language** — C++17  
- **STL** — `vector`, `map`, `set`, `fstream`, `sstream`  
- **Timing** — `std::chrono::steady_clock`, `std::this_thread::sleep_for`  
- **Concurrency** — `std::thread`, `std::atomic`, `std::async`, `std::future`  
- **Randomness** — `std::shuffle`, `std::mt19937`  
- **Build** — GNU Make + g++

---

## 📂 Project Structure

```
QuizApp/
├── src/
│   ├── main.cpp          # Entry point + main menu
│   ├── Question.h/.cpp   # MCQ data class
│   ├── Player.h/.cpp     # Session score + topic stats
│   ├── Timer.h/.cpp      # Countdown timer (background thread)
│   ├── QuizEngine.h/.cpp # Core game loop + adaptive selection
│   ├── Leaderboard.h/.cpp# Persistent top-10 management
│   ├── FileParser.h/.cpp # questions.txt + progress.txt I/O
│   └── Utils.h           # Colors, screen helpers, string utils
├── data/
│   ├── questions.txt     # Question bank (48 questions, 4 topics)
│   ├── leaderboard.txt   # Auto-managed: top-10 high scores
│   └── progress.txt      # Auto-managed: per-player topic stats
├── Makefile
├── .gitignore
└── README.md
```

---

## 🚀 How to Build & Run

### Prerequisites
- `g++` with C++17 support  
- `make`

### Linux / macOS
```bash
git clone https://github.com/<your-username>/QuizApp.git
cd QuizApp
make
./quiz
```

### Windows (MinGW)
```bash
g++ -std=c++17 -Wall -O2 -pthread src/*.cpp -o quiz.exe
quiz.exe
```

> ⚠️ **Run the binary from the `QuizApp/` root folder** so it can find the `data/` directory.

---

## 📖 How to Play

1. Enter your name.  
2. Select topic(s) — or press **ENTER** to include all.  
3. Answer each question with **A / B / C / D** within **15 seconds**.  
4. See instant feedback after each answer.  
5. View your **Result Card** with topic-wise accuracy bars.  
6. If you crack the **Top-10**, your score is saved to the leaderboard!

---

## ➕ Adding Your Own Questions

Open `data/questions.txt` and add a line:

```
TOPIC|Question text|Option A|Option B|Option C|Option D|CORRECT|DIFFICULTY
```

- `CORRECT` : 1=A · 2=B · 3=C · 4=D  
- `DIFFICULTY` : 1=Easy · 2=Medium · 3=Hard  
- Lines beginning with `#` are treated as comments.

---

## 🧠 Key OOP Concepts Used

| Concept | Where |
|---|---|
| Classes & Encapsulation | `Question`, `Player`, `Timer`, `Leaderboard` |
| Vectors | `allQuestions`, `options`, `entries` |
| File I/O (`fstream`) | `FileParser` — questions, progress, leaderboard |
| `std::chrono` | `Timer` — steady_clock countdown |
| `std::shuffle` / `mt19937` | `QuizEngine::selectQuestions` |
| Sorted insert | `Leaderboard::addEntry` |
| String handling | `Utils::split`, `Utils::trim` |
| Adaptive AI | Weighted distribution in `QuizEngine::selectQuestions` |

---


