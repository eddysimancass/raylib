#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define POINTS 8
#define EDGES  12
#define SCALE  50

typedef struct {
    int x;
    int y;
    int z;
} vertex;

typedef struct {
    vertex *a;
    vertex *b;
} edge;

typedef struct {
    int s;
    int e;
} pair;

typedef struct {
    edge *p1;
    edge *p2;
    edge *p3;
    edge *p4;
} face;

typedef struct {
    face *f1;
    face *f2;
    face *f3;
    face *f4;
    face *f5;
    face *f6;
} cube;

void draw_edge(edge *pair);
void get_projection(vertex *points[], int SIZE, const int focal_length);


int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    const int focal_length = 100;

    vertex *points[POINTS];
    edge *lines[EDGES];
    pair list[EDGES] = {{.s = 0, .e = 1},
                        {.s = 0, .e = 3},
                        {.s = 0, .e = 7},
                        {.s = 1, .e = 2},
                        {.s = 1, .e = 4},
                        {.s = 2, .e = 3},
                        {.s = 2, .e = 5},
                        {.s = 3, .e = 6},
                        {.s = 4, .e = 5},
                        {.s = 4, .e = 7},
                        {.s = 5, .e = 6},
                        {.s = 6, .e = 7}};

    for (int i = 0; i < POINTS; i++) {
        points[i] = malloc(sizeof(vertex));
        points[i]->x = 0;
        points[i]->y = 0;
        points[i]->z = 0;
    }

    for (int i = 0; i < EDGES; i++) {
        lines[i] = malloc(sizeof(edge));
        lines[i]->a = NULL;
        lines[i]->b = NULL;
    }

    for (int i = 0; i < EDGES; i++) {
        lines[i]->a = points[list[i].s];
        lines[i]->b = points[list[i].e];
    }

    // set points
    points[0]->x = 5*SCALE;
    points[0]->y = -20*SCALE;
    points[0]->z = 5*SCALE;

    points[1]->x = 5*SCALE;
    points[1]->y = -10*SCALE;
    points[1]->z = 5*SCALE;

    points[2]->x = -5*SCALE;
    points[2]->y = -10*SCALE;
    points[2]->z = 5*SCALE;

    points[3]->x = -5*SCALE;
    points[3]->y = -20*SCALE;
    points[3]->z = 5*SCALE;

    points[4]->x = 5*SCALE;
    points[4]->y = -10*SCALE;
    points[4]->z = -5*SCALE;

    points[5]->x = -5*SCALE;
    points[5]->y = -10*SCALE;
    points[5]->z = -5*SCALE;

    points[6]->x = -5*SCALE;
    points[6]->y = -20*SCALE;
    points[6]->z = -5*SCALE;

    points[7]->x = 5*SCALE;
    points[7]->y = -20*SCALE;
    points[7]->z = -5*SCALE;

    get_projection(points, POINTS, focal_length);


    InitWindow(screenWidth, screenHeight, "ray casting");

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        BeginDrawing();
        for (int i = 0; i < EDGES; i++) {
            draw_edge(lines[i]);
        }
        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    // deallocate structs
    for (int i = 0; i < POINTS; i++) {
        free(points[i]);
    }

    for (int i = 0; i < EDGES; i++) {
        free(lines[i]);
    }

    return 0;
}

void draw_edge(edge *pair) {
    DrawLine(pair->a->x, pair->a->z, pair->b->x, pair->b->z, RED); // start x,y end x,y color
}

void get_projection(vertex *points[], int SIZE, const int focal_length) {
    for (int i = 0; i < SIZE; i++) {
        points[i]->x = ((points[i]->x)*focal_length)/(focal_length+points[i]->y) + 800/2;
        points[i]->z = ((points[i]->z)*focal_length)/(focal_length+points[i]->y) + 450/2;
        points[i]->y = 0;
    }
}