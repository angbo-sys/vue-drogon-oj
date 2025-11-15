//
// Created by xiaohua jie on 25-9-12.
//

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "Question.h"
#include "QuestionType.h"

#ifndef QUESTIONCOLLECTION_H
#define QUESTIONCOLLECTION_H

class QuestionCollection {
    public:
    // 构造函数
    QuestionCollection() {
        std::cout << "QuestionCollection created" << std::endl;
    }
    
    // 析构函数
    ~QuestionCollection() {
        std::cout << "QuestionCollection destroyed" << std::endl;
    }
    
    // 添加题目
    int addQuestion(const Question& t_question) {
        questions.push_back(t_question);
        std::cout << "Question added: " << t_question.getQuestionName() << std::endl;
        return 1;
    }
    
    // 添加题目类型
    int addQuestionType(const QuestionType& t_type) {
        question_types.push_back(t_type);
        std::cout << "Question type added: " << t_type.getTypeName() << std::endl;
        return 1;
    }
    
    // 根据ID获取题目
    Question* getQuestionById(int t_question_id) {
        for (auto& question : questions) {
            if (question.getQuestionId() == t_question_id) {
                return &question;
            }
        }
        return nullptr;
    }
    
    // 根据类型ID获取题目列表
    std::vector<Question> getQuestionsByTypeId(int t_type_id) {
        std::vector<Question> result;
        for (const auto& question : questions) {
            if (question.getTypeId() == t_type_id) {
                result.push_back(question);
            }
        }
        return result;
    }
    
    // 根据难度获取题目列表
    std::vector<Question> getQuestionsByDifficulty(int t_difficulty_id) {
        std::vector<Question> result;
        for (const auto& question : questions) {
            if (question.getDifficultyId() == t_difficulty_id) {
                result.push_back(question);
            }
        }
        return result;
    }
    
    // 根据类型ID获取题目类型
    QuestionType* getQuestionTypeById(int t_type_id) {
        for (auto& type : question_types) {
            if (type.getTypeId() == t_type_id) {
                return &type;
            }
        }
        return nullptr;
    }
    
    // 搜索题目（按名称）
    std::vector<Question> searchQuestions(const std::string& t_keyword) {
        std::vector<Question> result;
        std::string lower_keyword = t_keyword;
        std::transform(lower_keyword.begin(), lower_keyword.end(), lower_keyword.begin(), ::tolower);
        
        for (const auto& question : questions) {
            std::string lower_name = question.getQuestionName();
            std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);
            
            if (lower_name.find(lower_keyword) != std::string::npos) {
                result.push_back(question);
            }
        }
        return result;
    }
    
    // 更新题目
    int updateQuestion(int t_question_id, const Question& t_updated_question) {
        for (auto& question : questions) {
            if (question.getQuestionId() == t_question_id) {
                question = t_updated_question;
                std::cout << "Question updated: " << t_question_id << std::endl;
                return 1;
            }
        }
        return 0;
    }
    
    // 更新题目类型
    int updateQuestionType(int t_type_id, const QuestionType& t_updated_type) {
        for (auto& type : question_types) {
            if (type.getTypeId() == t_type_id) {
                type = t_updated_type;
                std::cout << "Question type updated: " << t_type_id << std::endl;
                return 1;
            }
        }
        return 0;
    }
    
    // 删除题目
    int removeQuestion(int t_question_id) {
        for (auto it = questions.begin(); it != questions.end(); ++it) {
            if (it->getQuestionId() == t_question_id) {
                questions.erase(it);
                std::cout << "Question removed: " << t_question_id << std::endl;
                return 1;
            }
        }
        return 0;
    }
    
    // 删除题目类型
    int removeQuestionType(int t_type_id) {
        for (auto it = question_types.begin(); it != question_types.end(); ++it) {
            if (it->getTypeId() == t_type_id) {
                question_types.erase(it);
                std::cout << "Question type removed: " << t_type_id << std::endl;
                return 1;
            }
        }
        return 0;
    }
    
    // 获取所有题目
    const std::vector<Question>& getAllQuestions() const {
        return questions;
    }
    
    // 获取所有题目类型
    const std::vector<QuestionType>& getAllQuestionTypes() const {
        return question_types;
    }
    
    // 获取题目数量
    int getQuestionCount() const {
        return questions.size();
    }
    
    // 获取题目类型数量
    int getQuestionTypeCount() const {
        return question_types.size();
    }
    
    // 根据类型获取题目数量
    int getQuestionCountByType(int t_type_id) const {
        int count = 0;
        for (const auto& question : questions) {
            if (question.getTypeId() == t_type_id) {
                count++;
            }
        }
        return count;
    }
    
    // 根据难度获取题目数量
    int getQuestionCountByDifficulty(int t_difficulty_id) const {
        int count = 0;
        for (const auto& question : questions) {
            if (question.getDifficultyId() == t_difficulty_id) {
                count++;
            }
        }
        return count;
    }
    
    // 获取统计信息
    void getStatistics() {
        std::cout << "=== Question Collection Statistics ===" << std::endl;
        std::cout << "Total Questions: " << getQuestionCount() << std::endl;
        std::cout << "Total Types: " << getQuestionTypeCount() << std::endl;
        
        // 按难度统计
        std::cout << "\nBy Difficulty:" << std::endl;
        for (int i = 0; i < 4; i++) {
            std::cout << "  Difficulty " << i << ": " << getQuestionCountByDifficulty(i) << " questions" << std::endl;
        }
        
        // 按类型统计
        std::cout << "\nBy Type:" << std::endl;
        for (const auto& type : question_types) {
            std::cout << "  " << type.getTypeName() << " (ID: " << type.getTypeId() 
                      << "): " << getQuestionCountByType(type.getTypeId()) << " questions" << std::endl;
        }
    }
    
    // 清空所有数据
    void clear() {
        questions.clear();
        question_types.clear();
        std::cout << "Question collection cleared" << std::endl;
    }
    
    private:
    std::vector<Question> questions;
    std::vector<QuestionType> question_types;
};

#endif //QUESTIONCOLLECTION_H
