#include "raylib.h"
#include <stdlib.h>
#include <time.h>

int main(void)
{
    const int screenWidth = 1000;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "White Hell Descent - Bergfilm Edition");
    SetTargetFPS(60);
    srand(time(NULL));

    // Player
    Vector2 playerPos = {150, 400};
    float playerSpeed = 0.0f;
    float velocity = 0.0f;
    bool onGround = true;
    int score = 0;

    // Obstacles
    #define MAX_OBSTACLES 8
    Rectangle obstacles[MAX_OBSTACLES];
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i] = (Rectangle){ screenWidth + i*220, 420, 45, 60 };
    }

    float gameSpeed = 5.0f;
    bool gameOver = false;

    while (!WindowShouldClose())
    {
        if (!gameOver)
        {
            score += 1;

            // Controls
            if (IsKeyPressed(KEY_SPACE) && onGround) {
                velocity = -16.0f;
                onGround = false;
            }

            // Physics
            velocity += 0.8f;                    // gravity
            playerPos.y += velocity;

            if (playerPos.y >= 400) {
                playerPos.y = 400;
                velocity = 0;
                onGround = true;
            }

            // Move obstacles
            for (int i = 0; i < MAX_OBSTACLES; i++) {
                obstacles[i].x -= gameSpeed;

                if (obstacles[i].x < -60) {
                    obstacles[i].x = screenWidth + GetRandomValue(100, 400);
                }

                // Collision
                Rectangle playerRect = {playerPos.x + 20, playerPos.y + 10, 50, 70};
                if (CheckCollisionRecs(playerRect, obstacles[i])) {
                    gameOver = true;
                }
            }

            if (score % 600 == 0) gameSpeed += 0.4f;   // increase difficulty
        }

        // Restart
        if (gameOver && IsKeyPressed(KEY_R)) {
            gameOver = false;
            gameSpeed = 5.0f;
            score = 0;
            playerPos.y = 400;
            velocity = 0;
            for (int i = 0; i < MAX_OBSTACLES; i++) {
                obstacles[i].x = screenWidth + i*220;
            }
        }

        BeginDrawing();
        ClearBackground((Color){20, 30, 50, 255});  // cold mountain blue

        // Sky gradient
        DrawRectangleGradientV(0, 0, screenWidth, 350, (Color){80,100,140,255}, (Color){40,50,80,255});

        // Snow ground
        DrawRectangle(0, 460, screenWidth, screenHeight - 460, WHITE);

        // Mountains (classic Bergfilm look)
        DrawTriangle((Vector2){-100,460}, (Vector2){400,150}, (Vector2){800,460}, (Color){220,220,230,255});
        DrawTriangle((Vector2){300,460}, (Vector2){750,80}, (Vector2){1200,460}, (Color){200,205,220,255});

        // Draw obstacles (rocks & trees)
        for (int i = 0; i < MAX_OBSTACLES; i++) {
            DrawRectangle(obstacles[i].x, obstacles[i].y, obstacles[i].width, obstacles[i].height, DARKGRAY);
            DrawRectangle(obstacles[i].x + 8, obstacles[i].y - 35, 25, 40, GREEN); // simple tree top
        }

        // Draw skier
        DrawRectangle(playerPos.x, playerPos.y, 65, 75, BLUE);                    // body
        DrawCircle(playerPos.x + 35, playerPos.y + 25, 18, BEIGE);               // head
        DrawRectangle(playerPos.x + 10, playerPos.y + 70, 45, 12, DARKBLUE);     // skis

        if (onGround)
            DrawText("SKIING", playerPos.x + 5, playerPos.y - 25, 18, WHITE);
        else
            DrawText("FLYING", playerPos.x + 5, playerPos.y - 25, 18, YELLOW);

        // HUD
        DrawText(TextFormat("DISTANCE: %05i m", score/3), 20, 20, 28, RAYWHITE);
        DrawText("SPACE = Jump", screenWidth - 220, 20, 22, LIGHTGRAY);

        if (gameOver) {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
            DrawText("THE MOUNTAIN WON", screenWidth/2 - 180, 180, 50, RED);
            DrawText("Press R to Try Again", screenWidth/2 - 140, 260, 28, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
