#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 700;
    
    InitWindow(screenWidth, screenHeight, "The Holy Mountain - Arnold Fanck Inspired");
    SetTargetFPS(60);
    srand(time(NULL));

    Vector2 player = {180, 480};
    float vy = 0.0f;
    int progress = 0;
    bool gameOver = false;
    bool won = false;

    float cameraY = 0.0f;

    // Snow particles
    #define MAX_SNOW 160
    Vector2 snow[MAX_SNOW];
    for (int i = 0; i < MAX_SNOW; i++) {
        snow[i].x = GetRandomValue(0, screenWidth);
        snow[i].y = GetRandomValue(-300, screenHeight);
    }

    while (!WindowShouldClose())
    {
        if (!gameOver && !won)
        {
            // Controls
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) player.x += 3.0f;
            if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) player.x -= 3.0f;
            if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) vy -= 0.55f;
            if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) vy += 0.4f;

            vy += 0.22f;           // gravity
            if (vy > 7.0f) vy = 7.0f;
            player.y += vy;

            // Keep player in bounds
            if (player.x < 50) player.x = 50;
            if (player.x > screenWidth - 80) player.x = screenWidth - 80;

            progress += 1;
            cameraY = progress * 0.22f;

            if (progress > 3200) won = true;

            // Random falling ice hazard
            if (GetRandomValue(1, 55) == 1) {
                float hazardX = GetRandomValue(100, screenWidth - 100);
                if (fabs(player.x - hazardX) < 80 && player.y < 450) {
                    gameOver = true;
                }
            }
        }

        // Restart
        if ((gameOver || won) && IsKeyPressed(KEY_R)) {
            player = (Vector2){180, 480};
            vy = 0.0f;
            progress = 0;
            cameraY = 0.0f;
            gameOver = false;
            won = false;
        }

        BeginDrawing();
        ClearBackground((Color){5, 10, 25, 255});

        // Background mountains (parallax)
        DrawRectangleGradientV(0, -cameraY*0.25f, screenWidth, screenHeight + 400, 
                             (Color){35,40,60,255}, (Color){90,95,115,255});

        // Distant peaks
        DrawTriangle((Vector2){-150, 300 - cameraY*0.25f}, 
                     (Vector2){500, 20 - cameraY*0.25f}, 
                     (Vector2){1050, 380 - cameraY*0.25f}, (Color){70,75,90,255});

        // Main majestic mountain
        DrawTriangle((Vector2){-200, screenHeight + 100 - cameraY*0.75f}, 
                     (Vector2){620, screenHeight - 480 - cameraY*0.75f}, 
                     (Vector2){1450, screenHeight + 120 - cameraY*0.75f}, 
                     (Color){225, 230, 245, 255});

        // Snow field
        DrawRectangle(0, 520 - (int)cameraY, screenWidth, 400, (Color){240, 245, 255, 255});

        // Snowfall
        for (int i = 0; i < MAX_SNOW; i++) {
            snow[i].y += 1.6f;
            if (snow[i].y > screenHeight + 50) snow[i].y = -30;
            DrawCircle(snow[i].x, snow[i].y - cameraY * 0.3f, 2.5f, WHITE);
        }

        // Climber
        DrawRectangle(player.x + 10, player.y - 48, 32, 52, (Color){20, 40, 90, 255});   // jacket
        DrawCircle(player.x + 26, player.y - 58, 15, BEIGE);                            // head
        DrawRectangle(player.x + 15, player.y - 22, 38, 12, (Color){30, 25, 20, 255});  // backpack
        DrawLine(player.x + 30, player.y - 45, player.x + 55, player.y - 75, GOLD);     // ice axe

        // HUD
        DrawText(TextFormat("ALTITUDE  %dm", progress / 4), 40, 25, 28, RAYWHITE);
        DrawText("WASD / Arrows - Move", screenWidth - 320, 25, 20, Fade(WHITE, 0.6f));

        if (gameOver) {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.8f));
            DrawText("THE MOUNTAIN CLAIMED YOU", screenWidth/2 - 280, 220, 48, RED);
            DrawText("Press R to Try Again", screenWidth/2 - 160, 300, 28, WHITE);
        }

        if (won) {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.75f));
            DrawText("SUMMIT REACHED", screenWidth/2 - 220, 200, 52, GOLD);
            DrawText("Press R to Climb Again", screenWidth/2 - 170, 290, 28, WHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
