#include "XPM2.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;
namespace prog {

    int hex_to_dec (string hex)
    {
        //returns the decimal value of a hexadecimal number
        int dec = 0;

        //Converts the second number to hexadecimal (*16⁰ = 1)
        if (hex[1] >= '0' && hex[1] <= '9') 
        {
            dec += (int(hex[1]) - 48) * 1;
        }
        else if (hex[1] >= 'A' && hex[1] <= 'F') 
        {
            dec += (int(hex[1]) - 55) * 1;
        }
        else if (hex[0] >= 'a' && hex[0] <= 'f') 
        {
            dec += (int(hex[0]) - 'a' + 10) * 1;
        }

        //Converts the first number to hexadecimal (*16¹ = 1)
        if (hex[0] >= '0' && hex[0] <= '9') 
        {
            dec += (int(hex[0]) - 48) * 16;
        }
        else if (hex[0] >= 'A' && hex[0] <= 'F')
        {
            dec += (int(hex[0]) - 55) * 16;
        }
        else if (hex[0] >= 'a' && hex[0] <= 'f')
        {
            dec += (int(hex[0]) - 'a' + 10) * 16;
        }

        return dec;
    }

    string dec_to_hex(Color dec)
    {
        //returns the hexadeximal value of a decimal number
        string hex = "#000000";
        char chr[] = "0123456789ABCDEF";

        hex[1] = chr[dec.red()/16]; // most significant
        hex[2] = chr[dec.red()%16]; // less significant

        hex[3] = chr[dec.green()/16];
        hex[4] = chr[dec.green()%16];

        hex[5] = chr[dec.blue()/16];
        hex[6] = chr[dec.blue()%16];

        return hex;
    }


    Image* loadFromXPM2(const std::string& file)
    {

        //Read image stored in the XPM2 file format.

        ifstream f(file); // Read file.
        string line;
        getline(f,line); //Trash.
        getline(f,line); // Read information.
        istringstream info(line); //Reads number by number.
        int w, h, n;
        info >> w >> h >> n; //Receives the inputs from the second line, and give them variables
        Image* image = new Image(w,h); //Create a new image w*h
        map<char,Color> dic; //Map to relate a char to a Color

        for(int i = 0; i < n; i++) //Runs n (number of symbols/colors) times
        {
            getline(f,line); //Read the line(with char and corresponding colors)
            istringstream iss(line);
            string symbol, the_useless_c, colour;
            iss >> symbol >> the_useless_c >> colour; //Receives the inputs from the line
            int red, green, blue;
            red = hex_to_dec(colour.substr(1,2)); //(Int with position where it starts, Int with size) note:starts on 1 because 0 is for the #
            green = hex_to_dec(colour.substr(3,2));
            blue = hex_to_dec(colour.substr(5,2));
            Color c(red,green,blue); 
            dic[symbol[0]] = c; //Associates in de dict the character to the color(r,g,b)
        }

        for(int y = 0; y < h; y++) //Runs through all the lines
        {
            getline(f,line);
            for(int x = 0; x < w; x++) //Runs through all the chars in the line
            {
                image->at(x,y) = dic[line[x]]; //Paints the pixel in the image with the color corresponding to the char((line[x])

            }
        }


        return image;
    }

    void saveToXPM2(const std::string& file, const Image* image)
    {
        
        //Save image to the XPM2 file format.
        ofstream f(file);
        f << "! XPM2\n";
        map<string,char> dic; //Create a map to relate a string to a char
        char cod[] = "*!#$&/()=-.,ZXCVBNMLKJHGFDSAQWERTYUIOPplokmijnuhbygvtfcrdxeszwaq2134567890"; //Each color will be related to one of this symbols
        int w = image -> width();
        int h = image -> height();       
        f << w << ' ' << h << ' ';
       

        for(int y = 0; y < h; y++) //Cycles to check all the colors and relate them to the symbols
        { 
            for(int x = 0; x < w; x++)
            {
                string curColor = dec_to_hex(image->at(x,y));
                if(dic.find(curColor) == dic.end()) //Check if color is in map, if not place it
                {
                    dic[curColor] = cod[dic.size()]; 
                }
            }

        }
        f << dic.size() << " 1"; //Writes the number of colors 

        for(auto c : dic) //Cycle that runs through the map and writes the different colors
        {
            f << '\n' << c.second << " c " << c.first;
        }


        for(int y = 0; y < h; y++) //Cycles to write an image with chars
        { 
            f << '\n';
            for(int x = 0; x < w; x++)
            {
                string curColor = dec_to_hex(image->at(x,y));
                f << dic[curColor];
            }
        }
    }
    
}