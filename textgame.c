#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define MAX_TEXTS 120

typedef struct {
    Vector2 position;
    Color color;
    float rotation;
    float scale;
    float speed;
    char text[32];
} GlitchText;

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;
    
    InitWindow(screenWidth, screenHeight, "Glitchy Text - Color Chaos");
    SetTargetFPS(60);
    srand(time(NULL));

    GlitchText texts[MAX_TEXTS];

    const char* words[] = {
        "GROK", "xAI", "NEURAL", "COSMOS", "VOID", "ECHO", 
        "GLITCH", "NEXUS", "QUANTUM", "DREAM", "CHAOS", "LIGHT",
        "MATRIX", "PULSE", "SYNAPSE", "ETHEREAL", "LUMEN", "FLUX"
    };

    // Initialize texts
    for (int i = 0; i < MAX_TEXTS; i++) {
        texts[i].position = (Vector2){
            (float)GetRandomValue(0, screenWidth),
            (float)GetRandomValue(0, screenHeight)
        };
        
        texts[i].color = WHITE;
        texts[i].rotation = (float)GetRandomValue(-15, 15);
        texts[i].scale = (float)GetRandomValue(40, 110) / 100.0f;
        texts[i].speed = (float)GetRandomValue(10, 35) / 10.0f;
        
        int wordIndex = GetRandomValue(0, 17);
        TextCopy(texts[i].text, words[wordIndex]);
    }

    float time = 0.0f;

    while (!WindowShouldClose())
    {
        time += GetFrameTime();

        // Update glitch effect
        for (int i = 0; i < MAX_TEXTS; i++)
        {
            // Color cycling (beautiful rainbow + flicker)
            float hue = fmodf(time * texts[i].speed * 40 + i * 8, 360);
            texts[i].color = ColorFromHSV(hue, 0.85f, 1.0f);
            
            // Subtle floating movement
            texts[i].position.y += sinf(time * texts[i].speed + i) * 0.6f;
            
            // Glitch jitter
            if (GetRandomValue(0, 100) < 8) {
                texts[i].position.x += GetRandomValue(-6, 6);
                texts[i].position.y += GetRandomValue(-4, 4);
                texts[i].rotation += GetRandomValue(-8, 8);
            }
            
            // Keep on screen
            if (texts[i].position.x < -100) texts[i].position.x = screenWidth + 50;
            if (texts[i].position.x > screenWidth + 100) texts[i].position.x = -50;
            if (texts[i].position.y < -50) texts[i].position.y = screenHeight + 50;
            if (texts[i].position.y > screenHeight + 50) texts[i].position.y = -30;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        // Draw all glitch texts
        for (int i = 0; i < MAX_TEXTS; i++)
        {
            // Optional: Draw a faint shadow for depth
            DrawTextEx(GetFontDefault(), texts[i].text,
                (Vector2){texts[i].position.x + 3, texts[i].position.y + 3},
                48 * texts[i].scale, 2, Fade(BLACK, 0.4f));

            DrawTextEx(GetFontDefault(), texts[i].text,
                texts[i].position,
                48 * texts[i].scale, 2, texts[i].color);
        }

        // Overlay info
        DrawText("GLITCH AESTHETIC", 20, 20, 30, Fade(WHITE, 0.3f));
        DrawFPS(screenWidth - 100, 20);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
