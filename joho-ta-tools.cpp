#include <iostream>
#include <string>
#include <cstring>

#include "tools/testgen.hpp"
#include "tools/merge_result.hpp"

int main(int argc, char** argv)
{
    const std::string help = "Usage:\n"
                             "joho-ta-tools gen -- to generate testcase.\n"
                             "joho-ta-tools merge -- to merge result.";
    if(argc < 2)
    {
        puts(help.c_str());
        return 0;
    }
    std::string ope = argv[1];
    if(ope != "gen" && ope != "merge")
    {
        puts(help.c_str());
        return 0;
    }
    char* newargv[argc-1];
    memcpy(newargv, argv+1, (argc-1) * sizeof(char*));

    if(ope == "gen") testgen::generate(argc-1, newargv);
    else if(ope == "merge") merge_result::merge(argc-1, newargv);

    return 0;
}
