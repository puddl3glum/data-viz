
CC = g++

target = raw2obj

src = src/main.cpp

# cflags = -fopenmp
# lflags = -lSDL2

$(target) : $(src)
	$(CC) $(cflags) $(lflags) -o $(target) $(src)

debug : cflags += -g
debug : $(target)

release : cflags += -O3
release : $(target)

clean:
	rm $(target)
