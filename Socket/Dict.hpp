#pragma once
#include<iostream>
#include<string>
#include<unordered_map>
#include"UdpServer.hpp"


const std::string default_dict_path = "./dictionary.txt";

class Dict
{
public:
    Dict(const std::string &path = default_dict_path) : _dict_path(path) {}
    ~Dict() 
    {

    }
    std::string Translate(const std::string &word)
    {
        auto it = _dict.find(word);
        if (it != _dict.end())
        {
            return it->second;
        }
        else
        {
            return "not found";
        }
    }
    bool LoadDict()
    {
        
    }

private:
    std::string _dict_path;//路径+文件名
    std::unordered_map<std::string,std::string> _dict;

};