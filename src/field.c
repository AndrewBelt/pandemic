#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <png.h>
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
	// Is the following correct?
	if (!(*bc)(x, y))
		return 0.0f;
	
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

void field_save_png(const float *field, const char *filename,
	float low, float high)
{
	typedef struct
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	} pixel_t;
	
	// Open file for writing
	FILE *file = fopen(filename, "wb");
	check(file, "Could not open '%s' for output", filename);
	
	// Set up PNG
	png_struct *png = png_create_write_struct(PNG_LIBPNG_VER_STRING,
		NULL, NULL, NULL);
	check(png, "Could not create png_struct");
	
	png_info *png_info = png_create_info_struct(png);
	
	if (!png_info)
	{
		png_destroy_write_struct(&png, NULL);
		check(false, "Could not create png_info");
	}
	
	if (setjmp(png_jmpbuf(png)))
	{
		png_destroy_write_struct(&png, &png_info);
		check(false, "libpng exception");
	}
	
	png_init_io(png, file);
	png_set_IHDR(png, png_info, WORLD_W, WORLD_H, 8, PNG_COLOR_TYPE_RGB_ALPHA,
		PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png, png_info);
	
	// Create pixel buffer
	pixel_t *buffer = calloc(WORLD_W * WORLD_H, sizeof(pixel_t));
	unsigned char *rows[WORLD_H];
	
	for (int y = 0; y < WORLD_H; y++)
	{
		for (int x = 0; x < WORLD_W; x++)
		{
			float value = field_get(field, x, y);
			uint8_t mapped_value = clip(map(value, low, high, 0, 256), 0, 255);
			
			pixel_t *pixel = &buffer[x + y * WORLD_W];
			pixel->r = 255;
			pixel->a = mapped_value;
		}
		
		rows[y] = (unsigned char *) &buffer[y * WORLD_W];
	}
	
	// Write PNG to file
	png_write_image(png, rows);
	
	// Cleanup
	free(buffer);
	png_write_end(png, NULL);
	png_destroy_write_struct(&png, &png_info);
	fclose(file);
}
