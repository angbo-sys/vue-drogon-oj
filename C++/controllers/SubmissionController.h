//
// Created by xiaohua jie on 25-9-12.
// 提交控制器 - 处理代码提交相关的HTTP请求
//

#ifndef SUBMISSIONCONTROLLER_H
#define SUBMISSIONCONTROLLER_H

#include <drogon/drogon.h>
#include <json/json.h>
#include "../services/SubmissionService.h"

using namespace drogon;

class SubmissionController {
public:
    // 提交代码
    static void submitCode(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 获取用户提交历史
    static void getUserSubmissions(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 获取题目提交历史
    static void getQuestionSubmissions(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 获取提交详情
    static void getSubmissionDetail(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 获取所有提交记录
    static void getAllSubmissions(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 获取提交统计
    static void getSubmissionStatistics(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 删除提交记录
    static void deleteSubmission(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);

private:
    // 解析JSON请求体
    static Json::Value parseJsonRequest(const HttpRequestPtr& req);
    
    // 创建JSON响应
    static HttpResponsePtr createJsonResponse(const Json::Value& data, int statusCode = 200);
    
    // 验证用户身份
    static bool validateUser(const HttpRequestPtr& req, std::string& user_id);
    
    // 验证管理员身份
    static bool validateAdmin(const HttpRequestPtr& req);
    
    // 提取路径参数
    static std::string extractPathParam(const HttpRequestPtr& req, const std::string& param);
    
    // 提取查询参数
    static std::string extractQueryParam(const HttpRequestPtr& req, const std::string& param);
    
    // 提取整数查询参数
    static int extractIntQueryParam(const HttpRequestPtr& req, const std::string& param, int defaultValue = 0);
};

#endif //SUBMISSIONCONTROLLER_H
