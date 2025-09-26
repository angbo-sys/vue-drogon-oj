#include <drogon/drogon.h>
#include "controllers/QuestionController.h"
#include "controllers/SubmissionController.h"
#include "services/GetUserInfo.h"
#include "services/AIChatService.h"
using namespace drogon;

int main() {
    //Set HTTP listener address and port
    app().addListener("0.0.0.0", 5555);
    
    // Add a simple route handler for the root path
    app().registerHandler("/", [](const HttpRequestPtr &req,
                                 std::function<void (const HttpResponsePtr &)> &&callback) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k200OK);
        resp->setContentTypeCode(CT_TEXT_HTML);
        resp->setBody("<h1>Hello from Drogon!</h1><p>Server is running successfully.</p>");
        callback(resp);
    });

    // Add API endpoints for Vue frontend
    app().registerHandler("/test", [](const HttpRequestPtr &req,
                                     std::function<void (const HttpResponsePtr &)> &&callback) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k200OK);
        resp->setContentTypeCode(CT_APPLICATION_JSON);
        
        // Get JSON data from request
        Json::Value jsonData;
        if (req->getJsonObject()) {
            jsonData = *req->getJsonObject();
        }
        
        // Create response
        Json::Value response;
        response["status"] = "success";
        response["message"] = "POST request received successfully";
        response["received_data"] = jsonData;
        response["timestamp"] = std::to_string(time(nullptr));
        
        resp->setBody(response.toStyledString());
        callback(resp);
    });

    // Add problems endpoints using QuestionController
    app().registerHandler("/api/problems", [](const HttpRequestPtr &req,
                                             std::function<void (const HttpResponsePtr &)> &&callback) {
        QuestionController::getProblemList(req, std::move(callback));
    });
    
    app().registerHandler("/api/problems/{id}", [](const HttpRequestPtr &req,
                                                   std::function<void (const HttpResponsePtr &)> &&callback) {
        QuestionController::getProblemDetail(req, std::move(callback));
    });
    
    // Add additional problem endpoints
    app().registerHandler("/api/problems/types", [](const HttpRequestPtr &req,
                                                    std::function<void (const HttpResponsePtr &)> &&callback) {
        QuestionController::getQuestionTypes(req, std::move(callback));
    });
    
    app().registerHandler("/api/problems/search", [](const HttpRequestPtr &req,
                                                     std::function<void (const HttpResponsePtr &)> &&callback) {
        QuestionController::searchProblems(req, std::move(callback));
    });
    
    app().registerHandler("/api/problems/statistics", [](const HttpRequestPtr &req,
                                                         std::function<void (const HttpResponsePtr &)> &&callback) {
        QuestionController::getProblemStatistics(req, std::move(callback));
    });
    
    // Add auth endpoints
    app().registerHandler("/api/auth/login", [](const HttpRequestPtr &req,
                                               std::function<void (const HttpResponsePtr &)> &&callback) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k200OK);
        resp->setContentTypeCode(CT_APPLICATION_JSON);
        
        // 解析请求体获取登录信息
        std::string body = std::string(req->getBody());
        Json::Value requestJson;
        Json::CharReaderBuilder builder;
        std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        std::string errors;
        
        bool parsingSuccessful = reader->parse(
            body.c_str(),
            body.c_str() + body.size(),
            &requestJson,
            &errors
        );
        
        Json::Value response;
        
        if (!parsingSuccessful) {
            response["status"] = "error";
            response["message"] = "请求格式错误: " + errors;
            resp->setStatusCode(k400BadRequest);
        } else {
            std::string username = requestJson.get("username", "").asString();
            std::string password = requestJson.get("password", "").asString();
            
            // 调用用户服务进行实际验证
            UserMethods userMethods;
            Json::Value loginResult = userMethods.authenticateUser(username, password);
            
            if (loginResult["status"].asString() == "success") {
                std::string userId = loginResult["user_id"].asString();
                
                // 设置响应
                response["status"] = "success";
                response["message"] = "登录成功";
                response["token"] = "login_token_" + username;
                response["user"]["id"] = userId;
                response["user"]["username"] = username;
                response["user"]["email"] = username + "@example.com";
            } else {
                // 登录失败
                response["status"] = "error";
                response["message"] = loginResult["message"];
            }
        }
        
        resp->setBody(response.toStyledString());
        callback(resp);
    });
    
    app().registerHandler("/api/auth/register", [](const HttpRequestPtr &req,
                                                  std::function<void (const HttpResponsePtr &)> &&callback) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k200OK);
        resp->setContentTypeCode(CT_APPLICATION_JSON);
        
        // 解析请求体获取注册信息
        std::string body = std::string(req->getBody());
        Json::Value requestJson;
        Json::CharReaderBuilder builder;
        std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        std::string errors;
        
        bool parsingSuccessful = reader->parse(
            body.c_str(),
            body.c_str() + body.size(),
            &requestJson,
            &errors
        );
        
        Json::Value response;
        
        if (!parsingSuccessful) {
            response["status"] = "error";
            response["message"] = "请求格式错误: " + errors;
            resp->setStatusCode(k400BadRequest);
        } else {
            // 获取注册信息
            std::string username = requestJson.get("username", "").asString();
            std::string email = requestJson.get("email", "").asString();
            std::string password = requestJson.get("password", "").asString();
            
            // 参数验证
            if (username.empty() || password.empty()) {
                response["status"] = "error";
                response["message"] = "用户名和密码不能为空";
                resp->setStatusCode(k400BadRequest);
                return;
            }
            
            std::cerr << "Processing registration for username: " << username << ", email: " << email << std::endl;
            
            // 调用用户服务进行实际注册
            UserMethods userMethods;
            Json::Value registerResult = userMethods.registerUser(username, password);
            
            if (registerResult["status"].asString() == "success") {
                std::string userId = registerResult["user_id"].asString();
                
                // 设置响应
                response["status"] = "success";
                response["message"] = "注册成功";
                response["token"] = "register_token_" + username;
                response["user"]["id"] = userId;
                response["user"]["username"] = username;
                response["user"]["email"] = email;
            } else {
                // 注册失败
                response["status"] = "error";
                response["message"] = registerResult["message"];
            }
        }
        
        resp->setBody(response.toStyledString());
        callback(resp);
    });
    
    app().registerHandler("/api/user/profile", [](const HttpRequestPtr &req,
                                                 std::function<void (const HttpResponsePtr &)> &&callback) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k200OK);
        resp->setContentTypeCode(CT_APPLICATION_JSON);
        
        // 从请求头获取token进行验证
        auto token = req->getHeader("Authorization");
        
        // 从token中提取用户ID（实际应用中应该解析JWT或其他token格式）
        std::string userId = "1"; // 默认值
        if (!token.empty()) {
            // 简单示例：假设token格式为 "login_token_username" 或 "register_token_username"
            // 实际应用中应该使用更安全的方式验证token并获取用户ID
        }
        
        // 获取用户信息
        UserMethods userMethods;
        Json::Value userInfo = userMethods.getUserById(userId);
        
        Json::Value response;
        if (userInfo["status"].asString() == "success") {
            response["status"] = "success";
            response["user"] = userInfo["user"];
            response["user"]["email"] = userInfo["user"]["user_name"].asString() + "@example.com";
        } else {
            response["status"] = "error";
            response["message"] = "获取用户信息失败";
        }
        
        resp->setBody(response.toStyledString());
        callback(resp);
    });

    // Add submission endpoints
    app().registerHandler("/api/submit", [](const HttpRequestPtr &req,
                                           std::function<void (const HttpResponsePtr &)> &&callback) {
        SubmissionController::submitCode(req, std::move(callback));
    });
    
    app().registerHandler("/api/submit/history", [](const HttpRequestPtr &req,
                                                   std::function<void (const HttpResponsePtr &)> &&callback) {
        SubmissionController::getUserSubmissions(req, std::move(callback));
    });
    
    app().registerHandler("/api/submit/statistics", [](const HttpRequestPtr &req,
                                                      std::function<void (const HttpResponsePtr &)> &&callback) {
        SubmissionController::getSubmissionStatistics(req, std::move(callback));
    });

    // AI聊天接口
    app().registerHandler("/api/ai/chat", [](const HttpRequestPtr &req,
                                             std::function<void (const HttpResponsePtr &)> &&callback) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k200OK);
        resp->setContentTypeCode(CT_APPLICATION_JSON);
        
        // 解析请求体
        std::string body = std::string(req->getBody());
        Json::Value requestJson;
        Json::CharReaderBuilder builder;
        std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        std::string errors;
        
        bool parsingSuccessful = reader->parse(
            body.c_str(),
            body.c_str() + body.size(),
            &requestJson,
            &errors
        );
        
        Json::Value response;
        
        if (!parsingSuccessful) {
            response["status"] = "error";
            response["message"] = "请求格式错误: " + errors;
            resp->setStatusCode(k400BadRequest);
        } else {
            // 获取请求参数
            std::string userId = requestJson.get("user_id", "").asString();
            std::string questionId = requestJson.get("question_id", "").asString();
            std::string message = requestJson.get("message", "").asString();
            Json::Value problemInfo = requestJson.get("problem_info", Json::Value());
            
            // 使用AI聊天服务处理请求
            AIChatService aiService;
            Json::Value aiResponse = aiService.processChatRequest(userId, questionId, message, problemInfo);
            
            if (aiResponse["status"].asString() == "success") {
                response = aiResponse;
            } else {
                response["status"] = "error";
                response["message"] = aiResponse["message"];
            }
        }
        
        resp->setBody(response.toStyledString());
        callback(resp);
    });

    // Add CORS headers for all responses
    app().registerPostHandlingAdvice([](const HttpRequestPtr &req, const HttpResponsePtr &resp) {
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        resp->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
    });
    
    // Load config file
    app().loadConfigFile("../config.json");
    // app().loadConfigFile("../config.yaml"); // Commented out due to yaml-cpp dependency
    
    // Run HTTP framework,the method will block in the internal event loop
    app().run();
    return 0;
}