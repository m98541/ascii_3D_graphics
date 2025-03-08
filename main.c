#include "consolGL.h"
#include "console_screen.h"
#include "z_buffer_screen.h"
#include <math.h>
#include <conio.h>
#define SCREEN_WIDTH 100
#define SCREEN_HEGITH 100

float dx = 0;
float dy = 0;
float dz = 0;
float angle_y = 0.F;
float angle_z = 0.F;
float angle_x = 0.F;
float camAngle_x = 0.785398F;
float camAngle_y = 0.785398F;
float camAngle_z = 0.785398F;
float cam_r = 5.F;

vec3_t eye = { 0.F , 5.F , -10.F };
vec3_t center = { 0.F , 0.F  , 0.F };
vec3_t up = { 0.F , 1.F , 0.F };


void eventCall()
{

	int key = getch();
	if (key == 'w')
		dy += 1.F;
	else if (key == 's')
		dy -= 1.F;
	else if (key == 'a')
		dx -= 1.F;
	else if (key == 'd')
		dx += 1.F;
	else if (key == 'q')
		angle_y += -0.1F;
	else if (key == 'e')
		angle_y += +0.1F;
	else if (key == 'r')
	{
		dz += 1.F;
	}
	
	

	vec3_t rotate_v = { 0.F , 1.F , 0.F };
	
	vec3_t translate = {dx , dy , dz};

	rotate_object(angle_y , rotate_v);

	translate_object(translate);

	

}

int main(void)
{

	vec4_t tri[3] = {
			{ -1.F  , 0.F ,0.F ,1.F},
			{ 0.F ,  1.F ,0.F ,1.F},
			{ 1.F , 0.F,0.F ,1.F},
	};

	vec4_t cube[8] = {

		{ -1.F  , -1.F ,-1.F ,0.1},
		{ -1.F  , -1.F ,1.F ,0.1F},
		{ -1.F  , 1.F ,-1.F ,1.F},
		{ -1.F  , 1.F ,1.F ,1.F},

		{ 1.F  , -1.F ,-1.F ,0.1F},
		{ 1.F  , -1.F ,1.F ,0.1F},
		{ 1.F  , 1.F ,-1.F ,1.F},
		{ 1.F  , 1.F ,1.F ,1.F},
	};


	int indices[36] = {
		//p1 , p2 , p3
		//bottom
		5,4,0,  // 0 1 2
		5,0,1,  // 3 4 5

		//back
		7,5,1,	// 6 7 8
		7,1,3,	// 9 10 11

		//top
		6,7,3,	//12 13 14
		6,3,2,	//15 16 17

		//front
		4,6,2,	//18 19 20
		4,2,0,	//21 22 23

		//left
		3,1,0,	//24 25 26
		3,0,2,	//27 28 29

		//right
		6,4,5,	//30 31 32
		6,5,7	//33 34 35
	};

	vec4_t cube_vertex_buf[36];

	for (int idx = 0; idx < 36; idx++)
	{
		cube_vertex_buf[idx] = cube[indices[idx]];
	}
	init_screen(SCREEN_WIDTH, SCREEN_HEGITH);
	init_depth_buffer(SCREEN_WIDTH , SCREEN_HEGITH);
	
	
	scale_object((vec3_t) {10.F ,10.F ,10.F});
	view_cam(eye, center, up);
	projection_screen(-(float)SCREEN_WIDTH/2.F, (float)SCREEN_WIDTH / 2.F ,-(float)SCREEN_HEGITH/2.F, (float)SCREEN_HEGITH/2.F, -30.F, 80.F);

	while (1)
	{
		set_vertex_array(cube_vertex_buf);
		//translate_object((vec3_t) {-15.F, 0.F, 0.F });
		draw_vertex_array(DRAW_TRIANGLES, 0, 36);



		draw_screen();
		
		clear_depth_buffer();
		clear_screen();
		eventCall();

		
	}

	release_depth_buffer();
	release_screen();
	return 0;
}