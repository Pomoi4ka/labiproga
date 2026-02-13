#version 330

#define PI 3.14159265359f
#define STEP (60.f/180.f*PI)

uniform vec2 cam;
uniform float scale;
uniform float radius;
uniform ivec2 screen;

#define DIAGONAL 0

#define FOREGROUND vec3(0.0)
#define BACKGROUND vec3(1.0)

vec3 test(vec2 p)
{
    float x = p.x, y = p.y;

    float ang = PI/2;
#if DIAGONAL
    vec2 t = p/(radius*2);
    int tx = int(t.x), ty = int(t.y);
    switch ((int(p.x < 0)<<1)|int(p.y < 0)) {
    case 1: if (abs(tx) + 1 != abs(ty)) return BACKGROUND; break;
    case 2: if (abs(tx) - 1 != abs(ty)) return BACKGROUND; break;
    default: if (abs(tx) != abs(ty)) return BACKGROUND; break;
    }
#endif

    if (radius > 0) p = mod(p, radius * 2);
    p -= vec2(radius, radius);
    x = p.x, y = p.y;
    // x += x > 0 ? -radius : radius;
    // y += y > 0 ? -radius : radius;

    if (x*x + y*y > radius*radius) return BACKGROUND;

    for (int i = 0; i < 6; ++i) {
        float lhs, rhs, sv, cv;
        sv = sin(ang);
        cv = cos(ang);
        lhs = sqrt(3) * abs(y*cv - x*sv);
        rhs = y*sv + x*cv;
        if (abs(lhs - rhs) <= 1e-1) return FOREGROUND;
        if (lhs <= rhs && lhs <= radius - rhs) return BACKGROUND;
        ang += STEP;
    }

    return FOREGROUND;
}

void main()
{
    vec2 d = gl_FragCoord.xy - screen/2;
    vec2 p = (d - vec2(cam.x, -cam.y))*scale;

    gl_FragColor = vec4(test(p), 1.0);
}
