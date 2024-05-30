#include <config.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

const int WIDTH = 800, HEIGHT = 600;

int window(void);
void repl(void);
void parse(void);
void openDocument(void);

int main(void) {
  puts("Hello World!");
  puts("This is " PACKAGE_STRING ".");
  repl();
  return 0;
}

/**
 * Read evaluate print loop
 */
void repl(void) {
  char *inpt;
  int i = 0;
  while ( i < 10 ) {
    inpt = readline(">> ");
    if (!inpt) return;
    add_history(inpt);
    ++i;

    if (strcmp(inpt, "quit") == 0 ||
        strcmp(inpt, "exit") == 0) {
      break;
    } else if (strcmp(inpt, "open") == 0) {
      openDocument();
    } else if (strcmp(inpt, "window") == 0 ||
               strcmp(inpt, "win") == 0) {
      window();
    } else if (strcmp(inpt, "") != 0) {
      printf("%s\n", inpt);
    }
  }
}

/**
 * Parse the document
 */
void parse(void) {
  xmlDocPtr document;
  document = xmlReadFile("/home/alxbary/vm-shared-folder/toyprogram-art/tiles.tsx", NULL, 0);
  xmlChar* filname = document->last->children->next->properties->children->content;
  xmlChar* tilewidth   = document->children->properties->next->next->next->children->content;
  xmlChar* tileheight   = document->children->properties->next->next->next->next->children->content;
  printf("%s,%s,%s\n", filname, tilewidth, tileheight);
  xmlFreeDoc(document);
}

/**
 * Open the document
 */
void openDocument(void) {
  LIBXML_TEST_VERSION parse();
  xmlCleanupParser();
}

/**
 * Display window
 */
int window(void) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("SDL_Init failed: %s\n", SDL_GetError());
    return 1;
  }

  window = SDL_CreateWindow("Hello, World!",
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    WIDTH, HEIGHT,
			    SDL_WINDOW_ALLOW_HIGHDPI);
  if(window == NULL) {
    printf("Could not create window: %s\n", SDL_GetError());
    return 1;
  }
  
  renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderClear(renderer);

  SDL_RenderPresent(renderer);
  
  SDL_Event event;
  while(1) {
    if(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
        break;
      }
    }
  }

  SDL_DestroyWindow(window);

  SDL_Quit();
  return 0;
}
