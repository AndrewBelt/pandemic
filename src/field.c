#include <stdlib.h>
#include <stdio.h>
#include "pandemic.h"


void field_init(field_t *field)
{
	field->values = calloc(WORLD_W * WORLD_H, sizeof(float));
}

void field_destroy(field_t *field)
{
	free(field->values);
}

float field_get(const field_t *field, int x, int y)
{
	// Compute x % WORLD_W quickly
	x &= WORLD_W_MASK;
	y &= WORLD_H_MASK;
	return field->values[x + WORLD_W * y];
}

void field_set(field_t *field, int x, int y, float value)
{
	x &= WORLD_W_MASK;
	y &= WORLD_H_MASK;
	field->values[x + WORLD_W * y] = value;
}

float field_laplacian(const field_t *field, int x, int y)
{
	return (field_get(field, x - 1, y) - 2 * field_get(field, x, y) +
			field_get(field, x + 1, y)) / (DELTA_X * DELTA_X) +
		(field_get(field, x, y - 1) - 2 * field_get(field, x, y) +
			field_get(field, x, y + 1)) / (DELTA_Y * DELTA_Y);
}

void field_debug_print(const field_t *field)
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

float field_sum(const field_t *field)
{
	float sum = 0.0f;
	for (int i = 0; i < WORLD_W * WORLD_H; i++)
	{
		sum += field->values[i];
	}
	return sum;
}
