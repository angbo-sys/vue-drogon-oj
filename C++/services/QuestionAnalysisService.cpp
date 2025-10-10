//
// Created by AI Assistant on 25-10-16.
// 题目解析服务实现
//

#include "QuestionAnalysisService.h"
#include <drogon/drogon.h>
#include <json/json.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <random>
#include <algorithm>
#include <iomanip>
#include <ctime>

using namespace drogon;

QuestionAnalysisService::QuestionAnalysisService() {
    // 构造函数
}

// 加载解析数据
Json::Value QuestionAnalysisService::loadAnalysisData() {
    Json::Value data;
    std::ifstream file(getAnalysisDataPath());
    
    if (file.is_open()) {
        Json::Reader reader;
        if (!reader.parse(file, data)) {
            std::cerr << "Error parsing analysis JSON: " << reader.getFormattedErrorMessages() << std::endl;
            // 返回默认结构
            data["analysisNumber"] = 0;
            data["analyses"] = Json::arrayValue;
        }
        file.close();
    } else {
        // 文件不存在，创建默认结构
        data["analysisNumber"] = 0;
        data["analyses"] = Json::arrayValue;
    }
    
    return data;
}

// 保存解析数据
bool QuestionAnalysisService::saveAnalysisData(const Json::Value& data) {
    std::ofstream file(getAnalysisDataPath());
    if (file.is_open()) {
        Json::StreamWriterBuilder builder;
        builder["indentation"] = "    ";
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(data, &file);
        file.close();
        return true;
    }
    return false;
}

// 加载点赞数据
Json::Value QuestionAnalysisService::loadLikeData() {
    Json::Value data;
    std::ifstream file(getLikeDataPath());
    
    if (file.is_open()) {
        Json::Reader reader;
        if (!reader.parse(file, data)) {
            std::cerr << "Error parsing like JSON: " << reader.getFormattedErrorMessages() << std::endl;
            // 返回默认结构
            data["likeNumber"] = 0;
            data["likes"] = Json::arrayValue;
        }
        file.close();
    } else {
        // 文件不存在，创建默认结构
        data["likeNumber"] = 0;
        data["likes"] = Json::arrayValue;
    }
    
    return data;
}

// 保存点赞数据
bool QuestionAnalysisService::saveLikeData(const Json::Value& data) {
    std::ofstream file(getLikeDataPath());
    if (file.is_open()) {
        Json::StreamWriterBuilder builder;
        builder["indentation"] = "    ";
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(data, &file);
        file.close();
        return true;
    }
    return false;
}

// 获取解析数据文件路径
std::string QuestionAnalysisService::getAnalysisDataPath() {
    return "../SQL/title/analysis.json";
}

// 获取点赞数据文件路径
std::string QuestionAnalysisService::getLikeDataPath() {
    return "../SQL/title/analysis_likes.json";
}

// 生成唯一ID
int QuestionAnalysisService::generateUniqueId() {
    // 简单的ID生成策略，实际应用中可以使用更复杂的方法
    return static_cast<int>(time(nullptr)) % 1000000 + rand() % 1000;
}

// 获取题目解析
Json::Value QuestionAnalysisService::getAnalysisByQuestionId(int questionId) {
    Json::Value result;
    Json::Value data = loadAnalysisData();
    
    Json::Value analyses;
    int count = 0;
    
    for (const auto& analysis : data["analyses"]) {
        if (analysis["question_id"].asInt() == questionId) {
            analyses.append(analysis);
            count++;
        }
    }
    
    result["status"] = "success";
    result["analysis_count"] = count;
    result["analyses"] = analyses;
    
    return result;
}

// 获取指定解析
Json::Value QuestionAnalysisService::getAnalysisById(int analysisId) {
    Json::Value result;
    Json::Value data = loadAnalysisData();
    
    for (const auto& analysis : data["analyses"]) {
        if (analysis["analysis_id"].asInt() == analysisId) {
            result["status"] = "success";
            result["analysis"] = analysis;
            
            // 获取点赞数
            Json::Value likeData = loadLikeData();
            int likeCount = 0;
            for (const auto& like : likeData["likes"]) {
                if (like["analysis_id"].asInt() == analysisId) {
                    likeCount++;
                }
            }
            result["analysis"]["like_count"] = likeCount;
            
            return result;
        }
    }
    
    result["status"] = "error";
    result["message"] = "Analysis not found";
    
    return result;
}

// 添加用户解析
Json::Value QuestionAnalysisService::addUserAnalysis(int questionId, const std::string& userId, const std::string& analysisContent) {
    Json::Value result;
    Json::Value data = loadAnalysisData();
    
    // 创建新解析
    Json::Value newAnalysis;
    newAnalysis["analysis_id"] = generateUniqueId();
    newAnalysis["question_id"] = questionId;
    newAnalysis["user_id"] = userId;
    newAnalysis["analysis_content"] = analysisContent;
    newAnalysis["like_count"] = 0;
    newAnalysis["created_at"] = static_cast<long long>(time(nullptr));
    newAnalysis["updated_at"] = static_cast<long long>(time(nullptr));
    
    // 添加到数据中
    data["analyses"].append(newAnalysis);
    data["analysisNumber"] = data["analyses"].size();
    
    if (saveAnalysisData(data)) {
        result["status"] = "success";
        result["analysis_id"] = newAnalysis["analysis_id"];
        result["message"] = "Analysis added successfully";
    } else {
        result["status"] = "error";
        result["message"] = "Failed to save analysis";
    }
    
    return result;
}

// 更新用户解析
Json::Value QuestionAnalysisService::updateUserAnalysis(int analysisId, const std::string& analysisContent) {
    Json::Value result;
    Json::Value data = loadAnalysisData();
    
    bool found = false;
    for (auto& analysis : data["analyses"]) {
        if (analysis["analysis_id"].asInt() == analysisId) {
            analysis["analysis_content"] = analysisContent;
            analysis["updated_at"] = static_cast<long long>(time(nullptr));
            found = true;
            break;
        }
    }
    
    if (!found) {
        result["status"] = "error";
        result["message"] = "Analysis not found";
        return result;
    }
    
    if (saveAnalysisData(data)) {
        result["status"] = "success";
        result["message"] = "Analysis updated successfully";
    } else {
        result["status"] = "error";
        result["message"] = "Failed to save analysis";
    }
    
    return result;
}

// 删除用户解析
Json::Value QuestionAnalysisService::deleteUserAnalysis(int analysisId, const std::string& userId) {
    Json::Value result;
    Json::Value data = loadAnalysisData();
    
    Json::Value newAnalyses;
    bool found = false;
    
    for (const auto& analysis : data["analyses"]) {
        if (analysis["analysis_id"].asInt() == analysisId && analysis["user_id"].asString() == userId) {
            found = true;
            // 不添加到新列表中，相当于删除
        } else {
            newAnalyses.append(analysis);
        }
    }
    
    if (!found) {
        result["status"] = "error";
        result["message"] = "Analysis not found or not owned by user";
        return result;
    }
    
    data["analyses"] = newAnalyses;
    data["analysisNumber"] = newAnalyses.size();
    
    if (saveAnalysisData(data)) {
        result["status"] = "success";
        result["message"] = "Analysis deleted successfully";
    } else {
        result["status"] = "error";
        result["message"] = "Failed to save analysis";
    }
    
    return result;
}

// 获取解析点赞数
Json::Value QuestionAnalysisService::getAnalysisLikes(int analysisId) {
    Json::Value result;
    Json::Value data = loadLikeData();
    
    int likeCount = 0;
    for (const auto& like : data["likes"]) {
        if (like["analysis_id"].asInt() == analysisId) {
            likeCount++;
        }
    }
    
    result["status"] = "success";
    result["analysis_id"] = analysisId;
    result["like_count"] = likeCount;
    
    return result;
}

// 点赞解析
Json::Value QuestionAnalysisService::likeAnalysis(int analysisId, const std::string& userId) {
    Json::Value result;
    Json::Value data = loadLikeData();
    
    // 检查是否已经点赞
    for (const auto& like : data["likes"]) {
        if (like["analysis_id"].asInt() == analysisId && like["user_id"].asString() == userId) {
            result["status"] = "error";
            result["message"] = "You have already liked this analysis";
            return result;
        }
    }
    
    // 添加点赞
    Json::Value newLike;
    newLike["analysis_id"] = analysisId;
    newLike["user_id"] = userId;
    newLike["liked_at"] = static_cast<long long>(time(nullptr));
    
    data["likes"].append(newLike);
    data["likeNumber"] = data["likes"].size();
    
    if (saveLikeData(data)) {
        result["status"] = "success";
        result["message"] = "Analysis liked successfully";
        
        // 更新解析的点赞数
        Json::Value analysisData = loadAnalysisData();
        for (auto& analysis : analysisData["analyses"]) {
            if (analysis["analysis_id"].asInt() == analysisId) {
                analysis["like_count"] = analysis["like_count"].asInt() + 1;
                saveAnalysisData(analysisData);
                break;
            }
        }
    } else {
        result["status"] = "error";
        result["message"] = "Failed to save like";
    }
    
    return result;
}

// 取消点赞解析
Json::Value QuestionAnalysisService::unlikeAnalysis(int analysisId, const std::string& userId) {
    Json::Value result;
    Json::Value data = loadLikeData();
    
    Json::Value newLikes;
    bool found = false;
    
    for (const auto& like : data["likes"]) {
        if (like["analysis_id"].asInt() == analysisId && like["user_id"].asString() == userId) {
            found = true;
            // 不添加到新列表中，相当于删除
        } else {
            newLikes.append(like);
        }
    }
    
    if (!found) {
        result["status"] = "error";
        result["message"] = "Like not found";
        return result;
    }
    
    data["likes"] = newLikes;
    data["likeNumber"] = newLikes.size();
    
    if (saveLikeData(data)) {
        result["status"] = "success";
        result["message"] = "Like removed successfully";
        
        // 更新解析的点赞数
        Json::Value analysisData = loadAnalysisData();
        for (auto& analysis : analysisData["analyses"]) {
            if (analysis["analysis_id"].asInt() == analysisId) {
                analysis["like_count"] = std::max(0, analysis["like_count"].asInt() - 1);
                saveAnalysisData(analysisData);
                break;
            }
        }
    } else {
        result["status"] = "error";
        result["message"] = "Failed to save like";
    }
    
    return result;
}

// 获取热门解析
Json::Value QuestionAnalysisService::getPopularAnalyses(int limit) {
    Json::Value result;
    Json::Value data = loadAnalysisData();
    
    // 复制解析数组以便排序
    std::vector<Json::Value> analyses;
    for (const auto& analysis : data["analyses"]) {
        analyses.push_back(analysis);
    }
    
    // 按点赞数排序（降序）
    std::sort(analyses.begin(), analyses.end(), [](const Json::Value& a, const Json::Value& b) {
        return a["like_count"].asInt() > b["like_count"].asInt();
    });
    
    // 限制返回数量
    Json::Value popularAnalyses;
    int count = 0;
    for (const auto& analysis : analyses) {
        if (count >= limit) break;
        popularAnalyses.append(analysis);
        count++;
    }
    
    result["status"] = "success";
    result["analysis_count"] = count;
    result["analyses"] = popularAnalyses;
    
    return result;
}

// 获取用户的解析列表
Json::Value QuestionAnalysisService::getUserAnalyses(const std::string& userId) {
    Json::Value result;
    Json::Value data = loadAnalysisData();
    
    Json::Value userAnalyses;
    int count = 0;
    
    for (const auto& analysis : data["analyses"]) {
        if (analysis["user_id"].asString() == userId) {
            userAnalyses.append(analysis);
            count++;
        }
    }
    
    // 按创建时间排序（降序）
    std::vector<Json::Value> sortedAnalyses;
    for (const auto& analysis : userAnalyses) {
        sortedAnalyses.push_back(analysis);
    }
    
    std::sort(sortedAnalyses.begin(), sortedAnalyses.end(), [](const Json::Value& a, const Json::Value& b) {
        return a["created_at"].asInt64() > b["created_at"].asInt64();
    });
    
    Json::Value sortedJson;
    for (const auto& analysis : sortedAnalyses) {
        sortedJson.append(analysis);
    }
    
    result["status"] = "success";
    result["analysis_count"] = count;
    result["analyses"] = sortedJson;
    
    return result;
}