#include <bits/stdc++.h>
#include <chrono>

bool is_file_exist(std::string fname)
{
    FILE *fp;
    if((fp = fopen(fname.c_str(), "r")) == NULL) return false;
    fclose(fp);
    return true;
}

bool compile_file(std::string fname)
{
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
    return is_success;
}

void make_zip(int sample_num, bool use_spj)
{
    std::string fzip = "zip testcase.zip";
    for(int i=1; i<sample_num; i++)
    {
        std::string n = std::to_string(i);
        fzip += " " + n + ".in";
        if(!use_spj) fzip += " " + n + ".out";
    }
    system(fzip.c_str());
}

void make_samples(bool use_spj)
{
    std::cout << "compile success" << std::endl;
    int i = 1;
    std::cin.ignore();
    for(;;i++)
    {
        std::cout << "Input" << i << ":" << std::endl;
        std::vector<std::string> vs;
        while(true)
        {
            // std::string s;
            // std::getline(std::cin, s);
            char buf[256];
            fgets(buf, sizeof(buf), stdin);
            std::string s = buf;
            if(s == "\n") break;
            s.pop_back();
            vs.push_back(s);
        }
        if(vs.empty()) break;
        std::ofstream ofs((std::to_string(i) + ".in").c_str());
        for(std::string s : vs)
        {
            ofs << s << std::endl;
        }
        std::cout << "Output" << i << ":" << std::endl;
        system(("./a.out < " + std::to_string(i) + ".in").c_str());
        std::cout << std::endl;
        if(!use_spj) system(("./a.out < " + std::to_string(i) + ".in > " + std::to_string(i) + ".out").c_str());
    }
    make_zip(i, use_spj);
    system("rm a.out");
}

void process(std::string fname)
{
    if(!is_file_exist(fname))
    {
        std::cout << "\033[31mCannot find \"" << fname << "\". \033[m" << std::endl; 
        exit(EXIT_FAILURE);
    }

    if(compile_file(fname))
    {
        std::cout << "Do you use spj? y/n" << std::endl;
        char ans;
        std::cin >> ans;
        bool use_spj = (ans == 'y') ? true : false;
        make_samples(use_spj);
    }
}

int main(int argc, char* argv[])
{
    if(argc == 1) process("main.cpp");
    else if(argc == 2) process(argv[1]);
    else
    {
        std::string help = "Usage:\njoho-ta-tools <file name> -- to make sample";
        std::cout << help << std::endl;
    }
}