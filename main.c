#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SCREEN_W 800
#define SCREEN_H 600

typedef struct {
    Vector2 pos;
    Vector2 target;
    float speed;
    double nextQuoteTime;
    int currentQuote;
    bool showingQuote;
} Goblin;

float Vector2Distance(Vector2 v1, Vector2 v2) {
    return sqrtf((v2.x - v1.x)*(v2.x - v1.x) + (v2.y - v1.y)*(v2.y - v1.y));
}

Vector2 Vector2Subtract(Vector2 v1, Vector2 v2) {
    Vector2 result = {v1.x - v2.x, v1.y - v2.y};
    return result;
}

Vector2 Vector2Add(Vector2 v1, Vector2 v2) {
    Vector2 result = {v1.x + v2.x, v1.y + v2.y};
    return result;
}

Vector2 Vector2Scale(Vector2 v, float scale) {
    Vector2 result = {v.x * scale, v.y * scale};
    return result;
}

float Vector2Length(Vector2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

Vector2 Vector2Normalize(Vector2 v) {
    float len = Vector2Length(v);
    if (len == 0) return (Vector2){0,0};
    return Vector2Scale(v, 1.0f / len);
}

const char *quotes[] = {
    "Life is a fart. If you force it, it's probably shit.",
    "Bro I just woke up and I'm already tired of tomorrow.",
    "This ain't a phase, it's a cry for help.",
    "I tried my best and it was mid.",
    "Motivation is a scam invented by productivity YouTubers.",
    "I'm not lazy, I'm on energy saving mode.",
    "The universe really said: good luck with that.",
    "Existing is expensive."
};

int main() {
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_ALWAYS_RUN);
    InitWindow(SCREEN_W, SCREEN_H, "Desktop Goblin");
    SetTargetFPS(60);

    srand(time(NULL));

    Goblin g;
    g.pos = (Vector2){(float)SCREEN_W/2, (float)SCREEN_H/2};
    g.target = g.pos;
    g.speed = 50;
    g.nextQuoteTime = GetTime() + GetRandomValue(3, 10);
    g.showingQuote = false;
    double quoteStartTime = 0;
    double quoteDuration = 3.0;

    while (!WindowShouldClose()) {

        float dt = GetFrameTime();

        if (Vector2Distance(g.pos, g.target) < 5) {
            g.target = (Vector2){
                GetRandomValue(50, SCREEN_W-50),
                GetRandomValue(50, SCREEN_H-50)
            };
        }

        Vector2 dir = Vector2Subtract(g.target, g.pos);
        if (Vector2Length(dir) > 0) {
            dir = Vector2Normalize(dir);
            g.pos = Vector2Add(g.pos, Vector2Scale(dir, g.speed * dt));
        }

        if (GetTime() > g.nextQuoteTime) {
            g.currentQuote = GetRandomValue(0, 7);
            g.showingQuote = true;
            quoteStartTime = GetTime();
            g.nextQuoteTime = GetTime() + GetRandomValue(5, 15);
        }

        if (g.showingQuote && (GetTime() - quoteStartTime) > quoteDuration) {
            g.showingQuote = false;
        }

        BeginDrawing();
        ClearBackground((Color){0,0,0,0});

        DrawCircleV(g.pos, 15, GREEN);
        DrawText("goblin", g.pos.x - 18, g.pos.y + 18, 10, DARKGREEN);

        if (g.showingQuote) {
            int bubbleW = 380;
            int bubbleH = 60;
            int bx = g.pos.x - (float)bubbleW/2;
            int by = g.pos.y - 90;

            DrawRectangle(bx, by, bubbleW, bubbleH, (Color){20,20,20,220});
            DrawRectangleLines(bx, by, bubbleW, bubbleH, WHITE);
            DrawText(quotes[g.currentQuote], bx + 10, by + 10, 12, RAYWHITE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
