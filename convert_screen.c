#include "console_screen.h"
#include "convert_screen.h"

void convert_screen_coordinate(const float x, const float y,float* out_screenX, float* out_screenY) {
	int width = get_screen_width();
	int height = get_screen_height();

	*out_screenX = ((x + 1) / 2 * (width - 1));
	*out_screenY = ((1 - y) / 2 * (height - 1));
}

void draw_point(float x, float y, char pixel) {
	int screenX = 0, screenY = 0;
	//convert_screen(x, y, &screenX, &screenY);
	push_pixel(pixel, screenX, screenY);
}