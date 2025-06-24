#include <string>
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
    std::string desc;

};