#pragma once
#include<iostream>
#include<string>
#include<fstream>


//工具类

class Util
{
public:
    static bool ReadFileContent(const std::string &filename, std::string *out)
    {
        std::ifstream in(filename);
        if (!in.is_open())
        {
            return false;
        }

        std::string line;
        while (std::getline(in, line))
        {
            *out += line + "\n";
        }


        in.close();
        return true;
    }
    static bool ReadOneLine(std::string &bigstr , std::string *out ,const std::string &sep)
    {
        auto pos = bigstr.find(sep);
        if(pos == std::string::npos)
        {
            return false;
        }
        *out = bigstr.substr(0, pos);
        bigstr.erase(0, pos + sep.size());
        
        return true;
    }
};