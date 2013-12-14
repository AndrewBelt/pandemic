#include <stdlib.h>
#include "pandemic.h"


void world_init()
{
	for (int i = 0; i < FIELD_TYPE_LENGTH; i++)
	{
		fields[i] = field_create();
	}
}

void world_destroy()
{
	for (int i = 0; i < FIELD_TYPE_LENGTH; i++)
	{
		field_destroy(fields[i]);
	}
}

void world_step()
{
	for (int i = 0; i < FIELD_TYPE_LENGTH; i++)
	{
		if (field_callbacks[i])
		{
			// Create a new field
			float *new_field = field_create();
			
			(*field_callbacks[i])(new_field);
			
			field_destroy(fields[i]);
			fields[i] = new_field;
		}
	}
}
