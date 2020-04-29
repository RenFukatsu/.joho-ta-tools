#include <bits/stdc++.h>
#include <chrono>

bool is_file_exist(std::string fname)
{
    FILE *fp;
    if((fp = fopen(fname.c_str(), "r")) == NULL) return false;
    fclose(fp);
    return true;
}

void make_sample(std::string fname)
{
    if(!is_file_exist(fname))
    {
        std::cout << "\033[31mCannot find \"" << fname << "\". \033[m" << std::endl; 
        exit(EXIT_FAILURE);
    }

    bool is_success = false;
    size_t idx_ext = fname.find_last_of('.');
    std::string ext = fname.substr(idx_ext, fname.size() - idx_ext);
    if(ext == ".c") is_success = !system(("/usr/bin/gcc -DONLINE_JUDGE -O2 -w -fmax-errors=3 -std=c11 " + fname + " -lm -o a.out").c_str());
    else if(ext == ".cpp") is_success = !system(("/usr/bin/g++ -DONLINE_JUDGE -O2 -w -fmax-errors=3 -std=c++14 " + fname + " -lm -o a.out").c_str());
    else
    {
        std::cout << "\033[31mThis file does not have a .c or .cpp extension. \033[m" << std::endl;
        exit(EXIT_FAILURE);
    }
    if(is_success)
    {
        std::cout << "compile success" << std::endl;
        int i = 1;
        for(;;i++)
        {
            std::cout << "Input" << i << ":" << std::endl;
            std::vector<std::string> vs;
            while(true)
            {
                std::string s;
                std::cin >> s;
                if(std::strcmp(s.c_str(), "q") == 0) break;
                vs.push_back(s);
            }
            if(vs.empty()) break;
            std::ofstream ofs((std::to_string(i) + ".in").c_str());
            for(std::string s : vs)
            {
                ofs << s << std::endl;
            }
            ofs << std::endl;
            std::cout << "Output" << i << ":" << std::endl;
            system(("./a.out < " + std::to_string(i) + ".in").c_str());
            system(("./a.out < " + std::to_string(i) + ".in > " + std::to_string(i) + ".out").c_str());
            if(!is_file_exist(std::to_string(i) + ".out"))
            {
                std::cout << "None" << std::endl; 
                exit(EXIT_FAILURE);
            }
        }
        system("rm a.out");
        std::string fzip = "zip testcase.zip";
        for(int j=1; j<i; j++)
        {
            std::string n = std::to_string(j);
            fzip += " " + n + ".in " + n + ".out";
        }
        system(fzip.c_str());
    }
}

int main(int argc, char* argv[])
{
    if(argc == 1) make_sample("main.cpp");
    else if(argc == 2) make_sample(argv[1]);
    else
    {
        std::string help = "Usage:\njoho-ta-tools <file name> -- to make sample";
        std::cout << help << std::endl;
    }
}