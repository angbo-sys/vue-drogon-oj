#include "AIChatService.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <drogon/drogon.h>

AIChatService::AIChatService() {
    // 初始化默认配置
    deepseekApiUrl = "https://api.deepseek.com";
    deepseekApiKey = "自行补全";
    deepseekModel = "deepseek-chat";
    maxTokens = 1000;
    temperature = 0.7;
    timeout = 30;
    useMockResponse = true;
    
    // 尝试加载配置文件
    loadConfig();
}

AIChatService::~AIChatService() {
    // 清理资源
}

Json::Value AIChatService::processChatRequest(const std::string& userId, 
                                             const std::string& questionId,
                                             const std::string& message,
                                             const Json::Value& problemInfo) {
    Json::Value response;
    
    try {
        // 构建AI提示词
        std::string prompt = buildPrompt(message, problemInfo);
        
        // 调用DeepSeek API
        std::cout << "开始调用DeepSeek API..." << std::endl;
        std::string aiResponse = callDeepSeekAPI(prompt);
        
        if (!aiResponse.empty()) {
            std::cout << "DeepSeek API调用成功，响应长度: " << aiResponse.length() << std::endl;
            response["status"] = "success";
            response["data"]["response"] = aiResponse;
            response["data"]["timestamp"] = std::to_string(time(nullptr));
        } else {
            std::cout << "DeepSeek API调用失败，使用模拟响应" << std::endl;
            // 如果API调用失败，使用模拟响应
            std::string mockResponse = generateMockResponse(message, problemInfo);
            response["status"] = "success";
            response["data"]["response"] = mockResponse;
            response["data"]["timestamp"] = std::to_string(time(nullptr));
            response["data"]["note"] = "使用模拟响应";
        }
        
    } catch (const std::exception& e) {
        response["status"] = "error";
        response["message"] = "AI服务处理失败: " + std::string(e.what());
    }
    
    return response;
}

std::string AIChatService::callDeepSeekAPI(const std::string& prompt) {
    try {
        // 创建HTTP客户端
        auto client = createHttpClient();
        
        // 构建请求体 - 仿照example.txt的方式
        Json::Value requestBody;
        requestBody["model"] = deepseekModel;
        requestBody["messages"] = Json::Value(Json::arrayValue);
        
        // 添加系统消息（可选）
        Json::Value systemMessage;
        systemMessage["role"] = "system";
        systemMessage["content"] = "你是一个专业的算法题目助手，专门帮助用户理解算法题目、提供解题思路和代码实现建议。请用中文回答，回答要专业、准确、易懂。";
        requestBody["messages"].append(systemMessage);
        
        // 添加用户消息
        Json::Value userMessage;
        userMessage["role"] = "user";
        userMessage["content"] = prompt;
        requestBody["messages"].append(userMessage);
        
        // 设置参数
        requestBody["temperature"] = temperature;
        requestBody["max_tokens"] = maxTokens;
        
        std::cout << "发送请求到DeepSeek API..." << std::endl;
        std::cout << "请求体: " << requestBody.toStyledString() << std::endl;
        
        // 创建HTTP请求
        auto req = drogon::HttpRequest::newHttpJsonRequest(requestBody);
        req->setMethod(drogon::Post);
        req->setPath("/chat/completions");
        req->addHeader("Authorization", "Bearer " + deepseekApiKey);
        req->addHeader("Content-Type", "application/json");
        
        // 发送请求（同步方式）
        auto resp = client->sendRequest(req);
        
        if (resp.first == drogon::ReqResult::Ok && resp.second && resp.second->getStatusCode() == drogon::k200OK) {
            std::string responseBody = std::string(resp.second->getBody());
            std::cout << "DeepSeek API响应: " << responseBody << std::endl;
            return parseDeepSeekResponse(responseBody);
        } else {
            std::cerr << "DeepSeek API调用失败，状态码: " 
                      << (resp.second ? resp.second->getStatusCode() : -1) << std::endl;
            if (resp.second) {
                std::cerr << "响应体: " << std::string(resp.second->getBody()) << std::endl;
            }
            return "";
        }
        
    } catch (const std::exception& e) {
        std::cerr << "DeepSeek API调用异常: " << e.what() << std::endl;
        return "";
    }
}

drogon::HttpClientPtr AIChatService::createHttpClient() {
    auto client = drogon::HttpClient::newHttpClient(deepseekApiUrl);
    // Drogon HttpClient 默认超时时间足够，不需要手动设置
    return client;
}

std::string AIChatService::parseDeepSeekResponse(const std::string& response) {
    try {
        Json::Value jsonResponse;
        Json::CharReaderBuilder builder;
        std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        std::string errors;
        
        bool parsingSuccessful = reader->parse(
            response.c_str(),
            response.c_str() + response.size(),
            &jsonResponse,
            &errors
        );
        
        if (!parsingSuccessful) {
            std::cerr << "解析DeepSeek响应失败: " << errors << std::endl;
            return "";
        }
        
        // 检查是否有错误
        if (jsonResponse.isMember("error")) {
            std::cerr << "DeepSeek API错误: " << jsonResponse["error"]["message"].asString() << std::endl;
            return "";
        }
        
        // 仿照example.txt的方式提取回复内容
        // response.json()['choices'][0]['message']['content']
        if (jsonResponse.isMember("choices") && jsonResponse["choices"].isArray() && 
            !jsonResponse["choices"].empty()) {
            auto choice = jsonResponse["choices"][0];
            if (choice.isMember("message") && choice["message"].isMember("content")) {
                std::string content = choice["message"]["content"].asString();
                std::cout << "成功提取AI回复内容，长度: " << content.length() << std::endl;
                return content;
            }
        }
        
        std::cerr << "DeepSeek响应格式异常，无法找到choices[0].message.content" << std::endl;
        std::cerr << "响应内容: " << response << std::endl;
        return "";
        
    } catch (const std::exception& e) {
        std::cerr << "解析DeepSeek响应异常: " << e.what() << std::endl;
        return "";
    }
}

std::string AIChatService::buildPrompt(const std::string& message, 
                                     const Json::Value& problemInfo) {
    std::ostringstream prompt;
    
    prompt << "请根据以下题目信息回答用户的问题：\n\n";
    
    // 添加题目信息
    if (problemInfo.isObject()) {
        if (problemInfo.isMember("name") && !problemInfo["name"].asString().empty()) {
            prompt << "题目名称: " << problemInfo["name"].asString() << "\n";
        }
        
        if (problemInfo.isMember("description") && !problemInfo["description"].asString().empty()) {
            prompt << "题目描述: " << problemInfo["description"].asString() << "\n";
        }
        
        if (problemInfo.isMember("difficulty") && !problemInfo["difficulty"].asString().empty()) {
            prompt << "难度等级: " << problemInfo["difficulty"].asString() << "\n";
        }
        
        if (problemInfo.isMember("tags") && problemInfo["tags"].isArray()) {
            prompt << "题目标签: ";
            for (const auto& tag : problemInfo["tags"]) {
                prompt << tag.asString() << " ";
            }
            prompt << "\n";
        }
    }
    
    prompt << "\n用户问题: " << message << "\n\n";
    prompt << "请提供专业、准确的回答，帮助用户理解题目和解题思路。";
    
    return prompt.str();
}

std::string AIChatService::processAIResponse(const std::string& aiResponse) {
    // 简单的响应处理，可以在这里添加格式化、过滤等逻辑
    return aiResponse;
}

bool AIChatService::loadConfig() {
    try {
        std::ifstream configFile("../config/ai_config.json");
        if (!configFile.is_open()) {
            std::cerr << "无法打开AI配置文件，使用默认配置" << std::endl;
            return false;
        }
        
        Json::Value config;
        Json::CharReaderBuilder builder;
        std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
        std::string errors;
        
        std::string configContent((std::istreambuf_iterator<char>(configFile)),
                                 std::istreambuf_iterator<char>());
        configFile.close();
        
        bool parsingSuccessful = reader->parse(
            configContent.c_str(),
            configContent.c_str() + configContent.size(),
            &config,
            &errors
        );
        
        if (!parsingSuccessful) {
            std::cerr << "解析AI配置文件失败: " << errors << std::endl;
            return false;
        }
        
        // 加载DeepSeek配置
        if (config.isMember("deepseek")) {
            auto deepseekConfig = config["deepseek"];
            if (deepseekConfig.isMember("api_url")) {
                deepseekApiUrl = deepseekConfig["api_url"].asString();
            }
            if (deepseekConfig.isMember("api_key")) {
                deepseekApiKey = deepseekConfig["api_key"].asString();
            }
            if (deepseekConfig.isMember("model")) {
                deepseekModel = deepseekConfig["model"].asString();
            }
            if (deepseekConfig.isMember("max_tokens")) {
                maxTokens = deepseekConfig["max_tokens"].asInt();
            }
            if (deepseekConfig.isMember("temperature")) {
                temperature = deepseekConfig["temperature"].asDouble();
            }
            if (deepseekConfig.isMember("timeout")) {
                timeout = deepseekConfig["timeout"].asInt();
            }
        }
        
        // 加载备用配置
        if (config.isMember("fallback")) {
            auto fallbackConfig = config["fallback"];
            if (fallbackConfig.isMember("use_mock_response")) {
                useMockResponse = fallbackConfig["use_mock_response"].asBool();
            }
        }
        
        std::cout << "AI配置加载成功" << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "加载AI配置异常: " << e.what() << std::endl;
        return false;
    }
}

std::string AIChatService::generateMockResponse(const std::string& message, 
                                               const Json::Value& problemInfo) {
    std::ostringstream response;
    
    std::string problemName = "这道题";
    if (problemInfo.isObject() && problemInfo.isMember("name")) {
        problemName = problemInfo["name"].asString();
    }
    
    // 根据用户问题生成不同的模拟响应
    if (message.find("理解") != std::string::npos || message.find("题目") != std::string::npos) {
        response << "关于题目\"" << problemName << "\"，我来帮你分析一下：\n\n";
        response << "1. 首先需要仔细阅读题目描述，理解题目要求\n";
        response << "2. 分析输入输出格式和约束条件\n";
        response << "3. 考虑可能的算法思路\n";
        response << "4. 注意边界情况的处理\n\n";
        response << "建议你先尝试理解题目的核心要求，然后选择合适的算法来解决。";
    } else if (message.find("算法") != std::string::npos || message.find("思路") != std::string::npos) {
        response << "针对\"" << problemName << "\"的解题思路：\n\n";
        response << "1. **分析问题**：理解题目要求，确定输入输出\n";
        response << "2. **选择算法**：根据问题特点选择合适的算法\n";
        response << "3. **实现代码**：编写清晰的代码实现\n";
        response << "4. **测试验证**：确保代码能处理各种情况\n\n";
        response << "你可以告诉我具体遇到了什么困难，我会提供更详细的指导。";
    } else if (message.find("代码") != std::string::npos || message.find("实现") != std::string::npos) {
        response << "关于\"" << problemName << "\"的代码实现建议：\n\n";
        response << "1. **数据结构选择**：根据题目特点选择合适的数据结构\n";
        response << "2. **算法复杂度**：考虑时间和空间复杂度\n";
        response << "3. **边界处理**：注意特殊情况和小数据\n";
        response << "4. **代码规范**：保持代码清晰易读\n\n";
        response << "如果你有具体的代码问题，可以分享给我看看。";
    } else {
        response << "你好！我是AI题目助手，专门帮助解答算法题目相关问题。\n\n";
        response << "关于题目\"" << problemName << "\"，你可以问我：\n";
        response << "- 题目理解和分析\n";
        response << "- 解题思路和算法选择\n";
        response << "- 代码实现建议\n";
        response << "- 复杂度分析\n\n";
        response << "请告诉我你具体想了解什么，我会提供详细的帮助！";
    }
    
    return response.str();
}
