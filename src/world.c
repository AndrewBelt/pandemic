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
			// Create a new field to replace the old one
			float *new_field = field_create();
			
			for (int y = 0; y < WORLD_H; y++)
			{
				for (int x = 0; x < WORLD_W; x++)
				{
					float u = (*field_callbacks[i])(x, y);
					field_set(new_field, x, y, u);
				}
			}
			
			field_destroy(fields[i]);
			fields[i] = new_field;
		}
	}
}
