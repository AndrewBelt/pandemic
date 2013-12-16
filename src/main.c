#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "pandemic.h"


int main(int argc, const char **argv)
{
	// Set up world
	world_init();
	model_init();
	
	// Evolve the world
	for (int i = 0; i < 256; i++)
	{
		printf(".");
		fflush(stdout);
		world_step();
	}
	printf("\n");
	
	// Save PNG results
	field_save_png(fields[ELEVATION],
		"output/elevation.png", -490, 2500);
	field_save_png(fields[PREY_POPULATION],
		"output/prey_population.png", 0.0, 1.0);
	field_save_png(fields[PREDATOR_POPULATION],
		"output/predator_population.png", 0.0, 1.0);
	
	// Cleanup
	world_destroy();
	return 0;
}