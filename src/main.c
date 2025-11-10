#include "tga.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
	uint16_t x;
	uint16_t y;
} Vec2;

typedef struct {
	uint8_t b;
	uint8_t g;
	uint8_t r;
	uint8_t a;
} BGRAColor;

void set_bgra_at_pos(BGRABuf *buf, Vec2 pos, BGRAColor color)
{
	uint32_t packed_bgra = (color.a << 24) | (color.r << 16) | (color.g << 8) | color.b;

	assert(pos.x <= buf->width);
	assert(pos.y <= buf->height);

	buf->data[buf->width * pos.y + pos.x] = packed_bgra;
}

void draw_line(BGRABuf *buf, Vec2 start, Vec2 end, BGRAColor color)
{
	Vec2 pos = { 0 };
	uint diffx = abs(start.x - end.x);
	uint diffy = abs(start.y - end.y);
	Vec2 a = { 0 };
	Vec2 e = { 0 };
	if (diffy > diffx) {
		if (end.y < start.y) {
			a = end;
			e = start;
		} else {
			a = start;
			e = end;
		}
		for (uint16_t y = a.y; y <= e.y; ++y) {
			float t = (y - a.y) / (float)(e.y - a.y);
			pos.x = round(a.x + t * (e.x - a.x));
			pos.y = y;
			set_bgra_at_pos(buf, pos, color);
		}
	} else {
		if (end.x < start.x) {
			a = end;
			e = start;
		} else {
			a = start;
			e = end;
		}

		for (uint16_t x = a.x; x <= e.x; ++x) {
			float t = (x - a.x) / (float)(e.x - a.x);
			pos.x = x;
			pos.y = round(a.y + t * (e.y - a.y));
			set_bgra_at_pos(buf, pos, color);
		}
	}
}

const BGRAColor BLUE = { 0xFF, 0x00, 0x00, 0xFF };
const BGRAColor WHITE = { 0xFF, 0xFF, 0xFF, 0xFF };
const BGRAColor BLACK = { 0x00, 0x00, 0x00, 0xFF };
const BGRAColor GREEN = { 0x00, 0xFF, 0x00, 0xFF };
const BGRAColor YELLOW = { 0x00, 0xFF, 0xFF, 0xFF };
const BGRAColor RED = { 0x00, 0x00, 0xFF, 0xFF };

int main()
{
	BGRABuf buf = {
		.width = 64,
		.height = 64,
	};

	buf.data = calloc(buf.width * buf.height, sizeof(uint32_t));
	if (buf.data == NULL)
		exit(1);
	for (uint16_t y = 0; y < buf.height; ++y)
		for (uint16_t x = 0; x < buf.width; ++x)
			set_bgra_at_pos(&buf, (Vec2){ x, y }, BLACK);

	Vec2 a = { .x = 7, .y = 3 };
	Vec2 b = { .x = 12, .y = 37 };
	Vec2 c = { .x = 62, .y = 53 };

	draw_line(&buf, a, b, BLUE);
	draw_line(&buf, c, b, GREEN);
	draw_line(&buf, c, a, YELLOW);
	draw_line(&buf, a, c, RED);

	set_bgra_at_pos(&buf, a, WHITE);
	set_bgra_at_pos(&buf, b, WHITE);
	set_bgra_at_pos(&buf, c, WHITE);

	tga_write("frame.tga", buf.data, buf.width, buf.height);
}
