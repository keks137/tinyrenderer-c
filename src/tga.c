#include "tga.h"
#include <stdio.h>
bool tga_write(const char *filename, uint32_t *buf, uint16_t width, uint16_t height)
{
	FILE *f = fopen(filename, "wb");
	if (f == NULL)
		return false;

	TGAHeader header = {
		.id_length = 0,
		.color_map_type = 0,
		.image_type = 2, // uncompressed RGB
		.color_map_origin = 0,
		.color_map_length = 0,
		.color_map_depth = 0,
		.x_origin = 0,
		.y_origin = 0,
		.width = width,
		.height = height,
		.pixel_depth = 32,
		.image_descriptor = 0x28 // top-left origin + 8-bit alpha
	};

	fwrite(&header, sizeof(header), 1, f);

	for (int i = 0; i < width * height; i++) {
		uint32_t pixel = buf[i];
		fwrite(&pixel, 4, 1, f);
	}

	fclose(f);
	return true;
}

