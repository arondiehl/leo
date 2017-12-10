#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_scancode.h>


/*
static const char *blackkeys[]	= {"0"};
static const char *whitekeys[]	= {"Space"};

static const char *redkeys[]	= {"1", "7", "R"};
static const char *greenkeys[]	= {"2", "8", "T"};
static const char *bluekeys[]	= {"3","9", "Z"};

static const char *cyankeys[]		= {"4", "Q", "U"};
static const char *magentakeys[]	= {"5", "W", "I"};
static const char *yellowkeys[]		= {"6","E", "O"};

#define NHASH	251
*/
typedef struct color Color;
struct color {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

Color black	= {0, 0, 0};
Color white	= {255, 255, 255};

Color red 	= {255, 0, 0};
Color green	= {0, 255, 0};
Color blue	= {0, 0, 255};

Color cyan	= {0, 255, 255};
Color magenta	= {255, 0, 255};
Color yellow	= {255, 255, 0};

typedef struct keycolor Keycolor;
struct keycolor {
	const char	*key;
	Color		color;
	Keycolor	*next;
};

/*
enum { MULTIPLIER = 31 };
Keycolor	*symtab[NHASH];
*/

static Color keys[SDL_NUM_SCANCODES];

typedef struct leo Leo;
struct leo {
	SDL_Renderer *renderer;
};

void leohandlekey(Leo *leo, SDL_Scancode scancode);
void leosetbgcol(Leo *leo, Color c);
//Keycolor* lookup(const char *key, bool create, Color color);
uint8_t hash(const char *str);


/*
void associate(const char **keys, size_t len, Color color) {
	while (len--)
		lookup(*keys++, true, color);
}
*/

int main(int argc, char *argv[]) {
	Leo		leo;
	SDL_Window	*window		= NULL;
	SDL_Surface	*surface	= NULL;

	for (int i=0; i < SDL_NUM_SCANCODES; i++) {
		Color c;
		switch (i%8) {
			case 0: c=black;	break;
			case 1: c=white;	break;

			case 2: c=red;		break;
			case 3: c=green;	break;
			case 4: c=blue;		break;

			case 5: c=cyan;		break;
			case 6: c=magenta;	break;
			case 7: c=yellow;	break;
		}

		keys[i] = c;
	}

	/*
	associate(blackkeys, sizeof(blackkeys)/sizeof(blackkeys[0]), black);
	associate(whitekeys, sizeof(whitekeys)/sizeof(whitekeys[0]), white);
	associate(greenkeys, sizeof(greenkeys)/sizeof(greenkeys[0]), green);
	associate(redkeys, sizeof(redkeys)/sizeof(redkeys[0]), red);
	associate(bluekeys, sizeof(bluekeys)/sizeof(bluekeys[0]), blue);
*/
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL error initializing: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	window = SDL_CreateWindow("Leo",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			640,
			480,
			SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (window == NULL) {
		fprintf(stderr, "SDL error creating window: %s\n", SDL_GetError());
		return EXIT_FAILURE;
	}

	leo.renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(leo.renderer, 0, 0, 0, 255);
	SDL_RenderClear(leo.renderer);
	SDL_RenderPresent(leo.renderer);


	while (1) {
		SDL_Event e;
		SDL_PollEvent(&e);
		if (e.type == SDL_QUIT)
			break;
		if (e.type == SDL_KEYDOWN) {
			leohandlekey(&leo, e.key.keysym.scancode);
		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}

void leohandlekey(Leo *leo, SDL_Scancode scancode) {
	leosetbgcol(leo, keys[scancode]);
}

void leosetbgcol(Leo *leo, Color c) {
		SDL_SetRenderDrawColor(leo->renderer, c.r, c.g, c.b, 255);
		SDL_RenderClear(leo->renderer);
		SDL_RenderPresent(leo->renderer);
}

/*
Keycolor* lookup(const char *key, bool create, Color color) {
	int h;
	Keycolor *sym;
	
	h = hash(key);
	for (sym = symtab[h]; sym != NULL; sym = sym->next)
		if (strcmp(key, sym->key) == 0)
			return sym;
	if (create) {
		if ((sym = malloc(sizeof(Keycolor))) == NULL)
			return NULL;
		sym->key = key;
		sym->color = color;
		sym->next = symtab[h];
		symtab[h] = sym;
	}
	return sym;
}

uint8_t hash(const char *str) {
	uint8_t	h;
	uint8_t *p;

	h = 0;
	for (p = (uint8_t*) str; *p != '\0'; p++)
		h = MULTIPLIER * h + *p;
	return h % NHASH;
}
*/
