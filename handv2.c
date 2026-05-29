#include "raylib.h"
#include "rlgl.h"

#define MAX_POINTS 128
#define MAX_LINES  256

typedef struct {
    Vector3 points[MAX_POINTS];
    int pointCount;
} HandModel;

void InitHandModel(HandModel* hand) {
    hand->pointCount = 0;
    // We'll add points manually below
}

// Add a point to the model
int AddPoint(HandModel* hand, float x, float y, float z) {
    if (hand->pointCount >= MAX_POINTS) return -1;
    hand->points[hand->pointCount] = (Vector3){x, y, z};
    return hand->pointCount++;
}

// Draw wireframe hand using lines between points
void DrawWireframeHand(HandModel* hand, Color color) {
    // Palm base (quad-like)
    DrawLine3D(hand->points[0], hand->points[1], color);
    DrawLine3D(hand->points[1], hand->points[2], color);
    DrawLine3D(hand->points[2], hand->points[3], color);
    DrawLine3D(hand->points[3], hand->points[0], color);

    // Palm to fingers connections
    DrawLine3D(hand->points[1], hand->points[4], color);  // to index
    DrawLine3D(hand->points[1], hand->points[7], color);  // to middle
    DrawLine3D(hand->points[2], hand->points[10], color); // to ring
    DrawLine3D(hand->points[2], hand->points[13], color); // to pinky

    // Thumb connections
    DrawLine3D(hand->points[0], hand->points[16], color);

    // Fingers - each has 3 segments
    for (int i = 0; i < 4; i++) {
        int base = 4 + i*3;
        DrawLine3D(hand->points[base],   hand->points[base+1], color);
        DrawLine3D(hand->points[base+1], hand->points[base+2], color);
    }

    // Thumb segments
    DrawLine3D(hand->points[16], hand->points[17], color);
    DrawLine3D(hand->points[17], hand->points[18], color);

    // Draw all points as small spheres
    for (int i = 0; i < hand->pointCount; i++) {
        DrawSphere(hand->points[i], 0.08f, WHITE);
    }
}

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Raylib - Wireframe Hand (Catmull Style)");
    SetTargetFPS(60);

    Camera3D camera = { 0 };
    camera.position = (Vector3){ 0.0f, 8.0f, 18.0f };
    camera.target = (Vector3){ 0.0f, 3.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    HandModel hand = {0};
    InitHandModel(&hand);

    // === Build a simplified wireframe hand (inspired by Catmull's 1972 model) ===

    // Palm (4 points forming a rough quadrilateral)
    int p0 = AddPoint(&hand, -3.0f, 0.0f,  2.0f);   // left bottom
