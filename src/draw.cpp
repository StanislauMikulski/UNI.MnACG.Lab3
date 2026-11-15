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

double R = 150;
double side = 200;
double t_anim = 0;
bool asked = false;

void pixel(SDL_Surface *s, int x, int y, Uint32 color){
  if( x >= 0 && x < SCREEN_WIDTH && y >= 0 && y <= SCREEN_HEIGHT){
    put_pixel32(s, x, y, color);
  }
}

void draw_circle(SDL_Surface *s, int cx, int cy, double r){
  for(double a = 0; a < 6.2832, a += 0.002){
    int x = cx + r * cos(a);
    int y = cy + r * sin(a);

    pixel(s,x,y, RGB32(255, 255, 255));
  }
}

void draw_line(SDL_Surface *s, double x1, double y1, double x2, double y2, int cx, int cy, double r){
  double dx = x2 - x1;
  double dy = y2 - y1;
  int n = (int)sqrt(dx * dx + dy * dy);

  for (int i = 0; i <= n; i++){
    double t = (double)i / n;
    double x = x1 + t * dx;
    double y = y1 + t * dy;
    double d = (x - cx) * (x - cx) + (y - cy) * (y - cy);

    if (d <= r * r){
      pixel(s, (int)x, (int)y, RGB32(0, 255, 0));
      }
    else{
      if ((i / 6) % 2 == 0) pixel(s, (int)x, (int)y, RGB32(255, 0, 0));
      }
  }
}


void draw(SDL_Surface *s)
{
    if (asked == false)
    {
        std::cout << "Enter window radius ";
        std::cin >> R;
        asked = true;
    }

    int cx = SCREEN_WIDTH / 2;
    int cy = SCREEN_HEIGHT / 2;

    t_anim = t_anim + 0.02;
    double sx = cx + 250 * sin(t_anim);
    double sy = cy;
    double angle = t_anim * 2;

    double h = side / 2;

    double px[4] = {-h, h, h, -h};
    double py[4] = {-h, -h, h, h};

    double vx[4];
    double vy[4];
    for (int i = 0; i < 4; i++)
    {
        vx[i] = sx + px[i] * cos(angle) - py[i] * sin(angle);
        vy[i] = sy + px[i] * sin(angle) + py[i] * cos(angle);
    }

    draw_circle(s, cx, cy, R);

    for (int i = 0; i < 4; i++)
    {
        int next = i + 1;
        if (next == 4)
            next = 0;
        draw_line(s, vx[i], vy[i], vx[next], vy[next], cx, cy, R);
    }
}
