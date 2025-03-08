#pragma once

#define DRAW_LINE (0)
#define DRAW_LINE_TRIANGLES (1)
#define DRAW_TRIANGLES (2)

#include "affine_math.h"


void scale_object(vec3_t v);
void translate_object(vec3_t v);
void rotate_object(float angle, vec3_t v);

void view_cam(vec3_t eye, vec3_t center, vec3_t up);
void projection_screen(float left, float right, float bottom, float top, float near, float far);


void set_vertex_array(vec4_t* pointer);
void draw_vertex_array(int mode, int first, int count);

void draw_line(char* out_buffer, const size_t width, const size_t height, const ivec2_t* const start_point, const ivec2_t* const dest_point,const float start_point_alpha, const float dest_point_alpha);