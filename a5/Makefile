
CC = gcc

target = raw2obj

src = src/main.c

cflags = -fopenmp
lflags = -lSDL2

$(target) : $(src)
	$(CC) $(cflags) $(lflags) -o $(target) $(src)

debug : cflags += -gdwarf
debug : $(target)

release : cflags += -O3
release : $(target)

clean:
	rm $(target)
