//
// Created by AI Assistant on 25-10-16.
// 题目解析控制器实现
//

#include "QuestionAnalysisController.h"
#include "../services/QuestionAnalysisService.h"
#include <drogon/drogon.h>
#include <json/json.h>
#include <iostream>

using namespace drogon;

// 解析JSON请求
Json::Value QuestionAnalysisController::parseJsonRequest(const HttpRequestPtr& req) {
    Json::Value jsonData;
    if (req->getJsonObject()) {
        jsonData = *req->getJsonObject();
    }
    return jsonData;
}

// 创建JSON响应
HttpResponsePtr QuestionAnalysisController::createJsonResponse(const Json::Value& data, int statusCode) {
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(static_cast<HttpStatusCode>(statusCode));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    resp->setBody(data.toStyledString());
    return resp;
}

// 验证用户身份
bool QuestionAnalysisController::validateUser(const HttpRequestPtr& req, const std::string& user_id) {
    // 简单的用户验证逻辑
    // 在实际应用中，这里应该验证token等
    return !user_id.empty();
}

// 获取题目解析
void QuestionAnalysisController::getQuestionAnalysis(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
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
        
        int questionId = std::stoi(idStr);
        QuestionAnalysisService analysisService;
        Json::Value result = analysisService.getAnalysisByQuestionId(questionId);
        
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

// 提交用户自己的解析
void QuestionAnalysisController::submitUserAnalysis(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        Json::Value jsonData = parseJsonRequest(req);
        
        // 验证必要参数
        if (!jsonData.isMember("question_id") || !jsonData.isMember("user_id") || !jsonData.isMember("analysis_content")) {
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Missing required parameters";
            
            auto resp = createJsonResponse(error, 400);
            callback(resp);
            return;
        }
        
        int questionId = jsonData["question_id"].asInt();
        std::string userId = jsonData["user_id"].asString();
        std::string analysisContent = jsonData["analysis_content"].asString();
        
        // 验证用户
        if (!validateUser(req, userId)) {
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Invalid user";
            
            auto resp = createJsonResponse(error, 401);
            callback(resp);
            return;
        }
        
        QuestionAnalysisService analysisService;
        Json::Value result = analysisService.addUserAnalysis(questionId, userId, analysisContent);
        
        auto resp = createJsonResponse(result, 201);
        callback(resp);
        
    } catch (const std::exception& e) {
        Json::Value error;
        error["status"] = "error";
        error["message"] = "Internal server error: " + std::string(e.what());
        
        auto resp = createJsonResponse(error, 500);
        callback(resp);
    }
}

// 获取解析点赞数
void QuestionAnalysisController::getAnalysisLikes(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        // 从URL路径中提取解析ID
        std::string path = req->getPath();
        std::string idStr = path.substr(path.find_last_of('/') + 1);
        
        if (idStr.empty()) {
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Missing analysis ID";
            
            auto resp = createJsonResponse(error, 400);
            callback(resp);
            return;
        }
        
        int analysisId = std::stoi(idStr);
        QuestionAnalysisService analysisService;
        Json::Value result = analysisService.getAnalysisLikes(analysisId);
        
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

// 点赞解析
void QuestionAnalysisController::likeAnalysis(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        Json::Value jsonData = parseJsonRequest(req);
        
        // 验证必要参数
        if (!jsonData.isMember("analysis_id") || !jsonData.isMember("user_id")) {
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Missing required parameters";
            
            auto resp = createJsonResponse(error, 400);
            callback(resp);
            return;
        }
        
        int analysisId = jsonData["analysis_id"].asInt();
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
        
        QuestionAnalysisService analysisService;
        Json::Value result = analysisService.likeAnalysis(analysisId, userId);
        
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

// 获取热门解析
void QuestionAnalysisController::getPopularAnalyses(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        QuestionAnalysisService analysisService;
        Json::Value result = analysisService.getPopularAnalyses();
        
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