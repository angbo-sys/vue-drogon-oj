//
// Created by AI Assistant on 2024-10-31.
// 评论服务头文件
//

#ifndef COMMENTSERVICE_H
#define COMMENTSERVICE_H

#include "../models/Comment/Comment.h"
#include <drogon/drogon.h>
#include <json/json.h>
#include <string>

class CommentService {
public:
    CommentService();
    
    // 获取题目评论列表
    Json::Value getCommentsByProblemId(int t_problem_id, int t_page = 1, int t_page_size = 10);
    
    // 添加评论
    Json::Value addComment(int t_problem_id, const std::string& t_user_id, const std::string& t_username, 
                          const std::string& t_content);
    
    // 删除评论
    Json::Value deleteComment(int t_comment_id, const std::string& t_user_id);
    
    // 获取评论总数
    Json::Value getCommentCount(int t_problem_id);
    
    // 获取单个评论详情
    Json::Value getCommentById(int t_comment_id);
    
private:
    // JSON文件操作
    Json::Value loadCommentData();
    bool saveCommentData(const Json::Value& t_data);
    
    // 文件路径
    std::string getCommentDataPath();
    
    // 辅助方法
    int generateCommentId();
    bool commentExists(int t_comment_id);
    
    // 数据转换
    Json::Value commentToJson(const Comment& t_comment);
    Comment jsonToComment(const Json::Value& t_json);
    
    // 获取当前时间
    std::string getCurrentTime();
};

#endif // COMMENTSERVICE_H