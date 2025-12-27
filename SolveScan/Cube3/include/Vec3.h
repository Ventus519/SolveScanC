#pragma once
#ifndef VEC3_H
#define VEC3_H

typedef struct Vec3 {
    int x;
    int y;
    int z;
} Vec3;

Vec3* create_vec3(int x, int y, int z);
void free_vec3(Vec3* vec);

void set_position(Vec3* vec, int x, int y, int z);

Vec3* add_vec3(const Vec3* a, const Vec3* b);
Vec3* subtract_vec3(const Vec3* a, const Vec3* b);
Vec3* cross_product(const Vec3* a, const Vec3* b);
int dot_product(const Vec3* a, const Vec3* b);

double magnitude(const Vec3* vec);

int is_equal_vec3(const Vec3* a, const Vec3* b);

#endif // VEC3_H