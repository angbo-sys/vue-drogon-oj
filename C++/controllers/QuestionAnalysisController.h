//
// Created by AI Assistant on 25-10-16.
// 题目解析控制器头文件
//

#ifndef QUESTIONANALYSISCONTROLLER_H
#define QUESTIONANALYSISCONTROLLER_H

#include <drogon/drogon.h>
#include <json/json.h>
#include <memory>

using namespace drogon;
#include <string>
#include <functional>

class QuestionAnalysisController
{
public:
    // 获取题目解析
    static void getQuestionAnalysis(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 提交用户自己的解析
    static void submitUserAnalysis(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 获取解析点赞数
    static void getAnalysisLikes(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 点赞解析
    static void likeAnalysis(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 获取热门解析
    static void getPopularAnalyses(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);

private:
    // 辅助方法
    static Json::Value parseJsonRequest(const HttpRequestPtr& req);
    static HttpResponsePtr createJsonResponse(const Json::Value& data, int statusCode = 200);
    static bool validateUser(const HttpRequestPtr& req, const std::string& user_id);
};

#endif // QUESTIONANALYSISCONTROLLER_H