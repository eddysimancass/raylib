#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

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


void rotate(vertex *points[], int SIZE, double angle);
void draw_edge(edge *pair);
void pair_points(pair list[], vertex *points[], edge *lines[], int SIZE);
void get_projection(vertex *points[], int SIZE, const int focal_length);
void init_points(vertex *points[]);
void shift_points(vertex *points[]);
void copy_points(vertex *to[], vertex *from[], int SIZE);

void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
        now = clock();
}

const int screenWidth = 800;
const int screenHeight = 450;

int main(void)
{
    const int focal_length = 100;

    vertex *points[POINTS];
    vertex *projected_points[POINTS];
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

    for (int i = 0; i < POINTS; i++) {
        projected_points[i] = malloc(sizeof(vertex));
        projected_points[i]->x = 0;
        projected_points[i]->y = 0;
        projected_points[i]->z = 0;
    }

    for (int i = 0; i < EDGES; i++) {
        lines[i] = malloc(sizeof(edge));
        lines[i]->a = NULL;
        lines[i]->b = NULL;
    }

    for (int i = 0; i < EDGES; i++) {
        lines[i]->a = projected_points[list[i].s];
        lines[i]->b = projected_points[list[i].e];
    }

    init_points(projected_points);
    init_points(points);

    shift_points(projected_points);
    get_projection(projected_points, POINTS, focal_length);

    InitWindow(screenWidth, screenHeight, "ray casting");

    // Main game loop
    while (!WindowShouldClose())        // Detect window close button or ESC key
    {
        for (int a = 0; a < 360; a += 10) {

            BeginDrawing();
            // draws
            for (int i = 0; i < EDGES; i++) {
                draw_edge(lines[i]);
            }

            ClearBackground(BLACK);
            copy_points(projected_points, points, POINTS);
            rotate(projected_points, POINTS, a);
            shift_points(projected_points);
            get_projection(projected_points, POINTS, focal_length);

            EndDrawing();
            delay(100);
        }
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



void pair_points(pair list[], vertex *points[], edge *lines[], int SIZE) {
    for (int i = 0; i < SIZE; i++) {
        lines[i]->a = points[list[i].s];
        lines[i]->b = points[list[i].e];
    }
}

void draw_edge(edge *pair) {
    DrawLine(pair->a->x, pair->a->z, pair->b->x, pair->b->z, RED); // start x,y end x,y color
}

void get_projection(vertex *points[], int SIZE, const int focal_length) {
    for (int i = 0; i < SIZE; i++) {
        points[i]->x = ((points[i]->x)*focal_length)/(focal_length+points[i]->y) + screenWidth/2;
        points[i]->z = ((points[i]->z)*focal_length)/(focal_length+points[i]->y) + screenHeight/2;
        points[i]->y = 0;
    }
}


void rotate(vertex *points[], int SIZE, double angle) {
    int new_x;
    int new_y;

    angle = angle*PI/180;

    // for (int i = 0; i < SIZE; i++) {
    //     new_x = points[i]->x*cos(angle) + -1*points[i]->y*sin(angle);
    //     new_y = points[i]->x*sin(angle) + points[i]->y*cos(angle);

    //     points[i]->x = new_x;
    //     points[i]->y = new_y;
    // }

    for (int i = 0; i < SIZE; i++) {
        new_x = points[i]->y*cos(angle) + -1*points[i]->z*sin(angle);
        new_y = points[i]->y*sin(angle) + points[i]->z*cos(angle);

        points[i]->y = new_x;
        points[i]->z = new_y;
    }
}


void shift_points(vertex *points[]) {
    for (int i = 0; i < POINTS; i++) {
        points[i]->y -= 15*SCALE;
    }    
}

void init_points(vertex *points[]) {
    // set points
    points[0]->x = 5*SCALE;
    points[0]->y = -5*SCALE;
    points[0]->z = 5*SCALE;

    points[1]->x = 5*SCALE;
    points[1]->y = 5*SCALE;
    points[1]->z = 5*SCALE;

    points[2]->x = -5*SCALE;
    points[2]->y = 5*SCALE;
    points[2]->z = 5*SCALE;

    points[3]->x = -5*SCALE;
    points[3]->y = -5*SCALE;
    points[3]->z = 5*SCALE;

    points[4]->x = 5*SCALE;
    points[4]->y = 5*SCALE;
    points[4]->z = -5*SCALE;

    points[5]->x = -5*SCALE;
    points[5]->y = 5*SCALE;
    points[5]->z = -5*SCALE;

    points[6]->x = -5*SCALE;
    points[6]->y = -5*SCALE;
    points[6]->z = -5*SCALE;

    points[7]->x = 5*SCALE;
    points[7]->y = -5*SCALE;
    points[7]->z = -5*SCALE;
}


void copy_points(vertex *to[], vertex *from[], int SIZE) {
    for (int i = 0; i < SIZE; i++) {
        to[i]->x = from[i]->x;
        to[i]->y = from[i]->y;
        to[i]->z = from[i]->z; 
    }
}

