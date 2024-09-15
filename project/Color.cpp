#include "Color.hpp"
#include<cassert>

namespace prog {
    Color::Color()
    {
        //Default constructor. By default, the color should correspond to black, i.e., (0, 0, 0).
        r = 0;
        g = 0;
        b = 0;
    }

    Color::Color(const Color& other)
    {
        //Copy constructor.
         this -> r = other.r;
         this -> g = other.g;
         this -> b = other.b;
    }

    Color::Color(rgb_value red, rgb_value green, rgb_value blue)
    {
        //Constructor using supplied (r, g, b) values.
        this -> r = red;
        this -> g = green;
        this -> b = blue;
    }

    //(Next 3 functions): Get values for individual RGB color channels.
    rgb_value Color::red() const 
    {

        return r;
    }

    rgb_value Color::green() const 
    {
        return g;
    }

    rgb_value Color::blue() const 
    {

        return b;
    }
    
    //(Next 3 functions): Get (mutable) references for individual RGB color channels.
    rgb_value& Color::red()  
    {  
        return r;
    }

    rgb_value& Color::green()  
    {
        return g;
    }

    rgb_value& Color::blue()  
    {
        return b; 
    }

}

