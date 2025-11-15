//
// Created by xiaohua jie on 25-9-12.
//

#include "GetUserInfo.h"
#include <drogon/drogon.h>
#include <json/json.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <random>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>

using namespace drogon;

UserMethods::UserMethods() {
    // 构造函数
}

// 加载用户数据
Json::Value UserMethods::loadUserData() {
    Json::Value data;
    std::ifstream file(getUserDataPath());
    
    if (file.is_open()) {
        Json::Reader reader;
        if (!reader.parse(file, data)) {
            std::cerr << "Error parsing user JSON: " << reader.getFormattedErrorMessages() << std::endl;
            // 返回默认结构
            data["userNumber"] = 0;
            data["userInfo"] = Json::arrayValue;
        }
        file.close();
    } else {
        // 文件不存在，创建默认结构
        data["userNumber"] = 0;
        data["userInfo"] = Json::arrayValue;
    }
    
    return data;
}

// 保存用户数据
bool UserMethods::saveUserData(const Json::Value &t_data) {
    std::ofstream file(getUserDataPath());
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

// 加载问题数据
Json::Value UserMethods::loadQuestionData() {
    Json::Value data;
    std::ifstream file(getQuestionDataPath());
    
    if (file.is_open()) {
        Json::Reader reader;
        if (!reader.parse(file, data)) {
            std::cerr << "Error parsing question JSON: " << reader.getFormattedErrorMessages() << std::endl;
        }
        file.close();
    }
    
    return data;
}

// 获取用户数据文件路径
std::string UserMethods::getUserDataPath() {
    return "../SQL/user/user.json";
}

// 获取问题数据文件路径
std::string UserMethods::getQuestionDataPath() {
    return "../SQL/title/question.json";
}

// 获取用户个人记录文件路径
std::string UserMethods::getUserRecordPath(const std::string &t_user_id) {
    return "../SQL/user/" + t_user_id + "_record.json";
}

// 创建用户个人做题记录文件
bool UserMethods::createUserRecordFile(const std::string &t_user_id, const std::string &t_user_name, const Json::Value &t_progress) {
    try {
        // 参数验证
        if (t_user_id.empty()) {
            std::cerr << "Error: Empty user ID provided to createUserRecordFile" << std::endl;
            return false;
        }
        
        // 确保目录存在
        std::string dirPath = "../SQL/user/";
        std::string filePath = getUserRecordPath(t_user_id);
        
        // 检查目录是否存在
        struct stat info;
        if (stat(dirPath.c_str(), &info) != 0) {
            std::cerr << "Directory does not exist: " << dirPath << std::endl;
            // 尝试创建目录
            int result = system(("mkdir -p " + dirPath).c_str());
            if (result != 0) {
                std::cerr << "Failed to create directory: " << dirPath << ", error code: " << result << std::endl;
                return false;
            }
            std::cerr << "Created directory: " << dirPath << std::endl;
        } else if (!(info.st_mode & S_IFDIR)) {
            std::cerr << "Path exists but is not a directory: " << dirPath << std::endl;
            return false;
        }
        
        // 检查文件是否已存在
        std::ifstream fileCheck(filePath);
        if (fileCheck.good()) {
            std::cerr << "User record file already exists: " << filePath << std::endl;
            fileCheck.close();
            // 文件已存在，我们可以选择返回true，因为文件已经存在
            return true;
        }
        
        // 创建并写入文件
        std::ofstream file(filePath);
        
        if (file.is_open()) {
            Json::Value userData;
            userData["user_id"] = t_user_id;
            userData["user_name"] = t_user_name;
            userData["created_at"] = std::to_string(time(nullptr));
            userData["question_progress"] = t_progress;
            
            Json::StreamWriterBuilder builder;
            builder["indentation"] = "    ";
            std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
            writer->write(userData, &file);
            file.close();
            std::cerr << "Successfully created user record file: " << filePath << std::endl;
            return true;
        } else {
            std::cerr << "Failed to open file for writing: " << filePath << std::endl;
            // 检查文件权限
            std::cerr << "Checking file permissions and directory access..." << std::endl;
            system(("ls -la " + dirPath).c_str());
        }
    } catch (const std::exception& e) {
        std::cerr << "Error creating user record file: " << e.what() << std::endl;
    }
    return false;
}

// 加载用户个人记录文件
Json::Value UserMethods::loadUserRecordData(const std::string &t_user_id) {
    Json::Value data;
    std::string filePath = getUserRecordPath(t_user_id);
    std::ifstream file(filePath);
    
    if (file.is_open()) {
        Json::Reader reader;
        if (!reader.parse(file, data)) {
            std::cerr << "Error parsing user record JSON: " << reader.getFormattedErrorMessages() << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Could not open user record file: " << filePath << std::endl;
    }
    
    return data;
}

// 保存用户个人记录文件
bool UserMethods::saveUserRecordData(const std::string &t_user_id, const Json::Value &t_data) {
    std::string filePath = getUserRecordPath(t_user_id);
    std::ofstream file(filePath);
    
    if (file.is_open()) {
        Json::StreamWriterBuilder builder;
        builder["indentation"] = "    ";
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(t_data, &file);
        file.close();
        return true;
    }
    
    std::cerr << "Could not open user record file for writing: " << filePath << std::endl;
    return false;
}

// 生成用户ID
std::string UserMethods::generateUserId() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(1000, 9999);
    
    return std::to_string(dis(gen));
}

// 检查用户是否存在
bool UserMethods::userExists(const std::string &t_user_id) {
    Json::Value data = loadUserData();
    for (const auto &user : data["userInfo"]) {
        if (user["user_id"].asString() == t_user_id) {
            return true;
        }
    }
    return false;
}

// 检查用户名是否存在
bool UserMethods::userExistsByName(const std::string &t_user_name) {
    Json::Value data = loadUserData();
    for (const auto &user : data["userInfo"]) {
        if (user["user_name"].asString() == t_user_name) {
            return true;
        }
    }
    return false;
}

// 用户认证
Json::Value UserMethods::authenticateUser(const std::string &t_user_name, const std::string &t_user_password) {
    Json::Value result;
    
    try {
        Json::Value data = loadUserData();
        
        for (const auto &user : data["userInfo"]) {
            if (user["user_name"].asString() == t_user_name && 
                user["user_password"].asString() == t_user_password) {
                result["status"] = "success";
                result["message"] = "用户认证成功";
                result["user_id"] = user["user_id"];
                result["user_name"] = user["user_name"];
                return result;
            }
        }
        
        result["status"] = "error";
        result["message"] = "用户名或密码错误";
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "认证过程中发生错误: " + std::string(e.what());
    }
    
    return result;
}

// 用户注册
Json::Value UserMethods::registerUser(const std::string &t_user_name, const std::string &t_user_password) {
    Json::Value result;
    
    try {
        std::cerr << "Starting registration process for user: " << t_user_name << std::endl;
        
        // 参数验证
        if (t_user_name.empty() || t_user_password.empty()) {
            result["status"] = "error";
            result["message"] = "用户名或密码不能为空";
            std::cerr << "Registration failed: Empty username or password" << std::endl;
            return result;
        }
        
        if (t_user_name.length() < 3 || t_user_name.length() > 20) {
            result["status"] = "error";
            result["message"] = "用户名长度必须在3到20个字符之间";
            std::cerr << "Registration failed: Username length invalid" << std::endl;
            return result;
        }
        
        if (t_user_password.length() < 6 || t_user_password.length() > 20) {
            result["status"] = "error";
            result["message"] = "密码长度必须在6到20个字符之间";
            std::cerr << "Registration failed: Password length invalid" << std::endl;
            return result;
        }
        
        // 检查用户名是否已存在
        if (userExistsByName(t_user_name)) {
            result["status"] = "error";
            result["message"] = "用户名已存在";
            std::cerr << "Registration failed: Username already exists: " << t_user_name << std::endl;
            return result;
        }
        
        // 加载用户数据
        std::cerr << "Loading user data..." << std::endl;
        Json::Value data = loadUserData();
        if (data.isNull() || !data.isMember("userInfo") || !data.isMember("userNumber")) {
            result["status"] = "error";
            result["message"] = "无法加载用户数据或数据格式错误";
            std::cerr << "Registration failed: Could not load user data or invalid data format" << std::endl;
            return result;
        }
        
        // 生成用户ID
        std::string new_user_id = generateUserId();
        std::cerr << "Generated user ID: " << new_user_id << std::endl;
        
        // 确保生成的ID唯一
        int attempts = 0;
        while (userExists(new_user_id) && attempts < 10) {
            new_user_id = generateUserId();
            std::cerr << "Regenerated user ID: " << new_user_id << std::endl;
            attempts++;
        }
        
        if (attempts >= 10) {
            result["status"] = "error";
            result["message"] = "无法生成唯一用户ID，请稍后再试";
            std::cerr << "Registration failed: Could not generate unique user ID after 10 attempts" << std::endl;
            return result;
        }
        
        // 创建新用户对象
        Json::Value newUser;
        newUser["user_id"] = new_user_id;
        newUser["user_name"] = t_user_name;
        newUser["user_password"] = t_user_password;
        
        // 初始化用户问题进度
        Json::Value progress;
        progress["question_finish_number"] = 0;
        progress["question_hasDoes_number"] = 0;
        progress["question_finish_info"] = Json::arrayValue;
        progress["question_unfinish_info"] = Json::arrayValue;
        newUser["user_question_down_info"] = progress;
        
        // 添加用户到数据中
        data["userInfo"].append(newUser);
        data["userNumber"] = data["userInfo"].size();
        
        // 先创建用户个人做题记录文件
        std::cerr << "Creating user record file..." << std::endl;
        bool userRecordSaveSuccess = createUserRecordFile(new_user_id, t_user_name, progress);
        std::cerr << "User record file creation result: " << (userRecordSaveSuccess ? "success" : "failure") << std::endl;
        
        if (!userRecordSaveSuccess) {
            result["status"] = "error";
            result["message"] = "创建用户记录文件失败";
            std::cerr << "Failed to create user record file for user: " << t_user_name << " with ID: " << new_user_id << std::endl;
            return result;
        }
        
        // 保存用户数据到主用户文件
        std::cerr << "Saving main user data..." << std::endl;
        bool mainSaveSuccess = saveUserData(data);
        std::cerr << "Main user data save result: " << (mainSaveSuccess ? "success" : "failure") << std::endl;
        
        if (!mainSaveSuccess) {
            result["status"] = "error";
            result["message"] = "保存主用户数据失败";
            std::cerr << "Failed to save main user data for user: " << t_user_name << " with ID: " << new_user_id << std::endl;
            return result;
        }
        
        // 注册成功
        result["status"] = "success";
        result["message"] = "用户注册成功";
        result["user_id"] = new_user_id;
        std::cerr << "Registration successful for user: " << t_user_name << " with ID: " << new_user_id << std::endl;
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "注册过程中发生错误: " + std::string(e.what());
        std::cerr << "Registration exception: " << e.what() << std::endl;
    }
    
    return result;
}

// 根据ID获取用户信息
Json::Value UserMethods::getUserById(const std::string &t_user_id) {
    Json::Value result;
    
    try {
        Json::Value data = loadUserData();
        
        for (const auto &user : data["userInfo"]) {
            if (user["user_id"].asString() == t_user_id) {
                result["status"] = "success";
                result["user"] = user;
                return result;
            }
        }
        
        result["status"] = "error";
        result["message"] = "用户不存在";
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "获取用户信息时发生错误: " + std::string(e.what());
    }
    
    return result;
}

// 更新用户密码
Json::Value UserMethods::updateUserPassword(const std::string &t_user_id, const std::string &t_old_password, const std::string &t_new_password) {
    Json::Value result;
    
    try {
        Json::Value data = loadUserData();
        
        for (auto &user : data["userInfo"]) {
            if (user["user_id"].asString() == t_user_id) {
                if (user["user_password"].asString() == t_old_password) {
                    user["user_password"] = t_new_password;
                    if (saveUserData(data)) {
                        result["status"] = "success";
                        result["message"] = "密码更新成功";
                    } else {
                        result["status"] = "error";
                        result["message"] = "保存数据失败";
                    }
                    return result;
                } else {
                    result["status"] = "error";
                    result["message"] = "原密码错误";
                    return result;
                }
            }
        }
        
        result["status"] = "error";
        result["message"] = "用户不存在";
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "更新密码时发生错误: " + std::string(e.what());
    }
    
    return result;
}

// 删除用户
Json::Value UserMethods::deleteUser(const std::string &t_user_id) {
    Json::Value result;
    
    try {
        Json::Value data = loadUserData();
        Json::Value newUserInfo = Json::arrayValue;
        
        bool found = false;
        for (const auto &user : data["userInfo"]) {
            if (user["user_id"].asString() != t_user_id) {
                newUserInfo.append(user);
            } else {
                found = true;
            }
        }
        
        if (found) {
            data["userInfo"] = newUserInfo;
            data["userNumber"] = data["userInfo"].size();
            
            if (saveUserData(data)) {
                result["status"] = "success";
                result["message"] = "用户删除成功";
            } else {
                result["status"] = "error";
                result["message"] = "保存数据失败";
            }
        } else {
            result["status"] = "error";
            result["message"] = "用户不存在";
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "删除用户时发生错误: " + std::string(e.what());
    }
    
    return result;
}

// 获取用户问题进度
Json::Value UserMethods::getUserQuestionProgress(const std::string &t_user_id) {
    Json::Value result;
    
    try {
        // 优先从用户个人记录文件中获取
        Json::Value recordData = loadUserRecordData(t_user_id);
        if (!recordData.isNull() && recordData.isMember("question_progress")) {
            result["status"] = "success";
            result["progress"] = recordData["question_progress"];
            return result;
        }
        
        // 如果个人记录文件不存在或没有问题进度，则从主用户文件获取
        Json::Value userData = getUserById(t_user_id);
        if (userData["status"].asString() == "success") {
            result["status"] = "success";
            
            // 从主用户文件中获取该用户的真实进度数据
            Json::Value userProgress;
            if (userData["user"].isMember("user_question_down_info")) {
                userProgress = userData["user"]["user_question_down_info"];
            } else {
                // 如果主用户文件中也没有进度数据，则创建空进度记录
                userProgress = Json::Value(Json::objectValue);
                userProgress["question_finish_info"] = Json::arrayValue;
                userProgress["question_unfinish_info"] = Json::arrayValue;
                userProgress["question_finish_number"] = 0;
                userProgress["question_hasDoes_number"] = 0;
            }
            
            result["progress"] = userProgress;
            
            // 同步到用户个人记录文件
            if (recordData.isNull()) {
                recordData = Json::Value(Json::objectValue);
                recordData["user_id"] = t_user_id;
                
                // 尝试获取用户名
                if (userData["user"].isMember("user_name")) {
                    recordData["user_name"] = userData["user"]["user_name"];
                }
                
                recordData["created_at"] = std::to_string(time(nullptr));
            }
            
            recordData["question_progress"] = userProgress;
            saveUserRecordData(t_user_id, recordData);
        } else {
            result = userData;
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "获取问题进度时发生错误: " + std::string(e.what());
    }
    
    return result;
}

// 添加问题进度
Json::Value UserMethods::addQuestionProgress(const std::string &t_user_id, int t_type_id, int t_question_id, bool t_is_finish) {
    Json::Value result;
    
    try {
        // 首先尝试从用户个人记录文件获取进度
        Json::Value recordData = loadUserRecordData(t_user_id);
        bool userRecordExists = !recordData.isNull() && recordData.isMember("question_progress");
        Json::Value progress;
        
        if (userRecordExists) {
            progress = recordData["question_progress"];
        } else {
            // 如果个人记录不存在，创建新的空进度记录
            progress = Json::Value(Json::objectValue);
            progress["question_finish_info"] = Json::arrayValue;
            progress["question_unfinish_info"] = Json::arrayValue;
            progress["question_finish_number"] = 0;
            progress["question_hasDoes_number"] = 0;
            
            recordData = Json::Value(Json::objectValue);
            recordData["user_id"] = t_user_id;
            recordData["created_at"] = std::to_string(time(nullptr));
            
            // 尝试获取用户名
            Json::Value userData = getUserById(t_user_id);
            if (userData["status"].asString() == "success" && userData["user"].isMember("user_name")) {
                recordData["user_name"] = userData["user"]["user_name"];
            }
        }
        
        // 添加问题信息
        Json::Value questionInfo;
        questionInfo["type_id"] = t_type_id;
        questionInfo["question_id"] = t_question_id;
        questionInfo["is_finish"] = t_is_finish;
        
        if (t_is_finish) {
            progress["question_finish_info"].append(questionInfo);
            progress["question_finish_number"] = progress["question_finish_number"].asInt() + 1;
        } else {
            progress["question_unfinish_info"].append(questionInfo);
            progress["question_hasDoes_number"] = progress["question_hasDoes_number"].asInt() + 1;
        }
        
        // 更新用户个人记录文件
        recordData["question_progress"] = progress;
        bool recordSaveSuccess = saveUserRecordData(t_user_id, recordData);
        
        // 同时更新主用户文件
        Json::Value data = loadUserData();
        bool userFound = false;
        
        for (auto &user : data["userInfo"]) {
            if (user["user_id"].asString() == t_user_id) {
                userFound = true;
                user["user_question_down_info"] = progress;
                break;
            }
        }
        
        if (!userFound) {
            result["status"] = "error";
            result["message"] = "用户不存在";
            return result;
        }
        
        // 保存主用户文件
        bool mainSaveSuccess = saveUserData(data);
        
        if (mainSaveSuccess && recordSaveSuccess) {
            result["status"] = "success";
            result["message"] = "问题进度添加成功";
        } else {
            result["status"] = "error";
            result["message"] = "保存数据失败";
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "添加问题进度时发生错误: " + std::string(e.what());
    }
    
    return result;
}

// 更新问题进度
Json::Value UserMethods::updateQuestionProgress(const std::string &t_user_id, int t_type_id, int t_question_id, bool t_is_finish) {
    Json::Value result;
    
    try {
        // 首先尝试从用户个人记录文件获取进度
        Json::Value recordData = loadUserRecordData(t_user_id);
        bool userRecordExists = !recordData.isNull() && recordData.isMember("question_progress");
        Json::Value progress;
        
        if (userRecordExists) {
            progress = recordData["question_progress"];
        } else {
            // 如果个人记录不存在，创建新的空进度记录
            progress = Json::Value(Json::objectValue);
            progress["question_finish_info"] = Json::arrayValue;
            progress["question_unfinish_info"] = Json::arrayValue;
            progress["question_finish_number"] = 0;
            progress["question_hasDoes_number"] = 0;
            
            recordData = Json::Value(Json::objectValue);
            recordData["user_id"] = t_user_id;
            recordData["created_at"] = std::to_string(time(nullptr));
            
            // 尝试获取用户名
            Json::Value userData = getUserById(t_user_id);
            if (userData["status"].asString() == "success" && userData["user"].isMember("user_name")) {
                recordData["user_name"] = userData["user"]["user_name"];
            }
        }
        
        // 从未完成列表中移除
        Json::Value newUnfinish = Json::arrayValue;
        for (const auto &item : progress["question_unfinish_info"]) {
            if (!(item["type_id"].asInt() == t_type_id && item["question_id"].asInt() == t_question_id)) {
                newUnfinish.append(item);
            }
        }
        progress["question_unfinish_info"] = newUnfinish;
        
        // 从已完成列表中移除
        Json::Value newFinish = Json::arrayValue;
        for (const auto &item : progress["question_finish_info"]) {
            if (!(item["type_id"].asInt() == t_type_id && item["question_id"].asInt() == t_question_id)) {
                newFinish.append(item);
            }
        }
        
        // 添加到相应列表
        Json::Value questionInfo;
        questionInfo["type_id"] = t_type_id;
        questionInfo["question_id"] = t_question_id;
        questionInfo["is_finish"] = t_is_finish;
        
        if (t_is_finish) {
            newFinish.append(questionInfo);
        } else {
            newUnfinish.append(questionInfo);
        }
        
        progress["question_finish_info"] = newFinish;
        progress["question_unfinish_info"] = newUnfinish;
        progress["question_finish_number"] = newFinish.size();
        progress["question_hasDoes_number"] = newUnfinish.size();
        
        // 更新用户个人记录文件
        recordData["question_progress"] = progress;
        bool recordSaveSuccess = saveUserRecordData(t_user_id, recordData);
        
        // 同时更新主用户文件
        Json::Value data = loadUserData();
        bool userFound = false;
        
        for (auto &user : data["userInfo"]) {
            if (user["user_id"].asString() == t_user_id) {
                userFound = true;
                user["user_question_down_info"] = progress;
                break;
            }
        }
        
        if (!userFound) {
            result["status"] = "error";
            result["message"] = "用户不存在";
            return result;
        }
        
        // 保存主用户文件
        bool mainSaveSuccess = saveUserData(data);
        
        if (mainSaveSuccess && recordSaveSuccess) {
            result["status"] = "success";
            result["message"] = "问题进度更新成功";
        } else {
            result["status"] = "error";
            result["message"] = "保存数据失败";
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "更新问题进度时发生错误: " + std::string(e.what());
    }
    
    return result;
}

// 获取已完成的问题
Json::Value UserMethods::getFinishedQuestions(const std::string &t_user_id) {
    Json::Value result;
    
    try {
        Json::Value progress = getUserQuestionProgress(t_user_id);
        if (progress["status"].asString() == "success") {
            result["status"] = "success";
            result["finished_questions"] = progress["progress"]["question_finish_info"];
        } else {
            result = progress;
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "获取已完成问题时发生错误: " + std::string(e.what());
    }
    
    return result;
}

// 获取未完成的问题
Json::Value UserMethods::getUnfinishedQuestions(const std::string &t_user_id) {
    Json::Value result;
    
    try {
        Json::Value progress = getUserQuestionProgress(t_user_id);
        if (progress["status"].asString() == "success") {
            result["status"] = "success";
            result["unfinished_questions"] = progress["progress"]["question_unfinish_info"];
        } else {
            result = progress;
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "获取未完成问题时发生错误: " + std::string(e.what());
    }
    
    return result;
}

// 获取问题进度统计
Json::Value UserMethods::getQuestionProgressStats(const std::string &t_user_id) {
    Json::Value result;
    
    try {
        Json::Value progress = getUserQuestionProgress(t_user_id);
        if (progress["status"].asString() == "success") {
            result["status"] = "success";
            result["stats"]["finished_count"] = progress["progress"]["question_finish_number"];
            result["stats"]["unfinished_count"] = progress["progress"]["question_hasDoes_number"];
            result["stats"]["total_count"] = progress["progress"]["question_finish_number"].asInt() + 
                                           progress["progress"]["question_hasDoes_number"].asInt();
        } else {
            result = progress;
        }
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "获取问题进度统计时发生错误: " + std::string(e.what());
    }
    
    return result;
}

// 获取所有用户
Json::Value UserMethods::getAllUsers() {
    Json::Value result;
    
    try {
        Json::Value data = loadUserData();
        result["status"] = "success";
        result["users"] = data["userInfo"];
        result["total_count"] = data["userNumber"];
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "获取用户列表时发生错误: " + std::string(e.what());
    }
    
    return result;
}

// 获取用户数量
Json::Value UserMethods::getUserCount() {
    Json::Value result;
    
    try {
        Json::Value data = loadUserData();
        result["status"] = "success";
        result["user_count"] = data["userNumber"];
        
    } catch (const std::exception& e) {
        result["status"] = "error";
        result["message"] = "获取用户数量时发生错误: " + std::string(e.what());
    }
    
    return result;
}
