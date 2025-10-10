//
// Created by AI Assistant on 2024-10-31.
// 评论控制器头文件
//

#ifndef COMMENTCONTROLLER_H
#define COMMENTCONTROLLER_H

#include <drogon/drogon.h>
#include <json/json.h>
#include <memory>

using namespace drogon;
#include <string>
#include <functional>

class CommentController {
public:
    // 获取题目评论列表
    static void getProblemComments(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 添加评论
    static void createComment(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 删除评论
    static void deleteComment(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 获取评论总数
    static void getCommentCount(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);
    
    // 获取单个评论详情
    static void getCommentById(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback);

private:
    // 辅助方法
    static Json::Value parseJsonRequest(const HttpRequestPtr& req);
    static HttpResponsePtr createJsonResponse(const Json::Value& data, int statusCode = 200);
    static bool validateUser(const HttpRequestPtr& req, const std::string& user_id);
};

#endif // COMMENTCONTROLLER_H