CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

TARGET = main
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET) 5

clean:
	rm -f $(OBJ) $(TARGET)
