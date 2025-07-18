CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -Iinclude

LIB_NAME = console_log_msg

SRC_DIR = src
INCLUDE_DIR = include
EXAMPLE_DIR = example

LIB_SRC = $(SRC_DIR)/$(LIB_NAME).cpp
LIB_OBJ = $(SRC_DIR)/$(LIB_NAME).o

EXAMPLE_SRC = $(EXAMPLE_DIR)/main.cpp
EXAMPLE_BIN = $(EXAMPLE_DIR)/main_example

.PHONY: all lib example clean

all: lib example

lib: $(LIB_OBJ)
	@echo "Biblioteca $(LIB_NAME) compilada."

$(LIB_OBJ): $(LIB_SRC) $(INCLUDE_DIR)/$(LIB_NAME).hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

example: $(EXAMPLE_BIN)

$(EXAMPLE_BIN): $(EXAMPLE_SRC) $(LIB_OBJ)
	$(CXX) $(CXXFLAGS) $< $(LIB_OBJ) -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(EXAMPLE_DIR)/*_example


