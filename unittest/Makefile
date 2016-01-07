CPP_CODE_FILES := $(wildcard *.cpp)
CPP_OBJ_FILES := $(addsuffix .o,$(basename $(CPP_CODE_FILES)))
PARENT_CPP_CODE_FILES := $(wildcard ../*.cpp)
PARENT_CPP_OBJ_FILES := $(addsuffix .o,$(basename $(PARENT_CPP_CODE_FILES)))
H_FILES := $(wildcard *.h)
BINARY_FILES := $(basename $(CPP_CODE_FILES))

#RUN_OUTPUT_FILES := $(addsuffix .out,$(BINARY_FILES))

#Default Flags
CXXFLAGS = -std=c++11 -Wall -Werror -Wextra -pedantic -g -DDEBUG

.PHONY: all
.SUFFIXES:

all: $(BINARY_FILES)


%: %.o $(PARENT_CPP_OBJ_FILES)
	g++ -z muldefs $^ -o $@ $(CXXFLAGS)

%.o: %.cpp $(H_FILES)
	@echo "Building $@"
	g++ -I../ $< -c -o $@ $(CXXFLAGS)

clean:
	rm -f *.o $(BINARY_FILES)

#run: $(RUN_OUTPUT_FILES)
