#include "raylib.h"
#include <stdlib.h>
#include <time.h>

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 700;
    InitWindow(screenWidth, screenHeight, "The Holy Mountain - Arnold Fanck Inspired");
    SetTargetFPS(60);
    srand(time(NULL));

    // Player (climber)
    Vector2 player = {180, 500};
    float vy = 0.0f;
    int progress = 0;
    bool gameOver = false;
    bool won = false;

    // Camera scroll
    float cameraY = 0.0f;

    // Snow particles
    #define MAX_SNOW 180
    Vector2 snow[MAX_SNOW];
    for (int i = 0; i < MAX_SNOW; i++) {
        snow[i] = (Vector2){ GetRandomValue(0, screenWidth), GetRandomValue(-200, screenHeight) };
    }

    while (!WindowShouldClose())
    {
        if (!gameOver && !won)
        {
            // Controls - deliberate and slow (mountaineering feel)
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) player.x += 2.8f;
            if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) player.x -= 2.8f;
            if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) vy -= 0.45f;
            if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) vy += 0.35f;

            // Gravity + air resistance
            vy += 0.18f;
            if (vy > 6.0f) vy = 6.0f;
            player.y += vy;

            // Progress
            progress += 1;
            cameraY = progress * 0.18f;

            // Boundaries
            if (player.x < 60) player.x = 60;
            if (player.x > screenWidth - 100) player.x = screenWidth - 100;

            // Win condition
            if (progress > 2800) won = true;

            // Random hazards (falling ice)
            if (GetRandomValue(1, 65) == 1 && player.y < 400) {
                if (abs(player.x - GetRandomValue(100, screenWidth-100)) < 90) {
                    gameOver = true;
                }
            }
        }

        // Restart
        if ((gameOver || won) && IsKeyPressed(KEY_R)) {
            player = (Vector2){180, 500};
            vy = 0;
            progress = 0;
            cameraY = 0;
            gameOver = false;
            won = false;
        }

        BeginDrawing();
        ClearBackground((Color){8, 12, 22, 255}); // deep alpine blue

        // Distant mountains (parallax)
        DrawRectangleGradientV(0, -cameraY*0.3f, screenWidth, 900, (Color){40,45,65,255}, (Color){90,95,120,255});
        DrawTriangle((Vector2){-200,-cameraY*0.3f+300}, (Vector2){450,-cameraY*0.3f-180}, (Vector2){1100,-cameraY*0.3f+380}, (Color){75,80,95,255});
        DrawTriangle((Vector2){400,-cameraY*0.3f+280}, (Vector2){850,-cameraY*0.3f-120}, (Vector2){1600,-cameraY*0.3f+320}, (Color){65,70,88,255});

        // Closer mountain
        DrawTriangle((Vector2){-100, screenHeight - cameraY*0.7f}, 
                     (Vector2){600, screenHeight - cameraY*0.7f - 520}, 
                     (Vector2){1300, screenHeight - cameraY*0.7f}, 
                     (Color){235, 240, 250, 255});

        // Snow ground / ledges
        DrawRectangle(0, 520 - (int)cameraY, screenWidth, 300, (Color){245, 248, 255, 255});

        // Snow particles
        for (int i = 0; i < MAX_SNOW; i++) {
            snow[i].y += 1.8f + (i % 5) * 0.1f;
            if (snow[i].y > screenHeight) snow[i].y = -20;
            DrawCircle(snow[i].x, snow[i].y - cameraY*0.4f, 2.2f, WHITE);
        }

        // The Climber
        DrawRectangle(player.x + 8, player.y - 45, 28, 48, DARKBLUE);           // body
        DrawCircle(player.x + 22, player.y - 55, 14, BEIGE);                    // head
        DrawRectangle(player.x + 12, player.y - 18, 35, 8, (Color){40,30,20,255}); // backpack
        DrawLine(player.x + 25, player.y - 35, player.x + 48, player.y - 65, GOLD); // ice axe

        // HUD
        DrawText(TextFormat("ALTITUDE: %dm", progress/3), 30, 30, 26, RAYWHITE);
        DrawText("WASD / Arrow Keys", screenWidth - 280, 30, 20, Fade(WHITE, 0.5f));

        if (gameOver) {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.75f));
            DrawText("THE MOUNTAIN CLAIMED YOU", screenWidth/2 - 260, 220, 48, RED);
            DrawText("Press R to Return to Base Camp", screenWidth/2 - 200, 300, 24, WHITE);
        }

        if (won) {
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
            DrawText("YOU REACHED THE SUMMIT", screenWidth/2 - 240, 200, 48, GOLD);
            DrawText("A true Bergfilm moment.", screenWidth/2 - 160, 280, 28, WHITE);
            DrawText("Press R to Climb Again", screenWidth/2 - 140, 340, 24, LIGHTGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
