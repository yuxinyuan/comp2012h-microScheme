CPP_CODE_FILES := $(wildcard src/*.cpp)
DEBUG_CPP_OBJ_FILES := $(addsuffix .d.o,$(basename $(CPP_CODE_FILES)))
CPP_OBJ_FILES := $(addsuffix .o,$(basename $(CPP_CODE_FILES)))
H_FILES := $(wildcard include/*.h)

#Default Flags
CFLAGS = -Iinclude/
DEBUGS = -g -DDEBUG
CXXFLAGS = -Wall -Werror -Wextra -pedantic -std=c++11 $(CFLAGS)

all: build 

build: scm

debug: $(DEBUG_CPP_OBJ_FILES)
	g++ $(DEBUG_CPP_OBJ_FILES) -o scm

scm: $(CPP_OBJ_FILES)
	g++ $(CPP_OBJ_FILES) -o scm

%.o: %.cpp $(H_FILES)
	@echo "Building $@"
	g++ $< -c -o $@ $(CXXFLAGS)

%.d.o: %.cpp $(H_FILES)
	@echo "Building $@"
	g++ $< -c -o $@ $(DEBUGS) $(CXXFLAGS)

clean: 
	-rm -f scm
	-rm -rf $(CPP_OBJ_FILES) $(DEBUG_CPP_OBJ_FILES)

.PHONY: all clean