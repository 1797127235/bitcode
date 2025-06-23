#include <string>
#include <iostream>
#include <json/json.h>
class Request
{
public:
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

        Json::StreamWriterBuilder builder;
        builder["indentation"] = "";                // 不缩进
        builder["enableYAMLCompatibility"] = false; // 避免特殊格式
        builder["dropNullPlaceholders"] = true;     // 可选：省略 null 值
        builder["commentStyle"] = "None";           // 去除注释
        *out = Json::writeString(builder, root);
        return true;
    }
    // 反序列化
    bool Deserialize(const std::string &in)
    {
        Json::CharReaderBuilder builder;
        builder["collectComments"] = false; // 不收集注释

        Json::Value root;
        std::string errs;

        std::istringstream s(in);
        std::string errs_out;
        bool ok = Json::parseFromStream(builder, s, &root, &errs_out);
        if (!ok)
        {
            std::cerr << "Failed to parse JSON: " << errs_out << std::endl;
            return false;
        }

        // 判断字段是否存在和类型是否正确
        if (!root.isMember("x") || !root["x"].isInt())
            return false;
        if (!root.isMember("op") || !root["op"].isString())
            return false;
        if (!root.isMember("y") || !root["y"].isInt())
            return false;

        // 赋值
        _x = root["x"].asInt();
        std::string op_str = root["op"].asString();
        _op = op_str.empty() ? '\0' : op_str[0];  // 取字符串的第一个字符
        _y = root["y"].asInt();

        return true;
    }

private:
    int _x;
    int _y;
    char _op;
};

class Response
{
public:
    // 序列化
    bool Serialize(std::string *out)
    {
        Json::Value root;
        root["result"] = _result;
        root["code"] = _code;
        Json::StreamWriterBuilder builder;
        builder["indentation"] = "";                // 不缩进
        builder["enableYAMLCompatibility"] = false; // 避免特殊格式
        builder["dropNullPlaceholders"] = true;     // 可选：省略 null 值
        builder["commentStyle"] = "None";           // 去除注释
        *out=Json::writeString(builder,root);
    }
    // 反序列化
    bool Deserialize(const std::string &in)
    {
        Json::CharReaderBuilder builder;
        builder["collectComments"] = false; // 不收集注释
        Json::Value root;
        std::string errs;
        std::istringstream s(in);
        std::string errs_out;
        bool ok = Json::parseFromStream(builder, s, &root, &errs_out);
        if (!ok)
        {
            std::cerr << "Failed to parse JSON: " << errs_out << std::endl;
            return false;
        }
        if (!root.isMember("result") || !root["result"].isInt())
            return false;
        if (!root.isMember("code") || !root["code"].isInt())
            return false;
        _result = root["result"].asInt();
        _code = root["code"].asInt();
        return true;
    }

private:
    int _result;
    int _code;
};