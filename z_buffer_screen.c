#include "z_buffer_screen.h"
#include <stdlib.h>
#include <assert.h>



size_t g_width_depth = 0;
size_t g_height_depth = 0;
size_t g_pitch_depth = 0;

float* g_depth_buffer = NULL;



void init_depth_buffer(size_t width, size_t height)
{

	g_width_depth = width;
	g_height_depth = height;
	g_pitch_depth = width + 1;

	g_depth_buffer = malloc(sizeof(float) * g_pitch_depth * height);
	assert(g_depth_buffer != NULL);

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			g_depth_buffer[i * g_pitch_depth + j] = 1.F;
		}
		
		g_depth_buffer[i * g_pitch_depth + width] = -1.F;
	}

}

void clear_depth_buffer()
{
	for (size_t i = 0; i < g_height_depth; i++)
	{
		for (size_t j = 0; j < g_width_depth; j++)
		{
			g_depth_buffer[i * g_pitch_depth + j] = 1.F;
		}

		g_depth_buffer[i * g_pitch_depth + g_width_depth] = -1.F;
	}
}

void release_depth_buffer()
{
	free(g_depth_buffer);
}

void set_depth_buffer(int x, int y, float z_value)
{
	if (0 <= x && x < g_width_depth && 0 <= y && y < g_height_depth)
	{
		g_depth_buffer[y * g_pitch_depth + x] = z_value;
	}
}

float get_depth_buffer(int x, int y)
{
	if (0 <= x && x < g_width_depth && 0 <= y && y < g_height_depth)
	{
		return g_depth_buffer[y * g_pitch_depth + x];
	}
	else
	{
		return -1.F;
	}
}

int is_depth_test(int x, int y,float new_z_value)
{
	if (0 <= x && x < g_width_depth && 0 <= y && y < g_height_depth)
	{
		if (new_z_value < g_depth_buffer[y * g_pitch_depth + x] && new_z_value > -1.F && new_z_value < 1.F)
		{
			g_depth_buffer[y * g_pitch_depth + x] = new_z_value;
			return DEPTH_FRONT;
		}
		else
		{
			return DEPTH_BACK;
		}
	
	}
	else
	{
		return DEPTH_BACK;
	}
}