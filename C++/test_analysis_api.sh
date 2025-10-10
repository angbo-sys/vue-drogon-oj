#!/bin/bash

# 题解功能测试脚本
echo "=========================================="
echo "    题解功能API测试"
echo "=========================================="
echo ""

BASE_URL="http://localhost:5555"
PROBLEM_ID=1
USER_ID="test_user_$(date +%s)"

echo "1. 测试获取题解列表..."
curl -s "$BASE_URL/api/analysis/$PROBLEM_ID" | jq '.status, .analysis_count'
echo ""

echo "2. 测试提交题解..."
curl -s -X POST "$BASE_URL/api/analysis/submit" \
  -H "Content-Type: application/json" \
  -d '{
    "question_id": '$PROBLEM_ID',
    "user_id": "'$USER_ID'",
    "analysis_content": "## 解题思路\n\n这是一道简单的A+B问题。\n\n### 算法分析\n1. 读取两个整数\n2. 计算它们的和\n3. 输出结果\n\n### 代码实现\n```cpp\n#include <iostream>\nusing namespace std;\n\nint main() {\n    int a, b;\n    cin >> a >> b;\n    cout << a + b << endl;\n    return 0;\n}\n```"
  }' | jq '.status, .analysis_id'
echo ""

echo "3. 再次获取题解列表验证..."
curl -s "$BASE_URL/api/analysis/$PROBLEM_ID" | jq '.status, .analysis_count'
echo ""

echo "=========================================="
echo "题解功能测试完成！"
echo "=========================================="
