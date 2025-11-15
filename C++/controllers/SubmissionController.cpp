//
// Created by xiaohua jie on 25-9-12.
// 提交控制器实现
//

#include "SubmissionController.h"
#include <drogon/drogon.h>
#include <json/json.h>
#include <iostream>

using namespace drogon;

void SubmissionController::submitCode(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        // 解析JSON请求体
        Json::Value requestData = parseJsonRequest(req);
        
        // 验证必需参数
        if (!requestData.isMember("user_id") || !requestData.isMember("question_id") || !requestData.isMember("code")) {
            auto resp = createJsonResponse(Json::Value(), 400);
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Missing required parameters: user_id, question_id, code";
            resp->setBody(error.toStyledString());
            callback(resp);
            return;
        }
        
        // 验证用户身份
        std::string user_id = requestData["user_id"].asString();
        if (!validateUser(req, user_id)) {
            auto resp = createJsonResponse(Json::Value(), 401);
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Unauthorized";
            resp->setBody(error.toStyledString());
            callback(resp);
            return;
        }
        
        // 提取参数
        int question_id = requestData["question_id"].asInt();
        std::string code = requestData["code"].asString();
        std::string language = requestData.get("language", "cpp").asString();
        
        // 验证代码不为空
        if (code.empty()) {
            auto resp = createJsonResponse(Json::Value(), 400);
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Code cannot be empty";
            resp->setBody(error.toStyledString());
            callback(resp);
            return;
        }
        
        // 创建提交服务
        SubmissionService submissionService;
        
        // 提交代码
        Json::Value result = submissionService.submitCode(user_id, question_id, code, language);
        
        // 返回结果
        auto resp = createJsonResponse(result);
        resp->setBody(result.toStyledString());
        callback(resp);
        
    } catch (const std::exception& e) {
        auto resp = createJsonResponse(Json::Value(), 500);
        Json::Value error;
        error["status"] = "error";
        error["message"] = "Internal server error: " + std::string(e.what());
        resp->setBody(error.toStyledString());
        callback(resp);
    }
}

void SubmissionController::getUserSubmissions(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        // 验证用户身份
        std::string user_id;
        if (!validateUser(req, user_id)) {
            auto resp = createJsonResponse(Json::Value(), 401);
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Unauthorized";
            resp->setBody(error.toStyledString());
            callback(resp);
            return;
        }
        
        // 提取分页参数
        int page = extractIntQueryParam(req, "page", 1);
        int limit = extractIntQueryParam(req, "limit", 10);
        
        // 创建提交服务
        SubmissionService submissionService;
        
        // 获取用户提交历史
        Json::Value result = submissionService.getUserSubmissions(user_id, page, limit);
        
        // 返回结果
        auto resp = createJsonResponse(result);
        resp->setBody(result.toStyledString());
        callback(resp);
        
    } catch (const std::exception& e) {
        auto resp = createJsonResponse(Json::Value(), 500);
        Json::Value error;
        error["status"] = "error";
        error["message"] = "Internal server error: " + std::string(e.what());
        resp->setBody(error.toStyledString());
        callback(resp);
    }
}

void SubmissionController::getQuestionSubmissions(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        // 提取题目ID
        std::string question_id_str = extractPathParam(req, "question_id");
        if (question_id_str.empty()) {
            auto resp = createJsonResponse(Json::Value(), 400);
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Missing question_id parameter";
            resp->setBody(error.toStyledString());
            callback(resp);
            return;
        }
        
        int question_id = std::stoi(question_id_str);
        
        // 提取分页参数
        int page = extractIntQueryParam(req, "page", 1);
        int limit = extractIntQueryParam(req, "limit", 10);
        
        // 创建提交服务
        SubmissionService submissionService;
        
        // 获取题目提交历史
        Json::Value result = submissionService.getQuestionSubmissions(question_id, page, limit);
        
        // 返回结果
        auto resp = createJsonResponse(result);
        resp->setBody(result.toStyledString());
        callback(resp);
        
    } catch (const std::exception& e) {
        auto resp = createJsonResponse(Json::Value(), 500);
        Json::Value error;
        error["status"] = "error";
        error["message"] = "Internal server error: " + std::string(e.what());
        resp->setBody(error.toStyledString());
        callback(resp);
    }
}

void SubmissionController::getSubmissionDetail(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        // 提取提交ID
        std::string submission_id = extractPathParam(req, "submission_id");
        if (submission_id.empty()) {
            auto resp = createJsonResponse(Json::Value(), 400);
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Missing submission_id parameter";
            resp->setBody(error.toStyledString());
            callback(resp);
            return;
        }
        
        // 创建提交服务
        SubmissionService submissionService;
        
        // 获取提交详情
        Json::Value result = submissionService.getSubmissionDetail(submission_id);
        
        // 返回结果
        auto resp = createJsonResponse(result);
        resp->setBody(result.toStyledString());
        callback(resp);
        
    } catch (const std::exception& e) {
        auto resp = createJsonResponse(Json::Value(), 500);
        Json::Value error;
        error["status"] = "error";
        error["message"] = "Internal server error: " + std::string(e.what());
        resp->setBody(error.toStyledString());
        callback(resp);
    }
}

void SubmissionController::getAllSubmissions(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        // 验证管理员身份
        if (!validateAdmin(req)) {
            auto resp = createJsonResponse(Json::Value(), 401);
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Unauthorized - Admin access required";
            resp->setBody(error.toStyledString());
            callback(resp);
            return;
        }
        
        // 提取分页参数
        int page = extractIntQueryParam(req, "page", 1);
        int limit = extractIntQueryParam(req, "limit", 10);
        
        // 创建提交服务
        SubmissionService submissionService;
        
        // 获取所有提交记录
        Json::Value result = submissionService.getAllSubmissions(page, limit);
        
        // 返回结果
        auto resp = createJsonResponse(result);
        resp->setBody(result.toStyledString());
        callback(resp);
        
    } catch (const std::exception& e) {
        auto resp = createJsonResponse(Json::Value(), 500);
        Json::Value error;
        error["status"] = "error";
        error["message"] = "Internal server error: " + std::string(e.what());
        resp->setBody(error.toStyledString());
        callback(resp);
    }
}

void SubmissionController::getSubmissionStatistics(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        // 提取用户ID参数（可选）
        std::string user_id = extractQueryParam(req, "user_id");
        
        // 创建提交服务
        SubmissionService submissionService;
        
        // 获取提交统计
        Json::Value result = submissionService.getSubmissionStatistics(user_id);
        
        // 返回结果
        auto resp = createJsonResponse(result);
        resp->setBody(result.toStyledString());
        callback(resp);
        
    } catch (const std::exception& e) {
        auto resp = createJsonResponse(Json::Value(), 500);
        Json::Value error;
        error["status"] = "error";
        error["message"] = "Internal server error: " + std::string(e.what());
        resp->setBody(error.toStyledString());
        callback(resp);
    }
}

void SubmissionController::deleteSubmission(const HttpRequestPtr& req, std::function<void(const HttpResponsePtr&)>&& callback) {
    try {
        // 验证管理员身份
        if (!validateAdmin(req)) {
            auto resp = createJsonResponse(Json::Value(), 401);
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Unauthorized - Admin access required";
            resp->setBody(error.toStyledString());
            callback(resp);
            return;
        }
        
        // 提取提交ID
        std::string submission_id = extractPathParam(req, "submission_id");
        if (submission_id.empty()) {
            auto resp = createJsonResponse(Json::Value(), 400);
            Json::Value error;
            error["status"] = "error";
            error["message"] = "Missing submission_id parameter";
            resp->setBody(error.toStyledString());
            callback(resp);
            return;
        }
        
        // 创建提交服务
        SubmissionService submissionService;
        
        // 删除提交记录
        Json::Value result = submissionService.deleteSubmission(submission_id);
        
        // 返回结果
        auto resp = createJsonResponse(result);
        resp->setBody(result.toStyledString());
        callback(resp);
        
    } catch (const std::exception& e) {
        auto resp = createJsonResponse(Json::Value(), 500);
        Json::Value error;
        error["status"] = "error";
        error["message"] = "Internal server error: " + std::string(e.what());
        resp->setBody(error.toStyledString());
        callback(resp);
    }
}

Json::Value SubmissionController::parseJsonRequest(const HttpRequestPtr& req) {
    Json::Value data;
    
    if (req->getJsonObject()) {
        data = *req->getJsonObject();
    } else {
        // 尝试从请求体中解析JSON
        std::string body = std::string(req->getBody());
        if (!body.empty()) {
            Json::Reader reader;
            reader.parse(body, data);
        }
    }
    
    return data;
}

HttpResponsePtr SubmissionController::createJsonResponse(const Json::Value& data, int statusCode) {
    auto resp = HttpResponse::newHttpResponse();
    resp->setStatusCode(static_cast<HttpStatusCode>(statusCode));
    resp->setContentTypeCode(CT_APPLICATION_JSON);
    
    // 添加CORS头
    resp->addHeader("Access-Control-Allow-Origin", "*");
    resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    resp->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
    
    return resp;
}

bool SubmissionController::validateUser(const HttpRequestPtr& req, std::string& user_id) {
    // 简单的用户验证实现
    // 在实际应用中，这里应该验证JWT token或其他认证机制
    
    // 从请求头中获取用户ID（临时实现）
    auto headers = req->getHeaders();
    auto it = headers.find("user-id");
    if (it != headers.end()) {
        user_id = it->second;
        return true;
    }
    
    // 从查询参数中获取用户ID（临时实现）
    user_id = extractQueryParam(req, "user_id");
    return !user_id.empty();
}

bool SubmissionController::validateAdmin(const HttpRequestPtr& req) {
    // 简单的管理员验证实现
    // 在实际应用中，这里应该验证管理员JWT token
    
    auto headers = req->getHeaders();
    auto it = headers.find("admin-token");
    if (it != headers.end()) {
        return it->second == "admin123"; // 临时的管理员token
    }
    
    return false;
}

std::string SubmissionController::extractPathParam(const HttpRequestPtr& req, const std::string& param) {
    // 从URL路径中提取参数
    // 这里需要根据实际的路由配置来实现
    return "";
}

std::string SubmissionController::extractQueryParam(const HttpRequestPtr& req, const std::string& param) {
    auto params = req->getParameters();
    auto it = params.find(param);
    if (it != params.end()) {
        return it->second;
    }
    return "";
}

int SubmissionController::extractIntQueryParam(const HttpRequestPtr& req, const std::string& param, int defaultValue) {
    std::string value = extractQueryParam(req, param);
    if (value.empty()) {
        return defaultValue;
    }
    
    try {
        return std::stoi(value);
    } catch (const std::exception& e) {
        return defaultValue;
    }
}
