CC = g++
CFLAGS = -Wall -std=c++17
INCLUDE = -Iinclude
SRC = src
OBJ = obj
BIN = bin
TARGET = mkpj

all: $(TARGET)

$(TARGET): $(OBJ)/main.o
	@mkdir -p $(BIN)
	@$(CC) $(CFLAGS) $(INCLUDE) -o $(BIN)/$(TARGET) $^

$(OBJ)/main.o: $(SRC)/main.cpp
	@mkdir -p $(OBJ)
	@$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

clean:
	@rm -rf $(OBJ) $(BIN)

.PHONY: all clean
install: all
	@cp $(BIN)/$(TARGET) /usr/local/bin

