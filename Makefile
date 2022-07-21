########################
### WRAPPER MAKEFILE ###
########################

PROJECT_DIR ?= $(shell pwd)

include $(PROJECT_DIR)/global.mk

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -Iinclude -g
LDFLAGS = -lm -lcrypto -fsanitize=address -fsanitize=undefined -fsanitize=leak

format:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC_CODE)/format.c -o $(BUILD)/format


.PHONY: module vm kernel kernel-module image dependencies clean-all clean-vm clean-vm-all clean-module clean-sytem-kernel-module clean-system-kernel clean-system-image clean-system-all
