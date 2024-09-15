#include <iostream>
#include <iomanip>
#include "show_file.h"
using namespace std;
#include <istream>

bool average(const string& input_fname, const string& output_fname)
{
    ifstream pepe(input_fname);
    ofstream to_return(output_fname);
    
    int linhas = 0;
    string mid;
    while(getline(pepe,mid))
    {
        linhas++;
        double soma = 0;
        double contagem = 0;
        istringstream iss(mid);
        double c;
        while(iss>>c)
        {
            soma +=  c;
            contagem++;
        }

        soma /= contagem;

        to_return << fixed << setprecision(3)<< soma << "\n";
    }
    to_return << "lines=" << linhas;

    return true;
}