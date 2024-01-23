//-----------------------------------------------------------------------------
// WordFrequency.cpp
// Naomi Rehman
// narehman
// pa8
//-----------------------------------------------------------------------------
#include<iostream>
#include<fstream>
#include<string>
#include<stdexcept>
#include"Dictionary.h"
#include<math.h>
#include<algorithm>
#include<cctype>
using namespace std;

#define MAX_LEN 300

int main(int argc, char * argv[]){

int token_count, line_count;
size_t begin, end, len;
ifstream in;
ofstream out;
string line;
string tokenBuffer;
string token;
//string delim = " "; 
string delim = " \n\t\\\"\',<.>/?;:[{]}|`%~!@#$^&*()-_=+0123456789";
Dictionary D;

// check command line for correct number of arguments
if( argc != 3 ){
    cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
    return(EXIT_FAILURE);
}

// open files for reading and writing
in.open(argv[1]);
if( !in.is_open() ){
    cerr << "Unable to open file " << argv[1] << " for reading" << endl;
    return(EXIT_FAILURE);
}

out.open(argv[2]);
if( !out.is_open() ){
    cerr << "Unable to open file " << argv[2] << " for writing" << endl;
    return(EXIT_FAILURE);
}

// read each line of input file, then count and print tokens 
line_count = 0;
while( getline(in, line) )  {
    line_count++;
    len = line.length();
    
    // get tokens in this line
    token_count = 0;
    tokenBuffer = "";

    // get first token
    begin = min(line.find_first_not_of(delim, 0), len);
    end   = min(line.find_first_of(delim, begin), len);
    token = line.substr(begin, end-begin);
    
    while( token!="" ){  // we have a token
        // update token 
        token.erase(std::remove_if(token.begin(), token.end(), [](char c) { return c == '\n'; }), token.end());

        if (!token.empty() && token[token.length()-1] == '\n') {
            //cout << "newline gone!" << endl;
            token.erase(token.length()-1);
        }
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        //cout << token << endl;

        //tokenBuffer += "   "+token+"\n";
        token_count++;
        if(token != ""){
            if(D.contains(token)){
                D.getValue(token)++;
            }
            else if (!D.contains(token))            {
                D.setValue(token, 1);
            }
        }
        
        // get next token
        begin = min(line.find_first_not_of(delim, end+1), len);
        end   = min(line.find_first_of(delim, begin), len);
        token = line.substr(begin, end-begin);
    }
}
out << D << endl;
//out << D.pre_string() << endl;

// close files 
in.close();
out.close();

return(EXIT_SUCCESS);
}
