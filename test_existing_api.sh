#!/bin/bash

# 现有功能API测试脚本
# 使用方法: ./test_existing_api.sh

BASE_URL="http://localhost:5555"

echo "=========================================="
echo "    现有功能API测试"
echo "=========================================="
echo ""

# 测试1: 获取题目列表
echo "测试1: 获取题目列表"
curl -s "$BASE_URL/api/problems" | jq '.problems[0:3]'  # 只显示前3个题目
echo ""

# 测试2: 获取题目详情
echo "测试2: 获取题目详情 (ID=1)"
curl -s "$BASE_URL/api/problems/1" | jq '.problem | {id, name, description}'  # 只显示关键信息
echo ""

# 测试3: 用户登录
echo "测试3: 用户登录"
curl -s -X POST "$BASE_URL/api/auth/login" \
  -H "Content-Type: application/json" \
  -d '{
    "username": "testuser",
    "password": "testpass"
  }' | jq .
echo ""

# 测试4: 用户注册
echo "测试4: 用户注册"
curl -s -X POST "$BASE_URL/api/auth/register" \
  -H "Content-Type: application/json" \
  -d '{
    "username": "newuser'$(date +%s)'",
    "email": "newuser@example.com",
    "password": "newpass123"
  }' | jq .
echo ""

# 测试5: 提交代码
echo "测试5: 提交代码"
curl -s -X POST "$BASE_URL/api/submit" \
  -H "Content-Type: application/json" \
  -d '{
    "user_id": "1",
    "question_id": 1,
    "code": "#include <iostream>\nusing namespace std;\nint main() { int a, b; cin >> a >> b; cout << a + b << endl; return 0; }",
    "language": "cpp"
  }' | jq '.status, .message, .submission_id'
echo ""

# 测试6: 获取提交历史
echo "测试6: 获取提交历史"
curl -s "$BASE_URL/api/submit/history?user_id=1&page=1&limit=5" | jq '.submissions[0:2]'  # 只显示前2个提交
echo ""

# 测试7: AI聊天
echo "测试7: AI聊天"
curl -s -X POST "$BASE_URL/api/ai/chat" \
  -H "Content-Type: application/json" \
  -d '{
    "user_id": "1",
    "question_id": "1",
    "message": "这道题怎么做？",
    "problem_info": {"name": "A+B Problem"}
  }' | jq '.status, .message'
echo ""

echo "=========================================="
echo "现有功能测试完成！"
echo "=========================================="
