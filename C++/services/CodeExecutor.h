//
// Created by xiaohua jie on 25-9-12.
// 代码执行引擎 - 负责编译和执行C++代码
//

#ifndef CODEEXECUTOR_H
#define CODEEXECUTOR_H

#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <json/json.h>

// 执行结果状态枚举
enum class ExecutionStatus {
    AC,     // Accepted (通过)
    WA,     // Wrong Answer (错误答案)
    TLE,    // Time Limit Exceeded (超时)
    RE,     // Runtime Error (运行时错误)
    CE,     // Compilation Error (编译错误)
    MLE,    // Memory Limit Exceeded (内存超限)
    SE      // System Error (系统错误)
};

// 执行结果结构
struct ExecutionResult {
    ExecutionStatus status;
    std::string message;
    double execution_time;    // 执行时间(秒)
    long memory_usage;        // 内存使用(KB)
    std::string output;       // 程序输出
    std::string error;        // 错误信息
    std::vector<std::string> test_case_results; // 测试用例结果
    
    ExecutionResult() : status(ExecutionStatus::SE), execution_time(0.0), memory_usage(0) {}
};

// 测试用例结构
struct TestCase {
    std::string input;
    std::string expected_output;
    std::string description;
    
    TestCase() = default;
    TestCase(const std::string& in, const std::string& out, const std::string& desc = "")
        : input(in), expected_output(out), description(desc) {}
};

class CodeExecutor {
public:
    // 构造函数
    CodeExecutor();
    
    // 析构函数
    ~CodeExecutor();
    
    // 执行代码并返回结果
    ExecutionResult executeCode(const std::string& code, 
                               const std::vector<TestCase>& test_cases,
                               int time_limit = 2,      // 2秒时间限制
                               int memory_limit = 256); // 256MB内存限制
    
    // 编译C++代码
    ExecutionResult compileCode(const std::string& code, const std::string& output_path);
    
    // 运行编译后的程序
    ExecutionResult runProgram(const std::string& program_path,
                              const std::vector<TestCase>& test_cases,
                              int time_limit,
                              int memory_limit);
    
    // 验证输出结果
    bool validateOutput(const std::string& actual_output, const std::string& expected_output);
    
    // 清理临时文件
    void cleanup(const std::string& file_path);
    
    // 获取状态字符串
    static std::string getStatusString(ExecutionStatus status);
    
    // 获取状态描述
    static std::string getStatusDescription(ExecutionStatus status);

private:
    // 生成临时文件路径
    std::string generateTempPath();
    
    // 写入代码到文件
    bool writeCodeToFile(const std::string& code, const std::string& file_path);
    
    // 执行系统命令
    std::string executeCommand(const std::string& command);
    
    // 检查文件是否存在
    bool fileExists(const std::string& file_path);
    
    // 获取文件大小
    long getFileSize(const std::string& file_path);
    
    // 读取文件内容
    std::string readFile(const std::string& file_path);
    
    // 设置资源限制
    void setResourceLimits(int time_limit, int memory_limit);
    
    // 临时文件目录
    std::string temp_dir;
    
    // 编译器路径
    std::string compiler_path;
    
    // 编译器选项
    std::string compiler_flags;
};

#endif //CODEEXECUTOR_H
