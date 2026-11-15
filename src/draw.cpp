#include "draw.h"
#include <cmath>
#include <iostream>

#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(b) << 8) | g) << 8) | r)

void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    assert(NULL != surface);
    assert(x < SCREEN_WIDTH);
    assert(y < SCREEN_HEIGHT);
    Uint32 *pixels = (Uint32 *)surface->pixels;
    pixels[(y * surface->w) + x] = pixel;
}

Uint32 get_pixel32(SDL_Surface *surface, int x, int y)
{
    assert(NULL != surface);
    assert(x < SCREEN_WIDTH);
    assert(y < SCREEN_HEIGHT);
    Uint32 *pixels = (Uint32 *)surface->pixels;
    return pixels[(y * surface->w) + x];
}

// рисуем пиксель только если он внутри экрана
void pixel(SDL_Surface *s, int x, int y, Uint32 color)
{
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
        put_pixel32(s, x, y, color);
}

// радиус окружности (задаёт пользователь)
double R = 150;
// размер стороны квадрата
double side = 200;
// время для анимации
double t_anim = 0;
// спросили ли уже радиус
bool asked = false;

// рисуем окружность (это отсекающее окно)
void draw_circle(SDL_Surface *s, int cx, int cy, double r)
{
    for (double a = 0; a < 6.2832; a = a + 0.002)
    {
        int x = cx + r * cos(a);
        int y = cy + r * sin(a);
        pixel(s, x, y, RGB32(255, 255, 255));
    }
}

// рисуем отрезок через параметрическое уравнение:
// x = x1 + t*(x2 - x1), y = y1 + t*(y2 - y1), t от 0 до 1
// внутри окружности линия сплошная, снаружи - пунктир
void draw_line(SDL_Surface *s, double x1, double y1, double x2, double y2,
               int cx, int cy, double r)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    int n = (int)sqrt(dx * dx + dy * dy); // сколько пикселей в отрезке

    for (int i = 0; i <= n; i++)
    {
        double t = (double)i / n;
        double x = x1 + t * dx;
        double y = y1 + t * dy;

        // проверяем, лежит ли точка внутри окружности
        double d = (x - cx) * (x - cx) + (y - cy) * (y - cy);

        if (d <= r * r)
        {
            // видимая часть - сплошная
            pixel(s, (int)x, (int)y, RGB32(0, 255, 0));
        }
        else
        {
            // невидимая часть - пунктир (рисуем 6 пикселей, 6 пропускаем)
            if ((i / 6) % 2 == 0)
                pixel(s, (int)x, (int)y, RGB32(255, 0, 0));
        }
    }
}

void draw(SDL_Surface *s)
{
    // в самом начале спрашиваем радиус окна у пользователя
    if (asked == false)
    {
        std::cout << "Vvedite radius okna: ";
        std::cin >> R;
        asked = true;
    }

    // центр окружности - середина экрана
    int cx = SCREEN_WIDTH / 2;
    int cy = SCREEN_HEIGHT / 2;

    // квадрат двигается влево-вправо и крутится
    t_anim = t_anim + 0.02;
    double sx = cx + 250 * sin(t_anim); // центр квадрата по x
    double sy = cy;                     // центр квадрата по y
    double angle = t_anim * 2;          // угол поворота

    double h = side / 2;

    // 4 вершины квадрата (сначала без поворота, относительно центра)
    double px[4] = {-h, h, h, -h};
    double py[4] = {-h, -h, h, h};

    // поворачиваем каждую вершину вокруг центра квадрата
    double vx[4];
    double vy[4];
    for (int i = 0; i < 4; i++)
    {
        vx[i] = sx + px[i] * cos(angle) - py[i] * sin(angle);
        vy[i] = sy + px[i] * sin(angle) + py[i] * cos(angle);
    }

    // рисуем окружность
    draw_circle(s, cx, cy, R);

    // рисуем 4 стороны квадрата
    for (int i = 0; i < 4; i++)
    {
        int next = i + 1;
        if (next == 4)
            next = 0;
        draw_line(s, vx[i], vy[i], vx[next], vy[next], cx, cy, R);
    }
}
