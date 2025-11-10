#include "tga.h"
#include "renderer.h"
#include "colors.h"
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

const uint MULTFACTOR = 10;

typedef struct {
	double x;
	double y;
	double z;
} OBJVertex;

typedef struct {
	OBJVertex *data;
	size_t len;
	size_t capacity;

} VertexBuf;

typedef struct {
	int a1;
	int b1;
	int c1;

} Face;

typedef struct {
	Face *data;
	size_t len;
	size_t capacity;

} FaceBuf;
bool fbuf_append(FaceBuf *fbuf, Face face)
{
	if (fbuf->capacity == 0) {
		size_t amount = 64;
		fbuf->data = malloc(sizeof(face) * amount);
		if (fbuf->data == NULL) {
			assert(0 == "Buy more ram");
			return false;
		}
		fbuf->capacity = amount;
	}
	if (fbuf->len == fbuf->capacity) {
		size_t amount = fbuf->len * 2;
		fbuf->data = realloc(fbuf->data, sizeof(face) * amount);
		if (fbuf->data == NULL) {
			assert(0 == "Buy more ram");
			return false;
		}
		fbuf->capacity = amount;
	}

	fbuf->data[fbuf->len] = face;
	fbuf->len++;

	return true;
}
bool vbuf_append(VertexBuf *vbuf, OBJVertex vertex)
{
	if (vbuf->capacity == 0) {
		size_t amount = 64;
		vbuf->data = malloc(sizeof(vertex) * amount);
		if (vbuf->data == NULL) {
			assert(0 == "Buy more ram");
			return false;
		}
		vbuf->capacity = amount;
	}
	if (vbuf->len == vbuf->capacity) {
		size_t amount = vbuf->len * 2;
		vbuf->data = realloc(vbuf->data, sizeof(vertex) * amount);
		if (vbuf->data == NULL) {
			assert(0 == "Buy more ram");
			return false;
		}
		vbuf->capacity = amount;
	}

	vbuf->data[vbuf->len] = vertex;
	vbuf->len++;

	return true;
}

char *read_file_to_malloced_str(const char *filename)
{
	FILE *file = fopen(filename, "rb");
	if (!file) {
		perror("Unable to open file");
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	long filesize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char *buffer = (char *)malloc(filesize + 1);
	if (!buffer) {
		perror("Memory allocation failed");
		fclose(file);
		return NULL;
	}

	size_t bytesRead = fread(buffer, 1, filesize, file);
	buffer[bytesRead] = '\0';

	fclose(file);
	return buffer;
}

void obj_parse_face_naive(FILE *f, FaceBuf *fbuf)
{
	char current_coord_str[20] = { 0 };
	size_t current_coord_str_index = 0;
	char c = 0;
	size_t bytesRead = fread(&c, sizeof(char), 1, f);
	Face current_face = { 0 };
	size_t coord_count = 0;
	while (bytesRead) {
		if (current_coord_str_index > 18) {
			printf("ERROR: incorrect vertex\n");
			return;
		}
		if (((c == ' ') && current_coord_str_index > 0) || c == '\n' || c == '/') {
			char *strend = &current_coord_str[current_coord_str_index + 1];
			int current_val = atoi(current_coord_str);
			current_coord_str_index = 0;
			switch (coord_count) {
			case 0:
				current_face.a1 = current_val;
				break;
			case 1:
				current_face.b1 = current_val;
				break;
			case 2:
				current_face.c1 = current_val;
				fbuf_append(fbuf, current_face);
				return;
				break;
			}
			coord_count++;
			if (c == '\n')
				return;
		}

		if (c >= '0' && c <= '9') {
			current_coord_str[current_coord_str_index] = c;
			current_coord_str_index++;
		}

		bytesRead = fread(&c, sizeof(char), 1, f);
	}
}

void obj_parse_vertex(FILE *f, VertexBuf *VBuf)
{
	char current_coord_str[20] = { 0 };
	size_t current_coord_str_index = 0;
	char c = 0;
	size_t bytesRead = fread(&c, sizeof(char), 1, f);
	OBJVertex current_vert = { 0 };
	size_t coord_count = 0;
	while (bytesRead) {
		if (current_coord_str_index > 18) {
			printf("ERROR: incorrect vertex\n");
			return;
		}
		if ((c == ' ' && current_coord_str_index > 0) || c == '\n') {
			char *strend = &current_coord_str[current_coord_str_index + 1];
			double current_val = strtod(current_coord_str, &strend);
			current_coord_str_index = 0;
			switch (coord_count) {
			case 0:
				current_vert.x = current_val;
				break;
			case 1:
				current_vert.y = current_val;
				break;
			case 2:
				current_vert.z = current_val;
				vbuf_append(VBuf, current_vert);
				return;
				break;
			}
			coord_count++;
			if (c == '\n')
				return;
		}

		if ((c >= '0' && c <= '9') || c == '-' || c == '.') {
			current_coord_str[current_coord_str_index] = c;
			current_coord_str_index++;
		}

		bytesRead = fread(&c, sizeof(char), 1, f);
	}
}
bool obj_parse_file(const char *filename, VertexBuf *vbuf, FaceBuf *fbuf)
{
	FILE *file = fopen(filename, "rb");
	if (!file) {
		perror("Unable to open file");
		return false;
	}

	char c = 0;
	size_t bytesRead = fread(&c, sizeof(char), 1, file);
	while (bytesRead) {
		if (c == 'v') {
			bytesRead = fread(&c, sizeof(char), 1, file);
			if (c == 'n') {
				//TODO: vertex normals
			} else if (c == 't') {
				//TODO: texture vertices
			} else if (c == ' ') {
				obj_parse_vertex(file, vbuf);
			}
		} else if (c == 'f') {
			obj_parse_face_naive(file, fbuf);
		}
		bytesRead = fread(&c, sizeof(char), 1, file);
	}

	fclose(file);
	return true;
}

int main(int argc, char *argv[])
{
	// char *contents = read_file_to_malloced_str("obj/diablo3_pose/diablo3_pose.obj");
	VertexBuf vbuf = { 0 };
	FaceBuf fbuf = { 0 };
	vbuf_append(&vbuf, (OBJVertex){ 0, 0, 0 }); // waste one space so indeces are right

	obj_parse_file("obj/diablo3_pose/diablo3_pose.obj", &vbuf, &fbuf);
	printf("Vertex count: %zu\n", vbuf.len);
	printf("Face count: %zu\n", fbuf.len);

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
