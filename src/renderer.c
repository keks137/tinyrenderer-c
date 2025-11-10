#include "renderer.h"
#include <assert.h>
#include <stdlib.h>
void set_bgra_at_pos(BGRABuf *buf, Vec2 pos, BGRAColor color)
{
	uint32_t packed_bgra = (color.a << 24) | (color.r << 16) | (color.g << 8) | color.b;
	assert(pos.x <= buf->width);
	assert(pos.y <= buf->height);
	buf->data[buf->width * pos.y + pos.x] = packed_bgra;
}

void draw_line(BGRABuf *buf, Vec2 start, Vec2 end, BGRAColor color)
{
	int dx = end.x - start.x;
	int dy = end.y - start.y;

	int abs_dx = abs(dx);
	int abs_dy = abs(dy);

	int sx = (dx > 0) ? 1 : -1;
	int sy = (dy > 0) ? 1 : -1;

	uint x = start.x;
	uint y = start.y;

	if (abs_dx >= abs_dy) {
		int64_t error = 2 * abs_dy - abs_dx;
		for (int i = 0; i <= abs_dx; ++i) {
			set_bgra_at_pos(buf, (Vec2){ x, y }, color);
			if (error >= 0) {
				y += sy;
				error -= 2 * abs_dx;
			}
			error += 2 * abs_dy;
			x += sx;
		}
	} else {
		int64_t error = 2 * abs_dx - abs_dy;
		for (int i = 0; i <= abs_dy; ++i) {
			set_bgra_at_pos(buf, (Vec2){ x, y }, color);
			if (error >= 0) {
				x += sx;
				error -= 2 * abs_dy;
			}
			error += 2 * abs_dx;
			y += sy;
		}
	}
}
