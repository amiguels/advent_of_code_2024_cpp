# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -g -std=c++23 -Wall -Werror

# Target executable
TARGET = main.out

# Source files
SRCS = main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule to build and run the executable
all: $(TARGET) run

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the executable
run: $(TARGET)
	./$(TARGET) input.txt

test: $(TARGET)
	./$(TARGET) test_input.txt

# Clean rule to remove generated files
clean:
	rm -f $(TARGET) $(OBJS)
