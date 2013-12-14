#ifndef PANDEMIC_H
#define PANDEMIC_H

#include "config.h"


void pandemic_init();


// maths
float map(float in, float in_low, float in_high,
	float out_low, float out_high);
float clip(float in, float low, float high);


// field

float *field_create();
void field_load(float *field, const char *filename);
void field_save(float *field, const char *filename);
void field_destroy(float *field);

float inline field_get(const float *field, int x, int y)
{
	return field[x + WORLD_W * y];
}

void inline field_set(float *field, int x, int y, float value)
{
	field[x + WORLD_W * y] = value;
}

float field_get_safe(const float *field, int x, int y);
void field_set_safe(float *field, int x, int y, float value);
float field_laplacian(const float *field, int x, int y);
void field_print(const float *field);
float field_sum(const float *field);


// world

float *fields[FIELD_TYPE_LENGTH];
typedef void (*field_callback)(float *new_field);
field_callback field_callbacks[FIELD_TYPE_LENGTH];

void world_init();
void world_destroy();
void world_step();

#endif