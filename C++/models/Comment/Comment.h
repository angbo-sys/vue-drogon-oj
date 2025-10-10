//
// Created by AI Assistant on 25-10-16.
// 评论模型头文件
//

#ifndef COMMENT_H
#define COMMENT_H

#include <string>
#include <json/json.h>

class Comment {
public:
    // 构造函数
    Comment();
    Comment(int id, int problem_id, const std::string& user_id, const std::string& content, 
            const std::string& username, const std::string& created_at);
    
    // Getter方法
    int getId() const;
    int getProblemId() const;
    std::string getUserId() const;
    std::string getContent() const;
    std::string getUsername() const;
    std::string getCreatedAt() const;
    
    // Setter方法
    void setId(int id);
    void setProblemId(int problem_id);
    void setUserId(const std::string& user_id);
    void setContent(const std::string& content);
    void setUsername(const std::string& username);
    void setCreatedAt(const std::string& created_at);
    
    // JSON转换方法
    Json::Value toJson() const;
    static Comment fromJson(const Json::Value& json);
    
private:
    int m_id;
    int m_problem_id;
    std::string m_user_id;
    std::string m_content;
    std::string m_username;
    std::string m_created_at;
};

#endif // COMMENT_H
