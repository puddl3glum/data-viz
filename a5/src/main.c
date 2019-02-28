#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <stdint.h>

#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {

  char* filename;
  char opt;

  FILE* infile = stdin;

  uint8_t isovalue = 127;

  while ((opt = getopt(argc, argv, "f:i:")) != -1) {
    switch (opt) {
      case 'f': 
        filename = optarg;
        infile = fopen(filename, "rb");
        break;
      case 'i':
        isovalue = atoi(optarg);
      default:
        printf("USAGE: %s\n", argv[0]);
        break;
    }
  }

   if (optind >= argc) {
    fprintf(stderr, "Expected arguments after options\n");
    exit(EXIT_FAILURE);
  }


  const size_t width = strtoul(argv[0 + optind], NULL, 10);
  const size_t height = strtoul(argv[1 + optind], NULL, 10);


  SDL_Event event;
  SDL_Renderer* renderer;
  SDL_Window* window;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer((int) width, (int) height, 0, &window, &renderer);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);



  // calloc buffer
  uint8_t* data = calloc(height * width, sizeof(uint8_t));

  fread(data, sizeof(uint8_t), height * width, infile);
  fclose(infile);
 
  // apply threshold
  # pragma omp parallel for num_threads(get_nprocs()) collapse(2)
  for (size_t row = 0; row < height; row++) {
    for (size_t column = 0; column < width; column++ ) {
      // printf("%d ", data[row * height + column]);
      uint8_t brightness = data[row * height + column];
      data[row * height + column] = (brightness > isovalue) ? 1 : 0;
        // SDL_SetRenderDrawColor(renderer, brightness, brightness, brightness, 255);
    }
  }

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  for (size_t row = 0; row < height; row++) {
    for (size_t column = 0; column < width; column++ ) {
      // printf("%d ", data[row * height + column]);
      if ( data[row * height + column] )
        SDL_RenderDrawPoint(renderer, (int) row, (int) column);
    }
    // puts("");
  }

  SDL_RenderPresent(renderer);

  // wait until close
  while (event.type != SDL_QUIT) {
    SDL_PollEvent(&event);
    SDL_Delay(5);
  }

  // cleanup SDL
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  // cleanup
  free(data);
}

