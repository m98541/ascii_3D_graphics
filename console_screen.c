#include "console_screen.h"

#include <stdlib.h>
#include <assert.h>


const long* const TERMINATE_STR = "\x1B[H";
const char ASCII_BRIGHTNESS[] = ".'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
const char BIRGHTNESS_RANGE = sizeof(ASCII_BRIGHTNESS);

char* g_output_buffer = NULL;
size_t g_width = 0;
size_t g_height = 0;
size_t g_pitch = 0;
char* get_screen_buffer(void)
{
	return g_output_buffer;
}

void init_screen(size_t width, size_t height)
{
	g_width = width;
	g_height = height;
	g_pitch = width + 1;
	g_output_buffer = malloc(g_pitch * height + 3);
	assert(g_output_buffer != NULL);

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			g_output_buffer[i * g_pitch + j] = '.';
		}
		g_output_buffer[i * g_pitch + width] = '\n';
	}
#ifndef __APPLE__
	*(long*)(g_output_buffer + height * g_pitch -1) = *TERMINATE_STR;
#else
	g_output_buffer[height * g_pitch] = 0;
#endif
}

void release_screen(void)
{
	free(g_output_buffer);
}

void push_pixel(char pixel, int x, int y)
{
	if (0 <= x && x < g_width && 0 <= y && y < g_height)
	{
		g_output_buffer[y * g_pitch + x] = pixel;
	}
}

void draw_screen(void)
{
	fputs(g_output_buffer, stdout);
}

void clear_screen(void)
{
	for (size_t i = 0; i < g_height; i++)
	{
		for (size_t j = 0; j < g_width; j++)
		{
			g_output_buffer[i * g_pitch + j] = '.';
		}
	}
}

size_t get_screen_width(void)
{
	return g_width;
}

size_t get_screen_height(void)
{
	return g_height;
}
