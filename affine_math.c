#include <math.h>
#include <stdio.h>
#include "affine_math.h"


vec4_t add_v4v4(const vec4_t* v1,const vec4_t* v2)
{
	return (vec4_t) { v1->x + v2->x , v1->y + v2->y , v1->z + v2->z , v1->w + v2->w };
}

vec3_t add_v3v3(const vec3_t* v1, const vec3_t* v2)
{
	return (vec3_t) { v1->x + v2->x, v1->y + v2->y, v1->z + v2->z};
}

vec4_t sub_v4v4(const vec4_t* v1, const vec4_t* v2)
{
	return (vec4_t) { v1->x - v2->x, v1->y - v2->y, v1->z - v2->z, v1->w - v2->w };
}

vec3_t sub_v3v3(const vec3_t* v1, const vec3_t* v2)
{
	 vec3_t re =  { v1->x - v2->x, v1->y - v2->y, v1->z - v2->z };
	 return re;
}



vec4_t mul_m4v4(const mat4_t* m,const vec4_t* v)
{
	vec4_t re = {
		m->m[0][0] * v->x + m->m[0][1] * v->y + m->m[0][2] * v->z + m->m[0][3] * v->w,

		m->m[1][0] * v->x + m->m[1][1] * v->y + m->m[1][2] * v->z + m->m[1][3] * v->w,

		m->m[2][0] * v->x + m->m[2][1] * v->y + m->m[2][2] * v->z + m->m[2][3] * v->w,

		m->m[3][0] * v->x + m->m[3][1] * v->y + m->m[3][2] * v->z + m->m[3][3] * v->w
	};

	return re;
}


vec4_t mul_v4m4(const vec4_t* v,const mat4_t* m)
{
	vec4_t re = {
		m->m[0][0] * v->x + m->m[1][0] * v->y + m->m[2][0] * v->z + m->m[3][0] * v->w,

		m->m[0][1] * v->x + m->m[1][1] * v->y + m->m[2][1] * v->z + m->m[3][1] * v->w,

		m->m[0][2] * v->x + m->m[1][2] * v->y + m->m[2][2] * v->z + m->m[3][2] * v->w,

		m->m[0][3] * v->x + m->m[1][3] * v->y + m->m[2][3] * v->z + m->m[3][3] * v->w
	};

	return re;
}
mat4_t mul_m4m4(const mat4_t* m1,const mat4_t* m2)
{
	mat4_t re;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			re.m[i][j] = (m1->m[i][0] * m2->m[0][j]
						+ m1->m[i][1] * m2->m[1][j]
						+ m1->m[i][2] * m2->m[2][j]
						+ m1->m[i][3] * m2->m[3][j]);
		}
	}

	return re;

}

vec4_t mul_s1v4(float s,const vec4_t* v);
vec4_t mul_s1m4(float s,const mat4_t* m);

vec3_t mul_m3v3(const mat3_t* m,const vec3_t* v);
mat3_t mul_m3m3(const mat3_t* m1,const mat3_t* m2);
vec3_t mul_s1v3(float s,const vec3_t* v)
{
	return (vec3_t) {v->x*s , v->y*s , v->z*s };
}
vec4_t mul_s1m3(float s,const mat3_t* m);


vec4_t cross_v4(const vec4_t* v1,const vec4_t* v2);
float dot_v4(const vec4_t* v1,const vec4_t* v2);

vec3_t cross_v3(const vec3_t* v1, const vec3_t* v2)
{
	return (vec3_t) {	(v1->y * v2->z - v1->z * v2->y),
						(v1->z * v2->x - v1->x * v2->z),
						(v1->x * v2->y - v1->y * v2->x)
	};
}

float cross_v2(const vec2_t* v1,const vec2_t* v2)
{
	return (float)((v1->x * v2->y) - (v1->y * v2->x));
}



float dot_v3(const vec3_t* v1, const vec3_t* v2)
{
	return (float) {v1->x* v2->x + v1->y * v2->y + v1->z * v2->z };
}

float length_v4(const vec4_t* v);
float length_v3(const vec3_t* v)
{
	return sqrtf(v->x*v->x + v->y * v->y + v->z * v->z);
}

float length_v2(const vec2_t* v)
{
	return sqrtf(v->x * v->x + v->y * v->y);
}

vec4_t normalize_v4(const vec4_t* v);
vec3_t normalize_v3(const vec3_t* v)
{
	float len = length_v3(v);
	return (vec3_t) {v->x / len , v->y / len , v->z / len };
}




mat4_t translate(const mat4_t* m,const vec3_t* v)
{
	return (mat4_t){
		1.F , 0.F , 0.F , 0.F,
		0.F , 1.F , 0.F , 0.F,
		0.F , 0.F , 1.F , 0.F,
		v->x , v->y , v->z , 1.F
	};

	
}
mat4_t rotate(const mat4_t* m,float angle,const vec3_t* v)
{
 
	float sinfv_x = sinf(angle * v->x);
	float cosfv_x = cosf(angle * v->x);

	float sinfv_y = sinf(angle * v->y) ;
	float cosfv_y = cosf(angle * v->y) ;

	float sinfv_z = sinf(angle * v->z);
	float cosfv_z = cosf(angle * v->z);

	mat4_t rx = {
		1.F , 0.F , 0.F , 0.F,
		0.F ,cosfv_x, sinfv_x , 0.F,
		0.F , - sinfv_x , cosfv_x , 0.F ,
		0.F , 0.F , 0.F , 1.F
	};

	mat4_t ry = {
		cosfv_y , 0.F , -sinfv_y , 0.F ,
		0.F , 1.F , 0.F ,0.F,
		sinfv_y , 0.F, cosfv_y, 0.F,
		0.F , 0.F , 0.F ,1.F 
	};

	mat4_t rz = {
		cosfv_z , sinfv_z , 0.F , 0.F,
		-sinfv_z , cosfv_z , 0.F , 0.F ,
		0.F , 0.F , 1.F, 0.F ,
		0.F , 0.F , 0.F ,1.F
	};

	mat4_t ryz = mul_m4m4(&ry, &rz);
	return mul_m4m4(&rx , &ryz);

}
mat4_t scale(const mat4_t* m,const vec3_t* v)
{
	return (mat4_t) {
		v->x  , 0.F	 , 0.F , 0.F,
		0.F		, v->y , 0.F , 0.F,
		0.F		, 0.F	 , v->z,0.F,
		0.F		, 0.F	 , 0.F	, 1.F
	};
}

mat4_t lookAt(const vec3_t* eye,const vec3_t* center,const vec3_t* up)
{
	

	vec3_t z_axis = sub_v3v3(eye, center);
	z_axis = normalize_v3(&z_axis);

	
	vec3_t x_axis = cross_v3(&z_axis , up);
	x_axis = normalize_v3(&x_axis);

	vec3_t y_axis = cross_v3(&x_axis , &z_axis);


	float translate_x = -dot_v3(&x_axis, eye);
	float translate_y = -dot_v3(&y_axis, eye);
	float translate_z = -dot_v3(&z_axis, eye);

	
	return (mat4_t) {
		x_axis.x, y_axis.x, z_axis.x, 0.F,
		x_axis.y, y_axis.y, z_axis.y, 0.F,
		x_axis.z, y_axis.z, z_axis.z, 0.F,
		translate_x, translate_y, translate_z, 1.F
	};

	
}

mat4_t projection(float left, float right, float bottom, float top, float near, float far)
{
	return (mat4_t) {
		2.F / (right - left), 0.F, 0.F, 0.F,
			0.F , 2/(top-bottom) , 0.F , 0.F,
			0.F , 0.F , -2/(far - near) , 0.F ,
		-(left + right) / (right - left) , (top+bottom)/(top - bottom) , -(far + near) / (far - near) , 1.F
	};
}

int absi(int n)
{
	return n < 0 ? n * -1 : n;
}


float absf(float n)
{
	return n < 0.F ? n * -1.F : n;
}


