CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

LDFLAGS1 =
LDFLAGS2 = -lgdi32

TARGET1 = prog1
SRC1 = main.cpp
OBJ1 = $(SRC1:.cpp=.o)

TARGET2 = prog2
SRC2 = main_proj2.cpp
OBJ2 = $(SRC2:.cpp=.o)

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJ1)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS1)

$(TARGET2): $(OBJ2)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS2)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run1: $(TARGET1)
	./$(TARGET1) 7

run2: $(TARGET2)
	./$(TARGET2) 7

clean:
	rm -f *.o $(TARGET1) $(TARGET2)
