# C Language Ladder Tutor build file.
# Windows: mingw32-make
# Linux/macOS: make

ifeq ($(OS),Windows_NT)
TARGET := c_tutor.exe
RM_CMD := del /Q
RUN_CMD := .\$(TARGET)
else
TARGET := c_tutor
RM_CMD := rm -f
RUN_CMD := ./$(TARGET)
endif

CC ?= gcc
CPPFLAGS ?= -Iinclude
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2 -g -finput-charset=UTF-8 -fexec-charset=UTF-8
LDLIBS ?= -lm
SOURCES := $(wildcard src/*.c)
HEADERS := $(wildcard include/*.h)

.PHONY: all run clean check test

all: $(TARGET)

# Build all translation units into one executable in a single command.
$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDLIBS)

run: $(TARGET)
	$(RUN_CMD)

clean:
	-$(RM_CMD) $(TARGET)

# Verify that every non-blank C/H line has a // comment.
check:
	powershell.exe -NoProfile -ExecutionPolicy Bypass -File tools/check_comments.ps1

# Compile with -Werror and run the scripted smoke tests.
test:
	powershell.exe -NoProfile -ExecutionPolicy Bypass -File tests/smoke_test.ps1
