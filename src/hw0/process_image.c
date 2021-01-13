#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

  // error checking
  if (x < 0)
    x = 0;
  else if (x > im.w)
    x = im.w - 1;
  if (y < 0)
    y = 0;
  else if (y > im.h)
    y = im.h - 1;
  if (c < 0)
    c = 0;
  else if (c > im.c)
    c = im.c - 1;

  return im.data[c * (im.w * im.h) + y * (im.w) + x];
}

void set_pixel(image im, int x, int y, int c, float v)
{
  // error checking
  if (x < 0)
    x = 0;
  else if (x > im.w)
    x = im.w - 1;
  if (y < 0)
    y = 0;
  else if (y > im.h)
    y = im.h - 1;
  if (c < 0)
    c = 0;
  else if (c > im.c)
    c = im.c - 1;

  im.data[c * (im.w * im.h) + y * (im.w) + x] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    for (int i = 0; i < im.w; i++) {
      for (int j = 0; j < im.h; j++) {
        for (int k = 0; k < im.c; k++) {
          set_pixel(copy, i, j, k, get_pixel(im, i, j, k));
        }
      }
    }
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    for (int i = 0; i < im.w; i++) {
      for (int j = 0; j < im.h; j++) {
        set_pixel(gray, i, j, 0, 0.299 * get_pixel(im, i, j, 0)
                                 + 0.587 * get_pixel(im, i, j, 1)
                                 + 0.114 * get_pixel(im, i, j, 2));
      }
    }

    return gray;
}

void shift_image(image im, int c, float v)
{
  for (int i = 0; i < im.w; i++) {
    for (int j = 0; j < im.h; j++) {
      set_pixel(im, i, j, c, v + get_pixel(im, i, j, c));
    }
  }
}

void clamp_image(image im)
{
  for (int i = 0; i < im.h * im.w * im.c; i++) {
    if (im.data[i] < 0.0)
      im.data[i] = 0;
    else if (im.data[i] > 1.0)
      im.data[i] = 1;
  }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
  float C, H, S, V, R, G, B;
  for (int i = 0; i < im.w; i++) {
    for (int j = 0; j < im.h; j++) {
      R = get_pixel(im, i, j, 0);
      G = get_pixel(im, i, j, 1);
      B = get_pixel(im, i, j, 2);

      V = three_way_max(R, G, B);

      C = V - three_way_min(R, G, B);
      if ((R == 0) && (G == 0) && B == 0)
        S = 0;
      else
        S = C / V;

      if (C == 0)
        H = 0;
      else if (V == R)
        H = (G - B) / C;
      else if (V == G)
        H = ((B - R) / C) + 2;
      else
        H = ((R - G) / C) + 4;
      H /= 6;
      if (H < 0) {
        H++;
      }

      set_pixel(im, i, j, 0, H);
      set_pixel(im, i, j, 1, S);
      set_pixel(im, i, j, 2, V);
    }
  }
}

void hsv_to_rgb(image im)
{
  float C, X, m, H, S, V, R, G, B;
  for (int i = 0; i < im.w; i++) {
    for (int j = 0; j < im.h; j++) {
      H = get_pixel(im, i, j, 0);
      S = get_pixel(im, i, j, 1);
      V = get_pixel(im, i, j, 2);

      H *= 6;
      C = V * S;
      X = C * (1 - fabs(fmod(H, 2.0) - 1.0));
      m = V - C;

      if (H < 1.0) {
        R = C;
        G = X;
        B = 0;
      } else if (H < 2.0) {
        R = X;
        G = C;
        B = 0;
      } else if (H < 3.0) {
        R = 0;
        G = C;
        B = X;
      } else if (H < 4.0) {
        R = 0;
        G = X;
        B = C;
      } else if (H < 5.0) {
        R = X;
        G = 0;
        B = C;
      } else {
        R = C;
        G = 0;
        B = X;
      }

      set_pixel(im, i, j, 0, R + m);
      set_pixel(im, i, j, 1, G + m);
      set_pixel(im, i, j, 2, B + m);
    }
  }
}
