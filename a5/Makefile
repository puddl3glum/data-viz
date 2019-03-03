
CC = g++

target = raw2obj

srcs = src/main.cpp src/pixelmap.cpp src/vertex.cpp

# cflags = -fopenmp
# lflags = -lSDL2

$(target) : $(src)
	$(CC) $(cflags) $(lflags) -o $(target) $(srcs)

debug : cflags += -g
debug : $(target)

release : cflags += -O3
release : $(target)

clean:
	rm $(target)
