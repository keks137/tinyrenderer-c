#ifndef INCLUDE_SRC_RENDERER_H_
#define INCLUDE_SRC_RENDERER_H_

#include "tga.h"
#include <stdint.h>
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
void set_bgra_at_pos(BGRABuf *buf, Vec2 pos, BGRAColor color);
void draw_line(BGRABuf *buf, Vec2 start, Vec2 end, BGRAColor color);


#endif  // INCLUDE_SRC_RENDERER_H_
