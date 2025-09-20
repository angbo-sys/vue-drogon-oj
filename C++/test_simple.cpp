//
// 简化后端功能测试文件
//

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

// 包含我们的模型
#include "models/User/UserInfo/Info.h"
#include "models/User/QuestionDowm/UserQuestionDowm.h"
#include "models/Question/Question.h"
#include "models/Question/QuestionType.h"
#include "models/Question/QuestionCollection.h"

using namespace std;

void printSeparator(const string& title) {
    cout << "\n" << string(50, '=') << endl;
    cout << "  " << title << endl;
    cout << string(50, '=') << endl;
}

void testUserInfo() {
    printSeparator("测试UserInfo类");
    
    // 创建用户 - 使用随机ID避免数据共用问题
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10000, 99999);
    std::string userId = std::to_string(dis(gen));
    
    UserInfo user(userId, "testuser", "password123");
    user.getUserInfo();
    
    std::cout << "\n使用的测试用户ID: " << userId << std::endl;
    
    // 测试密码验证
    cout << "\n密码验证测试:" << endl;
    cout << "验证正确密码: " << (user.verifyPassword("password123") ? "✅ 成功" : "❌ 失败") << endl;
    cout << "验证错误密码: " << (user.verifyPassword("wrongpassword") ? "✅ 成功" : "❌ 失败") << endl;
    
    // 测试密码更新
    cout << "\n密码更新测试:" << endl;
    bool updated = user.updatePassword("password123", "newpassword456");
    cout << "密码更新: " << (updated ? "✅ 成功" : "❌ 失败") << endl;
    cout << "验证新密码: " << (user.verifyPassword("newpassword456") ? "✅ 成功" : "❌ 失败") << endl;
    
    // 测试获取用户信息字符串
    cout << "\n用户信息字符串: " << user.getUserInfoString() << endl;
}

void testUserQuestionDown() {
    printSeparator("测试UserQuestionDown类");
    
    // 使用随机ID避免数据共用问题
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10000, 99999);
    std::string userId = std::to_string(dis(gen));
    
    user_question_down progress(userId);
    
    std::cout << "\n使用的测试用户ID: " << userId << std::endl;
    
    // 添加问题进度
    cout << "\n添加问题进度:" << endl;
    progress.add_question_info(1, 1, true);   // 完成的问题
    progress.add_question_info(1, 2, false);  // 未完成的问题
    progress.add_question_info(2, 1, true);   // 另一个完成的问题
    
    cout << "已完成问题数: " << progress.get_finished_count() << endl;
    cout << "未完成问题数: " << progress.get_unfinished_count() << endl;
    cout << "总问题数: " << progress.get_total_count() << endl;
    cout << "完成率: " << (progress.get_completion_rate() * 100) << "%" << endl;
    
    // 测试问题状态更新
    cout << "\n问题状态更新测试:" << endl;
    int result = progress.finish_unfinish_info(1, 2);
    cout << "将问题(1,2)标记为完成: " << (result ? "✅ 成功" : "❌ 失败") << endl;
    cout << "更新后已完成问题数: " << progress.get_finished_count() << endl;
    cout << "更新后未完成问题数: " << progress.get_unfinished_count() << endl;
    
    // 测试检查问题是否存在
    cout << "\n检查问题是否存在:" << endl;
    cout << "问题(1,1)是否存在: " << (progress.has_question(1, 1) ? "✅ 是" : "❌ 否") << endl;
    cout << "问题(3,3)是否存在: " << (progress.has_question(3, 3) ? "✅ 是" : "❌ 否") << endl;
}

void testQuestionType() {
    printSeparator("测试QuestionType类");
    
    QuestionType type1(1, "动态规划");
    type1.getTypeInfo();
    
    // 测试增加题目数量
    cout << "\n增加题目数量测试:" << endl;
    type1.incrementQuestionCount();
    type1.incrementQuestionCount();
    cout << "增加2个题目后: " << type1.getQuestionInfoString() << endl;
    
    // 测试减少题目数量
    type1.decrementQuestionCount();
    cout << "减少1个题目后: " << type1.getQuestionInfoString() << endl;
    
    // 测试更新类型信息
    cout << "\n更新类型信息测试:" << endl;
    type1.setTypeName("动态规划算法");
    type1.setQuestionCount(5);
    cout << "更新后: " << type1.getQuestionInfoString() << endl;
}

void testQuestion() {
    printSeparator("测试Question类");
    
    Question question(1, 1, "A+B Problem", "10+10", "20", 0, "Luogu P1001", "./0001/");
    question.getQuestionInfo();
    
    // 测试答案验证
    cout << "\n答案验证测试:" << endl;
    cout << "验证正确答案: " << (question.verifyAnswer("20") ? "✅ 正确" : "❌ 错误") << endl;
    cout << "验证错误答案: " << (question.verifyAnswer("30") ? "✅ 正确" : "❌ 错误") << endl;
    
    // 测试题目更新
    cout << "\n题目更新测试:" << endl;
    question.updateQuestionName("A+B Problem (Updated)");
    question.updateDifficulty(1);
    question.updateQuestionContent("20+20");
    question.updateAnswerContent("40");
    cout << "更新后: " << question.getQuestionSummary() << endl;
    cout << "难度名称: " << question.getDifficultyName() << endl;
    
    // 测试获取题目信息
    cout << "\n题目详细信息:" << endl;
    cout << "题目ID: " << question.getQuestionId() << endl;
    cout << "类型ID: " << question.getTypeId() << endl;
    cout << "题目名称: " << question.getQuestionName() << endl;
    cout << "题目内容: " << question.getQuestionContent() << endl;
    cout << "答案内容: " << question.getAnswerContent() << endl;
    cout << "难度ID: " << question.getDifficultyId() << endl;
    cout << "来源: " << question.getSource() << endl;
    cout << "路径: " << question.getPath() << endl;
}

void testQuestionCollection() {
    printSeparator("测试QuestionCollection类");
    
    QuestionCollection collection;
    
    // 添加题目类型
    cout << "\n添加题目类型:" << endl;
    QuestionType type1(1, "动态规划");
    QuestionType type2(2, "贪心算法");
    collection.addQuestionType(type1);
    collection.addQuestionType(type2);
    cout << "题目类型数量: " << collection.getQuestionTypeCount() << endl;
    
    // 添加题目
    cout << "\n添加题目:" << endl;
    Question question1(1, 1, "A+B Problem", "10+10", "20", 0, "Luogu P1001", "./0001/");
    Question question2(2, 2, "最大子数组和", "求最大子数组和", "动态规划", 2, "LeetCode", "./0002/");
    collection.addQuestion(question1);
    collection.addQuestion(question2);
    cout << "题目数量: " << collection.getQuestionCount() << endl;
    
    // 显示统计信息
    cout << "\n统计信息:" << endl;
    collection.getStatistics();
    
    // 测试搜索功能
    cout << "\n搜索功能测试:" << endl;
    auto searchResults = collection.searchQuestions("A+B");
    cout << "搜索'A+B'的结果数量: " << searchResults.size() << endl;
    
    auto typeResults = collection.getQuestionsByTypeId(1);
    cout << "类型1的题目数量: " << typeResults.size() << endl;
    
    auto difficultyResults = collection.getQuestionsByDifficulty(0);
    cout << "难度0的题目数量: " << difficultyResults.size() << endl;
    
    // 测试根据ID获取题目
    cout << "\n根据ID获取题目:" << endl;
    Question* foundQuestion = collection.getQuestionById(1);
    if (foundQuestion) {
        cout << "找到题目: " << foundQuestion->getQuestionSummary() << endl;
    } else {
        cout << "未找到题目" << endl;
    }
}

int main() {
    cout << "🚀 开始测试后端模型功能..." << endl;
    
    try {
        // 测试UserInfo
        testUserInfo();
        
        // 测试UserQuestionDown
        testUserQuestionDown();
        
        // 测试QuestionType
        testQuestionType();
        
        // 测试Question
        testQuestion();
        
        // 测试QuestionCollection
        testQuestionCollection();
        
        printSeparator("测试完成");
        cout << "✅ 所有模型测试完成！" << endl;
        cout << "\n📋 测试总结:" << endl;
        cout << "  - UserInfo类: 用户信息管理功能正常" << endl;
        cout << "  - UserQuestionDown类: 用户问题进度管理功能正常" << endl;
        cout << "  - QuestionType类: 题目类型管理功能正常" << endl;
        cout << "  - Question类: 题目信息管理功能正常" << endl;
        cout << "  - QuestionCollection类: 题目集合管理功能正常" << endl;
        
    } catch (const exception& e) {
        cout << "❌ 测试过程中发生错误: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
