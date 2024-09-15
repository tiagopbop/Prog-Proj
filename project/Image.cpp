#include "Image.hpp"

namespace prog
{
  Image::Image(int w, int h, const Color &fill)
  {
    //Create image with width w, height h, and all pixels set to color fill.
    // White is the default fill value, i.e., (255, 255, 255).
    this -> w = w;
    this -> h = h;
    pixels = new Color[w*h];
 
    for(int i=0; i<w*h; i++) //runs through all the pixels;
    {
      pixels[i] = fill;
    }
  }

  Image::~Image()
  {
    /*Destructor. If you use dynamically allocated memory explicitly,
    the destructor should take care of releasing that memory.
    Otherwise, the destructor code can be empty.*/
    delete[] pixels;
  }

  int Image::width() const
  {
    //Get image width;
    return w;
  }

  int Image::height() const
  {
    //Get image height;
    return h;
  }


  Color& Image::at(int x, int y)
  {
    //Get mutable reference to the value of pixel (x, y), where 0 <= x < width() and 0 <= y < height().
    return pixels[x + (y * w)];
  }

  const Color& Image::at(int x, int y) const
  {
    //Get read-only reference to the value of pixel (x, y).
    return pixels[x + (y * w)];
  }
}