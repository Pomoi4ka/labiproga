#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <raylib.h>
#include <raymath.h>

#include "ubuntu_mono.c"

#define ARRAY_LEN(xs) (sizeof(xs)/sizeof(*(xs)))

void draw_axis(Vector2 cam, int w, int h)
{
    int x = cam.x + w/2;
    int y = cam.y + h/2;
    DrawLine(x, 0, x, h, BLACK);
    DrawLine(0, y, w, y, BLACK);
}

static float rad = 5;

void map_xy(float *ox, float *oy)
{
    float x = *ox, y = *oy;

    if (rad > 0) {
        y = fmodf(y, rad*2);
        x = fmodf(x, rad*2);
        x += x > 0 ? -rad : rad;
        y += y > 0 ? -rad : rad;
    }

    *ox = x, *oy = y;
}

void draw_cursor_info(Font font, Vector2 cam, int w, int h, float scale)
{
    char buf[1024];
    Vector2 mpos = GetMousePosition();

    int dx = mpos.x - w/2;
    int dy = mpos.y - h/2;

    float ix = (dx - cam.x)*scale;
    float iy = (cam.y - dy)*scale;
    float fx, fy;

    fx = ix, fy = iy;
    map_xy(&fx, &fy);

    sprintf(buf, "(%.2f, %.2f) => (%.2f, %.2f)", ix, iy, fx, fy);
    DrawTextEx(font, buf, mpos, 30, 0, WHITE);
}

int test_xy(float x, float y)
{
    map_xy(&x, &y);
    static const float sqrt3 = sqrtf(3.f);

#define STEP (60.f/180.f*PI)
    float ang = STEP/2;

    if (x*x + y*y > rad*rad) return 0;
    for (int i = 0; i < 6; ++i) {
        float lhs, rhs, sv, cv;
        sv = sinf(ang);
        cv = cosf(ang);
        lhs = sqrt3*fabsf(y*cv - x*sv);
        rhs = y*sv + x*cv;
        if (fabsf(lhs - rhs) <= 1e-9) return 1;
        if (lhs <= rhs && lhs <= rad - rhs) return 0;
        ang += STEP;
    }
    return 1;
}

void draw_figures(Vector2 cam, int w, int h, float scale)
{
    for (int sy = 0; sy < h; ++sy) {
        for (int sx = 0; sx < w; ++sx) {
            int dx = sx - w/2;
            int dy = sy - h/2;

            float x = (cam.x - dx)*scale;
            float y = (cam.y - dy)*scale;

            if (test_xy(x, y)) DrawPixel(sx, sy, BLUE);
        }
    }
}

void random_nums(float *xs, int n)
{
    for (int i = 0; i < n; ++i) {
        xs[i] = rand() % 10;
    }
}

int main(int argc, char **argv)
{
    int w, h;
    Vector2 cam = {0};
    float scale = 1;

    InitWindow(800, 600, "Vis");

    Font font = LoadFontFromMemory(".ttf", ubuntu_mono, ubuntu_mono_len, 50, NULL, 0);
    Shader shader = LoadShader(0, "fragment.glsl");
    RenderTexture2D target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    int lScale = GetShaderLocation(shader, "scale");
    int lCam = GetShaderLocation(shader, "cam");
    int lRad = GetShaderLocation(shader, "radius");
    int lScreen = GetShaderLocation(shader, "screen");

    if (argc > 1) rad = strtof(argv[1], NULL);

    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        struct {int w, h;} screen = {w, h};
        w = GetScreenWidth();
        h = GetScreenHeight();
        if (w != screen.w || h != screen.h) {
            UnloadRenderTexture(target);
            target = LoadRenderTexture(w, h);
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            cam = Vector2Add(GetMouseDelta(), cam);
        ClearBackground(BLACK);
        BeginTextureMode(target);
        ClearBackground(BLACK);
        DrawRectangle(0, 0, w, h, BLACK);
        EndTextureMode();

        BeginDrawing();

        float ns = scale + GetMouseWheelMove() * -0.1*scale;
        if (ns > 0) scale = ns;

        // draw_figures(cam, w, h, scale);

        SetShaderValue(shader, lScale, &scale, SHADER_UNIFORM_FLOAT);
        SetShaderValue(shader, lRad, &rad, SHADER_UNIFORM_FLOAT);
        SetShaderValue(shader, lCam, &cam, SHADER_UNIFORM_VEC2);
        SetShaderValue(shader, lScreen, &screen, SHADER_UNIFORM_IVEC2);

        BeginShaderMode(shader);
        DrawTextureEx(target.texture, (Vector2){ 0.0f, 0.0f }, 0.0f, 1.0f, WHITE);
        EndShaderMode();

        draw_axis(cam, w, h);
        draw_cursor_info(font, cam, w, h, scale);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
