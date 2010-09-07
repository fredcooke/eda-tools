/*
 * ppmdiff.c - Mark differences in two PPM files
 *
 * Written 2010 by Werner Almesberger
 * Copyright 2010 Werner Almesberger
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */


#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


static uint8_t a_only[3] = { 255, 0, 0 };
static uint8_t b_only[3] = { 0, 255, 0 };
static uint8_t both[3] = { 220, 220, 220 };
static uint8_t frame[3] = { 0, 0, 255 };
static uint8_t frame_fill[3] = { 255, 255, 200 };
static int frame_dist = 40;
static int frame_width = 2;


static uint8_t *load_ppm(const char *name, int *x, int *y)
{
	FILE *file;
	char line[100];
	int this_x, this_y, depth;
	int n;
	uint8_t *img;

	file = fopen(name, "r");
	if (!file) {
		perror(name);
		exit(1);
	}
	if (!fgets(line, sizeof(line), file)) {
		fprintf(stderr, "can't read file type\n");
		exit(1);
	}
	if (strcmp(line, "P6\n")) {
		fprintf(stderr, "file type must be P6, not %s", line);
		exit(1);
	}
	if (!fgets(line, sizeof(line), file)) {
		fprintf(stderr, "can't read resolution\n");
		exit(1);
	}
	if (sscanf(line, "%d %d", &this_x, &this_y) != 2) {
		fprintf(stderr, "can't parse resolution: %s", line);
		exit(1);
	}
	if (*x || *y) {
		if (*x != this_x || *y != this_y) {
			fprintf(stderr,
			    "resolution changed from %dx%d to %dx%d\n",
			    *x, *y, this_x, this_y);
			exit(1);
		}
	} else {
		*x = this_x;
		*y = this_y;
	}
	if (!fgets(line, sizeof(line), file)) {
		fprintf(stderr, "can't read depth\n");
		exit(1);
	}
	if (sscanf(line, "%d", &depth) != 1) {
		fprintf(stderr, "can't parse depth: %s", line);
		exit(1);
	}
	if (depth != 255) {
		fprintf(stderr, "depth must be 255, not %d\n", depth);
		exit(1);
	}
	n = *x**y*3;
	img = malloc(n);
	if (!img) {
		perror("malloc");
		exit(1);
	}
	if (fread(img, 1, n, file) != n) {
		fprintf(stderr, "can't read %d bytes\n", n);
		exit(1);
	}
	fclose(file);
	return img;
}


static struct area {
	int x0, y0, x1, y1;
	struct area *next;
} *areas = NULL;


static void add_area(struct area **root, int x0, int y0, int x1, int y1)
{
	while (*root) {
		struct area *area = *root;

		if (area->x0 > x1 || area->y0 > y1 ||
		    area->x1 < x0 || area->y1 < y0) {
			root = &(*root)->next;
			continue;
		}
		x0 = x0 < area->x0 ? x0 : area->x0;
		y0 = y0 < area->y0 ? y0 : area->y0;
		x1 = x1 > area->x1 ? x1 : area->x1;
		y1 = y1 > area->y1 ? y1 : area->y1;
		*root = area->next;
		free(area);
		add_area(&areas, x0, y0, x1, y1);
		return;
	}
	*root = malloc(sizeof(**root));
	if (!*root) {
		perror("malloc");
		exit(1);
	}
	(*root)->x0 = x0;
	(*root)->y0 = y0;
	(*root)->x1 = x1;
	(*root)->y1 = y1;
	(*root)->next = NULL;
}


static void change(int x, int y)
{
	add_area(&areas,
	    x-frame_dist, y-frame_dist, x+frame_dist, y+frame_dist);
}


static void set_pixel(uint8_t *p, const uint8_t *color, const uint8_t *value)
{
	double f;
	int i;

	f = (255-(value[0] | value[1] | value[2]))/255.0;
	for (i = 0; i != 3; i++)
		p[i] = 255-(255-color[i])*f;
}


static uint8_t *diff(const uint8_t *a, const uint8_t *b, int xres, int yres,
    int mark_areas)
{
	uint8_t *res, *p;
	int x, y;
	unsigned val_a, val_b;

	res = p = malloc(xres*yres*3);
	if (!res) {
		perror("malloc");
		exit(1);
	}
	for (y = 0; y != yres; y++)
		for (x = 0; x != xres; x++) {
			val_a = a[0]+a[1]+a[2];
			val_b = b[0]+b[1]+b[2];
			if (val_a == val_b) {
				set_pixel(p, both, b);
			} else if (val_a < val_b) {
				set_pixel(p, a_only, a);
				if (mark_areas)
					change(x, y);
			} else if (val_a > val_b) {
				set_pixel(p, b_only, b);
				if (mark_areas)
					change(x, y);
			} else {
				abort();	/* no longer used */
				memset(p, 255, 3);
//				memcpy(p, "\0\0\xff", 3);
			}
			a += 3;
			b += 3;
			p += 3;
		}
	return res;
}


static void shadow_diff(const uint8_t *a, const uint8_t *b, int xres, int yres)
{
	int x, y;

	for (y = 0; y != yres; y++)
		for (x = 0; x != xres; x++) {
			if (memcmp(a, b, 3))
				change(x, y);
			a += 3;
			b += 3;
		}
}


static void point(uint8_t *img, int x, int y, int xres, int yres)
{
	uint8_t *p;

	if (x < 0 || y < 0 || x >= xres || y >= yres)
		return;
	p = img+(y*xres+x)*3;
	if ((p[0] & p[1] & p[2]) != 255)
		return;
	memcpy(p, frame, 3);
}


static void hline(uint8_t *img, int x0, int x1, int y, int xres, int yres)
{
	int x;

	for (x = x0; x <= x1; x++)
		point(img, x, y, xres, yres);
}


static void vline(uint8_t *img, int y0, int y1, int x, int xres, int yres)
{
	int y;

	for (y = y0; y <= y1; y++)
		point(img, x, y, xres, yres);
}


static void fill(uint8_t *img, int x0, int y0, int x1, int y1,
   int xres, int yres)
{
	int x, y;
	uint8_t *p;

	for (y = y0; y <= y1; y++) {
		if (y < 0 || y >= yres)
			continue;
		p = img+(xres*y+x0)*3;
		for (x = x0; x <= x1; x++) {
			if (x >= 0 && x < xres && (p[0] & p[1] & p[2]) == 255)
				memcpy(p, frame_fill, 3);
			p += 3;
		}
	}
}


static void mark_areas(uint8_t *img, int x, int y)
{
	const struct area *area;
	int r1 = 0, r2 = 0, i;

	if (frame_width) {
		r1 = (frame_width-1)/2;
		r2 = (frame_width-1)-r1;
	}
	for (area = areas; area; area = area->next) {
		if (frame_width)
			for (i = -r1; i <= r2; i++) {
				hline(img, area->x0-r1, area->x1+r2, area->y0+i,
				    x, y);
				hline(img, area->x0-r1, area->x1+r2, area->y1+i,
				    x, y);
				vline(img, area->y0+r1, area->y1-r2, area->x0+i,
				    x, y);
				vline(img, area->y0+r1, area->y1-r2, area->x1+i,
				    x, y);
			}
		fill(img,
		    area->x0+r1, area->y0+r1, area->x1-r2, area->y1-r2, x, y);
	}
}


static void usage(const char *name)
{
	fprintf(stderr,
"usage: %s [-f] [-a color] [-b color] [-c color] [-d pixels]\n"
"%6s %*s [-m color] [-n color] [-w pixels] file_a.ppm file_b.ppm\n"
"%6s %*s [file_a'.ppm file_b'.ppm] [out.ppm]\n\n"
"  file_a.ppm and file_b.ppm   are two input images\n"
"  file_a'.ppm and file_b'.ppm if present, are searched for changes as well\n"
"  out.ppm                     output file (default: standard output)\n\n"
"  -f         generate output (and return success) even if there is no change\n"
"  -a color   color of items only in image A\n"
"  -b color   color of items only in image B\n"
"  -c color   color of items in both images\n"
"  -d pixels  distance between change and marker box. 0 disables markers.\n"
"  -m color   color of the frame of the marker box.\n"
"  -n color   color of the background of the marker box\n"
"  -w pixels  width of the frame of the marker box. 0 disables frames.\n\n"
"  color is specified as R,B,G with each component as a floating-point\n"
"  value from 0 to 1. E.g., 1,1,1 is white.\n\n"
"  The images are expected to have dark colors on a perfectly white\n"
"  background.\n"
	    , name, "", (int) strlen(name), "", "", (int) strlen(name), "");
	exit(1);
}


static void parse_color(uint8_t *c, const char *s, const char *name)
{
	float r, g, b;

	if (sscanf(s, "%f,%f,%f", &r, &g, &b) != 3)
		usage(name);
	c[0] = 255*r;
	c[1] = 255*g;
	c[2] = 255*b;
}


int main(int argc, char *const *argv)
{
	int force = 0;
	int x = 0, y = 0;
	uint8_t *old, *new, *d, *a, *b;
	char *shadow_old = NULL, *shadow_new = NULL, *out_name = NULL;
	FILE *out;
	char *end;
	int c;

	while ((c = getopt(argc, argv, "a:b:c:d:fm:n:w:")) != EOF)
		switch (c) {
		case 'a':
			parse_color(a_only, optarg, *argv);
			break;
		case 'b':
			parse_color(b_only, optarg, *argv);
			break;
		case 'c':
			parse_color(both, optarg, *argv);
			break;
		case 'd':
			frame_dist = strtoul(optarg, &end, 0);
			if (*end)
				usage(*argv);
			break;
		case 'f':
			force = 1;
			break;
		case 'm':
			parse_color(frame, optarg, *argv);
			break;
		case 'n':
			parse_color(frame_fill, optarg, *argv);
			break;
		case 'w':
			frame_width = strtoul(optarg, &end, 0);
			if (*end)
				usage(*argv);
			break;
		default:
			usage(*argv);
		}
	switch (argc-optind) {
	case 2:
		break;
	case 3:
		out_name = argv[optind+2];
		break;
	case 5:
		out_name = argv[optind+4];
		/* fall through */
	case 4:
		shadow_old = argv[optind+2];
		shadow_new = argv[optind+3];
		break;
	default:
		usage(*argv);
	}

	old = load_ppm(argv[optind], &x, &y);
	new = load_ppm(argv[optind+1], &x, &y);
	if (shadow_old) {
		a = load_ppm(shadow_old, &x, &y);
		b = load_ppm(shadow_new, &x, &y);
		if (!force && !memcmp(a, b, x*y*3))
			return 1;
		shadow_diff(a, b, x, y);
	}
	if (!force && !areas && !memcmp(old, new, x*y*3))
		return 1;
	d = diff(old, new, x, y, !shadow_old);
	if (frame_dist)
		mark_areas(d, x, y);

	if (out_name) {
		out = fopen(out_name, "w");
		if (!out) {
			perror(out_name);
			exit(1);
		}
	} else {
		out = stdout;
	}
	fprintf(out, "P6\n%d %d\n255\n", x, y);
	fwrite(d, 1, x*y*3, out);
	if (fclose(out) == EOF) {
		perror("fclose");
		exit(1);
	}
	return 0;
}
