//
// Created by xiaohua jie on 25-9-12.
// 代码执行引擎实现
//

#include "CodeExecutor.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <sys/wait.h>
#include <sys/resource.h>
#include <unistd.h>
#include <signal.h>
#include <filesystem>
#include <chrono>
#include <algorithm>

CodeExecutor::CodeExecutor() {
    // 设置临时文件目录
    temp_dir = "/tmp/code_executor/";
    
    // 创建临时目录
    std::filesystem::create_directories(temp_dir);
    
    // 设置编译器路径和选项
    compiler_path = "g++";
    compiler_flags = "-std=c++17 -O2 -Wall -Wextra";
}

CodeExecutor::~CodeExecutor() {
    // 清理临时目录
    try {
        std::filesystem::remove_all(temp_dir);
    } catch (const std::exception& e) {
        std::cerr << "Warning: Failed to clean up temp directory: " << e.what() << std::endl;
    }
}

ExecutionResult CodeExecutor::executeCode(const std::string& code, 
                                        const std::vector<TestCase>& test_cases,
                                        int time_limit, 
                                        int memory_limit) {
    ExecutionResult result;
    
    try {
        // 生成临时文件路径
        std::string base_path = generateTempPath();
        std::string cpp_file = base_path + ".cpp";
        std::string exe_file = base_path;
        
        std::cout << "Generated files: " << cpp_file << " -> " << exe_file << std::endl;
        
        // 写入代码到文件
        if (!writeCodeToFile(code, cpp_file)) {
            result.status = ExecutionStatus::SE;
            result.message = "Failed to write code to file";
            return result;
        }
        
        // 编译代码
        result = compileCode(code, exe_file);
        if (result.status != ExecutionStatus::AC) {
            cleanup(cpp_file);
            return result;
        }
        
        // 运行程序
        result = runProgram(exe_file, test_cases, time_limit, memory_limit);
        
        // 清理文件
        cleanup(cpp_file);
        cleanup(exe_file);
        
    } catch (const std::exception& e) {
        result.status = ExecutionStatus::SE;
        result.message = "System error: " + std::string(e.what());
    }
    
    return result;
}

ExecutionResult CodeExecutor::compileCode(const std::string& code, const std::string& output_path) {
    ExecutionResult result;
    
    try {
        // 构建编译命令
        std::string command = compiler_path + " " + compiler_flags + " -o " + output_path + " " + output_path + ".cpp 2>&1";
        
        std::cout << "Compile command: " << command << std::endl;
        
        // 执行编译
        std::string compile_output = executeCommand(command);
        
        // 检查编译是否成功
        if (!fileExists(output_path)) {
            result.status = ExecutionStatus::CE;
            result.message = "Compilation failed";
            result.error = compile_output;
        } else {
            result.status = ExecutionStatus::AC;
            result.message = "Compilation successful";
        }
        
    } catch (const std::exception& e) {
        result.status = ExecutionStatus::SE;
        result.message = "Compilation error: " + std::string(e.what());
    }
    
    return result;
}

ExecutionResult CodeExecutor::runProgram(const std::string& program_path,
                                       const std::vector<TestCase>& test_cases,
                                       int time_limit,
                                       int memory_limit) {
    ExecutionResult result;
    result.status = ExecutionStatus::AC;
    result.message = "All test cases passed";
    
    try {
        for (size_t i = 0; i < test_cases.size(); ++i) {
            const auto& test_case = test_cases[i];
            
            // 创建临时输入文件
            std::string input_file = generateTempPath() + "_input.txt";
            std::string output_file = generateTempPath() + "_output.txt";
            
            // 写入输入数据
            std::ofstream input_stream(input_file);
            if (!input_stream.is_open()) {
                result.status = ExecutionStatus::SE;
                result.message = "Failed to create input file";
                cleanup(input_file);
                cleanup(output_file);
                return result;
            }
            input_stream << test_case.input;
            input_stream.close();
            
            // 构建运行命令 (macOS使用gtimeout，如果没有则使用perl实现超时)
            std::string command = "perl -e 'alarm(" + std::to_string(time_limit) + "); exec \"sh\", \"-c\", \"" + 
                                program_path + " < " + input_file + " > " + output_file + " 2>&1\"'";
            
            // 记录开始时间
            auto start_time = std::chrono::high_resolution_clock::now();
            
            // 执行程序
            int exit_code = system(command.c_str());
            
            // 记录结束时间
            auto end_time = std::chrono::high_resolution_clock::now();
            double execution_time = std::chrono::duration<double>(end_time - start_time).count();
            
            // 更新总执行时间
            result.execution_time = std::max(result.execution_time, execution_time);
            
            // 检查执行结果
            if (exit_code == 142) { // perl alarm信号返回142表示超时
                result.status = ExecutionStatus::TLE;
                result.message = "Time limit exceeded on test case " + std::to_string(i + 1);
                cleanup(input_file);
                cleanup(output_file);
                return result;
            } else if (exit_code != 0) {
                result.status = ExecutionStatus::RE;
                result.message = "Runtime error on test case " + std::to_string(i + 1);
                result.error = readFile(output_file);
                cleanup(input_file);
                cleanup(output_file);
                return result;
            }
            
            // 读取输出
            std::string actual_output = readFile(output_file);
            
            // 验证输出
            if (!validateOutput(actual_output, test_case.expected_output)) {
                result.status = ExecutionStatus::WA;
                result.message = "Wrong answer on test case " + std::to_string(i + 1);
                result.output = actual_output;
                cleanup(input_file);
                cleanup(output_file);
                return result;
            }
            
            // 记录测试用例结果
            result.test_case_results.push_back("Test case " + std::to_string(i + 1) + ": AC");
            
            // 清理临时文件
            cleanup(input_file);
            cleanup(output_file);
        }
        
    } catch (const std::exception& e) {
        result.status = ExecutionStatus::SE;
        result.message = "Runtime error: " + std::string(e.what());
    }
    
    return result;
}

bool CodeExecutor::validateOutput(const std::string& actual_output, const std::string& expected_output) {
    // 去除末尾的空白字符进行比较
    std::string actual = actual_output;
    std::string expected = expected_output;
    
    // 去除末尾的换行符和空格
    while (!actual.empty() && (actual.back() == '\n' || actual.back() == '\r' || actual.back() == ' ')) {
        actual.pop_back();
    }
    while (!expected.empty() && (expected.back() == '\n' || expected.back() == '\r' || expected.back() == ' ')) {
        expected.pop_back();
    }
    
    return actual == expected;
}

void CodeExecutor::cleanup(const std::string& file_path) {
    try {
        if (fileExists(file_path)) {
            std::filesystem::remove(file_path);
        }
    } catch (const std::exception& e) {
        std::cerr << "Warning: Failed to clean up file " << file_path << ": " << e.what() << std::endl;
    }
}

std::string CodeExecutor::getStatusString(ExecutionStatus status) {
    switch (status) {
        case ExecutionStatus::AC: return "AC";
        case ExecutionStatus::WA: return "WA";
        case ExecutionStatus::TLE: return "TLE";
        case ExecutionStatus::RE: return "RE";
        case ExecutionStatus::CE: return "CE";
        case ExecutionStatus::MLE: return "MLE";
        case ExecutionStatus::SE: return "SE";
        default: return "UNKNOWN";
    }
}

std::string CodeExecutor::getStatusDescription(ExecutionStatus status) {
    switch (status) {
        case ExecutionStatus::AC: return "Accepted";
        case ExecutionStatus::WA: return "Wrong Answer";
        case ExecutionStatus::TLE: return "Time Limit Exceeded";
        case ExecutionStatus::RE: return "Runtime Error";
        case ExecutionStatus::CE: return "Compilation Error";
        case ExecutionStatus::MLE: return "Memory Limit Exceeded";
        case ExecutionStatus::SE: return "System Error";
        default: return "Unknown Error";
    }
}

std::string CodeExecutor::generateTempPath() {
    // 生成基于时间戳的唯一文件名
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    
    return temp_dir + "code_" + std::to_string(timestamp) + "_" + std::to_string(rand());
}

bool CodeExecutor::writeCodeToFile(const std::string& code, const std::string& file_path) {
    try {
        std::ofstream file(file_path);
        if (!file.is_open()) {
            return false;
        }
        file << code;
        file.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error writing code to file: " << e.what() << std::endl;
        return false;
    }
}

std::string CodeExecutor::executeCommand(const std::string& command) {
    std::string result;
    char buffer[128];
    
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "Failed to execute command";
    }
    
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    
    pclose(pipe);
    return result;
}

bool CodeExecutor::fileExists(const std::string& file_path) {
    return std::filesystem::exists(file_path);
}

long CodeExecutor::getFileSize(const std::string& file_path) {
    try {
        return std::filesystem::file_size(file_path);
    } catch (const std::exception& e) {
        return 0;
    }
}

std::string CodeExecutor::readFile(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        return "";
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    file.close();
    return content;
}

void CodeExecutor::setResourceLimits(int time_limit, int memory_limit) {
    // 设置内存限制 (KB)
    struct rlimit mem_limit;
    mem_limit.rlim_cur = memory_limit * 1024; // 转换为KB
    mem_limit.rlim_max = memory_limit * 1024;
    setrlimit(RLIMIT_AS, &mem_limit);
}
