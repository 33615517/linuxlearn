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
        // out->clear();
        // std::ifstream in(filename);
        // if (!in.is_open())
        // {
        //     return false;
        // }

        // std::string line;
        // while (std::getline(in, line))
        // {
        //     *out += line + "\n";
        // }


        // in.close();
        //version 2
        //以二进制方式打开文件，读取文件内容到字符串中
        int size = FileSize(filename);
        if (size == -1)
        {
            return false;
        }
        *out = std::string(size, '\0');
        std::ifstream in(filename, std::ios::binary);
        if (!in.is_open())
        {
            return false;
        }
        out->reserve(size);
        in.read((char*)(out->c_str()), size);
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
    static int FileSize(const std::string &filename)
    {
        std::ifstream in(filename, std::ios::binary );
        if (!in.is_open())
        {
            return -1;
        } 
        in.seekg(0, std::ios::end);
        int size = in.tellg();
        in.seekg(0, std::ios::beg);
        in.close();
        return size;
    }
};