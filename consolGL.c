#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "convert_screen.h"
#include "console_screen.h"
#include "z_buffer_screen.h"
#include "consolGL.h"

int g_state_screen_close;

mat4_t g_mvp = {
	1.F , 0.F , 0.F , 0.F,
	0.F , 1.F , 0.F , 0.F,
	0.F , 0.F , 1.F , 0.F,
	0.F , 0.F , 0.F , 1.F
};
mat4_t g_model_scale = {
	1.F , 0.F , 0.F , 0.F,
	0.F , 1.F , 0.F , 0.F,
	0.F , 0.F , 1.F , 0.F,
	0.F , 0.F , 0.F , 1.F
};

mat4_t g_model_translate = {
	1.F , 0.F , 0.F , 0.F,
	0.F , 1.F , 0.F , 0.F,
	0.F , 0.F , 1.F , 0.F,
	0.F , 0.F , 0.F , 1.F
};

mat4_t g_model_rotate = {
	1.F , 0.F , 0.F , 0.F,
	0.F , 1.F , 0.F , 0.F,
	0.F , 0.F , 1.F , 0.F,
	0.F , 0.F , 0.F , 1.F
};


mat4_t g_view = {
	1.F , 0.F , 0.F , 0.F,
	0.F , 1.F , 0.F , 0.F,
	0.F , 0.F , 1.F , 0.F,
	0.F , 0.F , 0.F , 1.F
};

mat4_t g_projection = {
	1.F , 0.F , 0.F , 0.F,
	0.F , 1.F , 0.F , 0.F,
	0.F , 0.F , 1.F , 0.F,
	0.F , 0.F , 0.F , 1.F
};
//변환 적용 구현부

void scale_object(vec3_t v) 
{
	g_model_scale = scale(&g_model_scale , &v);
}

void translate_object(vec3_t v)
{
	g_model_translate = translate(&g_model_translate, &v);
}

void rotate_object(float angle, vec3_t v)
{
	g_model_rotate =  rotate(&g_model_rotate, angle, &v);

}

void view_cam(vec3_t eye, vec3_t center, vec3_t up)
{
	g_view = lookAt(&eye, &center, &up);
}

void projection_screen(float left, float right, float bottom, float top, float near_f, float far_f)
{
	g_projection = projection(left,right,bottom,top,near_f,far_f);
}


vec4_t* vertexArrayPoint;
void set_vertex_array(vec4_t* pointer)
{
	vertexArrayPoint = (vec4_t*)pointer;
}

void draw_line_legacy(vec4_t start_pont, vec4_t dst_point,float start_point_alpha,float dest_point_alpha)
{

	int dis_xi = (int)(dst_point.v[0] - start_pont.v[0]);
	int dis_yi = (int)(dst_point.v[1] - start_pont.v[1]);


	float cur_xf = start_pont.v[0];
	float cur_yf = start_pont.v[1];

	int totalDisi = absi(dis_xi) > absi(dis_yi) ? absi(dis_xi) : absi(dis_yi);

	float inc_xf = (float)dis_xi / totalDisi;
	float inc_yf = (float)dis_yi / totalDisi;

	for (int i = 0; i < totalDisi + 1; i++, cur_xf += inc_xf, cur_yf += inc_yf)
	{

		int put_xpos = (int)(cur_xf);
		int put_ypos = (int)(cur_yf);

		float alpha_weight = (float)i /(float) totalDisi;
		float alpha = (1.F - alpha_weight) * start_point_alpha + alpha_weight * dest_point_alpha;
		float z_value = (1.F - alpha_weight) * start_pont.z + alpha_weight * dst_point.z;
		char pixel = ASCII_BRIGHTNESS[(int)(alpha * 68) % 69];

		if (is_depth_test(put_xpos, put_ypos, z_value))
			push_pixel(pixel, put_xpos, put_ypos);
	}


}


void draw_polygon(vec4_t p1, vec4_t p2, vec4_t p3 , float p1_alpha , float p2_alpha , float p3_alpha)
{
	//edge line
	
	//bounding box
	float max_y = p1.y > p2.y ? p1.y : p2.y;
	max_y = p3.y > max_y ? p3.y : max_y;

	float max_x = p1.x > p2.x ? p1.x : p2.x;
	max_x = p3.x > max_x ? p3.x : max_x;

	float min_y = p1.y < p2.y ? p1.y : p2.y;
	min_y = p3.y < min_y ? p3.y : min_y;

	float min_x = p1.x < p2.x ? p1.x : p2.x;
	min_x = p3.x < min_x ? p3.x : min_x;
	
	

	//min_x 크기 만큼 더하고 min_y 크기 만큼 더해서0점 맞추기
	
	float width_f = max_x - min_x;
	float height_f = max_y - min_y;

	int width_i =(int) width_f;
	int height_i = (int)height_f;

	vec2_t line_p1p2_2f = { p2.x - p1.x , p2.y - p1.y }; // a , 1 , c  , 0 = ax -y +c 꼴의 선형방정식 a 기울기 c 
	vec2_t line_p2p3_2f = { p3.x - p2.x , p3.y - p2.y};
	vec2_t line_p3p1_2f = { p1.x - p3.x , p1.y - p3.y};
	
	vec2_t cur_point_2f;
	for (int cur_y = (int)min_y ; cur_y < (int)max_y; ++cur_y)
	{
		for (int cur_x = (int)min_x; cur_x < (int)max_x; ++cur_x)
		{
			cur_point_2f = (vec2_t){ (float)cur_x , (float)cur_y};

			vec2_t p1_cur_point_2f = { cur_point_2f.x - p1.x, cur_point_2f.y - p1.y};
			vec2_t p2_cur_point_2f = { cur_point_2f.x - p2.x, cur_point_2f.y - p2.y };
			vec2_t p3_cur_point_2f = { cur_point_2f.x - p3.x, cur_point_2f.y - p3.y };

			float edge1 = (line_p1p2_2f.x * p1_cur_point_2f.y) - (line_p1p2_2f.y * p1_cur_point_2f.x);
			float edge2 = (line_p2p3_2f.x * p2_cur_point_2f.y) - (line_p2p3_2f.y * p2_cur_point_2f.x);
			float edge3 = (line_p3p1_2f.x * p3_cur_point_2f.y) - (line_p3p1_2f.y * p3_cur_point_2f.x);

			if ((edge1 < 0) && (edge2 < 0) && (edge3 < 0))//Light Handle inner triangle Test
			{
			
				int put_xpos = (int)(cur_point_2f.x);
				int put_ypos = (int)(cur_point_2f.y);

				float len_p1_cur_f = length_v2(&p1_cur_point_2f);
				float len_p2_cur_f = length_v2(&p2_cur_point_2f);
				float len_p3_cur_f = length_v2(&p3_cur_point_2f);

				float total_len_f = len_p1_cur_f + len_p2_cur_f + len_p3_cur_f;
				float p1_weight_f = len_p1_cur_f / total_len_f;
				float p2_weight_f = len_p2_cur_f / total_len_f;
				float p3_weight_f = len_p3_cur_f / total_len_f;
				
				float alpha_weight =(cur_y - min_y) / height_f;
				float alpha = p1_weight_f * p1_alpha + p2_weight_f * p2_alpha + p3_weight_f* p3_alpha;
				float z_value = p1_weight_f * p1.z + p2_weight_f * p2.z + p3_weight_f * p3.z;
				char pixel = ASCII_BRIGHTNESS[(int)(alpha * 68) % 69];
				if (is_depth_test(put_xpos, put_ypos , z_value))
					push_pixel(pixel, put_xpos, put_ypos);
			}
				

		}
	}



}



void draw_vertex_array(int mode, int first, int count)
{

	if (mode == DRAW_LINE)
	{
		for (int idx = first; idx < count; idx++)
		{


			vec4_t p1 = vertexArrayPoint[idx];


			p1 = (vec4_t){ p1.x , p1.y  , p1.z  , 1.F};
			p1 = mul_v4m4(&p1, &g_model_scale);
			p1 = mul_v4m4(&p1, &g_model_rotate);
			p1 = mul_v4m4(&p1, &g_model_translate);
			


			vec4_t p2 = vertexArrayPoint[(idx + 1) % count];

			p2 = (vec4_t){ p2.x  , p2.y  , p2.z  , 1.F };
			p2 = mul_v4m4(&p2, &g_model_scale);
			p2 = mul_v4m4(&p2, &g_model_rotate);
			p2 = mul_v4m4(&p2, &g_model_translate);

			draw_line(get_screen_buffer(), get_screen_width(), get_screen_height(), &p2, &p1, p2.w , p1.w);



		}

	}
	else if (mode == DRAW_LINE_TRIANGLES)
	{

		for (int idx = first; idx < count; idx += 3)
		{


			vec4_t p1 = vertexArrayPoint[idx];
			float p1_alpha = p1.w;
			p1 = (vec4_t){ p1.x , p1.y  , p1.z  , 1.F };
			p1 = mul_v4m4(&p1, &g_model_scale);
			p1 = mul_v4m4(&p1, &g_model_rotate);
			p1 = mul_v4m4(&p1, &g_model_translate);
			p1 = mul_v4m4(&p1, &g_view);
			p1 = mul_v4m4(&p1, &g_projection);

			
			vec4_t p2 = vertexArrayPoint[(idx + 1) % count];
			float p2_alpha = p2.w;
			p2 = (vec4_t){ p2.x  , p2.y  , p2.z  , 1.F };
			p2 = mul_v4m4(&p2, &g_model_scale);
			p2 = mul_v4m4(&p2, &g_model_rotate);
			p2 = mul_v4m4(&p2, &g_model_translate);
			p2 = mul_v4m4(&p2, &g_view);
			p2 = mul_v4m4(&p2, &g_projection);
			vec4_t p3 = vertexArrayPoint[(idx + 2) % count];
			float p3_alpha = p3.w;
			p3 = (vec4_t){ p3.x  , p3.y  , p3.z  , 1.F };
			p3 = mul_v4m4(&p3, &g_model_scale);
			p3 = mul_v4m4(&p3, &g_model_rotate);
			p3 = mul_v4m4(&p3, &g_model_translate);
			p3 = mul_v4m4(&p3, &g_view);
			p3 = mul_v4m4(&p3, &g_projection);

			p1 = (vec4_t){ p1.x / p1.w , p1.y / p1.w  , p1.z  , 1.F };
			p2 = (vec4_t){ p2.x / p2.w  , p2.y / p2.w  , p2.z  , 1.F };
			p3 = (vec4_t){ p3.x / p3.w , p3.y / p3.w , p3.z  , 1.F };
			//projection을 통해 z축을 xy축에 반영 필요
			ivec2_t p1_v2i = {(int)p1.x , (int)p1.y};
			ivec2_t p2_v2i = { (int)p2.x , (int)p2.y };
			ivec2_t p3_v2i = { (int)p3.x , (int)p3.y };

			
			
			draw_line(get_screen_buffer(), get_screen_width(), get_screen_height(), &p1_v2i, &p2_v2i, p1_alpha, p2_alpha);
			draw_line(get_screen_buffer(), get_screen_width(), get_screen_height(), &p2_v2i, &p3_v2i, p2_alpha, p3_alpha);
			draw_line(get_screen_buffer(), get_screen_width(), get_screen_height(), &p3_v2i, &p1_v2i, p3_alpha, p1_alpha);
			
		}
	}
	else if (mode == DRAW_TRIANGLES)
	{
		for (int idx = first; idx < count; idx += 3)
		{


			vec4_t p1 = vertexArrayPoint[idx];
			float p1_alpha = p1.w;
			p1 = (vec4_t){ p1.x , p1.y  , p1.z  , 1.F };
			p1 = mul_v4m4(&p1, &g_model_scale);
			p1 = mul_v4m4(&p1, &g_model_rotate);
			p1 = mul_v4m4(&p1, &g_model_translate);
			p1 = mul_v4m4(&p1, &g_view);
			p1 = mul_v4m4(&p1, &g_projection);
			vec4_t p2 = vertexArrayPoint[(idx + 1) % count];
			float p2_alpha = p2.w;
			p2 = (vec4_t){ p2.x  , p2.y  , p2.z  , 1.F };
			p2 = mul_v4m4(&p2, &g_model_scale);
			p2 = mul_v4m4(&p2, &g_model_rotate);
			p2 = mul_v4m4(&p2, &g_model_translate);
			p2 = mul_v4m4(&p2, &g_view);
			p2 = mul_v4m4(&p2, &g_projection);
			vec4_t p3 = vertexArrayPoint[(idx + 2) % count];
			float p3_alpha = p3.w;
			p3 = (vec4_t){ p3.x  , p3.y  , p3.z  , 1.F };
			p3 = mul_v4m4(&p3, &g_model_scale);
			p3 = mul_v4m4(&p3, &g_model_rotate);
			p3 = mul_v4m4(&p3, &g_model_translate);
			p3 = mul_v4m4(&p3, &g_view);
			p3 = mul_v4m4(&p3, &g_projection);


			

			convert_screen_coordinate(p1.x , p1.y , &p1.x , &p1.y);
			convert_screen_coordinate(p2.x, p2.y, &p2.x, &p2.y);
			convert_screen_coordinate(p3.x, p3.y, &p3.x, &p3.y);


			draw_polygon(p1, p2, p3, p1_alpha, p2_alpha, p3_alpha);
			draw_line_legacy(p1 , p2 , p1_alpha, p2_alpha);
			draw_line_legacy(p2 , p3 , p2_alpha, p3_alpha);
			draw_line_legacy(p3 , p1 , p3_alpha, p1_alpha);
			

		}
	
	}
}




void draw_line(char* out_buffer, const size_t width, const size_t height, const ivec2_t* const start_point, const ivec2_t* const dest_point,const float start_point_alpha, const float dest_point_alpha)
{
	float dis_xf = (float)dest_point->v[0] - (float)start_point->v[0];
	float dis_yf = (float)dest_point->v[1] - (float)start_point->v[1];


	float cur_xf = (float)start_point->v[0];
	float cur_yf = (float)start_point->v[1];

	float totalDisi = absf(dis_xf) > absf(dis_yf) ? absf(dis_xf) : absf(dis_yf);

	float inc_xf = dis_xf / totalDisi;
	float inc_yf = dis_yf / totalDisi;

	for (int i = 0; i < (int)totalDisi+1; i++, cur_xf += inc_xf, cur_yf += inc_yf)
	{
		float origin_x = (float)width / 2;
		float origin_y = (float)height / 2;

		int put_xpos = (int)(cur_xf + origin_x);
		int put_ypos = (int)(cur_yf * (-1.F) + origin_y);
		float alpha_weight = i / totalDisi;
		float alpha = (1.F - alpha_weight) * start_point_alpha + alpha_weight * dest_point_alpha;

		char pixel = ASCII_BRIGHTNESS[(int)(alpha * 68) % 69];
		if (0 <= put_xpos && put_xpos < width && 0 <= put_ypos && put_ypos < (height-1))
		{
			out_buffer[put_ypos * (int)(width + 1) + put_xpos] = '*';
		}
	
	}



}
