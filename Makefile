# Flags
CXX = g++
CXXFLAGS = -g -c -Wall -std=c++17 
OUT_FILE = dsl.out

# Directories
SRC_DIR := src
INCLUDE_DIR := $(SRC_DIR)/include
OBJ_DIR := obj
BIN_DIR := bin

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Target: all
all: $(BIN_DIR)/$(OUT_FILE)

# Target: $(BIN_DIR)/$(OUT_FILE)
$(BIN_DIR)/$(OUT_FILE): $(OBJS)
#	@mkdir -p $(BIN_DIR)
	$(CXX) $^ -o $@

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
#	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)


# Target: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
