#include <raylib.h>
#include <time.h>
#include <math.h>
int main(void)
{
    const int screenWidth = 600;
    const int screenHeight = 600;
    
    InitWindow(screenWidth, screenHeight, "Minimal Clock");
    SetTargetFPS(60);

    Vector2 center = { screenWidth/2, screenHeight/2 };
    float radius = 260.0f;

    while (!WindowShouldClose())
    {
        // Get real system time
        time_t now = time(NULL);
        struct tm *tm = localtime(&now);
        
        float seconds = tm->tm_sec;
        float minutes = tm->tm_min + seconds / 60.0f;
        float hours   = tm->tm_hour % 12 + minutes / 60.0f;

        // Calculate angles (0° = 12 o'clock, clockwise)
        float secAngle  = seconds * 6.0f;           // 360/60 = 6
        float minAngle  = minutes * 6.0f;
        float hourAngle = hours * 30.0f;            // 360/12 = 30

        BeginDrawing();
        ClearBackground(BLACK);

        // Clock face
        DrawCircleLines(center.x, center.y, radius, GRAY);
        DrawCircle(center.x, center.y, 8, WHITE);   // Center dot

        // Hour markers
        for (int i = 0; i < 12; i++) {
            float angle = i * 30.0f * DEG2RAD;
            Vector2 outer = {
                center.x + cosf(angle) * (radius - 10),
                center.y + sinf(angle) * (radius - 10)
            };
            Vector2 inner = {
                center.x + cosf(angle) * (radius - 40),
                center.y + sinf(angle) * (radius - 40)
            };
            DrawLineEx(inner, outer, 4, LIGHTGRAY);
        }

        // Hour hand
        Vector2 hourEnd = {
            center.x + cosf((hourAngle - 90) * DEG2RAD) * 110,
            center.y + sinf((hourAngle - 90) * DEG2RAD) * 110
        };
        DrawLineEx(center, hourEnd, 12, WHITE);

        // Minute hand
        Vector2 minEnd = {
            center.x + cosf((minAngle - 90) * DEG2RAD) * 180,
            center.y + sinf((minAngle - 90) * DEG2RAD) * 180
        };
        DrawLineEx(center, minEnd, 8, LIGHTGRAY);

        // Second hand
        Vector2 secEnd = {
            center.x + cosf((secAngle - 90) * DEG2RAD) * 200,
            center.y + sinf((secAngle - 90) * DEG2RAD) * 200
        };
        DrawLineEx(center, secEnd, 3, RED);

        // Small center cap
        DrawCircle(center.x, center.y, 10, DARKGRAY);

        DrawText("Minimal Clock", screenWidth/2 - 70, 30, 20, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
