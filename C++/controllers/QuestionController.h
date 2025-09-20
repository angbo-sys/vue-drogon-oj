//
// Created by xiaohua jie on 25-9-12.
// 题目控制器头文件
//

#ifndef QUESTIONCONTROLLER_H
#define QUESTIONCONTROLLER_H

#include <drogon/drogon.h>
#include <json/json.h>
#include <memory>

using namespace drogon;
#include <string>
#include <functional>
class QuestionController
{
public:
    // 获取题目列表
    static void getProblemList(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 获取题目详情
    static void getProblemDetail(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 获取题目类型列表
    static void getQuestionTypes(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 根据类型获取题目
    static void getProblemsByType(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 根据难度获取题目
    static void getProblemsByDifficulty(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 搜索题目
    static void searchProblems(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 获取题目统计
    static void getProblemStatistics(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);

private:
    // 辅助方法
    static Json::Value parseJsonRequest(const HttpRequestPtr& req);
    static HttpResponsePtr createJsonResponse(const Json::Value& data, int statusCode = 200);
    static bool validateUser(const HttpRequestPtr& req, const std::string& user_id);
};

#endif // QUESTIONCONTROLLER_H
