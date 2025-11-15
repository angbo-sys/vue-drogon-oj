#ifndef AICHATSERVICE_H
#define AICHATSERVICE_H

#include <string>
#include <json/json.h>
#include <drogon/drogon.h>

class AIChatService {
public:
    // 构造函数
    AIChatService();
    
    // 析构函数
    ~AIChatService();
    
    // 处理AI聊天请求
    Json::Value processChatRequest(const std::string& userId, 
                                  const std::string& questionId,
                                  const std::string& message,
                                  const Json::Value& problemInfo);
    
private:
    // DeepSeek API配置
    std::string deepseekApiUrl;
    std::string deepseekApiKey;
    std::string deepseekModel;
    int maxTokens;
    double temperature;
    int timeout;
    bool useMockResponse;
    
    // 调用DeepSeek API
    std::string callDeepSeekAPI(const std::string& prompt);
    
    // 构建AI提示词
    std::string buildPrompt(const std::string& message, 
                           const Json::Value& problemInfo);
    
    // 处理AI响应
    std::string processAIResponse(const std::string& aiResponse);
    
    // 创建HTTP客户端
    drogon::HttpClientPtr createHttpClient();
    
    // 解析DeepSeek响应
    std::string parseDeepSeekResponse(const std::string& response);
    
    // 生成模拟响应（备用）
    std::string generateMockResponse(const std::string& message, 
                                   const Json::Value& problemInfo);
    
    // 加载配置文件
    bool loadConfig();
};

#endif // AICHATSERVICE_H
