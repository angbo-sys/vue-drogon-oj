#include "Users.h"
#include <drogon/drogon.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <uuid/uuid.h>
#include <random>
#include <chrono>

using namespace drogon;

// 读取 users.json 到 Json::Value（对象形式： username -> { password_hash, salt, token, ... }）
// 说明：
// - 使用互斥锁保护文件读写，防止多线程并发访问导致竞态。
// - 如果文件不存在或为空，返回一个空的 JSON 对象（成功返回 true），调用方可以据此创建新用户。
// - 如果解析失败（文件损坏等），记录错误并返回 false。
bool Users::loadUsers(Json::Value &root) {
    std::lock_guard<std::mutex> lk(fileMutex_); // 加锁保护文件操作
    std::ifstream ifs(usersFile_);
    if (!ifs.is_open()) {
        // 文件不存在：视为没有用户，返回空对象
        root = Json::Value(Json::objectValue);
        return true;
    }
    // 将整个文件读成字符串
    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    if (content.empty()) {
        // 文件为空，也返回空对象
        root = Json::Value(Json::objectValue);
        return true;
    }
    // 使用 JsonCpp 解析字符串到 Json::Value
    Json::CharReaderBuilder builder;
    std::string errs;
    std::istringstream iss(content);
    if (!Json::parseFromStream(builder, iss, &root, &errs)) {
        // 解析失败：记录错误并返回 false，表示读取失败（可能文件损坏）
        LOG_ERROR << "Failed to parse " << usersFile_ << ": " << errs;
        root = Json::Value(Json::objectValue);
        return false;
    }
    return true;
}

// 将 Json::Value 写回 users.json
// 说明：
// - 使用互斥锁防止并发写入导致文件损坏。
// - 采用 trunc 模式覆盖原文件，写入前请确保 root 内容正确。
// - 返回是否成功写入。
bool Users::saveUsers(const Json::Value &root) {
    std::lock_guard<std::mutex> lk(fileMutex_);
    std::ofstream ofs(usersFile_, std::ios::trunc);
    if (!ofs.is_open()) {
        LOG_ERROR << "Failed to open " << usersFile_ << " for writing";
        return false;
    }
    // 使用 JsonCpp 的 StreamWriter 美化输出（便于人工查看）
    Json::StreamWriterBuilder wbuilder;
    wbuilder["indentation"] = "  ";
    std::unique_ptr<Json::StreamWriter> writer(wbuilder.newStreamWriter());
    writer->write(root, &ofs);
    return true;
}

// 简单的 SHA-256 hex 实现（演示用途）
// 说明：对输入字符串计算 SHA-256，并返回小写十六进制字符串。
// 注意：生产环境应使用更强健的 KDF（如 PBKDF2/ bcrypt/ scrypt/ Argon2）。
std::string Users::sha256_hex(const std::string &input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.data(), input.size());
    SHA256_Final(hash, &sha256);
    std::ostringstream oss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        // 每个字节转换为两位十六进制表示，前导补零
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return oss.str();
}

// 产生随机 salt（16 字节 -> 32 hex 字符）
// 说明：salt 用来避免相同密码产生相同哈希（防止彩虹表攻击）。
std::string Users::genSalt() {
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint32_t> dist(0, 255);
    std::ostringstream oss;
    for (int i = 0; i < 16; ++i) {
        uint8_t b = static_cast<uint8_t>(dist(gen));
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)b;
    }
    return oss.str();
}

// 生成一个 UUID 字符串作为 token
// 说明：用于登录后返回给客户端的简单会话令牌（示例用）。生产环境可用 JWT 或其他机制。
std::string Users::genToken() {
    uuid_t u;
    uuid_generate_random(u);
    char s[37]; // 36 字符 + 终止符
    uuid_unparse_lower(u, s);
    return std::string(s);
}

// POST /api/users/register
// 说明：接收 JSON 参数 { "username": "...", "password": "..." }
// 流程：
// 1. 基本输入校验（存在性与密码长度）；
// 2. 读取 users.json，检查用户名是否已存在；
// 3. 生成盐并计算密码哈希（sha256(password + salt)）；
// 4. 写回 users.json（包含创建时间）；
// 5. 返回 JSON 响应，code==0 表示成功。
void Users::registerUser(const HttpRequestPtr &req,
                         std::function<void (const HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    Json::Value ret;
    if (!json || !(*json).isMember("username") || !(*json).isMember("password")) {
        ret["code"] = 1;
        ret["msg"] = "missing username or password";
        callback(HttpResponse::newHttpJsonResponse(ret));
        return;
    }
    std::string username = (*json)["username"].asString();
    std::string password = (*json)["password"].asString();

    // 简单校验
    if (username.empty() || password.size() < 4) {
        ret["code"] = 2;
        ret["msg"] = "invalid username or password too short";
        callback(HttpResponse::newHttpJsonResponse(ret));
        return;
    }

    Json::Value users;
    if (!loadUsers(users)) {
        // 读取或解析失败
        ret["code"] = 3;
        ret["msg"] = "internal error reading users";
        callback(HttpResponse::newHttpJsonResponse(ret));
        return;
    }

    if (users.isMember(username)) {
        // 用户已存在
        ret["code"] = 4;
        ret["msg"] = "username exists";
        callback(HttpResponse::newHttpJsonResponse(ret));
        return;
    }

    // 生成 salt 并计算哈希后保存
    std::string salt = genSalt();
    std::string hash = sha256_hex(password + salt);

    Json::Value u;
    u["password_hash"] = hash;
    u["salt"] = salt;
    u["created_at"] = (Json::Int64)std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    users[username] = u;
    if (!saveUsers(users)) {
        ret["code"] = 5;
        ret["msg"] = "failed to save user";
        callback(HttpResponse::newHttpJsonResponse(ret));
        return;
    }

    ret["code"] = 0;
    ret["msg"] = "success";
    callback(HttpResponse::newHttpJsonResponse(ret));
}

// POST /api/users/login
// 说明：接收 JSON 参数 { "username": "...", "password": "..." }
// 流程：
// 1. 读取 users.json，查找用户记录；
// 2. 用存储的 salt 计算输入密码哈希并比对；
// 3. 若通过，生成 token 并写回 users.json（保存 token 与过期时间）；
// 4. 返回包含 token 的成功响应。
void Users::loginUser(const HttpRequestPtr &req,
                      std::function<void (const HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    Json::Value ret;
    if (!json || !(*json).isMember("username") || !(*json).isMember("password")) {
        ret["code"] = 1;
        ret["msg"] = "missing username or password";
        callback(HttpResponse::newHttpJsonResponse(ret));
        return;
    }
    std::string username = (*json)["username"].asString();
    std::string password = (*json)["password"].asString();

    Json::Value users;
    if (!loadUsers(users)) {
        ret["code"] = 2;
        ret["msg"] = "internal error reading users";
        callback(HttpResponse::newHttpJsonResponse(ret));
        return;
    }

    if (!users.isMember(username)) {
        // 用户不存在
        ret["code"] = 3;
        ret["msg"] = "user not found";
        callback(HttpResponse::newHttpJsonResponse(ret));
        return;
    }

    // 验证密码
    const Json::Value &u = users[username];
    std::string salt = u["salt"].asString();
    std::string expectHash = u["password_hash"].asString();
    std::string realHash = sha256_hex(password + salt);

    if (realHash != expectHash) {
        ret["code"] = 4;
        ret["msg"] = "wrong password";
        callback(HttpResponse::newHttpJsonResponse(ret));
        return;
    }

    // 登录成功：生成 token 并写回文件（简单实现）
    std::string token = genToken();
    Json::Value mutableUsers = users; // 把读取到的 users 复制一份以便修改
    mutableUsers[username]["token"] = token;
    // token 过期时间（当前时间 + 24 小时，单位：秒）
    mutableUsers[username]["token_expire_at"] = (Json::Int64)(
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()).count() + 3600 * 24);

    if (!saveUsers(mutableUsers)) {
        ret["code"] = 5;
        ret["msg"] = "failed to save token";
        callback(HttpResponse::newHttpJsonResponse(ret));
        return;
    }

    ret["code"] = 0;
    ret["msg"] = "success";
    ret["token"] = token;
    callback(HttpResponse::newHttpJsonResponse(ret));
}