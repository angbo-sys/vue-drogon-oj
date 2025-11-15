//
// Created by xiaohua jie on 25-9-12.
//

#include "../models/User/UserInfo/Info.h"
#include <drogon/drogon.h>
#include <json/json.h>
#include <fstream>
#include <string>

class UserMethods
{
    public:
    UserMethods();
    
    // 用户认证相关
    Json::Value authenticateUser(const std::string &t_user_name, const std::string &t_user_password);
    Json::Value registerUser(const std::string &t_user_name, const std::string &t_user_password);
    Json::Value getUserById(const std::string &t_user_id);
    Json::Value updateUserPassword(const std::string &t_user_id, const std::string &t_old_password, const std::string &t_new_password);
    Json::Value deleteUser(const std::string &t_user_id);
    
    // 用户问题进度相关
    Json::Value getUserQuestionProgress(const std::string &t_user_id);
    Json::Value addQuestionProgress(const std::string &t_user_id, int t_type_id, int t_question_id, bool t_is_finish);
    Json::Value updateQuestionProgress(const std::string &t_user_id, int t_type_id, int t_question_id, bool t_is_finish);
    Json::Value getFinishedQuestions(const std::string &t_user_id);
    Json::Value getUnfinishedQuestions(const std::string &t_user_id);
    Json::Value getQuestionProgressStats(const std::string &t_user_id);
    
    // 用户统计相关
    Json::Value getAllUsers();
    Json::Value getUserCount();
    
    private:
    // JSON文件操作
    Json::Value loadUserData();
    bool saveUserData(const Json::Value &t_data);
    Json::Value loadQuestionData();
    std::string getUserDataPath();
    std::string getQuestionDataPath();
    std::string getUserRecordPath(const std::string &t_user_id);
    bool createUserRecordFile(const std::string &t_user_id, const std::string &t_user_name, const Json::Value &t_progress);
    Json::Value loadUserRecordData(const std::string &t_user_id);
    bool saveUserRecordData(const std::string &t_user_id, const Json::Value &t_data);
    
    // 辅助方法
    std::string generateUserId();
    bool userExists(const std::string &t_user_id);
    bool userExistsByName(const std::string &t_user_name);
};
