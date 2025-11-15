#include <iostream>
#include <vector>
#include <string>
#include <ctime>

#ifndef USERQUESTIONDOWN_H
#define USERQUESTIONDOWN_H

class question_finish_info_struct
{
    public:
    int type_id;
    int question_id;
    bool is_finish;
    time_t completed_at;  // 完成时间
    int attempts;         // 尝试次数
    
    question_finish_info_struct() : type_id(0), question_id(0), is_finish(false), completed_at(0), attempts(0) {}
    question_finish_info_struct(int t_type_id, int t_question_id, bool t_is_finish) 
        : type_id(t_type_id), question_id(t_question_id), is_finish(t_is_finish), completed_at(0), attempts(1) {
        if (t_is_finish) {
            completed_at = time(nullptr);
        }
    }
};

class user_question_down{
    public:
    // 构造函数
    user_question_down(){
        std::cout<<"Error: No information provided"<<std::endl;
        user_id = "";
        question_finish_number = 0;
        question_hasDoes_number = 0;
    }
    
    user_question_down(std::string t_user_id){
        user_id = t_user_id;
        question_finish_number = 0;
        question_hasDoes_number = 0;
        std::cout<<"user_question_down has created for user: " << user_id << std::endl;
    }
    
    // 析构函数
    ~user_question_down(){
        std::cout<<"user_question_down has destroyed for user: " << user_id << std::endl;
    }
    
    // 添加问题信息
    int add_question_info(int t_type_id, int t_question_id, bool t_is_finish){
        // 检查是否已存在
        if (has_question(t_type_id, t_question_id)) {
            return update_question_status(t_type_id, t_question_id, t_is_finish);
        }
        
        question_finish_info_struct new_question_finish_info(t_type_id, t_question_id, t_is_finish);
        
        if(t_is_finish){
            question_finish_info.push_back(new_question_finish_info);
            question_finish_number++;
        }
        else{
            question_unfinish_info.push_back(new_question_finish_info);
            question_hasDoes_number++;
        }
        return 1;
    }
    
    // 将未完成的问题标记为完成
    int finish_unfinish_info(int t_type_id, int t_question_id){
        for(int i = 0; i < question_unfinish_info.size(); i++){
            if(question_unfinish_info[i].type_id == t_type_id && question_unfinish_info[i].question_id == t_question_id){
                question_unfinish_info[i].is_finish = true;
                question_unfinish_info[i].completed_at = time(nullptr);
                question_finish_info.push_back(question_unfinish_info[i]);
                question_finish_number++;
                question_unfinish_info.erase(question_unfinish_info.begin() + i);
                question_hasDoes_number--;
                return 1;
            }
        }
        return 0; // 未找到
    }
    
    // 更新问题状态
    int update_question_status(int t_type_id, int t_question_id, bool t_is_finish) {
        // 先从未完成列表中查找
        for(int i = 0; i < question_unfinish_info.size(); i++){
            if(question_unfinish_info[i].type_id == t_type_id && question_unfinish_info[i].question_id == t_question_id){
                if (t_is_finish) {
                    question_unfinish_info[i].is_finish = true;
                    question_unfinish_info[i].completed_at = time(nullptr);
                    question_finish_info.push_back(question_unfinish_info[i]);
                    question_finish_number++;
                    question_unfinish_info.erase(question_unfinish_info.begin() + i);
                    question_hasDoes_number--;
                }
                return 1;
            }
        }
        
        // 再从已完成列表中查找
        for(int i = 0; i < question_finish_info.size(); i++){
            if(question_finish_info[i].type_id == t_type_id && question_finish_info[i].question_id == t_question_id){
                if (!t_is_finish) {
                    question_finish_info[i].is_finish = false;
                    question_finish_info[i].completed_at = 0;
                    question_unfinish_info.push_back(question_finish_info[i]);
                    question_hasDoes_number++;
                    question_finish_info.erase(question_finish_info.begin() + i);
                    question_finish_number--;
                }
                return 1;
            }
        }
        
        return 0; // 未找到
    }
    
    // 检查是否有特定问题
    bool has_question(int t_type_id, int t_question_id) {
        // 检查已完成列表
        for(const auto& item : question_finish_info) {
            if(item.type_id == t_type_id && item.question_id == t_question_id) {
                return true;
            }
        }
        
        // 检查未完成列表
        for(const auto& item : question_unfinish_info) {
            if(item.type_id == t_type_id && item.question_id == t_question_id) {
                return true;
            }
        }
        
        return false;
    }
    
    // 获取问题信息（返回副本）
    user_question_down get_question_info(int t_type_id, int t_question_id){
        user_question_down temp_user_question_down(user_id);
        temp_user_question_down.question_finish_number = question_finish_number;
        temp_user_question_down.question_hasDoes_number = question_hasDoes_number;
        temp_user_question_down.question_finish_info = question_finish_info;
        temp_user_question_down.question_unfinish_info = question_unfinish_info;
        return temp_user_question_down;
    }
    
    // 获取统计信息
    int get_finished_count() const { return question_finish_number; }
    int get_unfinished_count() const { return question_hasDoes_number; }
    int get_total_count() const { return question_finish_number + question_hasDoes_number; }
    
    // 获取完成率
    double get_completion_rate() const {
        int total = get_total_count();
        return total > 0 ? (double)question_finish_number / total : 0.0;
    }
    
    // 清空所有数据
    void clear() {
        question_finish_info.clear();
        question_unfinish_info.clear();
        question_finish_number = 0;
        question_hasDoes_number = 0;
    }
    
    // Getter方法
    std::string get_user_id() const { return user_id; }
    const std::vector<question_finish_info_struct>& get_finished_questions() const { return question_finish_info; }
    const std::vector<question_finish_info_struct>& get_unfinished_questions() const { return question_unfinish_info; }
    
    private:
    std::string user_id;
    int question_finish_number;
    int question_hasDoes_number;
    std::vector<question_finish_info_struct> question_finish_info;
    std::vector<question_finish_info_struct> question_unfinish_info;
};


#endif //USERQUESTIONDOWN_H 