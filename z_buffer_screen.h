#pragma once
#define DEPTH_FRONT (1)
#define DEPTH_BACK (0)

void init_depth_buffer(size_t width, size_t height);// 
void clear_depth_buffer();
void release_depth_buffer();
void set_depth_buffer(int x ,int y,float z_value);
float get_depth_buffer( int x, int y);
int is_depth_test(int x, int y,float new_z_value);