#ifndef PANDEMIC_H
#define PANDEMIC_H

#include <stdbool.h>
#include "config.h"


// pandemic.c

/** Aborts with an error message if success is false
*/
void check(bool success, const char *err_format, ...);

// maths.c

float map(float in, float in_low, float in_high,
	float out_low, float out_high);
float clip(float in, float low, float high);


// field.c

float *field_create();
void field_load(float *field, const char *filename);
void field_save(float *field, const char *filename);
void field_destroy(float *field);

static inline float field_get(const float *field, int x, int y)
{
	return field[x + WORLD_W * y];
}

static inline void field_set(float *field, int x, int y, float value)
{
	field[x + WORLD_W * y] = value;
}

/** Wraps out-of-bounds coordinates
*/
float field_get_safe(const float *field, int x, int y);
void field_set_safe(float *field, int x, int y, float value);
float field_laplacian(const float *field, int x, int y);

/** Callback function for boundaries testing
	Returns whether the given coordinates are within the boundary.
	The coordinates may not be in bounds, so you should use "safe"
	field accessor methods.
*/
typedef bool (*boundary_callback)(int x, int y);

/** Computes the Laplacian with a boundary condition u'(x, y) = 0
*/
float field_laplacian_boundary(const float *field, int x, int y,
	boundary_callback bc);

/** Prints a grid of numbers to stdout
	This is probably only useful for debugging small (~32 x 32) grids.
*/
void field_print(const float *field);
float field_sum(const float *field);


// world.c

float *fields[FIELD_TYPE_LENGTH];
// float *populations[POPULATION_TYPE_LENGTH];

typedef float (*field_callback)(int x, int y);
field_callback field_callbacks[FIELD_TYPE_LENGTH];

void world_init();
void world_destroy();
void world_add_field();
void world_step();

// model.c

void init_model();


#endif