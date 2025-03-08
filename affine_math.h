#pragma once

typedef union
{
	struct
	{
		int v[2];
	};
	struct
	{
		int x;
		int y;
	};
}ivec2_t;

typedef union vec2 {
	float v[2];
	struct
	{
		float x;
		float y;
	};
}vec2_t;

typedef union vec3{
	float v[3];
	struct
	{
		float x;
		float y;
		float z;
	};
}vec3_t;

typedef union vec4 {
	float v[4];
	struct
	{
		float x;
		float y;
		float z;
		float w;
	};
}vec4_t;

typedef union mat4_t {
	float m[4][4];
	struct
	{
		float m00, m01, m02, m03;
		float m10, m11, m12, m13;
		float m20, m21, m22, m23;
		float m30, m31, m32, m33;
	};
}mat4_t;

typedef union mat3 {
	float m[3][3];
	struct
	{
		float m00, m01, m02;
		float m10, m11, m12;
		float m20, m21, m22;
	};
}mat3_t;



vec4_t add_v4v4(const vec4_t* v1, const vec4_t* v2);
vec3_t add_v3v3(const vec3_t* v1, const vec3_t* v2);
vec4_t sub_v4v4(const vec4_t* v1, const vec4_t* v2);
vec3_t sub_v3v3(const vec3_t* v1, const vec3_t* v2);

vec4_t mul_m4v4(const mat4_t* m , const vec4_t* v);
vec4_t mul_v4m4(const vec4_t* v , const mat4_t* m);
mat4_t mul_m4m4(const mat4_t* m1, const mat4_t* m2);
vec4_t mul_s1v4(float s, const vec4_t* v);
vec4_t mul_s1m4(float s, const mat4_t* m);

vec3_t mul_m3v3(const mat3_t* m, const vec3_t* v);
vec3_t mul_v3m3(const vec3_t* v, const mat3_t* m);
mat3_t mul_m3m3(const mat3_t* m1, const mat3_t* m2);
vec3_t mul_s1v3(float s, const vec3_t* v);
vec4_t mul_s1m3(float s, const mat3_t* m);


vec4_t cross_v4(const vec4_t* v1, const vec4_t* v2);
float dot_v4(const vec4_t* v1, const vec4_t* v2);

vec3_t cross_v3(const vec3_t* v1, const vec3_t* v2);
float dot_v3(const vec3_t* v1, const vec3_t* v2);


vec4_t normalize_v4(const vec4_t* v);
vec3_t normalize_v3(const vec3_t* v);

float length_v4(const vec4_t* v);
float length_v3(const vec3_t* v);
float length_v2(const vec2_t* v);

mat4_t translate(const mat4_t* m, const vec3_t* v);
mat4_t rotate(const mat4_t* m,float angle,const vec3_t* v);
mat4_t scale(const mat4_t* m,const vec3_t* v);

mat4_t lookAt(const vec3_t* eye,const vec3_t* center,const vec3_t* up);

mat4_t projection(float left,float right,float bottom,float top,float near,float far);


int absi(int n);
float absf(float n);










