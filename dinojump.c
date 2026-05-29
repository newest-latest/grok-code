#include "raylib.h"

typedef struct {
    Vector2 position;
    float velocity;
    bool isJumping;
    bool isDucking;
    Rectangle frame;
} Player;

typedef struct {
    Vector2 position;
    int type; // 0 = cactus, 1 = bird
} Obstacle;

int main(void) {
    const int screenWidth = 900;
    const int screenHeight = 500;
    InitWindow(screenWidth, screenHeight, "Grok Runner - xAI Edition");
    SetTargetFPS(60);

    // Player (Robot)
    Player player = {0};
    player.position = (Vector2){100, 300};
    player.velocity = 0.0f;
    player.isJumping = false;
    player.isDucking = false;

    // Game variables
    float speed = 6.0f;
    int score = 0;
    int highScore = 0;
    bool gameOver = false;
    bool started = false;
    int frameCounter = 0;

    Obstacle obstacles[10] = {0};
    int obstacleCount = 0;

    float groundY = 340;

    while (!WindowShouldClose()) {
        if (!started) {
            if (IsKeyPressed(KEY_SPACE)) started = true;
        }

        if (!gameOver && started) {
            frameCounter++;
            score = frameCounter / 4;

            // Speed increase
            if (score > 0 && score % 300 == 0) speed += 0.3f;

            // Player controls
            player.isDucking = IsKeyDown(KEY_DOWN);

            if (IsKeyPressed(KEY_SPACE) && !player.isJumping) {
                player.velocity = -18.0f;
                player.isJumping = true;
            }

            // Physics
            if (player.isJumping) {
                player.velocity += 0.9f; // gravity
                player.position.y += player.velocity;
            }

            if (player.position.y >= groundY) {
                player.position.y = groundY;
                player.isJumping = false;
                player.velocity = 0;
            }

            // Spawn obstacles
            if (frameCounter % (60 - (int)(speed * 2)) == 0 && obstacleCount < 10) {
                obstacles[obstacleCount].position.x = screenWidth + GetRandomValue(50, 300);
                obstacles[obstacleCount].position.y = (GetRandomValue(0, 1) == 0) ? groundY : 220;
                obstacles[obstacleCount].type = (obstacles[obstacleCount].position.y < groundY) ? 1 : 0;
                obstacleCount++;
            }

            // Update obstacles
            for (int i = 0; i < obstacleCount; i++) {
                obstacles[i].position.x -= speed;

                // Collision
                Rectangle playerRect = { player.position.x + 10, player.position.y + (player.isDucking ? 30 : 10), 
                                       50, player.isDucking ? 40 : 70 };

                Rectangle obsRect = { obstacles[i].position.x, obstacles[i].position.y, 
                                    obstacles[i].type == 0 ? 40 : 60, obstacles[i].type == 0 ? 70 : 40 };

                if (CheckCollisionRecs(playerRect, obsRect)) {
                    gameOver = true;
                    if (score > highScore) highScore = score;
                }

                // Remove off-screen
                if (obstacles[i].position.x < -50) {
                    for (int j = i; j < obstacleCount - 1; j++) {
                        obstacles[j] = obstacles[j + 1];
                    }
                    obstacleCount--;
                    i--;
                }
            }
        }

        // Restart
        if (gameOver && IsKeyPressed(KEY_R)) {
            gameOver = false;
            started = false;
            score = 0;
            frameCounter = 0;
            speed = 6.0f;
            obstacleCount = 0;
            player.position.y = groundY;
            player.isJumping = false;
        }

        BeginDrawing();
        ClearBackground((score > 400) ? DARKBLUE : SKYBLUE);

        // Ground
        DrawRectangle(0, groundY + 60, screenWidth, screenHeight - groundY - 60, DARKGREEN);
        DrawLine(0, groundY + 60, screenWidth, groundY + 60, WHITE);

        if (started) {
            // Draw player (simple robot)
            Color robotColor = (score > 400) ? SKYBLUE : LIME;
            DrawRectangle(player.position.x, player.position.y, 70, 70, robotColor);           // body
            DrawRectangle(player.position.x + 15, player.position.y + 10, 40, 25, DARKGRAY);  // head
            DrawCircle(player.position.x + 25, player.position.y + 55, 12, DARKGRAY);         // leg
            DrawCircle(player.position.x + 50, player.position.y + 55, 12, DARKGRAY);

            // Draw obstacles
            for (int i = 0; i < obstacleCount; i++) {
                if (obstacles[i].type == 0) {
                    // Cactus
                    DrawRectangle(obstacles[i].position.x + 10, obstacles[i].position.y, 25, 70, GREEN);
                    DrawRectangle(obstacles[i].position.x, obstacles[i].position.y + 20, 45, 20, GREEN);
                } else {
                    // Bug Drone
                    DrawRectangle(obstacles[i].position.x, obstacles[i].position.y, 55, 35, PURPLE);
                    DrawTriangle((Vector2){obstacles[i].position.x + 55, obstacles[i].position.y + 10},
                                 (Vector2){obstacles[i].position.x + 70, obstacles[i].position.y + 17},
                                 (Vector2){obstacles[i].position.x + 55, obstacles[i].position.y + 25}, PURPLE);
                }
            }
        } else {
            DrawText("GROK RUNNER", screenWidth/2 - 140, 120, 50, WHITE);
            DrawText("Press SPACE to Start", screenWidth/2 - 130, 200, 25, LIGHTGRAY);
        }

        if (gameOver) {
            DrawText("GAME OVER", screenWidth/2 - 130, 150, 50, RED);
            DrawText("Press R to Restart", screenWidth/2 - 120, 220, 25, LIGHTGRAY);
        }

        // HUD
        DrawText(TextFormat("Score: %05i", score), 20, 20, 25, WHITE);
        DrawText(TextFormat("HI %05i", highScore), screenWidth - 180, 20, 25, LIGHTGRAY);

        DrawText("SPACE = Jump | DOWN = Duck", screenWidth/2 - 160, screenHeight - 30, 18, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
