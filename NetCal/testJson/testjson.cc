#include<iostream>
#include<string>
#include<jsoncpp/json/json.h>
#include<sstream>

int main()
{
    
    // std::string json_string = "{\"name\":\"张三\", \"age\":30,\"city\":\"北京\"}";
    // //反序列化,起手式Json::Value root;
    // Json::Value root;
    // Json::Reader reader;

    // bool ok = reader.parse(json_string, root);

    // if(!ok)
    // {
    //     std::cout << "parse error" << std::endl;
    //     return -1;
    // }
    
    // std::string name = root["name"].asString();
    // int age = root["age"].asInt();
    // std::string city = root["city"].asString();

    // std::cout << "name: " << name << std::endl;
    // std::cout << "age: " << age << std::endl;
    // std::cout << "city: " << city << std::endl;













    // Json::Value root;
    // root["name"] = "Xingguichutu";
    // root["sex"] = "男";
    // root["age"] = 18;
    // Json::StreamWriterBuilder sbuilder;
    // std::unique_ptr<Json::StreamWriter> writer(sbuilder.newStreamWriter());
    // std::ostringstream os;
    // writer->write(root, &os);
    // std::string s = os.str();
    // std::cout << s << std::endl;













    // Json::Value root;
    // root["name"] = "Xingguichutu"; 
    // root["sex"] = "男";
    // root["age"] = 18;

    // // Json::FastWriter writer;
    // Json::StyledWriter writer;
    // std::string out = writer.write(root);
    // std::cout << out << std::endl;


    // Json::Value sub;
    // sub["tel"] = "123456789";
    // sub["addr"] = "北京";
    // root["info"] = sub;

    // std::string out = root.toStyledString();
    // std::cout << out << std::endl;




    return 0;
}