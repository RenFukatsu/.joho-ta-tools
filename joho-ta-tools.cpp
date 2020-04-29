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

void make_spj(std::string fname)
{
    std::ifstream ifs(fname.c_str());
    std::vector<std::string> lines;
    while(!ifs.eof())
    {
        std::string s;
        std::getline(ifs, s);
        lines.push_back(s);
    }

    system("cp $HOME/.joho-ta-tools/template-spj.cpp spj.cpp");
    std::ofstream ofs("spj.cpp", std::ios::app);
    int num = 1;
    for(auto &s : lines)
    {
        if(s.find("#include") != std::string::npos) continue;
        else if(s.find("int main") != std::string::npos)
        {
            ofs << "int spj(FILE *input, FILE *user_output)" << std::endl;
        }
        else if(s.find("//") == std::string::npos && s.find("scanf") != std::string::npos)
        {
            s.insert(s.find("scanf"), "f");
            s.insert(s.find("(")+1, "input, ");
            ofs << s << std::endl;
        }
        else if(s.find("//") == std::string::npos && s.find("printf") != std::string::npos)
        {
            int st = s.find("\"");
            int gl = s.find("\"", st+1);
            int space = s.find_first_not_of(" ");
            std::string str_space(space, ' ');
            std::string subs = s.substr(st, gl-st);
            // if(st < s.find("lf") && s.find("lf") < gl)
            if(subs.find("lf") != std::string::npos)
            {
                ofs << str_space << "double user_val" << std::to_string(num) << ";" << std::endl;
                ofs << str_space << "fscanf(user_output, \"%lf\", &user_val" << std::to_string(num) << ");" << std::endl;
            }
            // else if(st < s.find("f") && s.find("f") < gl)
            else if(subs.find("f") != std::string::npos)
            {
                ofs << str_space << "float user_val" << std::to_string(num) << ";" << std::endl;
                ofs << str_space << "fscanf(user_output, \"%f\", &user_val" << std::to_string(num) << ");" << std::endl;
            }
            // else if(st < s.find("c") && s.find("c") < gl)
            else if(subs.find("c") != std::string::npos)
            {
                ofs << str_space << "char user_val" << std::to_string(num) << ";" << std::endl;
                ofs << str_space << "fscanf(user_output, \"%c\", &user_val" << std::to_string(num) << ");" << std::endl;
            }
            // else if(st < s.find("s") && s.find("s") < gl)
            else if(subs.find("s") != std::string::npos)
            {
                ofs << str_space << "char *user_val" << std::to_string(num) << "[2048]" << ";" << std::endl;
                ofs << str_space << "fscanf(user_output, \"%s\", user_val" << std::to_string(num) << ");" << std::endl;
            }
            else
            {
                ofs << str_space << "int user_val" << std::to_string(num) << ";" << std::endl;
                ofs << str_space << "fscanf(user_output, \"%d\", &user_val" << std::to_string(num) << ");" << std::endl;
            }
            std::string val(s, gl+2, s.size()-gl-4);
            ofs << str_space << "if(!equiv(" << val << ", user_val" << std::to_string(num) << ")) return WA;" << std::endl;
            num++;
        }
        else if(s.find("//") == std::string::npos && s.find("return 0") != std::string::npos)
        {
            ofs << "    return AC;" << std::endl;
        }
        else ofs << s << std::endl;
    }
}

void process(std::string fname, bool use_spj=false)
{
    if(!is_file_exist(fname))
    {
        std::cout << "\033[31mCannot find \"" << fname << "\". \033[m" << std::endl; 
        exit(EXIT_FAILURE);
    }

    if(compile_file(fname))
    {
        make_samples(use_spj);
        if(use_spj) make_spj(fname);
    }
}

int main(int argc, char* argv[])
{
    if(argc == 1) process("main.cpp");
    else if(argc == 2 && std::strcmp(argv[1], "spj") == 0) process("main.cpp", true);
    else if(argc == 2) process(argv[1]);
    else if(argc == 3 && std::strcmp(argv[1], "spj") == 0) process(argv[1], true);
    else
    {
        std::string help = "Usage"
                           "joho-ta-tools  -- to make samples"
                           "joho-ta-tools spj -- to make samples and spj-code";
        std::cout << help << std::endl;
    }
    return 0;
}