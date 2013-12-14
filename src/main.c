#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include "pandemic.h"


void draw_world(SDL_Texture *texture)
{
	Uint32 *pixels;
	int pitch;
	
	int err = SDL_LockTexture(texture, NULL, (void **) &pixels, &pitch);
	check(!err, "Could not lock texture");
	check(pitch == WORLD_W * sizeof(Uint32),
		"Pitch of texture is wrong size");
	
	for (int y = 0; y < WORLD_H; y++)
	{
		for (int x = 0; x < WORLD_W; x++)
		{
			union
			{
				Uint8 rgba[4];
				Uint32 val;
			} color;
			
			color.rgba[3] = field_get(fields[PREY_POPULATION], x, y) * 255;
			color.rgba[2] = clip(map(field_get(fields[ELEVATION], x, y),
				-500, 10000, 0, 255), 0, 255);
			color.rgba[1] = field_get(fields[PREDATOR_POPULATION], x, y) * 255;
			color.rgba[0] = 0;
			
			pixels[x + WORLD_W * y] = color.val;
		}
	}
	
	SDL_UnlockTexture(texture);
}

int main(int argc, const char **argv)
{
	srand(time(NULL));
	pandemic_init();
	
	// Set up world
	world_init();
	
	// Initial conditions
	// (define fields for t=0)
	
	field_load(fields[ELEVATION], "fields/elevation.bin");
	
	for (int i = 0; i < 20; i++)
	{
		field_set(fields[PREY_POPULATION],
			rand() % WORLD_W, rand() % WORLD_H, 1.0f);
		field_set(fields[PREDATOR_POPULATION],
			rand() % WORLD_W, rand() % WORLD_H, 1.0f);
	}
	
	// Set up SDL
	SDL_Init(SDL_INIT_VIDEO);
	
	// Create window
	const char *title = "pandemic";
	int width = WORLD_W;
	int height = WORLD_H;
	SDL_Window *window = SDL_CreateWindow(title,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_RESIZABLE);
	check(window, "Could not create window");
	
	// Create renderer
	Uint32 renderer_flags = SDL_RENDERER_ACCELERATED |
		SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, renderer_flags);
	check(renderer, "Could not create renderer");
	
	// Create world texture
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING, WORLD_W, WORLD_H);
	
	// Game loop
	bool running = true;
	for (int frame = 0; running; frame++)
	{
		// Handle events
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = false;
		}
		
		// Step the world
		world_step();
		
		// Render display
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		draw_world(texture);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
	
	// Shutdown
	SDL_Quit();
	world_destroy();
	return 0;
}