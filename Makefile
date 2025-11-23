# 
# ***					building the game					***
# 

# setup
SOURCE_FOLDER = src
BUILD_FOLDER = build
INCLUDE_FOLDER = include

TARGET_WINDOWS = game.exe
TARGET_LINUX = game

TESTS_TARGET_WINDOWS = unit_tests.exe
TESTS_TARGET_LINUX = unit_tests

# this wildcard avoids some errors due to files not found or something like that
SOURCES = $(wildcard $(SOURCE_FOLDER)/*.cpp)

# creates the list of obj files inside the build folder, based on the source files present 
OBJS = $(patsubst $(SOURCE_FOLDER)/%.cpp, $(BUILD_FOLDER)/%.o, $(SOURCES))

# compiler flags
CXX = g++
DEBUG_FLAGS = -O0 -g
CXXFLAGS = -Wall -Wextra

# OS specifics
ifeq ($(OS),Windows_NT)
    LIBS = -lraylib -lgdi32 -lwinmm
	TARGET = $(TARGET_WINDOWS)
	TESTS_TARGET = $(TESTS_TARGET_WINDOWS)
else
    LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	TARGET = $(TARGET_LINUX)
	TESTS_TARGET = $(TESTS_TARGET_LINUX)
endif

# creating the executable from the object files. the dependencies here are the object files, 
# and the existence of a build/ folder. 
game: $(BUILD_FOLDER) $(OBJS)
	$(CXX) $(DEBUG_FLAGS) $(OBJS) -o $(TARGET) $(LIBS)

# option to build and run the game
run: game
	$(TARGET)

# creating every object file, from a general make rule.
# $< means the current target, and $@ means the current dependency.
$(BUILD_FOLDER)/%.o: $(SOURCE_FOLDER)/%.cpp
	$(CXX) $(DEBUG_FLAGS) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_FOLDER) $(LIBS)

# creates the build/ folder, if it doesn't exists already.
$(BUILD_FOLDER):
	mkdir -p $(BUILD_FOLDER)

# 
# ***					building the tests					***
# 

# setup
TESTS_FOLDER = tests
TESTS_BUILD_FOLDER = $(TESTS_FOLDER)/build
CATCH2_FOLDER = $(TESTS_FOLDER)/catch2
CATCH_OBJ_FILE = $(CATCH2_FOLDER)/catch_amalgamated.o

# in order to build the tests, i can't have main.cpp in the sources (because the catch2 already
# has it's own main().
SOURCES_WITHOUT_MAIN = $(filter-out $(SOURCE_FOLDER)/main.cpp, $(SOURCES))
OBJS_WITHOUT_MAIN = $(patsubst $(SOURCE_FOLDER)/%.cpp, $(BUILD_FOLDER)/%.o, $(SOURCES_WITHOUT_MAIN))

# just like before, but now i need to compile the test files. 
TESTS_SOURCES = $(wildcard $(TESTS_FOLDER)/*.cpp)
TESTS_OBJS = $(patsubst $(TESTS_FOLDER)/%.cpp, $(TESTS_BUILD_FOLDER)/%.o, $(TESTS_SOURCES))

# the prerequisites are all of the object files, both from the project itself and from the tests, as 
# well as the catch2 object files. The $@ after the compilation executes the tests right after they 
# are built.
tests: $(CATCH_OBJ_FILE) $(BUILD_FOLDER) $(OBJS_WITHOUT_MAIN) $(TESTS_BUILD_FOLDER) $(TESTS_OBJS)
	$(CXX) $(OBJS_WITHOUT_MAIN) $(CATCH_OBJ_FILE) $(TESTS_OBJS) -o $(TESTS_TARGET) $(LIBS)
	./$(TESTS_TARGET)

# create the object files for the tests
$(TESTS_BUILD_FOLDER)/%.o: $(TESTS_FOLDER)/%.cpp
	$(CXX) $(DEBUG_FLAGS) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE_FOLDER) -I$(CATCH2_FOLDER) $(LIBS)

# create the object file for the catch2 library
$(CATCH_OBJ_FILE):
	$(CXX) -c $(CATCH2_FOLDER)/catch_amalgamated.cpp -o $(CATCH_OBJ_FILE)

# creates the build folder for the tests
$(TESTS_BUILD_FOLDER):
	mkdir -p $(TESTS_BUILD_FOLDER)

# 
# ***					cleanup					***
# 

clean:
	rm -rf $(TARGET_WINDOWS) $(TARGET_LINUX) $(TESTS_TARGET_WINDOWS) $(TESTS_TARGET_LINUX) $(BUILD_FOLDER) $(TESTS_BUILD_FOLDER) $(CATCH_OBJ_FILE)
