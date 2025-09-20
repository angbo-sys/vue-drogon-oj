//
// Created by xiaohua jie on 25-9-12.
// 代码执行引擎测试文件
//

#include <iostream>
#include <string>
#include <vector>
#include "services/CodeExecutor.h"

using namespace std;

int main() {
    cout << "=== 代码执行引擎测试 ===" << endl;
    
    // 创建代码执行器
    CodeExecutor executor;
    
    // 测试用例1: A+B Problem
    cout << "\n--- 测试用例1: A+B Problem ---" << endl;
    string code1 = R"(
#include <iostream>
using namespace std;
int main() {
    int a, b;
    cin >> a >> b;
    cout << a + b << endl;
    return 0;
}
)";
    
    vector<TestCase> test_cases1;
    test_cases1.push_back(TestCase("1 2", "3", "Basic addition"));
    test_cases1.push_back(TestCase("10 20", "30", "Larger numbers"));
    test_cases1.push_back(TestCase("0 0", "0", "Zero case"));
    
    ExecutionResult result1 = executor.executeCode(code1, test_cases1, 2, 256);
    
    cout << "状态: " << CodeExecutor::getStatusString(result1.status) << endl;
    cout << "消息: " << result1.message << endl;
    cout << "执行时间: " << result1.execution_time << " 秒" << endl;
    cout << "内存使用: " << result1.memory_usage << " KB" << endl;
    
    if (!result1.test_case_results.empty()) {
        cout << "测试用例结果:" << endl;
        for (const auto& test_result : result1.test_case_results) {
            cout << "  " << test_result << endl;
        }
    }
    
    // 测试用例2: 编译错误
    cout << "\n--- 测试用例2: 编译错误 ---" << endl;
    string code2 = R"(
#include <iostream>
using namespace std;
int main() {
    int a, b;
    cin >> a >> b;
    cout << a + b << endl;
    return 0;
    // 缺少右大括号
)";
    
    vector<TestCase> test_cases2;
    test_cases2.push_back(TestCase("1 2", "3", "Basic addition"));
    
    ExecutionResult result2 = executor.executeCode(code2, test_cases2, 2, 256);
    
    cout << "状态: " << CodeExecutor::getStatusString(result2.status) << endl;
    cout << "消息: " << result2.message << endl;
    if (!result2.error.empty()) {
        cout << "错误信息: " << result2.error << endl;
    }
    
    // 测试用例3: 运行时错误
    cout << "\n--- 测试用例3: 运行时错误 ---" << endl;
    string code3 = R"(
#include <iostream>
using namespace std;
int main() {
    int a, b;
    cin >> a >> b;
    cout << a / b << endl;  // 除零错误
    return 0;
}
)";
    
    vector<TestCase> test_cases3;
    test_cases3.push_back(TestCase("1 0", "3", "Division by zero"));
    
    ExecutionResult result3 = executor.executeCode(code3, test_cases3, 2, 256);
    
    cout << "状态: " << CodeExecutor::getStatusString(result3.status) << endl;
    cout << "消息: " << result3.message << endl;
    if (!result3.error.empty()) {
        cout << "错误信息: " << result3.error << endl;
    }
    
    // 测试用例4: 超时
    cout << "\n--- 测试用例4: 超时测试 ---" << endl;
    string code4 = R"(
#include <iostream>
using namespace std;
int main() {
    int a, b;
    cin >> a >> b;
    // 无限循环
    while (true) {
        a++;
    }
    cout << a + b << endl;
    return 0;
}
)";
    
    vector<TestCase> test_cases4;
    test_cases4.push_back(TestCase("1 2", "3", "Infinite loop"));
    
    ExecutionResult result4 = executor.executeCode(code4, test_cases4, 1, 256); // 1秒超时
    
    cout << "状态: " << CodeExecutor::getStatusString(result4.status) << endl;
    cout << "消息: " << result4.message << endl;
    
    cout << "\n=== 测试完成 ===" << endl;
    
    return 0;
}
