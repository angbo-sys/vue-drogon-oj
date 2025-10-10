//
// Created by AI Assistant on 2024-10-31.
// 评论控制器实现
//

#include "CommentController.h"
#include "../services/CommentService.h"
#include <drogon/drogon.h>
#include <json/json.h>
#include <iostream>

using namespace drogon;

// 解析JSON请求
Json::Value CommentController::parseJsonRequest(const HttpRequestPtr& req) {
    Json::Value jsonData;
    if (req->getJsonObject()) {
        jsonData = *req->getJsonObject();
    }
    return jsonData;
}

// 创建JSON响应
HttpResponsePtr CommentController::createJsonResponse(const Json::Value& data, int statusCode) {
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(static_cast<HttpStatusCode>(statusCode));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    resp->setBody(data.toStyledString());
    return resp;
}

// 验证用户身份
bool CommentController::validateUser(const HttpRequestPtr& req, const std::string& user_id) {
    // 简单的用户验证逻辑
    // 在实际应用中，这里应该验证token等
    return !user_id.empty();
}

// 获取题目评论列表
void CommentController::getProblemComments(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        // 从URL路径中提取题目ID
        std::string path = req->getPath();
        std::string idStr = path.substr(path.find_last_of('/') + 1);
        
        // 从查询参数中获取页码和每页数量
        auto pageStr = req->getParameter("page");
        auto pageSizeStr = req->getParameter("page_size");
        
        if (idStr.empty()) {
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Missing problem_id parameter";
            
            auto resp = createJsonResponse(error, 400);
            callback(resp);
            return;
        }
        
        int problemId = std::stoi(idStr);
        int page = (pageStr.empty()) ? 1 : std::stoi(pageStr);
        int pageSize = (pageSizeStr.empty()) ? 10 : std::stoi(pageSizeStr);
        
        CommentService commentService;
        Json::Value result = commentService.getCommentsByProblemId(problemId, page, pageSize);
        
        auto resp = createJsonResponse(result);
        callback(resp);
        
    } catch (const std::exception& e) {
        Json::Value error;
        error["status"] = "error";
        error["message"] = "Internal server error: " + std::string(e.what());
        
        auto resp = createJsonResponse(error, 500);
        callback(resp);
    }
}

// 添加评论
void CommentController::createComment(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        Json::Value jsonData = parseJsonRequest(req);
        
        // 检查必要的参数
        if (!jsonData.isMember("problem_id") || !jsonData.isMember("user_id") || 
            !jsonData.isMember("username") || !jsonData.isMember("content")) {
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Missing required parameters";
            
            auto resp = createJsonResponse(error, 400);
            callback(resp);
            return;
        }
        
        int problemId = jsonData["problem_id"].asInt();
        std::string userId = jsonData["user_id"].asString();
        std::string username = jsonData["username"].asString();
        std::string content = jsonData["content"].asString();
        
        // 验证用户
        if (!validateUser(req, userId)) {
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Invalid user";
            
            auto resp = createJsonResponse(error, 401);
            callback(resp);
            return;
        }
        
        // 验证评论内容
        if (content.empty() || content.length() > 500) {
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Comment content is empty or too long";
            
            auto resp = createJsonResponse(error, 400);
            callback(resp);
            return;
        }
        
        CommentService commentService;
        Json::Value result = commentService.addComment(problemId, userId, username, content);
        
        int statusCode = (result["status"].asString() == "success") ? 200 : 400;
        auto resp = createJsonResponse(result, statusCode);
        callback(resp);
        
    } catch (const std::exception& e) {
        Json::Value error;
        error["status"] = "error";
        error["message"] = "Internal server error: " + std::string(e.what());
        
        auto resp = createJsonResponse(error, 500);
        callback(resp);
    }
}

// 删除评论
void CommentController::deleteComment(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        Json::Value jsonData = parseJsonRequest(req);
        
        // 检查必要的参数
        if (!jsonData.isMember("comment_id") || !jsonData.isMember("user_id")) {
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Missing required parameters";
            
            auto resp = createJsonResponse(error, 400);
            callback(resp);
            return;
        }
        
        int commentId = jsonData["comment_id"].asInt();
        std::string userId = jsonData["user_id"].asString();
        
        // 验证用户
        if (!validateUser(req, userId)) {
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Invalid user";
            
            auto resp = createJsonResponse(error, 401);
            callback(resp);
            return;
        }
        
        CommentService commentService;
        Json::Value result = commentService.deleteComment(commentId, userId);
        
        int statusCode = (result["status"].asString() == "success") ? 200 : 400;
        auto resp = createJsonResponse(result, statusCode);
        callback(resp);
        
    } catch (const std::exception& e) {
        Json::Value error;
        error["status"] = "error";
        error["message"] = "Internal server error: " + std::string(e.what());
        
        auto resp = createJsonResponse(error, 500);
        callback(resp);
    }
}

// 获取评论总数
void CommentController::getCommentCount(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        // 从URL路径中提取题目ID
        std::string path = req->getPath();
        std::string idStr = path.substr(path.find_last_of('/') + 1);
        
        if (idStr.empty()) {
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Missing problem_id parameter";
            
            auto resp = createJsonResponse(error, 400);
            callback(resp);
            return;
        }
        
        int problemId = std::stoi(idStr);
        
        CommentService commentService;
        Json::Value result = commentService.getCommentCount(problemId);
        
        auto resp = createJsonResponse(result);
        callback(resp);
        
    } catch (const std::exception& e) {
        Json::Value error;
        error["status"] = "error";
        error["message"] = "Internal server error: " + std::string(e.what());
        
        auto resp = createJsonResponse(error, 500);
        callback(resp);
    }
}

// 获取单个评论详情
void CommentController::getCommentById(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        // 从查询参数中获取评论ID
        auto commentIdStr = req->getParameter("comment_id");
        
        if (commentIdStr.empty()) {
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Missing comment_id parameter";
            
            auto resp = createJsonResponse(error, 400);
            callback(resp);
            return;
        }
        
        int commentId = std::stoi(commentIdStr);
        
        CommentService commentService;
        Json::Value result = commentService.getCommentById(commentId);
        
        int statusCode = (result["status"].asString() == "success") ? 200 : 404;
        auto resp = createJsonResponse(result, statusCode);
        callback(resp);
        
    } catch (const std::exception& e) {
        Json::Value error;
        error["status"] = "error";
        error["message"] = "Internal server error: " + std::string(e.what());
        
        auto resp = createJsonResponse(error, 500);
        callback(resp);
    }
}