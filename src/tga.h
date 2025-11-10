#ifndef INCLUDE_SRC_TGA_H_
#define INCLUDE_SRC_TGA_H_



#include <stdbool.h>
#include <stdint.h>
#pragma pack(push, 1)
typedef struct {
    uint8_t  id_length;          // 0 (no image ID)
    uint8_t  color_map_type;     // 0 (no color map)
    uint8_t  image_type;         // 2 for uncompressed RGB
    uint16_t color_map_origin;   // 0
    uint16_t color_map_length;   // 0  
    uint8_t  color_map_depth;    // 0
    uint16_t x_origin;           // 0
    uint16_t y_origin;           // 0
    uint16_t width;              // image width
    uint16_t height;             // image height
    uint8_t  pixel_depth;        // 32 for RGBA
    uint8_t  image_descriptor;   // 0x28 for top-left origin + 8-bit alpha
} TGAHeader;
#pragma pack(pop)

typedef struct{
	uint32_t* data;
	uint16_t width;
	uint16_t height;
}BGRABuf;
bool tga_write(const char *filename, uint32_t *buf, uint16_t width, uint16_t height);
#endif  // INCLUDE_SRC_TGA_H_
