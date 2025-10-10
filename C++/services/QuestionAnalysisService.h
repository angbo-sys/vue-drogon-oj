//
// Created by AI Assistant on 25-10-16.
// 题目解析服务头文件
//

#ifndef QUESTIONANALYSISSERVICE_H
#define QUESTIONANALYSISSERVICE_H

#include <drogon/drogon.h>
#include <json/json.h>
#include <fstream>
#include <string>

class QuestionAnalysisService
{
public:
    QuestionAnalysisService();
    
    // 获取题目解析
    Json::Value getAnalysisByQuestionId(int questionId);
    
    // 获取指定解析
    Json::Value getAnalysisById(int analysisId);
    
    // 添加用户解析
    Json::Value addUserAnalysis(int questionId, const std::string& userId, const std::string& analysisContent);
    
    // 更新用户解析
    Json::Value updateUserAnalysis(int analysisId, const std::string& analysisContent);
    
    // 删除用户解析
    Json::Value deleteUserAnalysis(int analysisId, const std::string& userId);
    
    // 获取解析点赞数
    Json::Value getAnalysisLikes(int analysisId);
    
    // 点赞解析
    Json::Value likeAnalysis(int analysisId, const std::string& userId);
    
    // 取消点赞解析
    Json::Value unlikeAnalysis(int analysisId, const std::string& userId);
    
    // 获取热门解析
    Json::Value getPopularAnalyses(int limit = 10);
    
    // 获取用户的解析列表
    Json::Value getUserAnalyses(const std::string& userId);

private:
    // JSON文件操作
    Json::Value loadAnalysisData();
    bool saveAnalysisData(const Json::Value& data);
    Json::Value loadLikeData();
    bool saveLikeData(const Json::Value& data);
    
    // 获取文件路径
    std::string getAnalysisDataPath();
    std::string getLikeDataPath();
    
    // 生成唯一ID
    int generateUniqueId();
};

#endif // QUESTIONANALYSISSERVICE_H