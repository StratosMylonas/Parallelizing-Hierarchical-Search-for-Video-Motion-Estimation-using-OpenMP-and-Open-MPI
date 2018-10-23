# 2014/02
# version: 15
# Dasygenis multipurpose Makefile

#Override the compiler with:
#CC=clang;export CC


#-pg is for profiling
PG=-DDEBUG=1
CC=mpicc
CFLAGS=-O3 ${PG} -g -fopenmp -Wall
LDFLAGS=${PG} -lm -lc -lrt -pg -fopenmp
SOURCES=snr.c dastools.c read_write_frame.c motion_compensation.c hs.c main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@


.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f main *.o gmon.out callgrind.out.* a.out core *.core *.clang *.out *.raw *.gcda *.gcno main_extract_frame *.gcov


execute: ${EXECUTABLE}
	mpirun -n 3 `pwd`/$(EXECUTABLE) 1920 1080 16 7 500 10 /mnt/videonas/1920x1080/BigBuckBunny_1920_1080_24fps.yuv

extractframe: 
		@echo Execute:  ./main_extract_frame  1920 1080 500 /mnt/videonas/1920x1080/BigBuckBunny_1920_1080_24fps.yuv
		gcc main_extract_frame.c read_write_frame.c  -o main_extract_frame 
