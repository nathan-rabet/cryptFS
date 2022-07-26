########################
###     MAKEFILE     ###
########################

PROJECT_DIR ?= $(shell pwd)

include $(PROJECT_DIR)/global.mk

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -Iinclude -g
LDFLAGS = -lm -lcrypto -fsanitize=address -fsanitize=undefined -fsanitize=leak

SRC = $(shell find $(SRC_CORE_DIR) -name '*.c')
OBJ = $(subst $(PROJECT_DIR),$(BUILD_DIR),$(SRC:.c=.o))

TESTS_SRC = $(shell find $(TESTS_DIR) -name '*.c')
TESTS_OBJ = $(subst $(PROJECT_DIR),$(BUILD_DIR),$(TESTS_SRC:.c=.o))

FORMAT_SRC = $(SRC_CODE_DIR)/format_main.c
FORMAT_OBJ = $(subst $(PROJECT_DIR),$(BUILD_DIR),$(FORMAT_SRC:.c=.o))

format: $(BUILD_DIR)/format_main $(OBJ)

$(BUILD_DIR)/format_main: $(FORMAT_OBJ) $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(BUILD_DIR)/format_main

$(BUILD_DIR)/%.o: $(PROJECT_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

tests_suite: $(BUILD_DIR)/tests_suite
	
$(BUILD_DIR)/tests_suite: LDFLAGS += -lcriterion
$(BUILD_DIR)/tests_suite: $(TESTS_OBJ) $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(BUILD_DIR)/tests_suite

test_main: $(BUILD_DIR)/test_main
	

$(BUILD_DIR)/test_main: $(OBJ) $(BUILD_DIR)/tests/test_main.o
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/test_main $^ $(LDFLAGS)

check: tests_suite
	$(BUILD_DIR)/tests_suite

clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean
