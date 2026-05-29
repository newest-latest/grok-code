#include "raylib.h"
#include "rlgl.h"        // Required for rlPushMatrix / rlRotatef

#define MAX_POINTS 64

typedef struct {
    Vector3 points[MAX_POINTS];
    int count;
} HandModel;

int AddPoint(HandModel* hand, float x, float y, float z) {
    if (hand->count >= MAX_POINTS) return -1;
    hand->points[hand->count] = (Vector3){x, y, z};
    return hand->count++;
}

void DrawWireframeHand(const HandModel* hand, Color color) {
    if (hand->count < 19) return;

    // Palm
    DrawLine3D(hand->points[0], hand->points[1], color);
    DrawLine3D(hand->points[1], hand->points[2], color);
    DrawLine3D(hand->points[2], hand->points[3], color);
    DrawLine3D(hand->points[3], hand->points[0], color);

    // Connect palm to fingers
    DrawLine3D(hand->points[1], hand->points[4], color);   // Index
    DrawLine3D(hand->points[1], hand->points[7], color);   // Middle
    DrawLine3D(hand->points[2], hand->points[10], color);  // Ring
    DrawLine3D(hand->points[2], hand->points[13], color);  // Pinky

    // Thumb base
    DrawLine3D(hand->points[0], hand->points[16], color);

    // Finger segments
    for (int i = 0; i < 4; i++) {
        int base = 4 + i * 3;
        DrawLine3D(hand->points[base],   hand->points[base + 1], color);
        DrawLine3D(hand->points[base + 1], hand->points[base + 2], color);
    }

    // Thumb segments
    DrawLine3D(hand->points[16], hand->points[17], color);
    DrawLine3D(hand->points[17], hand->points[18], color);

    // Draw joints
    for (int i = 0; i < hand->count; i++) {
        DrawSphere(hand->points[i], 0.09f, WHITE);
    }
}

int main(void) {
    InitWindow(800, 600, "Raylib - Wireframe Hand (Catmull Style)");
    SetTargetFPS(60);

    Camera3D camera = { 0 };
    camera.position = (Vector3){0.0f, 9.0f, 20.0f};
    camera.target = (Vector3){0.0f, 4.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    HandModel hand = {0};

    // Palm
    AddPoint(&hand, -3.5f, 1.0f,  2.5f);
    AddPoint(&hand, -3.0f, 1.0f, -2.0f);
    AddPoint(&hand,  3.0f, 0.8f, -2.8f);
    AddPoint(&hand,  3.2f, 1.2f,  2.8f);

    // Index
    AddPoint(&hand, -2.3f, 1.2f, 3.2f);
    AddPoint(&hand, -2.1f, 4.5f, 3.5f);
    AddPoint(&hand, -2.0f, 7.2f, 3.6f);

    // Middle
    AddPoint(&hand, -0.6f, 1.1f, 3.8f);
    AddPoint(&hand, -0.5f, 4.8f, 4.1f);
    AddPoint(&hand, -0.4f, 8.0f, 4.2f);

    // Ring
    AddPoint(&hand,  1.1f, 1.0f, 3.4f);
    AddPoint(&hand,  1.2f, 4.3f, 3.6f);
    AddPoint(&hand,  1.3f, 7.0f, 3.7f);

    // Pinky
    AddPoint(&hand,  2.6f, 0.9f, 2.7f);
    AddPoint(&hand,  2.8f, 3.4f, 2.9f);
    AddPoint(&hand,  2.9f, 5.6f, 3.0f);

    // Thumb
    AddPoint(&hand, -3.2f, 1.8f, -1.2f);
    AddPoint(&hand, -4.5f, 3.5f, -0.3f);
    AddPoint(&hand, -4.8f, 5.2f,  0.8f);

    float rotation = 0.0f;

    while (!WindowShouldClose()) {
        rotation += 0.2f;
        if (rotation >= 360.0f) rotation = 0.0f;

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode3D(camera);

            DrawGrid(20, 1.0f);

            rlPushMatrix();
            rlRotatef(rotation, 0.0f, 1.0f, 0.0f);

            DrawWireframeHand(&hand, LIME);

            rlPopMatrix();

        EndMode3D();

        DrawText("Wireframe 3D Hand - Ed Catmull 1972 Inspired", 10, 10, 20, RAYWHITE);
        DrawText("Only points + lines", 10, 35, 20, GRAY);
        DrawText(TextFormat("Rotation: %.1f°", rotation), 10, 60, 20, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
