#include <stdlib.h>
#include "pandemic.h"


void prey_callback(float *new_field)
{
	static const float difn_alpha = 2.0f;
	static const float growth_rate = 2.0f;
	static const float carrying_capacity = 1.0f;
	static const float predator_beta = 4.0f;
	
	for (int y = 0; y < WORLD_H; y++)
	{
		for (int x = 0; x < WORLD_W; x++)
		{
			float u = field_get(fields[PREY_POPULATION], x, y);
			float lapl_u = field_laplacian(fields[PREY_POPULATION], x, y);
			float v = field_get(fields[PREDATOR_POPULATION], x, y);
			float D_u = difn_alpha * lapl_u +
				growth_rate * u * (1.0f - u / carrying_capacity) -
				predator_beta * u * v;
			
			field_set(new_field, x, y, u + DELTA_TIME * D_u);
		}
	}
}

void predator_callback(float *new_field)
{
	static const float difn_alpha = 1.0f;
	static const float growth_rate = -0.1f;
	static const float carrying_capacity = 1.0f;
	static const float prey_beta = 1.0f;
	
	for (int y = 0; y < WORLD_H; y++)
	{
		for (int x = 0; x < WORLD_W; x++)
		{
			float u = field_get(fields[PREDATOR_POPULATION], x, y);
			float lapl_u = field_laplacian(fields[PREDATOR_POPULATION], x, y);
			float v = field_get(fields[PREY_POPULATION], x, y);
			float D_u = difn_alpha * lapl_u +
				growth_rate * u * (1.0f - u / carrying_capacity) +
				prey_beta * u * v;
			
			field_set(new_field, x, y, u + DELTA_TIME * D_u);
		}
	}
}

void pandemic_init()
{
	// Set up callbacks
	field_callbacks[ELEVATION] = NULL;
	field_callbacks[PREY_POPULATION] = prey_callback;
	field_callbacks[PREDATOR_POPULATION] = predator_callback;
}
