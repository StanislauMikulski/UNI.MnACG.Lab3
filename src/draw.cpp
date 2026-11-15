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

void draw(SDL_Surface *s){
  vf
}
