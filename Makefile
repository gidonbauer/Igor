TARGETS = ${basename ${wildcard *.cpp}}

CXX_FLAGS = -Wall -Wextra -pedantic -Wconversion -Wshadow -std=c++2b -O0 -g
INCLUDE = -I./include/

all: ${TARGETS}

sanitize: CXX_FLAGS += -fsanitize=address,undefined,leak
sanitize: ${TARGETS}

%: %.cpp
	${CXX} ${CXX_FLAGS} ${INCLUDE} -o $@ $<

clean:
	rm -fr ${TARGETS} ${addsuffix .dSYM, ${TARGETS}}
