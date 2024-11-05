TARGETS = ${basename ${wildcard *.cpp}}
STD_TARGETS = ${addsuffix .std, ${TARGETS}}
FMT_TARGETS = ${addsuffix .fmt, ${TARGETS}}

CXX_FLAGS = -Wall -Wextra -pedantic -Wconversion -Wshadow -std=c++23 -O0 -g -march=native
FMT_FLAGS = `pkg-config fmt --cflags`
FMT_LIBS = `pkg-config fmt --libs`

IGOR_FILES = Igor/Igor.hpp Igor/Logging.hpp Igor/Macros.hpp Igor/Math.hpp Igor/MemoryToString.hpp Igor/ProgressBar.hpp Igor/StaticVector.hpp Igor/Timer.hpp Igor/TypeName.hpp

all: ${STD_TARGETS} ${FMT_TARGETS}

sanitize: CXX_FLAGS += -fsanitize=address,undefined,leak
sanitize: ${TARGETS}

%.std: %.cpp ${IGOR_FILES}
	${CXX} ${CXX_FLAGS} -o $@ $<

%.fmt: %.cpp ${IGOR_FILES}
	${CXX} ${CXX_FLAGS} ${FMT_FLAGS} -DIGOR_USE_FMT -o $@ $< ${FMT_LIBS}

clean:
	rm -fr ${STD_TARGETS} ${FMT_TARGETS} ${addsuffix .dSYM, ${STD_TARGETS} ${FMT_TARGETS}}
