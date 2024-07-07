#include <iostream>
#include <fstream>
#include <sstream>

#include "lex.h"


int main(int argc, char *argv[]) {
    // recall that argc counts the number of arguments
    // argv takes the commandline arguments are args

    // We could hence take straight for compile files, i.e.
    // the arguemnt which we will call eventually:
    //      esol file.tea

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename.tea>\n";
        return 1;
    }

    // std::cout << argv[1] << "\n";
    

    std::ifstream in(argv[1], std::ios::in);
    std::stringstream c_stream;
    c_stream << in.rdbuf();
    // std::cout << c_stream.str() << "\n"; 

    Tokenizer *t = new Tokenizer(c_stream.str());
    t->tokenize();
    

    return 0;
}
