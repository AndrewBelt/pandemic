#include <stdlib.h>
#include "pandemic.h"


void world_init(world_t *world)
{
	for (int i = 0; i < FIELD_TYPE_LENGTH; i++)
	{
		field_init(&world->fields[i]);
	}
}

void world_destroy(world_t *world)
{
	for (int i = 0; i < FIELD_TYPE_LENGTH; i++)
	{
		field_destroy(&world->fields[i]);
	}
}

void world_step(world_t *world)
{
	for (int i = 0; i < FIELD_TYPE_LENGTH; i++)
	{
		// Create a new field
		field_t *field = &world->fields[i];
		field_t new_field;
		field_init(&new_field);
		
		(*field_callbacks[i])(world, &new_field);
		
		// Copy the field by value to the world
		field_destroy(field);
		world->fields[i] = new_field;
	}
}
