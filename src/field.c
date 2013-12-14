#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "pandemic.h"


float *field_create()
{
	return calloc(WORLD_W * WORLD_H, sizeof(float));
}

void field_load(float *field, const char *filename)
{
	uint32_t size[2];
	FILE *file = fopen(filename, "r");
	fread(size, sizeof(uint32_t), 2, file);
	
	if (size[0] != WORLD_W || size[1] != WORLD_H)
	{
		fprintf(stderr, "field file of size [%u, %u], expected [%u, %u]\n",
			size[0], size[1], WORLD_W, WORLD_H);
		abort();
	}
	
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
	return (field_get_safe(field, x - 1, y) - 2 * field_get(field, x, y) +
			field_get_safe(field, x + 1, y)) / (DELTA_X * DELTA_X) +
		(field_get_safe(field, x, y - 1) - 2 * field_get(field, x, y) +
			field_get_safe(field, x, y + 1)) / (DELTA_Y * DELTA_Y);
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
