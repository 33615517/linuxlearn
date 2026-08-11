#pragma once
#include<iostream>
#include<string>
#include<unordered_map>
//#include"UdpServer.hpp"
#include"Log.hpp"
#include"InetAddr.hpp"

const std::string default_dict_path = "./dictionary.txt";

class Dict
{
public:
    Dict(const std::string &path = default_dict_path) : _dict_path(path) {}
    ~Dict() 
    {

    }
    std::string Translate(const std::string &word, InetAddr& client)
    {
        auto it = _dict.find(word);
        if (it != _dict.end())
        {
            LOG(Loglevel::DEBUG) << "进入到翻译模块，[" << client.Ip() << ":" << client.Port() << "] 请求翻译单词：" << word << " -> " << it->second;
            return it->second;
        }
        else
        {
            return "not found";
        }
    }
    bool LoadDict()
    {
        std::ifstream infile(_dict_path);
        if (!infile.is_open())
        {
            LOG(Loglevel::ERROR) << "Failed to open dictionary file: " << _dict_path;
            return false;
        }
        std::string line;
        while (std::getline(infile, line))
        {
            size_t pos = line.find(' ');
            if (pos != std::string::npos)
            {
                std::string key = line.substr(0, pos-1);
                std::string value = line.substr(pos + 1);
                _dict[key] = value; 
            }
        }
        infile.close();
        return true;
    }

private:
    std::string _dict_path;//路径+文件名
    std::unordered_map<std::string,std::string> _dict;

};