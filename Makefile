CXX := g++
CXXFLAGS := -std=c++20 -I external/glad/include -I external/stb -I external -Wall
LDFLAGS := -lglfw3 -lopengl32 -lgdi32

SRCS := $(shell find src -name '*.cpp') external/glad/gl.c
TARGET := a.exe

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(SRCS) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET)

clean:
	-rm -f $(TARGET)

.PHONY: all clean