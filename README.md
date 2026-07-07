# 🕹️ Quiz App with Timer and Leaderboard



---

## 👥 Team Members

| Role | Name |
|---|---|
| Quiz Engine & Adaptive AI | |
| Question Bank & File Parser | |
| Timer & Scoring System | |
| Leaderboard & Result Card | |
| Testing & Documentation | |

---

## 📋 About

A console-based C++ MCQ quiz engine that loads questions from a structured text file. Players choose a subject category, face a countdown timer per question, receive instant right/wrong feedback, and earn scores with negative marking. A result card summarises topic-wise accuracy. A persistent leaderboard file maintains the all-time top-10 scores. An adaptive AI tracks per-topic accuracy across sessions and serves more questions from weak topics.

---

## ✨ Features

- Multiple subject category selection
- Per-question countdown timer (15 seconds)
- Positive / Negative marking scheme
- Instant feedback with correct answer reveal
- Randomised question order
- Topic-wise accuracy result card
- Persistent top-10 leaderboard
- Adaptive AI difficulty based on past performance

---

## 📂 Folder Structure

```
QuizApp/
│
├── src/
│   ├── main.cpp
│   ├── Question.h / Question.cpp
│   ├── Player.h / Player.cpp
│   ├── Timer.h / Timer.cpp
│   ├── QuizEngine.h / QuizEngine.cpp
│   ├── Leaderboard.h / Leaderboard.cpp
│   ├── FileParser.h / FileParser.cpp
│   └── Utils.h
│
├── data/
│   ├── questions.txt
│   ├── leaderboard.txt
│   └── progress.txt
│
├── Makefile
└── README.md
```

---

## 🖥️ Main Menu

```
==========================================
       QUIZ APP — Race the Clock
==========================================
1. Start Quiz
2. View Leaderboard
3. How to Play
4. Exit
==========================================
Enter your choice:
```

---

## 🎮 Sample Quiz Flow

```
1. Enter Name
Enter your name: Muhammad

2. Select Topics
Available Topics:
  1) CPP
  2) DSA
  3) OOP
  4) OS
Select topics (e.g. 1 3) or press ENTER for all:

3. Question Screen
------------------------------------------
Q1 / 10   [OOP]   Difficulty: Easy
------------------------------------------

  What is a constructor?

    A)  A function called when an object is destroyed
    B)  A function called automatically when object is created
    C)  A function that copies an object
    D)  A static member function

  Time left: 15s
  Your answer: B

4. Feedback
------------------------------------------
  Correct! +4 points
  Correct answer: B) A function called automatically when object is created
------------------------------------------
  Running score: 4 pts
```

---

## 📊 Result Card

```
==========================================
             RESULT CARD
==========================================
  Player  : Arnav
  Score   : 32 pts
  Correct : 8
  Wrong   : 1
  Timeout : 1
  Overall accuracy: 80.0%

  Topic-wise Accuracy:

  OOP   [################....] 80.0%  (4/5)
  DSA   [############........] 60.0%  (3/5)
==========================================
```

---

## 🏆 Leaderboard

```
==========================================
         TOP-10 LEADERBOARD
==========================================
  RANK  PLAYER          SCORE   DATE
------------------------------------------
  1    Arnav            32     2025-06-01
  2    Khan             28     2025-05-30
  3   Ganesh            24     2025-05-29
==========================================
```

---

## 💯 Scoring System

```
Correct Answer  →  +4 pts
Wrong Answer    →  -1 pt
Timeout         →   0 pts

Example:
  8 Correct  =  +32 pts
  1 Wrong    =   -1 pt
  Total      =   31 pts
```

---

## 🛠️ Tech Stack

- **Language** — C++17
- **STL** — `vector`, `map`, `fstream`, `sstream`
- **Timing** — `std::chrono::steady_clock`
- **Concurrency** — `std::thread`, `std::atomic`, `std::async`
- **Randomness** — `std::shuffle`, `std::mt19937`
- **Build** — GNU Make + g++

---

## 🧠 Key Concepts Applied

| Concept | Where Used |
|---|---|
| Classes & Encapsulation | `Question`, `Player`, `Timer`, `Leaderboard` |
| Vectors | Question bank, options, leaderboard entries |
| File I/O (`fstream`) | questions.txt, leaderboard.txt, progress.txt |
| `std::chrono` | Per-question countdown timer |
| `std::shuffle` / `mt19937` | Random question order |
| Sorted Insert | Leaderboard top-10 maintenance |
| Adaptive AI | Weighted topic selection from progress file |

---

## 🚀 How to Build & Run

```bash
make
./quiz
```

> Always run `./quiz` from inside the `QuizApp/` folder so it can find the `data/` directory.

---

## ➕ Adding Your Own Questions

Open `data/questions.txt` and add a line:

```
TOPIC|Question text|Option A|Option B|Option C|Option D|CORRECT|DIFFICULTY
```

- `CORRECT` : 1=A · 2=B · 3=C · 4=D
- `DIFFICULTY` : 1=Easy · 2=Medium · 3=Hard
- Lines starting with `#` are ignored

---

