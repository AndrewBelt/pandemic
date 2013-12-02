#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include "pandemic.h"

void check(bool success, const char *err_message)
{
	if (!success)
	{
		if (err_message)
			fprintf(stderr, "%s\n", err_message);
		
		abort();
	}
}


void draw_world(world_t *world, SDL_Texture *texture)
{
	Uint32 *pixels;
	int pitch;
	
	int err = SDL_LockTexture(texture, NULL, (void **) &pixels, &pitch);
	check(!err, "Could not lock texture");
	check(pitch == WORLD_W * sizeof(Uint32),
		"Pitch of texture is wrong size");
	
	field_t *prey_field = &world->fields[PREY_POPULATION];
	field_t *predator_field = &world->fields[PREDATOR_POPULATION];
	
	for (int y = 0; y < WORLD_H; y++)
	{
		for (int x = 0; x < WORLD_W; x++)
		{
			Uint8 r = (Uint8) (field_get(prey_field, x, y) * 255);
			Uint8 g = (Uint8) (field_get(predator_field, x, y) * 255);
			pixels[x + WORLD_W * y] = ((Uint32) r) << 24 | ((Uint32) g) << 16;
		}
	}
	
	SDL_UnlockTexture(texture);
}


int main(int argc, const char **argv)
{
	init_pandemic();
	srand(time(NULL));
	
	// Set up world
	world_t world;
	world_init(&world);
	
	// Initial condition
	for (int i = 0; i < 5; i++)
	{
		field_set(&world.fields[PREY_POPULATION],
			rand() % WORLD_W, rand() % WORLD_H, 1.0f);
		field_set(&world.fields[PREDATOR_POPULATION],
			rand() % WORLD_W, rand() % WORLD_H, 1.0f);
	}
	
	// Set up SDL
	SDL_Init(SDL_INIT_VIDEO);
	
	// Create window
	const char *title = "pandemic";
	int width = 600;
	int height = 600;
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
	while (running)
	{
		// Handle events
		SDL_Event event;
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = false;
		}
		
		// Step the world
		world_step(&world);
		
		// Render display
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		draw_world(&world, texture);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
	
	// Shutdown
	SDL_Quit();
	world_destroy(&world);
	return 0;
}