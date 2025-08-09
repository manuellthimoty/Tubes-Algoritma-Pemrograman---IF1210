# Compiler
CC := gcc

# Folder dan Flag
SRC_DIR := c
HEADER_DIR := header
CFLAGS := -I$(HEADER_DIR) -Wall -Wextra -std=c99

# Daftar file sumber
MAIN := main.c
MODULES := $(notdir $(wildcard $(SRC_DIR)/*.c))      
MODULES_PATH := $(addprefix $(SRC_DIR)/, $(MODULES)) 
SOURCES := $(MAIN) $(MODULES_PATH)

# Output
TARGET := program

# Target build
all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

# Ngeclean File
clean:
	rm -f $(TARGET)

# Run
run: all
	./$(TARGET)

run-valgrind: all 
	valgrind --leak-check=full ./$(TARGET)

#  Commit
git-commit:
	@if [ -z "$(msg)" ]; then \
		echo " Harus pakai: make git-commit msg=\"pesan commit\""; \
		exit 1; \
	fi
	git add -A
	git commit -m "$(msg)"

# Push ke github ke main
git-push:
	git push origin main

# Pull dari main
git-pull:
	git pull origin main


.PHONY: all clean run run-valgrind git-commit git-push git-pull
