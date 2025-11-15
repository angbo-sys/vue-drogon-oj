#include <iostream>
#include <string>
#include <ctime>
#include "../QuestionDowm/UserQuestionDowm.h"

#ifndef USERINFO_H
#define USERINFO_H

class UserInfo{
    public:
    // 构造函数
    UserInfo(){
        std::cout<<"Error: No information provided"<<std::endl;
        user_id = "";
        user_name = "";
        user_password = "";
        created_at = 0;
        last_login = 0;
    }
    
    UserInfo(std::string t_user_id, std::string t_user_name, std::string t_user_password){
        user_id = t_user_id;
        user_name = t_user_name;
        user_password = t_user_password;
        created_at = time(nullptr);
        last_login = 0;
        user_question_down_info = user_question_down(t_user_id);
        std::cout<<"UserInfo has created for user: " << user_name << std::endl;
    }
    
    // 析构函数
    ~UserInfo(){
        std::cout<<"UserInfo has destroyed for user: " << user_name << std::endl;
    }
    
    // 设置用户信息
    void setUserInfo(std::string t_user_id, std::string t_user_name, std::string t_user_password){
        user_id = t_user_id;
        user_name = t_user_name;
        user_password = t_user_password;
        if (created_at == 0) {
            created_at = time(nullptr);
        }
    }
    
    // 获取用户信息（控制台输出）
    void getUserInfo(){
        std::cout<<"UserInfo: ID="<<user_id<<" Name="<<user_name<<" Password="<<user_password<<std::endl;
        std::cout<<"Created: " << created_at << " Last Login: " << last_login << std::endl;
    }
    
    // 获取用户信息（返回字符串）
    std::string getUserInfoString() {
        return "UserInfo: ID=" + user_id + " Name=" + user_name + " Created=" + std::to_string(created_at);
    }
    
    // 更新最后登录时间
    void updateLastLogin() {
        last_login = time(nullptr);
    }
    
    // 验证密码
    bool verifyPassword(const std::string& t_password) {
        return user_password == t_password;
    }
    
    // 更新密码
    bool updatePassword(const std::string& t_old_password, const std::string& t_new_password) {
        if (verifyPassword(t_old_password)) {
            user_password = t_new_password;
            return true;
        }
        return false;
    }
    
    // Getter方法
    std::string getUserId() const { return user_id; }
    std::string getUserName() const { return user_name; }
    std::string getUserPassword() const { return user_password; }
    time_t getCreatedAt() const { return created_at; }
    time_t getLastLogin() const { return last_login; }
    user_question_down& getQuestionDownInfo() { return user_question_down_info; }
    
    // Setter方法
    void setUserId(const std::string& t_user_id) { user_id = t_user_id; }
    void setUserName(const std::string& t_user_name) { user_name = t_user_name; }
    void setUserPassword(const std::string& t_password) { user_password = t_password; }
    
    private:
    std::string user_id;
    std::string user_name;
    std::string user_password;
    time_t created_at;
    time_t last_login;
    user_question_down user_question_down_info;
};

#endif //USERINFO_H