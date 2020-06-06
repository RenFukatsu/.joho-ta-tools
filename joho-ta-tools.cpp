#include <iostream>
#include <string>
#include <cstring>

#include "tools/testgen.hpp"

int main(int argc, char** argv)
{
    const std::string help = "Usage:\n"
                             "joho-ta-tools gen -- to generate testcase.";
    if(argc < 2)
    {
        puts(help.c_str());
        return 0;
    }
    std::string ope = argv[1];
    char* newargv[argc-1];
    memcpy(newargv, argv+1, (argc-1) * sizeof(char*));

    if(ope == "gen") testgen::generate(argc-1, newargv);

    return 0;
}