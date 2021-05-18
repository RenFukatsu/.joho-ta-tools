#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

#include "../argparse/argparse.hpp"

namespace testgen {

bool is_file_exist(std::string fname) {
    FILE *fp;
    if ((fp = fopen(fname.c_str(), "r")) == NULL) return false;
    fclose(fp);
    return true;
}

bool compile_file(std::string fname) {
    bool is_success = false;
    size_t idx_ext = fname.find_last_of('.');
    std::string ext = fname.substr(idx_ext, fname.size() - idx_ext);
    std::string name = fname.substr(0, idx_ext);

    if (ext == ".c") {
        is_success = !system(("/usr/bin/gcc -DONLINE_JUDGE -O2 -w -fmax-errors=3 -std=c11 src/" +
                              fname + " -lm -o a.out")
                                 .c_str());
    } else if (ext == ".cpp") {
        is_success = !system(("/usr/bin/g++ -DONLINE_JUDGE -O2 -w -fmax-errors=3 -std=c++14 src/" +
                              fname + " -lm -o a.out")
                                 .c_str());
    } else {
        std::cout << "\033[31mThis file does not have a .c or .cpp extension. \033[m" << std::endl;
        exit(EXIT_FAILURE);
    }

    system(("mkdir -p bin && cp a.out bin/" + name + ".out").c_str());
    return is_success;
}

void make_zip(int sample_num, bool use_spj) {
    std::string fzip = "zip testcases.zip";
    for (int i = 1; i <= sample_num; i++) {
        std::string n = std::to_string(i);
        fzip += " " + n + ".in";
        if (!use_spj) fzip += " " + n + ".out";
    }
    system(fzip.c_str());
}

void input_texts(std::vector<std::string> &output) {
    output.clear();
    while (true) {
        char buf[256];
        fgets(buf, sizeof(buf), stdin);
        std::string s = buf;
        if (s == "\n") break;
        s.pop_back();
        output.push_back(s);
    }
}

void make_samples_with_generator(std::string generator_path) {
    int i = 1;
    if (system(("/usr/bin/g++ -DONLINE_JUDGE -O2 -w -fmax-errors=3 -std=c++14 " + generator_path +
                " -lm -o gen.out")
                   .c_str())) {
        std::cout << "\033[31myour generator was wrong.\033[m" << std::endl;
        return;
    }
    for (; i <= 10; i++) {
        std::cout << "Input" << i << ":" << std::endl;
        system(("./gen.out > " + std::to_string(i) + ".in").c_str());
        system(("cat " + std::to_string(i) + ".in").c_str());
        std::cout << "Output" << i << ":" << std::endl;
        system(("./a.out < " + std::to_string(i) + ".in").c_str());
        std::cout << std::endl;
        system(("./a.out < " + std::to_string(i) + ".in > " + std::to_string(i) + ".out").c_str());
    }
    make_zip(i - 1, false);
    system("rm -f gen.out a.out");
}

void make_samples(bool use_spj) {
    int i = 1;
    for (;; i++) {
        std::cout << "Input" << i << ":" << std::endl;
        std::vector<std::string> vs;
        input_texts(vs);
        if (vs.empty()) break;
        std::ofstream ofs((std::to_string(i) + ".in").c_str());
        for (std::string s : vs) {
            ofs << s << std::endl;
        }
        std::cout << "Output" << i << ":" << std::endl;
        system(("./a.out < " + std::to_string(i) + ".in").c_str());
        std::cout << std::endl;
        if (!use_spj)
            system(
                ("./a.out < " + std::to_string(i) + ".in > " + std::to_string(i) + ".out").c_str());
    }
    make_zip(i, use_spj);
    system("rm -f a.out");
}

void convert(const std::vector<std::string> &format, std::vector<std::string> &output) {
    output.clear();
    for (auto &line : format) {
        std::string s = "";
        std::istringstream iss(line);
        std::string type;
        while (iss >> type) {
            std::mt19937 mt{std::random_device {}()};
            std::string smin;
            iss >> smin;
            std::string smax;
            iss >> smax;
            if (type == "i") {
                int min = std::stoi(smin);
                int max = std::stoi(smax);
                std::uniform_int_distribution<int> dist(min, max);
                s += std::to_string(dist(mt));
            } else if (type == "ll") {
                int64_t min = std::stoll(smin);
                int64_t max = std::stoll(smax);
                std::uniform_int_distribution<int64_t> dist(min, max);
                s += std::to_string(dist(mt));
            } else if (type == "f") {
                float min = std::stof(smin);
                float max = std::stof(smax);
                std::uniform_real_distribution<float> dist(min, max);
                s += std::to_string(dist(mt));
            } else if (type == "d") {
                double min = std::stod(smin);
                double max = std::stod(smax);
                std::uniform_real_distribution<double> dist(min, max);
                s += std::to_string(dist(mt));
            } else if (type == "c") {
                char min, max;
                if (smin.length() == 1 && smax.length() == 1) {
                    min = smin.at(0);
                    max = smax.at(0);
                } else {
                    std::cout << "\033[m031Even though char was asked for, the number of "
                                 "characters is not one.\033[m"
                              << std::endl;
                    exit(EXIT_FAILURE);
                }
                std::uniform_int_distribution<int> dist(min, max);
                s.push_back(static_cast<char>(dist(mt)));
            } else if (type == "s") {
                char min, max;
                if (smin.length() == 1 && smax.length() == 1) {
                    min = smin.at(0);
                    max = smax.at(0);
                } else {
                    std::cout << "\033[m031Even though char was asked for, the number of "
                                 "characters is not one.\033[m"
                              << std::endl;
                    exit(EXIT_FAILURE);
                }
                std::string ssmin, ssmax;
                iss >> ssmin;
                iss >> ssmax;
                int64_t lmin = std::stoll(ssmin);
                int64_t lmax = std::stoll(ssmax);
                std::uniform_int_distribution<int> dist(min, max);
                std::uniform_int_distribution<int64_t> ldist(lmin, lmax);
                int64_t cnt = ldist(mt);
                for (int64_t i = 0; i < cnt; i++) {
                    s.push_back(static_cast<char>(dist(mt)));
                }
            }
            s.push_back(' ');
        }
        output.push_back(s);
    }
}

void make_random_samples(int sample_num, bool use_spj) {
    std::cout << "Input format:" << std::endl;
    std::vector<std::string> format;
    input_texts(format);
    if (format.empty()) {
        std::cout << "\033[31mThe format was not entered.\033[m" << std::endl;
        exit(EXIT_FAILURE);
    }
    int i = 1;
    for (;; i++) {
        std::cout << "Input" << i << ":" << std::endl;
        std::vector<std::string> vs;
        convert(format, vs);
        for (auto &s : vs) std::cout << s << std::endl;
        std::ofstream ofs((std::to_string(i) + ".in").c_str());
        for (std::string s : vs) {
            ofs << s << std::endl;
        }
        std::cout << "Output" << i << ":" << std::endl;
        system(("./a.out < " + std::to_string(i) + ".in").c_str());
        std::cout << std::endl;
        if (!use_spj)
            system(
                ("./a.out < " + std::to_string(i) + ".in > " + std::to_string(i) + ".out").c_str());
        if (i == sample_num) break;
    }
    make_zip(i, use_spj);
    system("rm a.out");
}

void make_spj(std::string fname) {
    std::ifstream ifs(fname.c_str());
    std::vector<std::string> lines;
    while (!ifs.eof()) {
        std::string s;
        std::getline(ifs, s);
        lines.push_back(s);
    }

    system("cp $HOME/.joho-ta-tools/templates/template-spj.cpp spj.cpp");
    std::ofstream ofs("spj.cpp", std::ios::app);
    int num = 1;
    for (auto &s : lines) {
        if (s.find("#include") != std::string::npos) {
            continue;
        } else if (s.find("int main") != std::string::npos) {
            ofs << "int spj(FILE *input, FILE *user_output)" << std::endl;
        } else if (s.find("//") == std::string::npos && s.find("scanf") != std::string::npos) {
            s.insert(s.find("scanf"), "f");
            s.insert(s.find("(") + 1, "input, ");
            ofs << s << std::endl;
        } else if (s.find("//") == std::string::npos && s.find("printf") != std::string::npos) {
            int st = s.find("\"");
            int gl = s.find("\"", st + 1);
            int space = s.find_first_not_of(" ");
            std::string str_space(space, ' ');
            std::string subs = s.substr(st, gl - st);
            // if(st < s.find("lf") && s.find("lf") < gl)
            if (subs.find("lf") != std::string::npos) {
                ofs << str_space << "double user_val" << std::to_string(num) << ";" << std::endl;
                ofs << str_space << "fscanf(user_output, \"%lf\", &user_val" << std::to_string(num)
                    << ");" << std::endl;
            } else if (subs.find("f") !=
                       std::string::npos) {  // st < s.find("f") && s.find("f") < gl
                ofs << str_space << "float user_val" << std::to_string(num) << ";" << std::endl;
                ofs << str_space << "fscanf(user_output, \"%f\", &user_val" << std::to_string(num)
                    << ");" << std::endl;
            } else if (subs.find("c") !=
                       std::string::npos) {  // st < s.find("c") && s.find("c") < gl
                ofs << str_space << "char user_val" << std::to_string(num) << ";" << std::endl;
                ofs << str_space << "fscanf(user_output, \"%c\", &user_val" << std::to_string(num)
                    << ");" << std::endl;
            } else if (subs.find("s") !=
                       std::string::npos) {  // st < s.find("s") && s.find("s") < gl
                ofs << str_space << "char *user_val" << std::to_string(num) << "[2048]"
                    << ";" << std::endl;
                ofs << str_space << "fscanf(user_output, \"%s\", user_val" << std::to_string(num)
                    << ");" << std::endl;
            } else {
                ofs << str_space << "int user_val" << std::to_string(num) << ";" << std::endl;
                ofs << str_space << "fscanf(user_output, \"%d\", &user_val" << std::to_string(num)
                    << ");" << std::endl;
            }
            std::string val(s, gl + 2, s.size() - gl - 4);
            ofs << str_space << "if(!equiv(" << val << ", user_val" << std::to_string(num)
                << ")) return WA;" << std::endl;
            num++;
        } else if (s.find("//") == std::string::npos && s.find("return 0") != std::string::npos) {
            ofs << "    return AC;" << std::endl;
        } else {
            ofs << s << std::endl;
        }
    }
}

void folderize(std::string fname) {
    size_t idx_ext = fname.find_last_of('.');
    std::string name = fname.substr(0, idx_ext);
    system(("mkdir -p testcases/" + name + " && mv *.in testcases/" + name +
            "/ && mv *.out testcases/" + name)
               .c_str());
    system(("mkdir -p zips && mv testcases.zip zips/" + name + ".zip").c_str());
}

void process(std::string fname, bool only_compile, bool use_spj, int random_sample_num,
             std::string generator_path) {
    if (!is_file_exist("src/" + fname)) {
        std::cout << "\033[31mCannot find src/\"" << fname << "\". \033[m" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (compile_file(fname)) {
        std::cout << "compile success" << std::endl;
        if (only_compile) {
            system("rm -f a.out");
            return;
        }
        if (generator_path != "" && is_file_exist(generator_path))
            make_samples_with_generator(generator_path);
        else if (random_sample_num == -1)
            make_samples(use_spj);
        else
            make_random_samples(random_sample_num, use_spj);
        folderize(fname);
        if (use_spj) make_spj(fname);
    }
}

int generate(int argc, char **argv) {
    argparse::ArgumentParser parser(".joho-ta-tools gen", "generate testcase.");

    parser.addArgument({"--filename", "-f"}, "hoge.cpp");
    parser.addArgument({"--compile", "-c"}, "compile only", argparse::ArgumentType::StoreTrue);
    parser.addArgument({"--spj", "-s"}, "use spj", argparse::ArgumentType::StoreTrue);
    parser.addArgument({"--random", "-r"}, "random sample num");
    parser.addArgument({"--generator", "-g"},
                       "if you use your generator, input generator path with '-g'");

    auto args = parser.parseArgs(argc, argv);

    std::string fname = args.get<std::string>("filename");
    bool use_spj = args.has("spj");
    bool only_compile = args.has("compile");
    int random_sample_num = args.safeGet<int>("random", -1);
    std::string generator_path = args.safeGet<std::string>("generator", "");

    process(fname, only_compile, use_spj, random_sample_num, generator_path);

    return 0;
}
}  // namespace testgen
