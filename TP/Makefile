# ============================================================
# Makefile - Submissão Linux (VPL / UFMG)
# Compilar com: make all
# ============================================================

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

TARGET = $(BINDIR)/tp2.out

SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))

all: dirs $(TARGET)

dirs:
	mkdir -p $(OBJDIR) $(BINDIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all dirs clean
