

#include "../models/Question/Question.h"
#include "../models/Question/QuestionType.h"
#include "../models/Question/QuestionCollection.h"
#include <drogon/drogon.h>
#include <json/json.h>
#include <fstream>
#include <string>

class QuestionService
{
    public:
    QuestionService();
    
    // 题目类型相关
    Json::Value getAllQuestionTypes();
    Json::Value getQuestionTypeById(int t_type_id);
    Json::Value addQuestionType(const std::string& t_type_name);
    Json::Value updateQuestionType(int t_type_id, const std::string& t_type_name);
    Json::Value deleteQuestionType(int t_type_id);
    
    // 题目相关
    Json::Value getAllQuestions();
    Json::Value getQuestionById(int t_question_id);
    Json::Value getQuestionsByTypeId(int t_type_id);
    Json::Value getQuestionsByDifficulty(int t_difficulty_id);
    Json::Value searchQuestions(const std::string& t_keyword);
    Json::Value addQuestion(int t_type_id, const std::string& t_question_name, 
                           const std::string& t_question_content, const std::string& t_answer_content,
                           int t_difficulty_id = 0, const std::string& t_source = "", const std::string& t_path = "");
    Json::Value updateQuestion(int t_question_id, const std::string& t_question_name,
                              const std::string& t_question_content, const std::string& t_answer_content,
                              int t_difficulty_id = -1, const std::string& t_source = "", const std::string& t_path = "");
    Json::Value deleteQuestion(int t_question_id);
    
    // 统计相关
    Json::Value getQuestionStatistics();
    Json::Value getQuestionCountByType(int t_type_id);
    Json::Value getQuestionCountByDifficulty(int t_difficulty_id);
    
    // 题目验证
    Json::Value verifyAnswer(int t_question_id, const std::string& t_user_answer);
    
    private:
    // JSON文件操作
    Json::Value loadQuestionData();
    bool saveQuestionData(const Json::Value& t_data);
    Json::Value loadQuestionTypeData();
    bool saveQuestionTypeData(const Json::Value& t_data);
    
    // 文件路径
    std::string getQuestionDataPath();
    std::string getQuestionTypeDataPath();
    
    // 辅助方法
    int generateQuestionId();
    int generateTypeId();
    bool questionExists(int t_question_id);
    bool questionTypeExists(int t_type_id);
    
    // 数据转换
    Json::Value questionToJson(const Question& t_question);
    Json::Value questionTypeToJson(const QuestionType& t_type);
    Question jsonToQuestion(const Json::Value& t_json);
    QuestionType jsonToQuestionType(const Json::Value& t_json);
};
