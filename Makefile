CC ?= gcc
CFLAGS ?= -Wall -Wextra -std=c11 -O2

SRC_DIR := src
BIN_DIR := bin

SRCS := $(SRC_DIR)/main.c \
        $(SRC_DIR)/lexer.c \
        $(SRC_DIR)/parser.c \
        $(SRC_DIR)/ast.c \
        $(SRC_DIR)/codegen.c \
        $(SRC_DIR)/vm.c

OBJS := $(SRCS:.c=.o)

TARGET := $(BIN_DIR)/orioncc

all: $(TARGET)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TARGET): $(BIN_DIR) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(SRC_DIR) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
