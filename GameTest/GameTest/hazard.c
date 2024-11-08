#pragma once
#include "hazard.h"
#include "cprocessing.h"
#include <stdbool.h>

// Helper function to check if a point is inside the square
bool is_point_in_square(Point p, Point squareTopLeft, float width, float height) {
    return (p.x >= squareTopLeft.x && p.x <= squareTopLeft.x + width &&
        p.y >= squareTopLeft.y && p.y <= squareTopLeft.y + height);
}

// Helper function to calculate orientation
int orientation(Point a, Point b, Point c) {
    float val = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    if (val == 0) return 0;  // Collinear
    return (val > 0) ? 1 : 2; // Clockwise or counterclockwise
}

// Function to check if two line segments intersect
bool lines_intersect(Point p1, Point q1, Point p2, Point q2) {
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4) return true;

    return false;
}

// Main collision detection function
bool check_square_triangle_collision(Point squareTopLeft, float width, float height,
    Point A, Point B, Point C) {
    // Check if any triangle vertex is inside the square
    if (is_point_in_square(A, squareTopLeft, width, height) ||
        is_point_in_square(B, squareTopLeft, width, height) ||
        is_point_in_square(C, squareTopLeft, width, height)) {
        return true;
    }

    // Check if any square edge intersects with any triangle edge
    Point squarePoints[4] = {
        squareTopLeft,
        {squareTopLeft.x + width, squareTopLeft.y},
        {squareTopLeft.x + width, squareTopLeft.y + height},
        {squareTopLeft.x, squareTopLeft.y + height}
    };

    // Define square edges
    Point squareEdges[4][2] = {
        {squarePoints[0], squarePoints[1]},
        {squarePoints[1], squarePoints[2]},
        {squarePoints[2], squarePoints[3]},
        {squarePoints[3], squarePoints[0]}
    };

    // Check all triangle edges with square edges
    Point triangleEdges[3][2] = { {A, B}, {B, C}, {C, A} };
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            if (lines_intersect(squareEdges[i][0], squareEdges[i][1],
                triangleEdges[j][0], triangleEdges[j][1])) {
                return true;
            }
        }
    }

    return false;
}