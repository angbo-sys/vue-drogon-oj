//
// Created by AI Assistant on 25-10-16.
// 评论模型实现
//

#include "Comment.h"

Comment::Comment() : m_id(0), m_problem_id(0) {
}

Comment::Comment(int id, int problem_id, const std::string& user_id, const std::string& content, 
                 const std::string& username, const std::string& created_at)
    : m_id(id), m_problem_id(problem_id), m_user_id(user_id), m_content(content), 
      m_username(username), m_created_at(created_at) {
}

// Getter方法
int Comment::getId() const {
    return m_id;
}

int Comment::getProblemId() const {
    return m_problem_id;
}

std::string Comment::getUserId() const {
    return m_user_id;
}

std::string Comment::getContent() const {
    return m_content;
}

std::string Comment::getUsername() const {
    return m_username;
}

std::string Comment::getCreatedAt() const {
    return m_created_at;
}

// Setter方法
void Comment::setId(int id) {
    m_id = id;
}

void Comment::setProblemId(int problem_id) {
    m_problem_id = problem_id;
}

void Comment::setUserId(const std::string& user_id) {
    m_user_id = user_id;
}

void Comment::setContent(const std::string& content) {
    m_content = content;
}

void Comment::setUsername(const std::string& username) {
    m_username = username;
}

void Comment::setCreatedAt(const std::string& created_at) {
    m_created_at = created_at;
}

// JSON转换方法
Json::Value Comment::toJson() const {
    Json::Value json;
    json["id"] = m_id;
    json["problem_id"] = m_problem_id;
    json["user_id"] = m_user_id;
    json["content"] = m_content;
    json["username"] = m_username;
    json["created_at"] = m_created_at;
    return json;
}

Comment Comment::fromJson(const Json::Value& json) {
    Comment comment;
    comment.m_id = json.get("id", 0).asInt();
    comment.m_problem_id = json.get("problem_id", 0).asInt();
    comment.m_user_id = json.get("user_id", "").asString();
    comment.m_content = json.get("content", "").asString();
    comment.m_username = json.get("username", "").asString();
    comment.m_created_at = json.get("created_at", "").asString();
    return comment;
}
