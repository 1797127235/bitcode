#pragma once
#include <string>
#include<memory>
#include<functional>
#include <iostream>
#include <json/json.h>


//"len"\r\n"{json}"\r\n ---完整报文  len表示有效载荷的长度
static const std::string seq="\r\n";
std::string Encode(std::string &jsonstr)
{
    std::string strlen=std::to_string(jsonstr.size());
    return strlen+seq+jsonstr+seq;
}

std::string Decode(std::string &str)
{
    // 查找第一个分隔符
    size_t pos = str.find(seq);
    if (pos == std::string::npos) {
        return std::string(); // 没有找到分隔符，返回空字符串
    }
    // 提取长度字符串
    std::string strlen = str.substr(0, pos); 
    // 验证长度字符串是否为有效数字
    if (strlen.empty()) {
        return std::string();
    }
    size_t payload_len;
    try {
        payload_len = std::stoul(strlen);
    } catch (const std::exception&) {
        return std::string(); // 长度转换失败
    }
    
    // 计算完整报文的总长度：长度字符串 + 第一个分隔符 + 有效载荷 + 第二个分隔符
    size_t total = strlen.size() + seq.size() + payload_len + seq.size();
    
    // 检查当前字符串长度是否足够
    if (str.size() < total) {
        return std::string(); // 数据不完整
    }
    
    // 提取有效载荷
    std::string decode_str = str.substr(pos + seq.size(), payload_len);
    
    // 从原字符串中移除已处理的数据
    str.erase(0, total);
    
    return decode_str;
}

class Request
{
public:
    Request() : _x(0), _y(0), _op('\0') {};
    // 序列化
    bool Serialize(std::string *out)
    {
        // x op y
        Json::Value root;
        root["x"] = _x;
        root["op"] = _op;
        root["y"] = _y;
        // Json::FastWriter writer;
        // *out=writer.write(root);

        // Json::StreamWriterBuilder builder;
        // builder["indentation"] = "";                // 不缩进
        // builder["enableYAMLCompatibility"] = false; // 避免特殊格式
        // builder["dropNullPlaceholders"] = true;     // 可选：省略 null 值
        // builder["commentStyle"] = "None";           // 去除注释
        // *out = Json::writeString(builder, root);
        // return true;

        Json::FastWriter writer;
        // Json::StyledWriter writer;
        std::string s = writer.write(root);
        *out = s;
        return true;
    }
    // 反序列化
    bool Deserialize(const std::string &in)
    {
        // Json::CharReaderBuilder builder;
        // builder["collectComments"] = false; // 不收集注释

        // Json::Value root;
        // std::string errs;

        // std::istringstream s(in);
        // std::string errs_out;
        // bool ok = Json::parseFromStream(builder, s, &root, &errs_out);
        // if (!ok)
        // {
        //     std::cerr << "Failed to parse JSON: " << errs_out << std::endl;
        //     return false;
        // }

        // // 判断字段是否存在和类型是否正确
        // if (!root.isMember("x") || !root["x"].isInt())
        //     return false;
        // if (!root.isMember("op") || !root["op"].isString())
        //     return false;
        // if (!root.isMember("y") || !root["y"].isInt())
        //     return false;

        // // 赋值
        // _x = root["x"].asInt();
        // std::string op_str = root["op"].asString();
        // _op = op_str.empty() ? '\0' : op_str[0];  // 取字符串的第一个字符
        // _y = root["y"].asInt();

        // return true;


        Json::Value root;
        Json::Reader reader;
        bool res = reader.parse(in, root);
        if (!res) {
            std::cerr << "Failed to parse JSON" << std::endl;
            return false;
        }

        // 检查字段是否存在
        if (!root.isMember("x") || !root.isMember("y") || !root.isMember("op")) {
            std::cerr << "Missing required fields in JSON" << std::endl;
            return false;
        }

        _x = root["x"].asInt();
        _y = root["y"].asInt();
        _op = root["op"].asInt();

        std::cerr << "[Protocol DEBUG] Parsed: x=" << _x << ", y=" << _y << ", op=" << (int)_op << " (char: '" << (char)_op << "')" << std::endl;

        return true;
    }
    void SetRequest(int x,char op,int y)
    {
        _x=x;
        _op=op;
        _y=y;
    }
    int getx() const
    {
        return _x;
    }
    int gety() const
    {
        return _y;
    }
    char getop() const
    {
        return _op;
    }

private:
    int _x;
    int _y;
    char _op;
};

class Response
{
public:
    Response() : _result(0), _code(0), _desc("") {};
    Response(int result,int code,std::string desc):_result(result),_code(code),_desc(desc){};
    // 序列化
    bool Serialize(std::string *out)
    {
        // Json::Value root;
        // root["result"] = _result;
        // root["code"] = _code;
        // root["desc"] = _desc;
        // Json::StreamWriterBuilder builder;
        // builder["indentation"] = "";                // 不缩进
        // builder["enableYAMLCompatibility"] = false; // 避免特殊格式
        // builder["dropNullPlaceholders"] = true;     // 可选：省略 null 值
        // builder["commentStyle"] = "None";           // 去除注释
        // *out=Json::writeString(builder,root);
        // return true;

        Json::Value root;
        root["result"] = _result;
        root["code"] = _code;
        root["desc"] = _desc;
        Json::FastWriter writer;
        std::string s = writer.write(root);
        *out = s;
        return true;
    }
    // 反序列化
    bool Deserialize(const std::string &in)
    {
        // Json::CharReaderBuilder builder;
        // builder["collectComments"] = false; // 不收集注释
        // Json::Value root;
        // std::string errs;
        // std::istringstream s(in);
        // std::string errs_out;
        // bool ok = Json::parseFromStream(builder, s, &root, &errs_out);
        // if (!ok)
        // {
        //     std::cerr << "Failed to parse JSON: " << errs_out << std::endl;
        //     return false;
        // }
        // // 判断字段是否存在和类型是否正确
        // if (!root.isMember("result") || !root["result"].isInt())
        //     return false;
        // if (!root.isMember("code") || !root["code"].isInt())
        //     return false;
        // if (!root.isMember("desc") || !root["desc"].isString())
        //     return false;

        // _result = root["result"].asInt();
        // _code = root["code"].asInt();
        // _desc = root["desc"].asString();
        // return true;
        Json::Value root;
        Json::Reader reader;
        bool res = reader.parse(in, root);
        if (!res)
            return false;
        _result = root["result"].asInt();
        _code = root["code"].asInt();
        _desc = root["desc"].asString();

        return true;
    }

    int getresult() const
    {
        return _result;
    }
    int getcode() const
    {
        return _code;
    }
    std::string getdesc() const
    {
        return _desc;
    }
public:
    int _result;
    int _code;
    std::string _desc;

};


class Factory{
public:
    static std::shared_ptr<Request> CreateRequest(){
        return  std::make_shared<Request>();
    }
    static std::shared_ptr<Response> CreateResponse(){
        return std::make_shared<Response>();
    }
};