CPP_CODE_FILES := $(wildcard *.cpp)
CPP_OBJ_FILES := $(addsuffix .o,$(basename $(CPP_CODE_FILES)))
H_FILES := $(wildcard *.h)

#Default Flags
CXXFLAGS = -std=c++11 -Wall -Werror -Wextra -pedantic -g -DDEBUG

.PHONY: all clean

all: build 

build: scm

scm: $(CPP_OBJ_FILES)
	g++ $(CPP_OBJ_FILES) -o scm

%.o: %.cpp $(H_FILES)
	@echo "Building $@"
	g++ $< -c -o $@ $(CXXFLAGS)

clean: 
	-rm -f scm
	-rm -rf *.o

run: scm
	./scm
