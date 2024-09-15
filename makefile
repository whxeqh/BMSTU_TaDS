OBJS_DIR := ./out
INC_DIR  := ./inc
SRC_DIR  := ./src

CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wpedantic -Wextra -g3 -I$(INC_DIR) 

EXEC = app.exe

OBJS = $(OBJS_DIR)/main.o $(OBJS_DIR)/big_nums.o $(OBJS_DIR)/errors.o $(OBJS_DIR)/input_output.o $(OBJS_DIR)/division.o
DEPS =  $(INC_DIR)/big_nums.h $(INC_DIR)/errors.h $(INC_DIR)/input_output.h $(INC_DIR)/division.h

.PHONY: debug clean

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.c $(DEPS)
	mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS_DIR)/*.o $(EXEC)
	rm -rf $(OBJS_DIR)
