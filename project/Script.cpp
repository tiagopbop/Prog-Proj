#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"
#include <vector>
#include <algorithm>

using namespace std;

namespace prog {
    //Use to read color values from a script file
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) : //constructor, with filename indicating the script to be processed

            image(nullptr), input(filename) {
        }

    void Script::clear_image_if_any() 
    {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() 
    {          
        //Destructor. If an image is loaded, its memory is released
        clear_image_if_any();
    }

    void Script::run() 
    {   //Process commands in the script file
        string command;
        while (input >> command) //Checks all the commands and runs them
        {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            //Other commands require an image to be previously loaded
            if (command == "save") {
                save();
                continue;
            } 
            if (command == "invert")
            {
                invert();
                continue;
            }
            if (command == "to_gray_scale")
            {
                to_gray_scale();
                continue;
            }
            
            if (command == "replace")
            {   
                replace();
                continue;
            }
            
            if (command == "fill")
            {
                fill();
                continue;
            }
            
            if (command == "h_mirror")
            {
                h_mirror();
                continue;
            }
            
            if (command == "v_mirror")
            {
                v_mirror();
                continue;
            }
            
            if (command == "add")
            {
                add();
                continue;
            }
            
            if (command == "crop")
            {
                crop();
                continue;
            }
            
            if (command == "rotate_left")
            {
                rotate_left();
                continue;
            }
            
            if (command == "rotate_right")
            {
                rotate_right();
                continue;
            }
            
            if (command == "median_filter")
            {   
                median_filter();
                continue;
            }
            
            if (command == "xpm2_open")
            {
                clear_image_if_any();
                string fname;
                input >> fname;
                image = loadFromXPM2(fname);
                continue;
            }
            
            if (command == "xpm2_save")
            {
                string fname;
                input >> fname;
                saveToXPM2(fname,image);
                continue;
            }
        }
    }

    void Script::open() 
    {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }

    void Script::blank() 
    {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }

    void Script::save() 
    {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }

    void Script::invert() 
    {
        // Transforms each individual pixel (r, g, b) to (255-r,255-g,255-b).
        int w;
        int h;
        w = image -> width() ;
        h = image -> height() ;

        for(int x = 0; x < w; x++) //Runs through the width
        {
            for(int y =0; y < h; y++) //Runs through the height
            {
                Color& pixels = (*image).at(x,y); //Get the (r,g,b) of the pixel at cordinates(x,y)
                pixels.red() = 255 - pixels.red(); 
                pixels.green() = 255 - pixels.green(); 
                pixels.blue() = 255 - pixels.blue();

            }
        }
    }

    void Script:: to_gray_scale()
    {
        //Transforms each individual pixel (r, g, b) to (v, v, v) where v = (r + g + b)/3. 
        int w;
        int h;
        w = image -> width() ;
        h = image -> height() ;

        for(int x = 0; x < w; x++) //Runs through the width
        {
            for(int y =0; y < h; y++) //Runs through the height
            {
                Color& pixels = (*image).at(x,y); //Get the (r,g,b) of the pixel at cordinates(x,y)
                int grey;
                grey = (pixels.red() + pixels.green() + pixels.blue()) / 3;
                pixels.red() = grey;
                pixels.green() = grey; 
                pixels.blue() = grey;
            }
        }
    }

    void Script:: replace()
    {
        //Replaces all (r1,  g1, b1) pixels by (r2,  g2, b2).
        int r1,g1,b1,r2,g2,b2;
        input >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;
        int w;
        int h;
        w = image -> width() ;
        h = image -> height() ;

        for(int x = 0; x < w; x++) //Runs through the width
        {
            for(int y =0; y < h; y++) //Runs through the height
            {
                Color& pixels = (*image).at(x,y); //Get the (r,g,b) of the pixel at cordinates(x,y)

                if(pixels.red()==r1 && pixels.green()==g1 && pixels.blue()== b1) //Checks if the pixels(r,g,b) is equal to (r1,g1,b1)
                {
                    //If it is changes all this pixel's (r,g,b) to (r2,g2,b2)
                    pixels.red() = r2;
                    pixels.green() = g2;
                    pixels.blue() = b2; 
                }
            }
        }
    }

    void Script::fill()
    {
        //Assign (r, g, b) to all pixels contained in rectangle defined by top-left corner (x, y), width w, and height h, i.e., all pixels (x', y') 
        //such that x <= x' < x + w and y <= y' < y + h. You may assume that the rectangle is always within the current image bounds.
        int x,y,w,h,r,g,b;
        input >> x >> y >> w >> h >> r >> g >> b;
        int moving_w; 
        int moving_h;
        moving_w = image -> width();
        moving_h = image -> height();

        for(int moving_x = 0; moving_x < moving_w; moving_x++) //Runs through the width
        {
            for(int moving_y =0; moving_y < moving_h; moving_y++) //Runs through the height
            {
                Color& pixels = (*image).at(moving_x,moving_y); //Get the (r,g,b) of the pixel at cordinates(moving_x,moving_y)

                if(moving_x < x + w && x <= moving_x && moving_y < y + h && y <= moving_y) //Checks if the pixel is within the bounds of the retangle to fill
                {
                    pixels.red() = r;
                    pixels.green() = g;
                    pixels.blue() = b; 
                }
            }
        }
    }

    void Script::h_mirror()
    {
        //Mirror image horizontally. Pixels (x, y) and (width() - 1 - x, y) for all 0 <= x < width() / 2 and 0 <= y < height().
        int w;
        int h;
        w = image -> width() ;
        h = image -> height() ;

        for(int x = 0; x < w/2; x++) //Runs through half the width
        {
            for(int y =0; y < h; y++) //Runs through the height
            {   
                swap((*image).at(w-1-x,y), (*image).at(x,y)); //Swaps the (r,g,b) of the two pixels
            }
        }
    }

    void Script::v_mirror()
    {
        //Mirror image vertically. Pixels (x, y) and (x, height() - 1 - y) for all 0 <= x < width() and 0 <= y < height() / 2.
        int w;
        int h;
        w = image -> width() ;
        h = image -> height() ;

        for(int x = 0; x < w; x++) //Runs through the width
        {
            for(int y =0; y < h/2; y++) //Runs through half the height
            {   
                swap((*image).at(x,h-1-y),(*image).at(x,y)); //Swaps the (r,g,b) of the two pixels
            }
        }

    }    

    void Script:: add()
    {
        //Copy all pixels from image stored in PNG file filename, except pixels in that image with “neutral” color (r, g, b), 
        //to the rectangle of the current image with top-left corner (x, y) of the current image.
        string filename;
        int r,g,b,x,y;
        input >> filename >> r >> g >> b >> x >> y;
        Image *secondary = loadFromPNG(filename); //Loads a second image that is going to be placed in the main one
        int w2; 
        int h2;
        w2 = secondary -> width();
        h2 = secondary -> height();

        for(int moving_x = 0; moving_x < w2; moving_x++) //Runs through the width of the second image
        {
            for(int moving_y = 0; moving_y < h2; moving_y++) // Runs through the height of the second image
            {   
                Color& pixels = (*image).at(x+moving_x,y+moving_y); //Get the (r,g,b) of the pixel, from the main image, at cordinates(x+moving_x,y+moving_y)
                Color& secondary_pixels = (*secondary).at(moving_x, moving_y); //Get the (r,g,b) of the pixel,from the second image, at cordinates(moving_x,moving_y)
                
                if(secondary_pixels.red()!=r || secondary_pixels.green()!=g || secondary_pixels.blue()!=b) //Checks if the color is not neutral
                {
                    pixels.red() = secondary_pixels.red();
                    pixels.green() = secondary_pixels.green();
                    pixels.blue() = secondary_pixels.blue();
                }

            }
        }
        delete secondary; //Delete second image to prevent memeory leaks
    }

    void Script:: crop()
    {
        //Crop the image, reducing it to all pixels contained in the rectangle defined by top-left corner (x, y), width w, and height h.
        int x,y,w,h;
        input >> x >> y >> w >> h;

        Image* temporary_image = new Image(w,h); //Create a new image with the dimensions of the new rectangle

        for(int moving_x=0; moving_x<w; moving_x++) //Runs through the width of the contained rectangle 
        {
            for(int moving_y=0; moving_y<h;moving_y++) //Runs through the height of the contained rectangle
            {
                (*temporary_image).at(moving_x,moving_y) = (*image).at(x+moving_x,y+moving_y); 
            }
        }
        clear_image_if_any(); //Delete image to then make it the new reduced version
        image = temporary_image;
    }

    void Script:: rotate_left()
    {
        //Rotate image left by 90 degrees.
        int w;
        int h;
        w = image -> width() ;
        h = image -> height() ;
        Image* temporary_image = new Image(h,w); //Create a new image with swaped width and height from the main image

        for(int x = 0; x<h;x++) //Runs through the new width(main image height)
        {
            for(int y = 0; y < w; y++) //Runs through the new height(main image width)
            {
                (*temporary_image).at(x,y) = (*image).at(w-y-1,x);
            }
        }     
        clear_image_if_any(); //Delete image to then make it the new rotated_left version
        image = temporary_image;   
    }

    void Script:: rotate_right()
    {
        //Rotate image right by 90 degrees.
        int w;
        int h;
        w = image -> width() ;
        h = image -> height() ;
        Image* temporary_image = new Image(h,w); //Create a new image with swaped width and height from the main image
        
        for(int x = 0; x<h;x++) //Runs through the new width(main image height)
        {
            for(int y = 0; y < w; y++) //Runs through the new height(main image width)
            {
                (*temporary_image).at(x,y) = (*image).at(y,h-x-1);
            }
        }     
        clear_image_if_any(); //Delete image to then make it the new rotated_right version
        image = temporary_image;   
    }

    void Script:: median_filter()
    {
        //Apply a median filter with window size ws >= 3 to the current image.
        int ws;
        input >> ws;
        int w;
        int h;
        w = image -> width() ;
        h = image -> height() ;
        Image* middle = new Image(w,h); //Create a new image with the same dimensions as the main one
        vector<int> list_r, list_g, list_b; //3 vectors used to calculate the median_filter of every pixel(r,g,b)

        for(int x = 0; x<w;x++) //Runs through the width
        {
            for(int y =0 ; y < h; y++) //Runs through the height
            {
                for(int fx = max(0,x-ws/2); fx<=min(w-1,x+ws/2);fx++) //Runs through the envolving pixels width-wise
                { 
                    for(int fy = max(0,y-ws/2); fy<=min(h-1,y+ws/2);fy++) //Runs through the envolving pixels height-wise
                    {
                        Color& pixel = (*image).at(fx,fy); //Gets the (r,g,b) at given cordinates(fx,fy) from the neighourhood
                        //Place the (r,g,b) in the corresponding vector
                        list_r.push_back(pixel.red());
                        list_g.push_back(pixel.green());
                        list_b.push_back(pixel.blue());

                    }

                }
                    //Sort the (r,g,b) values from each vector to get the median value
                    sort(list_r.begin(), list_r.end());
                    sort(list_g.begin(), list_g.end());
                    sort(list_b.begin(), list_b.end());

                    int size = list_r.size(); //Size of all the vectors (they are all the same)

                    if(size%2 == 0) //If the number of values is even the median is the sum of the middle values divided by 2
                    {
                        (*middle).at(x,y).red() = (list_r.at(size/2) + list_r.at(size/2 -1 ))/2;
                        (*middle).at(x,y).green() = (list_g.at(size/2) + list_g.at(size/2 -1 ))/2;
                        (*middle).at(x,y).blue() = (list_b.at(size/2) + list_b.at(size/2 -1 ))/2; 
                    }
                    
                    else //Else the number is odd, then the median is the middle value
                    {
                    (*middle).at(x,y).red() = list_r.at(size/2);
                    (*middle).at(x,y).green() = list_g.at(size/2);
                    (*middle).at(x,y).blue() = list_b.at(size/2);
                    }
     
                    //Cleans the vectors to use them again for the following pixels
                    list_r.clear();
                    list_g.clear();
                    list_b.clear();            

            }
        }     
        clear_image_if_any();  //Delete image to then make it the new median_filtered one
        image = middle;
    }

}

