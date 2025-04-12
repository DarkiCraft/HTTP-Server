# === Variables === #
CXX := g++
CXXFLAGS := -std=c++17 \
            -Wall -Wextra -Wpedantic \
            -O0 -g \
            -fsanitize=address -fsanitize=undefined \
						-fno-omit-frame-pointer \
            -Iinclude

LDFLAGS  := -fsanitize=address -fsanitize=undefined 

AR := ar
ARFLAGS := rcs

BUILD_DIR := build
LIB := $(BUILD_DIR)/libcommon.a

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp, $(BUILD_DIR)/%.o, $(SRC))

CLIENT_OBJ := $(BUILD_DIR)/client.o
SERVER_OBJ := $(BUILD_DIR)/server.o

TARGETS := server client

# === Targets === #
all: $(TARGETS)

client: $(CLIENT_OBJ) $(LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

server: $(SERVER_OBJ) $(LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(LIB): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/server.o: server.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/client.o: client.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGETS)

.PHONY: all clean
# === End of Makefile === #