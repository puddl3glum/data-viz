
CC = g++

target = raw2obj

headers = src/pixelmap.hpp src/vertex.hpp src/line.hpp
srcs = src/main.cpp src/pixelmap.cpp src/vertex.cpp src/line.cpp

# cflags = -fopenmp
cflags = -Wall
# lflags = -lSDL2

$(target) : $(srcs) $(headers)
	$(CC) $(cflags) $(lflags) -o $(target) $(srcs)

debug : cflags += -g
debug : $(target)

release : cflags += -O3
release : $(target)

clean:
	rm $(target)
