CC = g++
CFLAGS = -Wall -std=c++17 -Iinclude
LDFLAGS =
LDLIBS = -lcurl -lcurses
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=obj/%.o)
BIN = bin
TARGET = mkpj

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $(BIN)/$@ $^ $(LDLIBS)

obj/%.o: src/%.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJ) $(BIN)/$(TARGET)

fclean: clean
	rm -f $(TARGET) $(BIN)/$(TARGET)

configure:
	mkdir -p obj && mkdir -p bin

install: configure all
	sudo cp $(BIN)/$(TARGET) /usr/local/bin
