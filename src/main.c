#include <config.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

/* static const int WIDTH = 800, HEIGHT = 600; */
static const int WIDTH = 256, HEIGHT = 256;

static int window(void);
static void repl(void);
static void parse(void);
static void open_document(void);
static void print_element_names(xmlNode *);
static int parse_example(void);
static int getopt_example(int, char**);

int main(int argc, char* argv[]){
  for (int i=0;i<argc;i++)
    printf("%s ", argv[i]);
  puts("");
  puts("This is " PACKAGE_STRING ".");
  getopt_example(argc, argv);
  repl();
  return 0;
}

static int getopt_example(int argc, char* argv[]) {
  int aflag = 0;
  int bflag = 0;
  char* cvalue = NULL;
  int index;
  int c;

  opterr = 0;

  while ((c = getopt (argc, argv, "abc:")) != -1)
    switch (c) {
      case 'a':
        aflag = 1;
        break;
      case 'b':
        bflag = 1;
        break;
      case 'c':
        cvalue = optarg;
        break;
      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        abort();
      }


  printf ("aflag = %d, bflag = %d, cvalue = %s\n",
          aflag, bflag, cvalue);

  for (index = optind; index < argc; index++)
    printf ("Non-option argument %s\n", argv[index]);
  return 0;
}

static void repl(void) {
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
      open_document();
    } else if (strcmp(inpt, "open2") == 0) {
      parse_example();
    } else if (strcmp(inpt, "window") == 0 ||
               strcmp(inpt, "win") == 0) {
      window();
    } else if (strcmp(inpt, "") != 0) {
      printf("%s\n", inpt);
    }
  }
}

static void print_element_names(xmlNode * a_node) {
   xmlNode *cur_node = NULL;

   for (cur_node = a_node; cur_node; cur_node =
      cur_node->next) {
      if (cur_node->type == XML_ELEMENT_NODE) {
         printf("node type: Element, name: %s\n",
            cur_node->name);
      }
      print_element_names(cur_node->children);
   }
}

static int parse_example(void) {
  char* url = "/home/alxbary/vm-shared-folder/toyprogram-art/tiles.tsx";
  xmlDoc *doc = NULL;
  xmlNode *root_element = NULL;
  LIBXML_TEST_VERSION
  if ((doc = xmlReadFile(url, NULL, 0)) == NULL){
    printf("error: could not parse file.\n");
    exit(-1);
  }

  root_element = xmlDocGetRootElement(doc);
  print_element_names(root_element);
  xmlFreeDoc(doc);
  xmlCleanupParser();
  return 0;
}

static void open_document(void) {
  xmlDocPtr document;
  LIBXML_TEST_VERSION
  document = xmlReadFile("/mnt/user/toyprogram-art/toyprogram-art/tiles.tsx", NULL, 0);
  xmlChar* filname = document->last->children->next->properties->children->content;
  xmlChar* tilewidth   = document->children->properties->next->next->next->children->content;
  xmlChar* tileheight   = document->children->properties->next->next->next->next->children->content;
  printf("%s,%s,%s\n", filname, tilewidth, tileheight);
  xmlFreeDoc(document);
  xmlCleanupParser();
}

static int window(void) {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Surface *screenSurface;
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    return 1;
  }

  if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    fprintf(stderr, "could not initialize sdl2_image: %s\n", IMG_GetError());
    return 1;
  }

  window = SDL_CreateWindow("Hello, World!",
			    SDL_WINDOWPOS_UNDEFINED,
			    SDL_WINDOWPOS_UNDEFINED,
			    WIDTH, HEIGHT,
			    SDL_WINDOW_ALLOW_HIGHDPI);
  if(window == NULL) {
    fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  screenSurface = SDL_GetWindowSurface(window);
  if (screenSurface == NULL) {
    fprintf(stderr, "could not get window: %s\n", SDL_GetError());
    return 1;
  }

  SDL_Surface *img = IMG_Load("/mnt/user/toyprogram-art/tropical-background.png");
  if (img == NULL) {
    fprintf(stderr, "could not load image: %s\n", IMG_GetError());
    return 1;
  }
  SDL_Surface* optimizedImg = SDL_ConvertSurface(img, screenSurface->format, 0);
  if (optimizedImg == NULL) fprintf(stderr, "could not optimize image: %s\n", SDL_GetError());
  SDL_FreeSurface(img);

  SDL_BlitSurface(optimizedImg, NULL, screenSurface, NULL);
  SDL_UpdateWindowSurface(window);

  /* SDL_Delay(2000); */

  SDL_FreeSurface(optimizedImg);

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

  SDL_FreeSurface(screenSurface);
  SDL_DestroyWindow(window);

  SDL_Quit();
  return 0;
}
