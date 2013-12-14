#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "pandemic.h"


// This file includes general stuff that doesn't belong anywhere

void check(bool success, const char *err_format, ...)
{
	va_list vl;
	va_start(vl, err_format);
	
	if (!success)
	{
		if (err_format)
		{
			vfprintf(stderr, err_format, vl);
			fprintf(stderr, "\n");
		}
		
		abort();
	}
	
	va_end(vl);
}

bool land_boundary(int x, int y)
{
	return field_get_safe(fields[ELEVATION], x, y) > -490.0f;
}

void prey_callback(float *new_field, int x, int y)
{
	static const float difn_alpha = 2.0f;
	static const float growth_rate = 2.0f;
	static const float carrying_capacity = 1.0f;
	static const float predator_beta = 4.0f;
	
	float u = field_get(fields[PREY_POPULATION], x, y);
	float lapl_u = field_laplacian_boundary(fields[PREY_POPULATION],
		x, y, land_boundary);
	float v = field_get(fields[PREDATOR_POPULATION], x, y);
	float D_u = difn_alpha * lapl_u +
		growth_rate * u * (1.0f - u / carrying_capacity) -
		predator_beta * u * v;
	
	field_set(new_field, x, y, u + DELTA_TIME * D_u);
}

void predator_callback(float *new_field, int x, int y)
{
	static const float difn_alpha = 1.0f;
	static const float growth_rate = -0.1f;
	static const float carrying_capacity = 1.0f;
	static const float prey_beta = 1.0f;
	
	float u = field_get(fields[PREDATOR_POPULATION], x, y);
	float lapl_u = field_laplacian(fields[PREDATOR_POPULATION], x, y);
	float v = field_get(fields[PREY_POPULATION], x, y);
	float D_u = difn_alpha * lapl_u +
		growth_rate * u * (1.0f - u / carrying_capacity) +
		prey_beta * u * v;
	
	field_set(new_field, x, y, u + DELTA_TIME * D_u);
}

void pandemic_init()
{
	// Set up callbacks
	field_callbacks[ELEVATION] = NULL;
	field_callbacks[PREY_POPULATION] = prey_callback;
	field_callbacks[PREDATOR_POPULATION] = predator_callback;
}
