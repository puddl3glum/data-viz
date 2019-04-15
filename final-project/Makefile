
CC = g++

target = omniview

srcs = main

sources = $(addsuffix .cpp, $(addprefix src/, $(srcs)))
headers = $(addsuffix .hpp, $(addprefix include/, $(srcs)))

# cflags = -fopenmp
cflags = -Wall
# lflags = -lSDL2

$(target) : $(sources) $(headers)
	$(CC) $(cflags) $(lflags) -o $(target) $(sources)

debug : cflags += -g
debug : $(target)

release : cflags += -O2
release : $(target)

clean:
	rm $(target)

