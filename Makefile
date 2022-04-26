SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

CXX_FLAGS = -std=c++20 -g -O3

all: bin/ffs

bin/ffs: dirs $(OBJ_FILES)
	$(CXX) $(LD_FLAGS) $(OBJ_FILES) -o $@

.PHONY: dirs
dirs:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

install: bin/ffs
	mv -f bin/ffs /usr/local/bin/ffs

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)