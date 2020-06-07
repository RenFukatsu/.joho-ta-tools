#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cassert>
#include <cstdlib>
#include <dirent.h>

#include "../argparse/argparse.hpp"

namespace merge_result
{
    class Student
    {
    public:
        std::string faculty;
        std::string year;
        std::string clas;
        std::string number;
        std::string name;
        std::string yomi;
        std::string student_num;
        std::vector<int> scores;

        Student(std::string faculty, std::string year, std::string clas, std::string number, std::string name, std::string yomi, std::string student_num) : faculty(faculty), year(year), clas(clas), number(number), name(name), yomi(yomi), student_num(student_num) {}

        Student(std::vector<std::string> &vstr)
        {
            assert(vstr.size() >= 7);
            faculty = vstr.at(0);
            year = vstr.at(1);
            clas = vstr.at(2);
            number = vstr.at(3);
            name = vstr.at(4);
            yomi = vstr.at(5);
            student_num = vstr.at(6);
        }
    };

    std::vector<std::string> split(std::string &inp, char delimiter)
    {
        std::istringstream stream(inp);
        std::string str;
        std::vector<std::string> result;
        while(std::getline(stream, str, delimiter))
        {
            std::string add = "";
            for(auto &c : str) if(isdigit(c) || isalpha(c)) add += c;
            if(!add.empty()) result.push_back(add);
            else result.push_back(str);
        }
        return result;
    }
    
    std::vector<std::vector<std::string>> read_csv(std::string fname)
    {
        std::ifstream ifs(fname);

        std::string line;
        std::vector<std::vector<std::string>> res;
        while(std::getline(ifs, line))
        {
            std::vector<std::string> st = split(line, ',');
            res.push_back(st);
        }
        return res;
    }

    std::vector<Student> read_default_csv(std::string fname)
    {
        std::vector<std::vector<std::string>> lines = read_csv(fname);
        std::vector<Student> student_infos;
        for(auto &line : lines)
        {
            student_infos.push_back(Student(line));
        }
        // for(auto &st : student_infos)
        // {
        //     std::cout << st.faculty << " " << st.year << " " << st.clas << " " << st.number << " " << st.name << " " << st.yomi << " " << st.student_num << std::endl;
        // }
        
        return student_infos;
    }

    std::vector<std::string> get_files_in_dir(std::string dirname)
    {
        DIR *dp;
        dirent* entry;

        std::vector<std::string> res;
        dp = opendir(dirname.c_str());
        if(dp == NULL)
        {
            std::cerr << "Don't find dir\n";
            exit(1);
        }
        do{
            entry = readdir(dp);
            if(entry != NULL) res.push_back(dirname + "/" + entry->d_name);
        } while(entry != NULL);
        return res;
    }

    bool is_csv(std::string fname)
    {
        auto ext_idx = fname.find_last_of('.');
        std::string extname = fname.substr(ext_idx, fname.size() - ext_idx);
        return extname == ".csv";
    }

    void append_info(std::string fname, std::vector<Student> &student_infos)
    {
        static int num_contest = 0;
        std::vector<std::vector<std::string>> lines = read_csv(fname);
        for(auto &line : lines)
        {
            if(!std::any_of(line.at(1).cbegin(), line.at(1).cend(), isdigit)) continue;
            for(auto &st : student_infos)
            {
                if(st.student_num == line.at(1))
                {
                    st.scores.push_back(std::stoi(line.at(3)));
                    break;
                }
            }
        }
        num_contest++;
        for(auto &st : student_infos)
        {
            if(st.scores.size() < num_contest) st.scores.push_back(0);
        }
    }

    void write_csv(std::string fname, std::vector<Student> &student_infos)
    {
        std::ofstream ofs(fname);
        for(auto &st : student_infos)
        {
            ofs << st.faculty << ',' << st.year << ',' << st.clas << ',' << st.number << ',' << st.name << ',' << st.yomi << ',' << st.student_num << ',' << std::accumulate(st.scores.begin(), st.scores.end(), 0);
            for(int score : st.scores)
            {
                ofs << ',' << score;
            }
            ofs << '\n';
        }
    }

    void process(std::string fname, std::string dirname, std::string oname)
    {
        std::vector<Student> student_infos = read_default_csv(fname);
        std::vector<std::string> files = get_files_in_dir(dirname);
        for(auto &file : files)
        {
            if(!is_csv(file)) continue;
            append_info(file, student_infos);
        }
        write_csv(oname, student_infos);
    }

    int merge(int argc, char** argv)
    {
        argparse::ArgumentParser parser(".joho-ta-tools merge", "merge result of csv file.");
        parser.addArgument({"--default-csv", "-d"}, "Value is required. path to default file");
        parser.addArgument({"--contests-dir", "-c"}, "Value is required. path to contests dir");
        parser.addArgument({"--output-file", "-o"}, "output file name. default name is 'merge.csv'.");

        auto args = parser.parseArgs(argc, argv);

        if(!args.has("default-csv") || !args.has("contests-dir"))
        {
            parser.printHelp();
            return 0;
        }
        std::string fname = args.get<std::string>("default-csv");
        std::string dirname = args.get<std::string>("contests-dir");
        std::string oname = args.safeGet<std::string>("output-file", "merge.csv");
        process(fname, dirname, oname);
        return 0;
    }
} // namespace merge_result