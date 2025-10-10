//
// Created by AI Assistant on 2024-10-31.
// 评论服务实现
//

#include "CommentService.h"
#include <drogon/drogon.h>
#include <json/json.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <random>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <ctime>

using namespace drogon;

CommentService::CommentService() {
    // 构造函数
}

// 加载评论数据
Json::Value CommentService::loadCommentData() {
    Json::Value data;
    std::ifstream file(getCommentDataPath());
    
    if (file.is_open()) {
        Json::Reader reader;
        if (!reader.parse(file, data)) {
            std::cerr << "Error parsing comment JSON: " << reader.getFormattedErrorMessages() << std::endl;
            // 返回默认结构
            data["commentNumber"] = 0;
            data["comments"] = Json::arrayValue;
        }
        file.close();
    } else {
        // 文件不存在，创建默认结构
        data["commentNumber"] = 0;
        data["comments"] = Json::arrayValue;
    }
    
    return data;
}

// 保存评论数据
bool CommentService::saveCommentData(const Json::Value& t_data) {
    std::ofstream file(getCommentDataPath());
    if (file.is_open()) {
        Json::StreamWriterBuilder builder;
        builder["indentation"] = "    ";
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(t_data, &file);
        file.close();
        return true;
    }
    return false;
}

// 获取评论数据文件路径
std::string CommentService::getCommentDataPath() {
    return "../SQL/title/comments.json";
}

// 生成评论ID
int CommentService::generateCommentId() {
    Json::Value data = loadCommentData();
    int maxId = 0;
    
    for (const auto& commentJson : data["comments"]) {
        int id = commentJson["id"].asInt();
        if (id > maxId) {
            maxId = id;
        }
    }
    
    return maxId + 1;
}

// 检查评论是否存在
bool CommentService::commentExists(int t_comment_id) {
    Json::Value data = loadCommentData();
    
    for (const auto& commentJson : data["comments"]) {
        if (commentJson["id"].asInt() == t_comment_id) {
            return true;
        }
    }
    
    return false;
}

// 转换Comment对象为JSON
Json::Value CommentService::commentToJson(const Comment& t_comment) {
    return t_comment.toJson();
}

// 转换JSON为Comment对象
Comment CommentService::jsonToComment(const Json::Value& t_json) {
    return Comment::fromJson(t_json);
}

// 获取当前时间
std::string CommentService::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
    
#ifdef _WIN32
    localtime_s(&tm_now, &now_time);
#else
    localtime_r(&now_time, &tm_now);
#endif
    
    std::stringstream ss;
    ss << std::put_time(&tm_now, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// 获取题目评论列表
Json::Value CommentService::getCommentsByProblemId(int t_problem_id, int t_page, int t_page_size) {
    Json::Value data = loadCommentData();
    Json::Value result;
    Json::Value commentsArray = Json::arrayValue;
    
    // 筛选特定题目的评论
    std::vector<Json::Value> problemComments;
    for (const auto& commentJson : data["comments"]) {
        if (commentJson["problem_id"].asInt() == t_problem_id) {
            problemComments.push_back(commentJson);
        }
    }
    
    // 按时间倒序排序
    std::sort(problemComments.begin(), problemComments.end(), [](const Json::Value& a, const Json::Value& b) {
        return a["create_time"].asString() > b["create_time"].asString();
    });
    
    // 分页
    int totalComments = problemComments.size();
    int startIndex = (t_page - 1) * t_page_size;
    int endIndex = std::min(startIndex + t_page_size, totalComments);
    
    for (int i = startIndex; i < endIndex; ++i) {
        commentsArray.append(problemComments[i]);
    }
    
    result["status"] = "success";
    result["total_comments"] = totalComments;
    result["total_pages"] = (totalComments + t_page_size - 1) / t_page_size;
    result["current_page"] = t_page;
    result["page_size"] = t_page_size;
    result["comments"] = commentsArray;
    
    return result;
}

// 添加评论
Json::Value CommentService::addComment(int t_problem_id, const std::string& t_user_id, const std::string& t_username, 
                                      const std::string& t_content) {
    Json::Value data = loadCommentData();
    
    // 创建新评论
    int newId = generateCommentId();
    std::string currentTime = getCurrentTime();
    
    Comment newComment(newId, t_problem_id, t_user_id, t_content, t_username, currentTime);
    Json::Value commentJson = newComment.toJson();
    
    // 添加到评论数组
    data["comments"].append(commentJson);
    data["commentNumber"] = data["comments"].size();
    
    // 保存数据
    if (saveCommentData(data)) {
        Json::Value result;
        result["status"] = "success";
        result["message"] = "Comment added successfully";
        result["comment"] = commentJson;
        return result;
    } else {
        Json::Value result;
        result["status"] = "error";
        result["message"] = "Failed to save comment";
        return result;
    }
}

// 删除评论
Json::Value CommentService::deleteComment(int t_comment_id, const std::string& t_user_id) {
    Json::Value data = loadCommentData();
    Json::Value newComments = Json::arrayValue;
    
    bool found = false;
    bool isOwner = false;
    
    for (const auto& commentJson : data["comments"]) {
        if (commentJson["id"].asInt() == t_comment_id) {
            found = true;
            if (commentJson["user_id"].asString() == t_user_id) {
                isOwner = true;
                // 不添加到新数组，即删除
            } else {
                newComments.append(commentJson);
            }
        } else {
            newComments.append(commentJson);
        }
    }
    
    if (!found) {
        Json::Value result;
        result["status"] = "error";
        result["message"] = "Comment not found";
        return result;
    }
    
    if (!isOwner) {
        Json::Value result;
        result["status"] = "error";
        result["message"] = "You are not allowed to delete this comment";
        return result;
    }
    
    data["comments"] = newComments;
    data["commentNumber"] = data["comments"].size();
    
    if (saveCommentData(data)) {
        Json::Value result;
        result["status"] = "success";
        result["message"] = "Comment deleted successfully";
        return result;
    } else {
        Json::Value result;
        result["status"] = "error";
        result["message"] = "Failed to save changes";
        return result;
    }
}

// 获取评论总数
Json::Value CommentService::getCommentCount(int t_problem_id) {
    Json::Value data = loadCommentData();
    int count = 0;
    
    for (const auto& commentJson : data["comments"]) {
        if (commentJson["problem_id"].asInt() == t_problem_id) {
            ++count;
        }
    }
    
    Json::Value result;
    result["status"] = "success";
    result["count"] = count;
    return result;
}

// 获取单个评论详情
Json::Value CommentService::getCommentById(int t_comment_id) {
    Json::Value data = loadCommentData();
    
    for (const auto& commentJson : data["comments"]) {
        if (commentJson["id"].asInt() == t_comment_id) {
            Json::Value result;
            result["status"] = "success";
            result["comment"] = commentJson;
            return result;
        }
    }
    
    Json::Value result;
    result["status"] = "error";
    result["message"] = "Comment not found";
    return result;
}