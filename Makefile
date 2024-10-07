TARGETS = ${basename ${wildcard *.cpp}}

CXX_FLAGS = -Wall -Wextra -pedantic -Wconversion -Wshadow -std=c++2b -O0 -g

IGOR_FILES = Igor/Igor.hpp Igor/Logging.hpp Igor/Macros.hpp Igor/Math.hpp Igor/MemoryToString.hpp Igor/ProgressBar.hpp Igor/StaticVector.hpp Igor/Timer.hpp Igor/TypeName.hpp

all: ${TARGETS}

sanitize: CXX_FLAGS += -fsanitize=address,undefined,leak
sanitize: ${TARGETS}

%: %.cpp ${IGOR_FILES}
	${CXX} ${CXX_FLAGS} -o $@ $<

clean:
	rm -fr ${TARGETS} ${addsuffix .dSYM, ${TARGETS}}
