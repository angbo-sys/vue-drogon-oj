#pragma once

#include <drogon/HttpController.h>
#include <json/json.h>
#include <mutex>
#include <string>

class Users : public drogon::HttpController<Users> {
public:
    METHOD_LIST_BEGIN
        // POST /api/users/register
        ADD_METHOD_TO(Users::registerUser, "/api/users/register", Post);
        // POST /api/users/login
        ADD_METHOD_TO(Users::loginUser, "/api/users/login", Post);
    METHOD_LIST_END

    void registerUser(const drogon::HttpRequestPtr &req,
                      std::function<void (const drogon::HttpResponsePtr &)> &&callback);
    void loginUser(const drogon::HttpRequestPtr &req,
                   std::function<void (const drogon::HttpResponsePtr &)> &&callback);

private:
    // 保护文件读写（多线程安全）
    std::mutex fileMutex_;
    const std::string usersFile_ = "./data/users.json";

    // 辅助函数
    bool loadUsers(Json::Value &root);
    bool saveUsers(const Json::Value &root);
    std::string sha256_hex(const std::string &input);
    std::string genSalt();
    std::string genToken();
};
