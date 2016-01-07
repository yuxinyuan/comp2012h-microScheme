CPP_CODE_FILES := $(wildcard *.cpp)
CPP_OBJ_FILES := $(addsuffix .o,$(basename $(CPP_CODE_FILES)))
H_FILES := $(wildcard *.h)

#Default Flags
CXXFLAGS = -std=c++11 -Wall -Werror -Wextra -pedantic -Wno-unused-parameter -g -DDEBUG

.PHONY: all clean

all: build cleanobj

build: scm

scm: $(CPP_OBJ_FILES)
	g++ $(CPP_OBJ_FILES) -o scm

%.o: %.cpp $(H_FILES)
	@echo "Building $@"
	g++ $< -c -o $@ $(CXXFLAGS)

clean: cleanobj
	-rm -f scm

.PHONY: cleanobj
cleanobj:
	-rm -rf *.o

run: scm
	./scm
