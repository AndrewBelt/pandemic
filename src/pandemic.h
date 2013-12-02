#ifndef PANDEMIC_H
#define PANDEMIC_H

#include "config.h"


void init_pandemic();


// field

#define WORLD_W_MASK (WORLD_W - 1)
#define WORLD_H_MASK (WORLD_H - 1)

typedef struct
{
	float *values;
} field_t;

void field_init(field_t *field);
void field_destroy(field_t *field);
float field_get(const field_t *field, int x, int y);
void field_set(field_t *field, int x, int y, float value);
void field_incr(field_t *field, int x, int y, float value);
float field_laplacian(const field_t *field, int x, int y);
void field_debug_print(const field_t *field);
float field_sum(const field_t *field);


// world

typedef struct
{
	field_t fields[FIELD_TYPE_LENGTH];
} world_t;

typedef void (*field_callback)(world_t *world, field_t *new_field);
field_callback field_callbacks[FIELD_TYPE_LENGTH];

void world_init(world_t *world);
void world_destroy(world_t *world);
void world_step(world_t *world);

#endif