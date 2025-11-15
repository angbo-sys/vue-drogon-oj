//
// Created by xiaohua jie on 25-9-12.
//
#include <iostream>
#include <string>
#include <vector>

#ifndef QUESTIONTYPE_H
#define QUESTIONTYPE_H

class QuestionType {
    public:
    // 构造函数
    QuestionType() {
        type_id = 0;
        type_name = "";
        question_count = 0;
        std::cout << "Error: No type information provided" << std::endl;
    }
    
    QuestionType(int t_type_id, const std::string& t_type_name) {
        type_id = t_type_id;
        type_name = t_type_name;
        question_count = 0;
        std::cout << "QuestionType created: " << type_name << std::endl;
    }
    
    // 析构函数
    ~QuestionType() {
        std::cout << "QuestionType destroyed: " << type_name << std::endl;
    }
    
    // 设置类型信息
    void setTypeInfo(int t_type_id, const std::string& t_type_name) {
        type_id = t_type_id;
        type_name = t_type_name;
    }
    
    // 更新问题数量
    void updateQuestionCount(int t_count) {
        question_count = t_count;
    }
    
    // 增加问题数量
    void incrementQuestionCount() {
        question_count++;
    }
    
    // 减少问题数量
    void decrementQuestionCount() {
        if (question_count > 0) {
            question_count--;
        }
    }
    
    // 获取类型信息（控制台输出）
    void getTypeInfo() {
        std::cout << "Type ID: " << type_id << ", Name: " << type_name 
                  << ", Question Count: " << question_count << std::endl;
    }
    
    // 获取类型信息（返回字符串）
    std::string getTypeInfoString() {
        return "Type ID: " + std::to_string(type_id) + ", Name: " + type_name 
               + ", Question Count: " + std::to_string(question_count);
    }
    
    // 获取类型信息（返回字符串）- 兼容性方法
    std::string getQuestionInfoString() {
        return getTypeInfoString();
    }
    
    // Getter方法
    int getTypeId() const { return type_id; }
    std::string getTypeName() const { return type_name; }
    int getQuestionCount() const { return question_count; }
    
    // Setter方法
    void setTypeId(int t_type_id) { type_id = t_type_id; }
    void setTypeName(const std::string& t_type_name) { type_name = t_type_name; }
    void setQuestionCount(int t_count) { question_count = t_count; }
    
    private:
    int type_id;
    std::string type_name;
    int question_count;
};

#endif //QUESTIONTYPE_H
