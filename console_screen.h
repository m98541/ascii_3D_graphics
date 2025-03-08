#pragma once
#include <stdio.h>

#ifndef CONSOLE_SCREEN_H
#define CONSOLE_SCREEN_H

#if defined(DEBUG) || defined(_DEBUG)
char* get_screen_buffer(void);
#endif


extern const char ASCII_BRIGHTNESS[];
extern const char BIRGHTNESS_RANGE;


void init_screen(size_t width, size_t height);
void release_screen(void);

void push_pixel(char pixel, int x, int y);

void draw_screen(void);
void clear_screen(void);

size_t get_screen_width(void);
size_t get_screen_height(void);

#endif
