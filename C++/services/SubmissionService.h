//
// Created by xiaohua jie on 25-9-12.
// 提交记录管理服务
//

#ifndef SUBMISSIONSERVICE_H
#define SUBMISSIONSERVICE_H

#include <string>
#include <vector>
#include <json/json.h>
#include "CodeExecutor.h"

class SubmissionService {
public:
    SubmissionService();
    
    // 提交代码
    Json::Value submitCode(const std::string& user_id, 
                          int question_id, 
                          const std::string& code,
                          const std::string& language = "cpp");
    
    // 获取用户提交历史
    Json::Value getUserSubmissions(const std::string& user_id, int page = 1, int limit = 10);
    
    // 获取题目提交历史
    Json::Value getQuestionSubmissions(int question_id, int page = 1, int limit = 10);
    
    // 获取提交详情
    Json::Value getSubmissionDetail(const std::string& submission_id);
    
    // 获取所有提交记录
    Json::Value getAllSubmissions(int page = 1, int limit = 10);
    
    // 获取提交统计
    Json::Value getSubmissionStatistics(const std::string& user_id = "");
    
    // 删除提交记录
    Json::Value deleteSubmission(const std::string& submission_id);

private:
    // 加载提交数据
    Json::Value loadSubmissionData();
    
    // 保存提交数据
    bool saveSubmissionData(const Json::Value& data);
    
    // 加载用户个人提交数据
    Json::Value loadUserSubmissionData(const std::string& user_id);
    
    // 保存用户个人提交数据
    bool saveUserSubmissionData(const std::string& user_id, const Json::Value& data);
    
    // 生成提交ID
    std::string generateSubmissionId();
    
    // 获取题目测试用例
    std::vector<TestCase> getQuestionTestCases(int question_id);
    
    // 获取提交数据文件路径
    std::string getSubmissionDataPath();
    
    // 获取用户个人提交数据文件路径
    std::string getUserSubmissionDataPath(const std::string& user_id);
    
    // 检查提交是否存在
    bool submissionExists(const std::string& submission_id);
    
    // 提交记录转JSON
    Json::Value submissionToJson(const Json::Value& submission);
    
    // 计算分页
    Json::Value paginateResults(const Json::Value& data, int page, int limit);
};

#endif //SUBMISSIONSERVICE_H
