########################
### WRAPPER MAKEFILE ###
########################

PROJECT_DIR ?= $(shell pwd)

include $(PROJECT_DIR)/global.mk

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -Iinclude -g
LDFLAGS = -lm -lcrypto -fsanitize=address -fsanitize=undefined -fsanitize=leak

SRC = $(shell find $(SRC_CORE_DIR) -name '*.c')
OBJ = $(subst $(PROJECT_DIR),$(BUILD_DIR),$(SRC:.c=.o))

FORMAT_SRC = $(SRC_CODE_DIR)/format.c
FORMAT_OBJ = $(subst $(PROJECT_DIR),$(BUILD_DIR),$(FORMAT_SRC:.c=.o))

format: $(BUILD_DIR)/format $(OBJ)

$(BUILD_DIR)/format: $(FORMAT_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(BUILD_DIR)/format

$(BUILD_DIR)/%.o: $(PROJECT_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@


clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean
