CXX=g++
CFLAGS=-Wall -Werror -O2

SRC_DIR=./src
BIN_DIR=./bin
OBJ_DIR=./obj

all: $(BIN_DIR)/mean_pos $(BIN_DIR)/ssr $(BIN_DIR)/calculate_elastic_energy

$(BIN_DIR)/mean_pos: $(SRC_DIR)/mean_pos.cpp
	$(CXX) $(CFLAGS) $< -o $@
$(BIN_DIR)/ssr: $(SRC_DIR)/ssr.cpp
	$(CXX) $(CFLAGS) $< -o $@
$(BIN_DIR)/calculate_elastic_energy: $(SRC_DIR)/calculate_elastic_energy.cpp
	$(CXX) $(CFLAGS) $< -o $@
clean:
	rm -f $(BIN_DIR)/*
