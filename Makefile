TARGETS = ${basename ${wildcard *.cpp}}

CXX_FLAGS = -Wall -Wextra -pedantic -Wconversion -Wshadow -std=c++2b -O0 -g -fsanitize=address,undefined,leak
INCLUDE = -I./include/

all: ${TARGETS}

%: %.cpp
	${CXX} ${CXX_FLAGS} ${INCLUDE} -o $@ $<

clean:
	rm -fr ${TARGETS} ${addsuffix .dSYM, ${TARGETS}}
