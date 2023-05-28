# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

# Source files
SRCS = ECEditorTest.cpp ECTextViewImp.cpp ECTextController.cpp ECTextModel.cpp ECCommand.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable
EXEC = myeditor

# Rule to compile the object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to build the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXEC)

# Rule to clean up the object files and the executable
clean:
	rm -f $(OBJS) $(EXEC)
