#include "raylib.h"
#include "rlgl.h"
int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "Raylib - Rotating 3D Hand");
    SetTargetFPS(60);

    // Camera setup
    Camera3D camera = { 0 };
    camera.position = (Vector3){ 12.0f, 8.0f, 12.0f };
    camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float rotation = 0.0f;

    while (!WindowShouldClose())
    {
        rotation += 0.3f;                    // Rotation speed (adjust as needed)
        if (rotation >= 360.0f) rotation = 0.0f;

        BeginDrawing();
        ClearBackground(DARKGRAY);

        BeginMode3D(camera);

            DrawGrid(20, 1.0f);

            rlPushMatrix();
            rlRotatef(rotation, 0.0f, 1.0f, 0.0f);  // Rotate whole hand around Y axis

            // === PALM ===
            DrawCube((Vector3){0.0f, 2.0f, 0.0f}, 5.0f, 1.8f, 6.0f, BEIGE);
            DrawCubeWires((Vector3){0.0f, 2.0f, 0.0f}, 5.0f, 1.8f, 6.0f, DARKBROWN);

            // === THUMB ===
            rlPushMatrix();
            rlTranslatef(3.0f, 2.5f, -2.5f);
            rlRotatef(40, 0, 0, 1);              // Angle thumb outward
            DrawCylinder((Vector3){0, 0, 0}, 0.7f, 0.6f, 3.5f, 8, ORANGE);
            DrawSphere((Vector3){0, 0, 3.5f}, 0.7f, ORANGE);  // Thumb tip
            rlPopMatrix();

            // === FINGERS ===
            // Index finger
            DrawCylinder((Vector3){-1.8f, 3.0f, 2.2f}, 0.55f, 0.5f, 4.0f, 8, BEIGE);
            DrawSphere((Vector3){-1.8f, 3.0f, 6.0f}, 0.6f, BEIGE);

            // Middle finger
            DrawCylinder((Vector3){-0.6f, 3.2f, 2.3f}, 0.6f, 0.55f, 4.5f, 8, BEIGE);
            DrawSphere((Vector3){-0.6f, 3.2f, 6.7f}, 0.65f, BEIGE);

            // Ring finger
            DrawCylinder((Vector3){0.7f, 3.0f, 2.2f}, 0.55f, 0.5f, 4.0f, 8, BEIGE);
            DrawSphere((Vector3){0.7f, 3.0f, 6.0f}, 0.6f, BEIGE);

            // Pinky
            DrawCylinder((Vector3){2.0f, 2.7f, 2.0f}, 0.45f, 0.4f, 3.3f, 8, BEIGE);
            DrawSphere((Vector3){2.0f, 2.7f, 5.1f}, 0.5f, BEIGE);

            rlPopMatrix();

        EndMode3D();

        // UI Info
        DrawText("Basic 3D Hand - Rotating 360°", 10, 10, 20, RAYWHITE);
        DrawText(TextFormat("Rotation: %.1f°", rotation), 10, 40, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
