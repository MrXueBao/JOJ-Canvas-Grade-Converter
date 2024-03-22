CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Werror -pedantic -O3 -g

all: GradeConverter
SOURCE = GradeConverter.cpp
OBJECTS = $(SOURCE:.cpp=.o)

GradeConverter: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

$(OBJECTS): $(SOURCE)
	$(CXX) $(CXXFLAGS) -c $(SOURCE)

.PHONY: clean
clean:
	rm -rf $(OBJECTS) GradeConverter *.dSYM/