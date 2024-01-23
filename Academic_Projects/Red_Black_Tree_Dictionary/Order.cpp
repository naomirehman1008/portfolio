//-----------------------------------------------------------------------------
// Order.cpp
// Naomi Rehman
// narehman
// pa7
//-----------------------------------------------------------------------------
#include<iostream>
#include<fstream>
#include<string>
#include<stdexcept>
#include"Dictionary.h"
#include<math.h>

using namespace std;

int main(int argc, char* argv[]){
    string sA;
    string sB;
    Dictionary A;

    if(argc != 3){
        printf("argc: %d\n", argc);
        printf("Usage: %s <input file> <output file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    ifstream in (argv[1]);
    ofstream out (argv[2]);
    int i = 1;
    //form dictionary:
    while(in){
        in >> sA;
        if(sA != ""){
            A.setValue(sA, i);
            i++;
            sA.clear();
        }
    }

    out << A.to_string() << endl;
    out << A.pre_string() << endl;

    in.close();
    out.close();
}

