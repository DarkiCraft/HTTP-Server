# === Variables === #
CXX := gcc

CXXFLAGS := -std=gnu99 \
						-O3 -flto \
						-Iinclude \
						-D_GNU_SOURCE -DNDEBUG \
						-Wall -Wextra -Wpedantic

LDFLAGS  := -pthread -lsqlite3

AR := ar
ARFLAGS := rcs

BUILD_DIR := build
LIB := $(BUILD_DIR)/libcommon.a

SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c, $(BUILD_DIR)/%.o, $(SRC))

CLIENT_SRC := src/client.c
SERVER_SRC := src/server.c

CLIENT_OBJ := $(BUILD_DIR)/client.o
SERVER_OBJ := $(BUILD_DIR)/server.o

TARGETS := server client

# === Targets === #
all: $(TARGETS)

client: $(CLIENT_OBJ) $(LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	strip $@

server: $(SERVER_OBJ) $(LIB)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)
	strip $@

$(LIB): $(OBJ)
	@mkdir -p $(BUILD_DIR)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD_DIR)/%.o: src/%.c
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(CLIENT_OBJ): $(CLIENT_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SERVER_OBJ): $(SERVER_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGETS)

.PHONY: all clean
# === End of Makefile === #