CXX := g++
AR := ar
ARFLAGS := rcs

CXXFLAGS := -std=c++20 -Wall -Wextra -Werror

INCLUDE_DIR := ./include
SRC_DIR := ./src
TEST_DIR := ./tests
BUILD_DIR := ./build
OBJ_DIR := $(BUILD_DIR)/obj

LIB_NAME := ip_log_filtering.a
DEMO_BIN := demo/demo.out
TEST_BIN := tests/tests.out

CPPFLAGS := \
	-I$(INCLUDE_DIR) \
	-I$(SRC_DIR)/filter \
	-I$(SRC_DIR)/filter/factory \
	-I$(SRC_DIR)/log_parser \
	-I$(SRC_DIR)/value_objects \
	-I$(SRC_DIR)/create_filter \
	-I$(SRC_DIR)/process_stream \
	-I$(SRC_DIR)/process_stream/output

API_HEADERS := $(shell find $(INCLUDE_DIR) -name '*.hpp')
SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
TEST_SOURCES := $(shell find $(TEST_DIR) -name '*.cpp')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

.PHONY: all lib build_demo build_test test clean

all: clean lib build_demo test

lib: $(LIB_NAME)

$(LIB_NAME): $(OBJ_FILES) $(API_HEADERS)
	$(AR) $(ARFLAGS) $@ $(OBJ_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

build_demo: lib
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) ./demo/main.cpp ./$(LIB_NAME) -o $(DEMO_BIN)

build_test: lib
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TEST_SOURCES) ./$(LIB_NAME) -lgtest -pthread -o $(TEST_BIN)

test: build_test
	./$(TEST_BIN)

demo: build_demo 
	cd $(dir $(DEMO_BIN)) && ./$(notdir $(DEMO_BIN))

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(LIB_NAME) $(DEMO_BIN) $(TEST_BIN) a.out
