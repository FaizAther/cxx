CXX=clang++
CXXFLAGS=-Wall -Werror -g -ggdb
LDD=-std=c++17

BINS=main.exe

TSTS=$(wildcard testing/*.txt)
OUTS=$(wildcard testing/*.txt)

LOGS=$(wildcard *.txt)
SRCS=$(wildcard *.cpp)
DEPS=$(wildcard *.hpp)

.PHONY: all clean run debug tar
all: ${BINS}

%.exe: %.o
	$(CXX) $(CXXFLAGS) $(LDD) $< -o $@

%.o: %.cpp ${DEPS}
	$(CXX) $(CXXFLAGS) $(LDD) $< -c

run: all
	for bin in ${BINS}; do ./$$bin; done

debug: all
	gdb ./${BINS} -q

dir:
	mkdir submit
	mkdir outtesting

tar: clean dir
	cp -r ${SRCS} ${DEPS} Makefile testing submit
	tar cvf `date +%F.%H.%Z`_assn2.tar.gz submit
	rm -rf submit

test: clean all dir
	for tst in ${TSTS}; do ./${BINS} < $$tst > out$$tst.out; cat ./bookings.txt > out$$tst; done

validate: test

clean:
	rm -rf ${BINS} *_assn2.tar.gz outtesting submit