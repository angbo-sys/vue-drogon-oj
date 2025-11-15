#include <drogon/drogon.h>
#include "controllers/QuestionController.h"
#include "controllers/SubmissionController.h"
#include "controllers/CommentController.h"
#include "controllers/QuestionAnalysisController.h"
#include "services/GetUserInfo.h"
#include "services/AIChatService.h"
#include <iostream>
#include <ctime>
#include <iomanip>
using namespace drogon;

int main() {
    // 打印启动信息
    std::cout << "===========================================" << std::endl;
    std::cout << "🚀 DrogonOJ Backend Server Starting..." << std::endl;
    std::cout << "===========================================" << std::endl;

    // 获取当前时间
    auto now = std::time(nullptr);
    auto tm = *std::localtime(&now);
    std::cout << "📅 Start Time: " << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << std::endl;

    //Set HTTP listener address and port
    app().addListener("0.0.0.0", 5555);
    std::cout << "🌐 Server listening on: http://0.0.0.0:5555" << std::endl;

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

    // Add logout endpoint
    app().registerHandler("/api/auth/logout", [](const HttpRequestPtr &req,
                                                std::function<void (const HttpResponsePtr &)> &&callback) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k200OK);
        resp->setContentTypeCode(CT_APPLICATION_JSON);

        // 从请求头获取token进行验证
        auto token = req->getHeader("Authorization");

        Json::Value response;

        // 简单的登出逻辑 - 在实际应用中应该使token失效
        if (!token.empty()) {
            response["status"] = "success";
            response["message"] = "登出成功";
        } else {
            response["status"] = "success";
            response["message"] = "登出成功";
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

    // 评论相关接口
    app().registerHandler("/api/comments/{problem_id}", [](const HttpRequestPtr &req,
                                                           std::function<void (const HttpResponsePtr &)> &&callback) {
        CommentController::getProblemComments(req, std::move(callback));
    });

    app().registerHandler("/api/comments/create", [](const HttpRequestPtr &req,
                                                     std::function<void (const HttpResponsePtr &)> &&callback) {
        CommentController::createComment(req, std::move(callback));
    });

    app().registerHandler("/api/comments/delete", [](const HttpRequestPtr &req,
                                                     std::function<void (const HttpResponsePtr &)> &&callback) {
        CommentController::deleteComment(req, std::move(callback));
    });

    app().registerHandler("/api/comments/count/{problem_id}", [](const HttpRequestPtr &req,
                                                                 std::function<void (const HttpResponsePtr &)> &&callback) {
        CommentController::getCommentCount(req, std::move(callback));
    });

    app().registerHandler("/api/comments/detail", [](const HttpRequestPtr &req,
                                                     std::function<void (const HttpResponsePtr &)> &&callback) {
        CommentController::getCommentById(req, std::move(callback));
    });

    // 题解分析相关接口
    app().registerHandler("/api/analysis/{problem_id}", [](const HttpRequestPtr &req,
                                                           std::function<void (const HttpResponsePtr &)> &&callback) {
        QuestionAnalysisController::getQuestionAnalysis(req, std::move(callback));
    });

    app().registerHandler("/api/analysis/submit", [](const HttpRequestPtr &req,
                                                     std::function<void (const HttpResponsePtr &)> &&callback) {
        QuestionAnalysisController::submitUserAnalysis(req, std::move(callback));
    });

    app().registerHandler("/api/analysis/likes/{analysis_id}", [](const HttpRequestPtr &req,
                                                                  std::function<void (const HttpResponsePtr &)> &&callback) {
        QuestionAnalysisController::getAnalysisLikes(req, std::move(callback));
    });

    app().registerHandler("/api/analysis/like", [](const HttpRequestPtr &req,
                                                   std::function<void (const HttpResponsePtr &)> &&callback) {
        QuestionAnalysisController::likeAnalysis(req, std::move(callback));
    });

    app().registerHandler("/api/analysis/popular", [](const HttpRequestPtr &req,
                                                      std::function<void (const HttpResponsePtr &)> &&callback) {
        QuestionAnalysisController::getPopularAnalyses(req, std::move(callback));
    });

    // Add CORS headers for all responses
    app().registerPostHandlingAdvice([](const HttpRequestPtr &req, const HttpResponsePtr &resp) {
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        resp->addHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
    });

    // 打印API端点信息
    std::cout << "\n📋 Available API Endpoints:" << std::endl;
    std::cout << "├── GET  /                           - Server status" << std::endl;
    std::cout << "├── POST /test                        - Test endpoint" << std::endl;
    std::cout << "├── GET  /api/problems                - Get problem list" << std::endl;
    std::cout << "├── GET  /api/problems/{id}           - Get problem detail" << std::endl;
    std::cout << "├── GET  /api/problems/types          - Get problem types" << std::endl;
    std::cout << "├── GET  /api/problems/search         - Search problems" << std::endl;
    std::cout << "├── GET  /api/problems/statistics     - Get problem statistics" << std::endl;
    std::cout << "├── POST /api/auth/login              - User login" << std::endl;
    std::cout << "├── POST /api/auth/register           - User registration" << std::endl;
    std::cout << "├── POST /api/auth/logout             - User logout" << std::endl;
    std::cout << "├── GET  /api/user/profile            - Get user profile" << std::endl;
    std::cout << "├── POST /api/submit                  - Submit code" << std::endl;
    std::cout << "├── GET  /api/submit/history          - Get submission history" << std::endl;
    std::cout << "├── GET  /api/submit/statistics       - Get submission statistics" << std::endl;
    std::cout << "├── POST /api/ai/chat                 - AI chat" << std::endl;
    std::cout << "├── GET  /api/comments/{problem_id}   - Get comments" << std::endl;
    std::cout << "├── POST /api/comments/create         - Create comment" << std::endl;
    std::cout << "├── POST /api/comments/delete         - Delete comment" << std::endl;
    std::cout << "├── GET  /api/comments/count/{id}     - Get comment count" << std::endl;
    std::cout << "├── GET  /api/comments/detail         - Get comment detail" << std::endl;
    std::cout << "├── GET  /api/analysis/{problem_id}   - Get analysis" << std::endl;
    std::cout << "├── POST /api/analysis/submit         - Submit analysis" << std::endl;
    std::cout << "├── GET  /api/analysis/likes/{id}    - Get analysis likes" << std::endl;
    std::cout << "├── POST /api/analysis/like           - Like analysis" << std::endl;
    std::cout << "└── GET  /api/analysis/popular       - Get popular analyses" << std::endl;

    std::cout << "\n🔧 Configuration:" << std::endl;
    std::cout << "├── CORS enabled for all origins" << std::endl;
    std::cout << "├── JSON request/response support" << std::endl;
    std::cout << "└── Multi-threaded request handling" << std::endl;

    // Load config file
    app().loadConfigFile("../config.json");
    // app().loadConfigFile("../config.yaml"); // Commented out due to yaml-cpp dependency

    std::cout << "\n✅ Server configuration loaded successfully!" << std::endl;
    std::cout << "🚀 Starting HTTP server..." << std::endl;
    std::cout << "===========================================" << std::endl;
    std::cout << "🎉 DrogonOJ Backend Server is now running!" << std::endl;
    std::cout << "📱 Frontend should connect to: http://localhost:5555" << std::endl;
    std::cout << "===========================================" << std::endl;

    // Run HTTP framework,the method will block in the internal event loop
    app().run();
    return 0;
}
