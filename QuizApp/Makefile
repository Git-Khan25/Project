# ============================================================
#  Makefile  –  Quiz App  (C++17, g++)
#  Usage:
#    make          → build
#    make run      → build + run
#    make clean    → remove build artefacts
# ============================================================

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -pthread
TARGET   := quiz
SRCDIR   := src
SRCS     := $(wildcard $(SRCDIR)/*.cpp)
OBJS     := $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo ""
	@echo "  Build successful!  Run with:  ./$(TARGET)"
	@echo ""

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(SRCDIR)/*.o $(TARGET)

.PHONY: all run clean
