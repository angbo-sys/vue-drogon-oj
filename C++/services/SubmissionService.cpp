//
// Created by xiaohua jie on 25-9-12.
// 提交记录管理服务实现
//

#include "SubmissionService.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <random>
#include <algorithm>
#include <iomanip>

SubmissionService::SubmissionService() {
    // 构造函数
}

Json::Value SubmissionService::submitCode(const std::string& user_id, 
                                        int question_id, 
                                        const std::string& code,
                                        const std::string& language) {
    Json::Value result;
    
    try {
        // 生成提交ID
        std::string submission_id = generateSubmissionId();
        
        // 获取题目测试用例
        std::vector<TestCase> test_cases = getQuestionTestCases(question_id);
        
        if (test_cases.empty()) {
            result["status"] = "error";
            result["message"] = "No test cases found for this question";
            return result;
        }
        
        // 创建代码执行器
        CodeExecutor executor;
        
        // 执行代码
        ExecutionResult exec_result = executor.executeCode(code, test_cases, 2, 256);
        
        // 创建提交记录
        Json::Value submission;
        submission["submission_id"] = submission_id;
        submission["user_id"] = user_id;
        submission["question_id"] = question_id;
        submission["code"] = code;
        submission["language"] = language;
        submission["status"] = CodeExecutor::getStatusString(exec_result.status);
        submission["message"] = exec_result.message;
        submission["execution_time"] = exec_result.execution_time;
        submission["memory_usage"] = static_cast<int>(exec_result.memory_usage);
        submission["submitted_at"] = std::to_string(time(nullptr));
        submission["error"] = exec_result.error;
        submission["output"] = exec_result.output;
        
        // 添加测试用例结果
        Json::Value test_case_results = Json::arrayValue;
        for (const auto& test_result : exec_result.test_case_results) {
            test_case_results.append(test_result);
        }
        submission["test_case_results"] = test_case_results;
        
        // 加载用户个人提交数据
        Json::Value userData = loadUserSubmissionData(user_id);
        
        // 添加新提交到用户个人提交记录
        userData["submissions"].append(submission);
        userData["submissionNumber"] = userData["submissions"].size();
        
        // 保存用户个人提交数据
        bool userDataSaved = saveUserSubmissionData(user_id, userData);
        
        // 同时保存到主提交记录文件（可选，如果需要保留全局提交记录）
        // 加载现有提交数据
        Json::Value data = loadSubmissionData();
        
        // 添加新提交
        data["submissions"].append(submission);
        data["submissionNumber"] = data["submissions"].size();
        
        // 保存数据
        bool mainDataSaved = saveSubmissionData(data);
        
        if (userDataSaved) {
            result["status"] = "success";
            result["message"] = "Code submitted successfully";
            result["submission_id"] = submission_id;
            result["execution_result"] = submission;
        } else {
            result["status"] = "error";
            result["message"] = "Failed to save submission";
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error submitting code: " + std::string(e.what());
    }
    
    return result;
}

Json::Value SubmissionService::getUserSubmissions(const std::string& user_id, int page, int limit) {
    Json::Value result;
    
    try {
        // 直接从用户个人提交记录文件中加载数据
        Json::Value data = loadUserSubmissionData(user_id);
        Json::Value user_submissions = Json::arrayValue;
        
        // 获取用户提交
        for (const auto& submission : data["submissions"]) {
            user_submissions.append(submission);
        }
        
        // 按时间倒序排序
        std::vector<Json::Value> submissions_vec;
        for (const auto& submission : user_submissions) {
            submissions_vec.push_back(submission);
        }
        std::sort(submissions_vec.begin(), submissions_vec.end(), 
                 [](const Json::Value& a, const Json::Value& b) {
                     return a["submitted_at"].asString() > b["submitted_at"].asString();
                 });
        
        // 更新排序后的数组
        user_submissions = Json::arrayValue;
        for (const auto& submission : submissions_vec) {
            user_submissions.append(submission);
        }
        
        // 分页
        result = paginateResults(user_submissions, page, limit);
        result["status"] = "success";
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error loading user submissions: " + std::string(e.what());
    }
    
    return result;
}

Json::Value SubmissionService::getQuestionSubmissions(int question_id, int page, int limit) {
    Json::Value result;
    
    try {
        Json::Value data = loadSubmissionData();
        Json::Value question_submissions = Json::arrayValue;
        
        // 筛选题目提交
        for (const auto& submission : data["submissions"]) {
            if (submission["question_id"].asInt() == question_id) {
                question_submissions.append(submission);
            }
        }
        
        // 按时间倒序排序
        std::vector<Json::Value> submissions_vec;
        for (const auto& submission : question_submissions) {
            submissions_vec.push_back(submission);
        }
        std::sort(submissions_vec.begin(), submissions_vec.end(), 
                 [](const Json::Value& a, const Json::Value& b) {
                     return a["submitted_at"].asString() > b["submitted_at"].asString();
                 });
        
        // 更新排序后的数组
        question_submissions = Json::arrayValue;
        for (const auto& submission : submissions_vec) {
            question_submissions.append(submission);
        }
        
        // 分页
        result = paginateResults(question_submissions, page, limit);
        result["status"] = "success";
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error loading question submissions: " + std::string(e.what());
    }
    
    return result;
}

Json::Value SubmissionService::getSubmissionDetail(const std::string& submission_id) {
    Json::Value result;
    
    try {
        // 首先从主提交记录文件中查找
        Json::Value data = loadSubmissionData();
        
        for (const auto& submission : data["submissions"]) {
            if (submission["submission_id"].asString() == submission_id) {
                result["status"] = "success";
                result["submission"] = submission;
                return result;
            }
        }
        
        // 如果在主文件中没找到，尝试从所有用户个人提交记录文件中查找
        // 这里需要一个用户列表，但由于没有实现，我们暂时不实现这部分功能
        
        result["status"] = "error";
        result["message"] = "Submission not found";
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error loading submission detail: " + std::string(e.what());
    }
    
    return result;
}

Json::Value SubmissionService::getAllSubmissions(int page, int limit) {
    Json::Value result;
    
    try {
        Json::Value data = loadSubmissionData();
        
        // 按时间倒序排序
        std::vector<Json::Value> submissions_vec;
        for (const auto& submission : data["submissions"]) {
            submissions_vec.push_back(submission);
        }
        std::sort(submissions_vec.begin(), submissions_vec.end(), 
                 [](const Json::Value& a, const Json::Value& b) {
                     return a["submitted_at"].asString() > b["submitted_at"].asString();
                 });
        
        // 更新排序后的数组
        data["submissions"] = Json::arrayValue;
        for (const auto& submission : submissions_vec) {
            data["submissions"].append(submission);
        }
        
        // 分页
        result = paginateResults(data["submissions"], page, limit);
        result["status"] = "success";
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error loading all submissions: " + std::string(e.what());
    }
    
    return result;
}

Json::Value SubmissionService::getSubmissionStatistics(const std::string& user_id) {
    Json::Value result;
    
    try {
        Json::Value data = loadSubmissionData();
        
        int total_submissions = 0;
        int accepted_submissions = 0;
        int wrong_answer = 0;
        int time_limit_exceeded = 0;
        int runtime_error = 0;
        int compilation_error = 0;
        
        for (const auto& submission : data["submissions"]) {
            // 如果指定了用户ID，只统计该用户的提交
            if (!user_id.empty() && submission["user_id"].asString() != user_id) {
                continue;
            }
            
            total_submissions++;
            std::string status = submission["status"].asString();
            
            if (status == "AC") {
                accepted_submissions++;
            } else if (status == "WA") {
                wrong_answer++;
            } else if (status == "TLE") {
                time_limit_exceeded++;
            } else if (status == "RE") {
                runtime_error++;
            } else if (status == "CE") {
                compilation_error++;
            }
        }
        
        result["status"] = "success";
        result["statistics"]["total_submissions"] = total_submissions;
        result["statistics"]["accepted_submissions"] = accepted_submissions;
        result["statistics"]["wrong_answer"] = wrong_answer;
        result["statistics"]["time_limit_exceeded"] = time_limit_exceeded;
        result["statistics"]["runtime_error"] = runtime_error;
        result["statistics"]["compilation_error"] = compilation_error;
        
        if (total_submissions > 0) {
            result["statistics"]["acceptance_rate"] = (double)accepted_submissions / total_submissions;
        } else {
            result["statistics"]["acceptance_rate"] = 0.0;
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error loading submission statistics: " + std::string(e.what());
    }
    
    return result;
}

Json::Value SubmissionService::deleteSubmission(const std::string& submission_id) {
    Json::Value result;
    
    try {
        // 从主提交记录文件中删除
        Json::Value data = loadSubmissionData();
        Json::Value new_submissions = Json::arrayValue;
        bool found = false;
        std::string user_id;
        
        for (const auto& submission : data["submissions"]) {
            if (submission["submission_id"].asString() != submission_id) {
                new_submissions.append(submission);
            } else {
                found = true;
                user_id = submission["user_id"].asString();
            }
        }
        
        bool mainDataSaved = false;
        bool userDataSaved = false;
        
        if (found) {
            // 更新主提交记录文件
            data["submissions"] = new_submissions;
            data["submissionNumber"] = data["submissions"].size();
            mainDataSaved = saveSubmissionData(data);
            
            // 从用户个人提交记录文件中删除
            if (!user_id.empty()) {
                Json::Value userData = loadUserSubmissionData(user_id);
                Json::Value new_user_submissions = Json::arrayValue;
                bool found_in_user_data = false;
                
                for (const auto& submission : userData["submissions"]) {
                    if (submission["submission_id"].asString() != submission_id) {
                        new_user_submissions.append(submission);
                    } else {
                        found_in_user_data = true;
                    }
                }
                
                if (found_in_user_data) {
                    userData["submissions"] = new_user_submissions;
                    userData["submissionNumber"] = userData["submissions"].size();
                    userDataSaved = saveUserSubmissionData(user_id, userData);
                }
            }
            
            if (mainDataSaved || userDataSaved) {
                result["status"] = "success";
                result["message"] = "Submission deleted successfully";
            } else {
                result["status"] = "error";
                result["message"] = "Failed to save data";
            }
        } else {
            result["status"] = "error";
            result["message"] = "Submission not found";
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "Error deleting submission: " + std::string(e.what());
    }
    
    return result;
}

Json::Value SubmissionService::loadSubmissionData() {
    Json::Value data;
    std::ifstream file(getSubmissionDataPath());
    
    if (file.is_open()) {
        Json::Reader reader;
        if (!reader.parse(file, data)) {
            std::cerr << "Error parsing submission JSON: " << reader.getFormattedErrorMessages() << std::endl;
            // 返回默认结构
            data["submissionNumber"] = 0;
            data["submissions"] = Json::arrayValue;
        }
        file.close();
    } else {
        // 文件不存在，创建默认结构
        data["submissionNumber"] = 0;
        data["submissions"] = Json::arrayValue;
    }
    
    return data;
}

bool SubmissionService::saveSubmissionData(const Json::Value& data) {
    std::ofstream file(getSubmissionDataPath());
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

std::string SubmissionService::generateSubmissionId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(100000, 999999);
    
    return std::to_string(time(nullptr)) + "_" + std::to_string(dis(gen));
}

std::vector<TestCase> SubmissionService::getQuestionTestCases(int question_id) {
    std::vector<TestCase> test_cases;
    
    try {
        // 从题目文件中读取测试用例
        // 根据题目ID动态生成路径
        // 题目ID 0 对应 0001, 题目ID 1 对应 0002
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(4) << (question_id + 1);
        std::string questionPath = "../SQL/title/" + ss.str() + "/question.json";
        
        std::ifstream testFile(questionPath);
        if (testFile.is_open()) {
            Json::Value testData;
            Json::Reader reader;
            if (reader.parse(testFile, testData)) {
                // 解析测试用例
                if (testData.isMember("questions") && testData["questions"].isArray()) {
                    for (const auto& question : testData["questions"]) {
                        if (question.isMember("questionContent") && question.isMember("answerContent")) {
                            std::string input = question["questionContent"].asString();
                            std::string expected = question["answerContent"].asString();
                            test_cases.push_back(TestCase(input, expected, "Test case"));
                        }
                    }
                }
            } else {
                std::cerr << "Error parsing test case JSON: " << reader.getFormattedErrorMessages() << std::endl;
            }
            testFile.close();
        } else {
            std::cerr << "Cannot open test case file: " << questionPath << std::endl;
        }
        
        // 如果没有找到测试用例，提供默认的测试用例
        if (test_cases.empty()) {
            if (question_id == 0) {
                test_cases.push_back(TestCase("1 2", "3", "Basic addition"));
                test_cases.push_back(TestCase("10 20", "30", "Larger numbers"));
                test_cases.push_back(TestCase("0 0", "0", "Zero case"));
            } else if (question_id == 1) {
                test_cases.push_back(TestCase("10+10", "20", "Basic addition"));
                test_cases.push_back(TestCase("20+20", "40", "Larger numbers"));
                test_cases.push_back(TestCase("100+100", "200", "Large numbers"));
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error loading test cases: " << e.what() << std::endl;
    }
    
    return test_cases;
}

std::string SubmissionService::getSubmissionDataPath() {
    return "../SQL/submissions/submissions.json";
}

std::string SubmissionService::getUserSubmissionDataPath(const std::string& user_id) {
    return "../SQL/submissions/" + user_id + "_submissions.json";
}

Json::Value SubmissionService::loadUserSubmissionData(const std::string& user_id) {
    Json::Value data;
    std::ifstream file(getUserSubmissionDataPath(user_id));
    
    if (file.is_open()) {
        Json::Reader reader;
        if (!reader.parse(file, data)) {
            std::cerr << "Error parsing user submission JSON: " << reader.getFormattedErrorMessages() << std::endl;
            // 返回默认结构
            data["submissionNumber"] = 0;
            data["submissions"] = Json::arrayValue;
        }
        file.close();
    } else {
        // 文件不存在，创建默认结构
        data["submissionNumber"] = 0;
        data["submissions"] = Json::arrayValue;
    }
    
    return data;
}

bool SubmissionService::saveUserSubmissionData(const std::string& user_id, const Json::Value& data) {
    std::ofstream file(getUserSubmissionDataPath(user_id));
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

bool SubmissionService::submissionExists(const std::string& submission_id) {
    Json::Value data = loadSubmissionData();
    
    for (const auto& submission : data["submissions"]) {
        if (submission["submission_id"].asString() == submission_id) {
            return true;
        }
    }
    return false;
}

Json::Value SubmissionService::submissionToJson(const Json::Value& submission) {
    return submission;
}

Json::Value SubmissionService::paginateResults(const Json::Value& data, int page, int limit) {
    Json::Value result;
    
    int total = data.size();
    int total_pages = (total + limit - 1) / limit;
    int start_index = (page - 1) * limit;
    int end_index = std::min(start_index + limit, total);
    
    result["total"] = total;
    result["page"] = page;
    result["limit"] = limit;
    result["total_pages"] = total_pages;
    
    Json::Value items = Json::arrayValue;
    for (int i = start_index; i < end_index; ++i) {
        items.append(data[i]);
    }
    result["items"] = items;
    
    return result;
}
