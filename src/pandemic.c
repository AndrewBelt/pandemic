#include <stdlib.h>
#include "pandemic.h"



void prey_callback(world_t *world, field_t *new_field)
{
	static const float difn_alpha = 1.0f;
	static const float growth_rate = 0.5f;
	static const float carrying_capacity = 1.0f;
	static const float predator_beta = 4.0f;
	
	const field_t *prey_field = &world->fields[PREY_POPULATION];
	const field_t *predator_field = &world->fields[PREDATOR_POPULATION];
	
	for (int y = 0; y < WORLD_H; y++)
	{
		for (int x = 0; x < WORLD_W; x++)
		{
			float u = field_get(prey_field, x, y);
			float lapl_u = field_laplacian(prey_field, x, y);
			float v = field_get(predator_field, x, y);
			float D_u = difn_alpha * lapl_u +
				growth_rate * u * (1.0f - u / carrying_capacity) -
				predator_beta * u * v;
			
			field_set(new_field, x, y, u + DELTA_TIME * D_u);
		}
	}
}

void predator_callback(world_t *world, field_t *new_field)
{
	static const float difn_alpha = 1.0f;
	static const float growth_rate = -0.1f;
	static const float carrying_capacity = 1.0f;
	static const float prey_beta = 1.0f;
	
	const field_t *prey_field = &world->fields[PREY_POPULATION];
	const field_t *predator_field = &world->fields[PREDATOR_POPULATION];
	
	for (int y = 0; y < WORLD_H; y++)
	{
		for (int x = 0; x < WORLD_W; x++)
		{
			float u = field_get(predator_field, x, y);
			float lapl_u = field_laplacian(predator_field, x, y);
			float v = field_get(prey_field, x, y);
			float D_u = difn_alpha * lapl_u +
				growth_rate * u * (1.0f - u / carrying_capacity) +
				prey_beta * u * v;
			
			field_set(new_field, x, y, u + DELTA_TIME * D_u);
		}
	}
}

void init_pandemic()
{
	// Set up callbacks
	field_callbacks[PREY_POPULATION] = prey_callback;
	field_callbacks[PREDATOR_POPULATION] = predator_callback;
}
