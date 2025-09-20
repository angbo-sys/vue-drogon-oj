//
// Created by xiaohua jie on 25-9-12.
// 题目控制器实现
//

#include "QuestionController.h"
#include "../services/GetQuestionService.h"
#include <drogon/drogon.h>
#include <json/json.h>
#include <iostream>

using namespace drogon;

// 解析JSON请求
Json::Value QuestionController::parseJsonRequest(const HttpRequestPtr& req) {
    Json::Value jsonData;
    if (req->getJsonObject()) {
        jsonData = *req->getJsonObject();
    }
    return jsonData;
}

// 创建JSON响应
HttpResponsePtr QuestionController::createJsonResponse(const Json::Value& data, int statusCode) {
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(static_cast<HttpStatusCode>(statusCode));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    resp->setBody(data.toStyledString());
    return resp;
}

// 验证用户身份
bool QuestionController::validateUser(const HttpRequestPtr& req, const std::string& user_id) {
    // 简单的用户验证逻辑
    // 在实际应用中，这里应该验证token等
    return !user_id.empty();
}

// 获取题目列表
void QuestionController::getProblemList(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        QuestionService questionService;
        Json::Value result = questionService.getAllQuestions();
        
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

// 获取题目详情
void QuestionController::getProblemDetail(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        // 从URL路径中提取题目ID
        std::string path = req->getPath();
        std::string idStr = path.substr(path.find_last_of('/') + 1);
        
        if (idStr.empty()) {
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Missing problem ID";
            
            auto resp = createJsonResponse(error, 400);
            callback(resp);
            return;
        }
        
        int problemId = std::stoi(idStr);
        QuestionService questionService;
        Json::Value result = questionService.getQuestionById(problemId);
        
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

// 获取题目类型列表
void QuestionController::getQuestionTypes(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        QuestionService questionService;
        Json::Value result = questionService.getAllQuestionTypes();
        
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

// 根据类型获取题目
void QuestionController::getProblemsByType(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        // 从查询参数中获取类型ID
        auto typeIdStr = req->getParameter("type_id");
        if (typeIdStr.empty()) {
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Missing type_id parameter";
            
            auto resp = createJsonResponse(error, 400);
            callback(resp);
            return;
        }
        
        int typeId = std::stoi(typeIdStr);
        QuestionService questionService;
        Json::Value result = questionService.getQuestionsByTypeId(typeId);
        
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

// 根据难度获取题目
void QuestionController::getProblemsByDifficulty(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        // 从查询参数中获取难度ID
        auto difficultyIdStr = req->getParameter("difficulty_id");
        if (difficultyIdStr.empty()) {
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Missing difficulty_id parameter";
            
            auto resp = createJsonResponse(error, 400);
            callback(resp);
            return;
        }
        
        int difficultyId = std::stoi(difficultyIdStr);
        QuestionService questionService;
        Json::Value result = questionService.getQuestionsByDifficulty(difficultyId);
        
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

// 搜索题目
void QuestionController::searchProblems(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        // 从查询参数中获取搜索关键词
        auto keyword = req->getParameter("keyword");
        if (keyword.empty()) {
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Missing keyword parameter";
            
            auto resp = createJsonResponse(error, 400);
            callback(resp);
            return;
        }
        
        QuestionService questionService;
        Json::Value result = questionService.searchQuestions(keyword);
        
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

// 获取题目统计
void QuestionController::getProblemStatistics(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        QuestionService questionService;
        Json::Value result = questionService.getQuestionStatistics();
        
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
