//
// Created by xiaohua jie on 25-9-12.
//

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "QuestionType.h"

#ifndef QUESTION_H
#define QUESTION_H

class Question {
    public:
    // 构造函数
    Question() {
        question_id = 0;
        type_id = 0;
        question_name = "";
        question_content = "";
        answer_content = "";
        difficulty_id = 0;
        source = "";
        path = "";
        created_at = 0;
        updated_at = 0;
        std::cout << "Error: No question information provided" << std::endl;
    }
    
    Question(int t_question_id, int t_type_id, const std::string& t_question_name, 
             const std::string& t_question_content, const std::string& t_answer_content,
             int t_difficulty_id = 0, const std::string& t_source = "", const std::string& t_path = "") {
        question_id = t_question_id;
        type_id = t_type_id;
        question_name = t_question_name;
        question_content = t_question_content;
        answer_content = t_answer_content;
        difficulty_id = t_difficulty_id;
        source = t_source;
        path = t_path;
        created_at = time(nullptr);
        updated_at = time(nullptr);
        std::cout << "Question created: " << question_name << std::endl;
    }
    
    // 析构函数
    ~Question() {
        std::cout << "Question destroyed: " << question_name << std::endl;
    }
    
    // 设置问题信息
    void setQuestionInfo(int t_question_id, int t_type_id, const std::string& t_question_name,
                        const std::string& t_question_content, const std::string& t_answer_content,
                        int t_difficulty_id = 0, const std::string& t_source = "", const std::string& t_path = "") {
        question_id = t_question_id;
        type_id = t_type_id;
        question_name = t_question_name;
        question_content = t_question_content;
        answer_content = t_answer_content;
        difficulty_id = t_difficulty_id;
        source = t_source;
        path = t_path;
        if (created_at == 0) {
            created_at = time(nullptr);
        }
        updated_at = time(nullptr);
    }
    
    // 更新问题内容
    void updateQuestionContent(const std::string& t_question_content) {
        question_content = t_question_content;
        updated_at = time(nullptr);
    }
    
    // 更新答案内容
    void updateAnswerContent(const std::string& t_answer_content) {
        answer_content = t_answer_content;
        updated_at = time(nullptr);
    }
    
    // 更新问题名称
    void updateQuestionName(const std::string& t_question_name) {
        question_name = t_question_name;
        updated_at = time(nullptr);
    }
    
    // 更新难度
    void updateDifficulty(int t_difficulty_id) {
        difficulty_id = t_difficulty_id;
        updated_at = time(nullptr);
    }
    
    // 获取问题信息（控制台输出）
    void getQuestionInfo() {
        std::cout << "Question ID: " << question_id << ", Type ID: " << type_id << std::endl;
        std::cout << "Name: " << question_name << std::endl;
        std::cout << "Content: " << question_content << std::endl;
        std::cout << "Answer: " << answer_content << std::endl;
        std::cout << "Difficulty: " << difficulty_id << ", Source: " << source << std::endl;
        std::cout << "Path: " << path << std::endl;
        std::cout << "Created: " << created_at << ", Updated: " << updated_at << std::endl;
    }
    
    // 获取问题信息（返回字符串）
    std::string getQuestionInfoString() {
        return "Question ID: " + std::to_string(question_id) + 
               ", Type ID: " + std::to_string(type_id) +
               ", Name: " + question_name +
               ", Difficulty: " + std::to_string(difficulty_id);
    }
    
    // 获取问题摘要（用于列表显示）
    std::string getQuestionSummary() {
        return "[" + std::to_string(question_id) + "] " + question_name + 
               " (Type: " + std::to_string(type_id) + ", Difficulty: " + std::to_string(difficulty_id) + ")";
    }
    
    // 验证答案
    bool verifyAnswer(const std::string& t_user_answer) {
        return answer_content == t_user_answer;
    }
    
    // 获取难度名称
    std::string getDifficultyName() {
        switch(difficulty_id) {
            case 0: return "入门";
            case 1: return "简单";
            case 2: return "中等";
            case 3: return "困难";
            default: return "未知";
        }
    }
    
    // Getter方法
    int getQuestionId() const { return question_id; }
    int getTypeId() const { return type_id; }
    std::string getQuestionName() const { return question_name; }
    std::string getQuestionContent() const { return question_content; }
    std::string getAnswerContent() const { return answer_content; }
    int getDifficultyId() const { return difficulty_id; }
    std::string getSource() const { return source; }
    std::string getPath() const { return path; }
    time_t getCreatedAt() const { return created_at; }
    time_t getUpdatedAt() const { return updated_at; }
    
    // Setter方法
    void setQuestionId(int t_question_id) { question_id = t_question_id; }
    void setTypeId(int t_type_id) { type_id = t_type_id; }
    void setQuestionName(const std::string& t_question_name) { question_name = t_question_name; }
    void setQuestionContent(const std::string& t_question_content) { question_content = t_question_content; }
    void setAnswerContent(const std::string& t_answer_content) { answer_content = t_answer_content; }
    void setDifficultyId(int t_difficulty_id) { difficulty_id = t_difficulty_id; }
    void setSource(const std::string& t_source) { source = t_source; }
    void setPath(const std::string& t_path) { path = t_path; }
    
    private:
    int question_id;
    int type_id;
    std::string question_name;
    std::string question_content;
    std::string answer_content;
    int difficulty_id;
    std::string source;
    std::string path;
    time_t created_at;
    time_t updated_at;
};

#endif //QUESTION_H
