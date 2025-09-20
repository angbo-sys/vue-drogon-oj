//
// Created by xiaohua jie on 25-9-12.
//

#include "GetQuestionService.h"
#include <drogon/drogon.h>
#include <json/json.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <random>
#include <algorithm>
#include <iomanip>

using namespace drogon;

QuestionService::QuestionService() {
    // 构造函数
}

// 加载题目数据
Json::Value QuestionService::loadQuestionData() {
    Json::Value data;
    std::ifstream file(getQuestionDataPath());
    
    if (file.is_open()) {
        Json::Reader reader;
        if (!reader.parse(file, data)) {
            std::cerr << "Error parsing question JSON: " << reader.getFormattedErrorMessages() << std::endl;
            // 返回默认结构
            data["subjectNumber"] = 0;
            data["subjects"] = Json::arrayValue;
        }
        file.close();
    } else {
        // 文件不存在，创建默认结构
        data["subjectNumber"] = 0;
        data["subjects"] = Json::arrayValue;
    }
    
    return data;
}

// 保存题目数据
bool QuestionService::saveQuestionData(const Json::Value& t_data) {
    std::ofstream file(getQuestionDataPath());
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

// 加载题目类型数据
Json::Value QuestionService::loadQuestionTypeData() {
    Json::Value data;
    std::ifstream file(getQuestionTypeDataPath());
    
    if (file.is_open()) {
        Json::Reader reader;
        if (!reader.parse(file, data)) {
            std::cerr << "Error parsing question type JSON: " << reader.getFormattedErrorMessages() << std::endl;
            // 返回默认结构
            data["typeNumber"] = 0;
            data["types"] = Json::arrayValue;
        }
        file.close();
    } else {
        // 文件不存在，创建默认结构
        data["typeNumber"] = 0;
        data["types"] = Json::arrayValue;
    }
    
    return data;
}

// 保存题目类型数据
bool QuestionService::saveQuestionTypeData(const Json::Value& t_data) {
    std::ofstream file(getQuestionTypeDataPath());
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

// 获取题目数据文件路径
std::string QuestionService::getQuestionDataPath() {
    return "../SQL/title/question.json";
}

// 获取题目类型数据文件路径
std::string QuestionService::getQuestionTypeDataPath() {
    return "../SQL/title/question_types.json";
}

// 生成题目ID
int QuestionService::generateQuestionId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1000, 9999);
    
    return dis(gen);
}

// 生成类型ID
int QuestionService::generateTypeId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1, 999);
    
    return dis(gen);
}

// 检查题目是否存在
bool QuestionService::questionExists(int t_question_id) {
    Json::Value data = loadQuestionData();
    for (const auto& question : data["subjects"]) {
        if (question["question_id"].asInt() == t_question_id) {
            return true;
        }
    }
    return false;
}

// 检查题目类型是否存在
bool QuestionService::questionTypeExists(int t_type_id) {
    Json::Value data = loadQuestionTypeData();
    for (const auto& type : data["types"]) {
        if (type["type_id"].asInt() == t_type_id) {
            return true;
        }
    }
    return false;
}

// 题目转JSON
Json::Value QuestionService::questionToJson(const Question& t_question) {
    Json::Value json;
    json["question_id"] = t_question.getQuestionId();
    json["type_id"] = t_question.getTypeId();
    json["name"] = t_question.getQuestionName();
    json["path"] = t_question.getPath();
    json["source"] = t_question.getSource();
    json["difficulty_id"] = t_question.getDifficultyId();
    return json;
}

// 题目类型转JSON
Json::Value QuestionService::questionTypeToJson(const QuestionType& t_type) {
    Json::Value json;
    json["type_id"] = t_type.getTypeId();
    json["typeName"] = t_type.getTypeName();
    json["count"] = t_type.getQuestionCount();
    return json;
}

// JSON转题目
Question QuestionService::jsonToQuestion(const Json::Value& t_json) {
    return Question(
        t_json["question_id"].asInt(),
        t_json["type_id"].asInt(),
        t_json["name"].asString(),
        "", // question_content 需要从其他文件读取
        "", // answer_content 需要从其他文件读取
        t_json["difficulty_id"].asInt(),
        t_json["source"].asString(),
        t_json["path"].asString()
    );
}

// JSON转题目类型
QuestionType QuestionService::jsonToQuestionType(const Json::Value& t_json) {
    return QuestionType(
        t_json["type_id"].asInt(),
        t_json["typeName"].asString()
    );
}

// 获取所有题目类型
Json::Value QuestionService::getAllQuestionTypes() {
    Json::Value result;
    
    try {
        Json::Value data = loadQuestionData();
        
        if (data.isMember("typeCalculate")) {
            result["status"] = "success";
            result["types"] = data["typeCalculate"];
            result["total_count"] = data["typeCalculate"].size();
        } else {
            result["status"] = "error";
            result["message"] = "No question types found";
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error loading question types: " + std::string(e.what());
    }
    
    return result;
}

// 根据ID获取题目类型
Json::Value QuestionService::getQuestionTypeById(int t_type_id) {
    Json::Value result;
    
    try {
        Json::Value data = loadQuestionData();
        
        if (data.isMember("typeCalculate")) {
            for (const auto& type : data["typeCalculate"]) {
                if (type["type_id"].asInt() == t_type_id) {
                    result["status"] = "success";
                    result["type"] = type;
                    return result;
                }
            }
        }
        
        result["status"] = "error";
        result["message"] = "Question type not found";
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error loading question type: " + std::string(e.what());
    }
    
    return result;
}

// 添加题目类型
Json::Value QuestionService::addQuestionType(const std::string& t_type_name) {
    Json::Value result;
    
    try {
        Json::Value data = loadQuestionData();
        
        if (!data.isMember("typeCalculate")) {
            data["typeCalculate"] = Json::arrayValue;
        }
        
        // 检查类型名是否已存在
        for (const auto& type : data["typeCalculate"]) {
            if (type["typeName"].asString() == t_type_name) {
                result["status"] = "error";
                result["message"] = "Question type already exists";
                return result;
            }
        }
        
        // 生成新的类型ID
        int new_type_id = generateTypeId();
        while (questionTypeExists(new_type_id)) {
            new_type_id = generateTypeId();
        }
        
        Json::Value newType;
        newType["type_id"] = new_type_id;
        newType["typeName"] = t_type_name;
        newType["count"] = 0;
        
        data["typeCalculate"].append(newType);
        
        if (saveQuestionData(data)) {
            result["status"] = "success";
            result["message"] = "Question type added successfully";
            result["type_id"] = new_type_id;
        } else {
            result["status"] = "error";
            result["message"] = "Failed to save question type";
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error adding question type: " + std::string(e.what());
    }
    
    return result;
}

// 更新题目类型
Json::Value QuestionService::updateQuestionType(int t_type_id, const std::string& t_type_name) {
    Json::Value result;
    
    try {
        Json::Value data = loadQuestionData();
        
        if (data.isMember("typeCalculate")) {
            for (auto& type : data["typeCalculate"]) {
                if (type["type_id"].asInt() == t_type_id) {
                    type["typeName"] = t_type_name;
                    
                    if (saveQuestionData(data)) {
                        result["status"] = "success";
                        result["message"] = "Question type updated successfully";
                    } else {
                        result["status"] = "error";
                        result["message"] = "Failed to save question type";
                    }
                    return result;
                }
            }
        }
        
        result["status"] = "error";
        result["message"] = "Question type not found";
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error updating question type: " + std::string(e.what());
    }
    
    return result;
}

// 删除题目类型
Json::Value QuestionService::deleteQuestionType(int t_type_id) {
    Json::Value result;
    
    try {
        Json::Value data = loadQuestionData();
        
        if (data.isMember("typeCalculate")) {
            Json::Value newTypes = Json::arrayValue;
            bool found = false;
            
            for (const auto& type : data["typeCalculate"]) {
                if (type["type_id"].asInt() != t_type_id) {
                    newTypes.append(type);
                } else {
                    found = true;
                }
            }
            
            if (found) {
                data["typeCalculate"] = newTypes;
                
                if (saveQuestionData(data)) {
                    result["status"] = "success";
                    result["message"] = "Question type deleted successfully";
                } else {
                    result["status"] = "error";
                    result["message"] = "Failed to save question type";
                }
            } else {
                result["status"] = "error";
                result["message"] = "Question type not found";
            }
        } else {
            result["status"] = "error";
            result["message"] = "No question types found";
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error deleting question type: " + std::string(e.what());
    }
    
    return result;
}

// 获取所有题目
Json::Value QuestionService::getAllQuestions() {
    Json::Value result;
    
    try {
        Json::Value data = loadQuestionData();
        
        if (data.isMember("subjects")) {
            result["status"] = "success";
            Json::Value questions = Json::arrayValue;
            
            for (const auto& question : data["subjects"]) {
                Json::Value fullQuestion = question;
                
                // 确保ID字段正确
                fullQuestion["id"] = question["question_id"];
                
                // 添加时间限制和内存限制
                fullQuestion["time_limit"] = 1000;  // 默认1秒
                fullQuestion["memory_limit"] = 256; // 默认256MB
                
                // 添加简化的描述（只取前200个字符）
                std::string questionPath = question["path"].asString();
                if (questionPath.empty()) {
                    questionPath = "./" + std::to_string(question["question_id"].asInt()) + "/";
                }
                
                std::string descriptionPath = "../SQL/title/" + questionPath + "text.md";
                std::ifstream descFile(descriptionPath);
                if (descFile.is_open()) {
                    std::stringstream descBuffer;
                    descBuffer << descFile.rdbuf();
                    std::string fullDesc = descBuffer.str();
                    // 只取前200个字符作为简介
                    if (fullDesc.length() > 200) {
                        fullQuestion["description"] = fullDesc.substr(0, 200) + "...";
                    } else {
                        fullQuestion["description"] = fullDesc;
                    }
                    descFile.close();
                } else {
                    fullQuestion["description"] = "题目描述未找到";
                }
                
                questions.append(fullQuestion);
            }
            
            result["problems"] = questions;
            result["total_count"] = questions.size();
        } else {
            result["status"] = "error";
            result["message"] = "No questions found";
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error loading questions: " + std::string(e.what());
    }
    
    return result;
}

// 根据ID获取题目
Json::Value QuestionService::getQuestionById(int t_question_id) {
    Json::Value result;
    
    try {
        Json::Value data = loadQuestionData();
        
        if (data.isMember("subjects")) {
            for (const auto& question : data["subjects"]) {
                if (question["question_id"].asInt() == t_question_id) {
                    result["status"] = "success";
                    
                    // 创建完整的题目信息
                    Json::Value fullQuestion = question;
                    
                    // 确保ID字段正确
                    fullQuestion["id"] = question["question_id"];
                    
                    // 读取题目详细内容
                    std::string questionPath = question["path"].asString();
                    if (questionPath.empty()) {
                        // 根据题目ID生成路径，确保格式为0001
                        std::stringstream ss;
                        ss << std::setfill('0') << std::setw(4) << t_question_id;
                        questionPath = "./" + ss.str() + "/";
                    }
                    
                    // 读取题目描述文件 (text.md)
                    std::string descriptionPath = "../SQL/title/" + questionPath + "text.md";
                    std::ifstream descFile(descriptionPath);
                    if (descFile.is_open()) {
                        std::stringstream descBuffer;
                        descBuffer << descFile.rdbuf();
                        fullQuestion["description"] = descBuffer.str();
                        descFile.close();
                    } else {
                        fullQuestion["description"] = "题目描述未找到";
                    }
                    
                    // 读取测试用例文件 (question.json)
                    std::string testCasePath = "../SQL/title/" + questionPath + "question.json";
                    std::ifstream testFile(testCasePath);
                    if (testFile.is_open()) {
                        Json::Value testData;
                        Json::Reader reader;
                        if (reader.parse(testFile, testData)) {
                            fullQuestion["test_cases"] = testData;
                        } else {
                            fullQuestion["test_cases"] = Json::objectValue;
                        }
                        testFile.close();
                    } else {
                        fullQuestion["test_cases"] = Json::objectValue;
                    }
                    
                    // 添加时间限制和内存限制
                    fullQuestion["time_limit"] = 1000;  // 默认1秒
                    fullQuestion["memory_limit"] = 256; // 默认256MB
                    
                    result["problem"] = fullQuestion;
                    return result;
                }
            }
        }
        
        result["status"] = "error";
        result["message"] = "Question not found";
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error loading question: " + std::string(e.what());
    }
    
    return result;
}

// 根据类型ID获取题目
Json::Value QuestionService::getQuestionsByTypeId(int t_type_id) {
    Json::Value result;
    Json::Value questions = Json::arrayValue;
    
    try {
        Json::Value data = loadQuestionData();
        
        if (data.isMember("subjects")) {
            for (const auto& question : data["subjects"]) {
                if (question["type_id"].asInt() == t_type_id) {
                    questions.append(question);
                }
            }
        }
        
        result["status"] = "success";
        result["questions"] = questions;
        result["total_count"] = questions.size();
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error loading questions by type: " + std::string(e.what());
    }
    
    return result;
}

// 根据难度获取题目
Json::Value QuestionService::getQuestionsByDifficulty(int t_difficulty_id) {
    Json::Value result;
    Json::Value questions = Json::arrayValue;
    
    try {
        Json::Value data = loadQuestionData();
        
        if (data.isMember("subjects")) {
            for (const auto& question : data["subjects"]) {
                if (question["difficulty_id"].asInt() == t_difficulty_id) {
                    questions.append(question);
                }
            }
        }
        
        result["status"] = "success";
        result["questions"] = questions;
        result["total_count"] = questions.size();
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error loading questions by difficulty: " + std::string(e.what());
    }
    
    return result;
}

// 搜索题目
Json::Value QuestionService::searchQuestions(const std::string& t_keyword) {
    Json::Value result;
    Json::Value questions = Json::arrayValue;
    
    try {
        Json::Value data = loadQuestionData();
        
        if (data.isMember("subjects")) {
            std::string lower_keyword = t_keyword;
            std::transform(lower_keyword.begin(), lower_keyword.end(), lower_keyword.begin(), ::tolower);
            
            for (const auto& question : data["subjects"]) {
                std::string lower_name = question["name"].asString();
                std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), ::tolower);
                
                if (lower_name.find(lower_keyword) != std::string::npos) {
                    questions.append(question);
                }
            }
        }
        
        result["status"] = "success";
        result["questions"] = questions;
        result["total_count"] = questions.size();
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error searching questions: " + std::string(e.what());
    }
    
    return result;
}

// 添加题目
Json::Value QuestionService::addQuestion(int t_type_id, const std::string& t_question_name,
                                        const std::string& t_question_content, const std::string& t_answer_content,
                                        int t_difficulty_id, const std::string& t_source, const std::string& t_path) {
    Json::Value result;
    
    try {
        Json::Value data = loadQuestionData();
        
        if (!data.isMember("subjects")) {
            data["subjects"] = Json::arrayValue;
        }
        
        // 生成新的题目ID
        int new_question_id = generateQuestionId();
        while (questionExists(new_question_id)) {
            new_question_id = generateQuestionId();
        }
        
        Json::Value newQuestion;
        newQuestion["question_id"] = new_question_id;
        newQuestion["type_id"] = t_type_id;
        newQuestion["name"] = t_question_name;
        newQuestion["path"] = t_path.empty() ? "./" + std::to_string(new_question_id) + "/" : t_path;
        newQuestion["source"] = t_source;
        newQuestion["difficulty_id"] = t_difficulty_id;
        
        data["subjects"].append(newQuestion);
        data["subjectNumber"] = data["subjects"].size();
        
        // 更新类型计数
        if (data.isMember("typeCalculate")) {
            for (auto& type : data["typeCalculate"]) {
                if (type["type_id"].asInt() == t_type_id) {
                    type["count"] = type["count"].asInt() + 1;
                    break;
                }
            }
        }
        
        if (saveQuestionData(data)) {
            result["status"] = "success";
            result["message"] = "Question added successfully";
            result["question_id"] = new_question_id;
        } else {
            result["status"] = "error";
            result["message"] = "Failed to save question";
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error adding question: " + std::string(e.what());
    }
    
    return result;
}

// 更新题目
Json::Value QuestionService::updateQuestion(int t_question_id, const std::string& t_question_name,
                                           const std::string& t_question_content, const std::string& t_answer_content,
                                           int t_difficulty_id, const std::string& t_source, const std::string& t_path) {
    Json::Value result;
    
    try {
        Json::Value data = loadQuestionData();
        
        if (data.isMember("subjects")) {
            for (auto& question : data["subjects"]) {
                if (question["question_id"].asInt() == t_question_id) {
                    if (!t_question_name.empty()) {
                        question["name"] = t_question_name;
                    }
                    if (t_difficulty_id >= 0) {
                        question["difficulty_id"] = t_difficulty_id;
                    }
                    if (!t_source.empty()) {
                        question["source"] = t_source;
                    }
                    if (!t_path.empty()) {
                        question["path"] = t_path;
                    }
                    
                    if (saveQuestionData(data)) {
                        result["status"] = "success";
                        result["message"] = "Question updated successfully";
                    } else {
                        result["status"] = "error";
                        result["message"] = "Failed to save question";
                    }
                    return result;
                }
            }
        }
        
        result["status"] = "error";
        result["message"] = "Question not found";
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error updating question: " + std::string(e.what());
    }
    
    return result;
}

// 删除题目
Json::Value QuestionService::deleteQuestion(int t_question_id) {
    Json::Value result;
    
    try {
        Json::Value data = loadQuestionData();
        
        if (data.isMember("subjects")) {
            Json::Value newQuestions = Json::arrayValue;
            bool found = false;
            int type_id = -1;
            
            for (const auto& question : data["subjects"]) {
                if (question["question_id"].asInt() != t_question_id) {
                    newQuestions.append(question);
                } else {
                    found = true;
                    type_id = question["type_id"].asInt();
                }
            }
            
            if (found) {
                data["subjects"] = newQuestions;
                data["subjectNumber"] = data["subjects"].size();
                
                // 更新类型计数
                if (data.isMember("typeCalculate") && type_id >= 0) {
                    for (auto& type : data["typeCalculate"]) {
                        if (type["type_id"].asInt() == type_id) {
                            type["count"] = std::max(0, type["count"].asInt() - 1);
                            break;
                        }
                    }
                }
                
                if (saveQuestionData(data)) {
                    result["status"] = "success";
                    result["message"] = "Question deleted successfully";
                } else {
                    result["status"] = "error";
                    result["message"] = "Failed to save question";
                }
            } else {
                result["status"] = "error";
                result["message"] = "Question not found";
            }
        } else {
            result["status"] = "error";
            result["message"] = "No questions found";
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error deleting question: " + std::string(e.what());
    }
    
    return result;
}

// 获取题目统计
Json::Value QuestionService::getQuestionStatistics() {
    Json::Value result;
    
    try {
        Json::Value data = loadQuestionData();
        
        result["status"] = "success";
        result["statistics"]["total_questions"] = data.get("subjectNumber", 0).asInt();
        
        if (data.isMember("difficultyCalculate")) {
            result["statistics"]["by_difficulty"] = data["difficultyCalculate"];
        }
        
        if (data.isMember("typeCalculate")) {
            result["statistics"]["by_type"] = data["typeCalculate"];
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error loading statistics: " + std::string(e.what());
    }
    
    return result;
}

// 根据类型获取题目数量
Json::Value QuestionService::getQuestionCountByType(int t_type_id) {
    Json::Value result;
    
    try {
        Json::Value questions = getQuestionsByTypeId(t_type_id);
        if (questions["status"].asString() == "success") {
            result["status"] = "success";
            result["type_id"] = t_type_id;
            result["count"] = questions["total_count"];
        } else {
            result = questions;
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error getting question count by type: " + std::string(e.what());
    }
    
    return result;
}

// 根据难度获取题目数量
Json::Value QuestionService::getQuestionCountByDifficulty(int t_difficulty_id) {
    Json::Value result;
    
    try {
        Json::Value questions = getQuestionsByDifficulty(t_difficulty_id);
        if (questions["status"].asString() == "success") {
            result["status"] = "success";
            result["difficulty_id"] = t_difficulty_id;
            result["count"] = questions["total_count"];
        } else {
            result = questions;
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error getting question count by difficulty: " + std::string(e.what());
    }
    
    return result;
}

// 验证答案
Json::Value QuestionService::verifyAnswer(int t_question_id, const std::string& t_user_answer) {
    Json::Value result;
    
    try {
        // 这里需要从具体的问题文件中读取答案
        // 暂时返回一个示例实现
        result["status"] = "success";
        result["question_id"] = t_question_id;
        result["correct"] = false; // 需要实际实现答案验证逻辑
        result["message"] = "Answer verification not implemented yet";
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error verifying answer: " + std::string(e.what());
    }
    
    return result;
}
