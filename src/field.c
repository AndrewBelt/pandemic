#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "pandemic.h"


float *field_create()
{
	return calloc(WORLD_W * WORLD_H, sizeof(float));
}

void field_load(float *field, const char *filename)
{
	uint32_t size[2];
	FILE *file = fopen(filename, "r");
	check(file, "Could not open '%s'\n", filename);
	
	fread(size, sizeof(uint32_t), 2, file);
	check(size[0] == WORLD_W && size[1] == WORLD_H,
		"field file of size [%u, %u], expected [%u, %u]\n",
		size[0], size[1], WORLD_W, WORLD_H);
	
	fread(field, sizeof(float), WORLD_W * WORLD_H, file);
}

void field_save(float *field, const char *filename)
{
	FILE *file = fopen(filename, "w");
	uint32_t size[2] = {WORLD_W, WORLD_H};
	fwrite(size, sizeof(uint32_t), 2, file);
	fwrite(field, sizeof(float), WORLD_W * WORLD_H, file);
}

void field_destroy(float *field)
{
	free(field);
}

float field_get_safe(const float *field, int x, int y)
{
	x &= WORLD_W - 1;
	y &= WORLD_H - 1;
	return field_get(field, x, y);
}

void field_set_safe(float *field, int x, int y, float value)
{
	x &= WORLD_W - 1;
	y &= WORLD_H - 1;
	field_set(field, x, y, value);
}

float field_laplacian(const float *field, int x, int y)
{
	float west = field_get_safe(field, x - 1, y);
	float east = field_get_safe(field, x + 1, y);
	float north = field_get_safe(field, x, y - 1);
	float south = field_get_safe(field, x, y + 1);
	float u = field_get(field, x, y);
	
	return (west - 2 * u + east) / (DELTA_X * DELTA_X) +
		(north - 2 * u + south) / (DELTA_Y * DELTA_Y);
}

float field_laplacian_boundary(const float *field, int x, int y,
	boundary_callback bc)
{
	bool has_west = (*bc)(x - 1, y);
	bool has_east = (*bc)(x + 1, y);
	bool has_north = (*bc)(x, y - 1);
	bool has_south = (*bc)(x, y + 1);
	
	float west = field_get_safe(field, x - 1, y);
	float east = field_get_safe(field, x + 1, y);
	float north = field_get_safe(field, x, y - 1);
	float south = field_get_safe(field, x, y + 1);
	
	float ddx = -2 * field_get(field, x, y);
	float ddy = ddx;
	
	// This is ugly
	
	if (has_west && has_east)
	{
		ddx += west + east;
	}
	else if (has_east)
	{
		ddx += 2 * east;
	}
	else if (has_west)
	{
		ddx += 2 * west;
	}
	else
	{
		ddx = 0;
	}
	
	if (has_north && has_south)
	{
		ddy += north + south;
	}
	else if (has_south)
	{
		ddy += 2 * south;
	}
	else if (has_north)
	{
		ddy += 2 * north;
	}
	else
	{
		ddy = 0;
	}
	
	return ddx / (DELTA_X * DELTA_X) + ddy / (DELTA_Y * DELTA_Y);
}

void field_print(const float *field)
{
	for (int y = 0; y < WORLD_H; y++)
	{
		for (int x = 0; x < WORLD_W; x++)
		{
			printf("%6.2f ", field_get(field, x, y));
		}
		printf("\n");
	}
}

float field_sum(const float *field)
{
	float sum = 0.0f;
	for (int i = 0; i < WORLD_W * WORLD_H; i++)
	{
		sum += field[i];
	}
	return sum;
}
