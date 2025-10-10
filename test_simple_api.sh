#!/bin/bash

# 简化版API测试脚本
# 使用方法: ./test_simple_api.sh

BASE_URL="http://localhost:5555"
PROBLEM_ID=1

echo "=========================================="
echo "    评论区和题解功能API快速测试"
echo "=========================================="
echo ""

# 测试1: 获取题目评论列表
echo "测试1: 获取题目评论列表"
curl -s "$BASE_URL/api/comments/$PROBLEM_ID" | jq .
echo ""

# 测试2: 创建评论
echo "测试2: 创建评论"
curl -s -X POST "$BASE_URL/api/comments/create" \
  -H "Content-Type: application/json" \
  -d '{
    "problem_id": 1,
    "user_id": "test_user",
    "username": "测试用户",
    "content": "这是一条测试评论"
  }' | jq .
echo ""

# 测试3: 获取评论总数
echo "测试3: 获取评论总数"
curl -s "$BASE_URL/api/comments/count/$PROBLEM_ID" | jq .
echo ""

# 测试4: 获取题目解析
echo "测试4: 获取题目解析"
curl -s "$BASE_URL/api/analysis/$PROBLEM_ID" | jq .
echo ""

# 测试5: 提交用户解析
echo "测试5: 提交用户解析"
curl -s -X POST "$BASE_URL/api/analysis/submit" \
  -H "Content-Type: application/json" \
  -d '{
    "problem_id": 1,
    "user_id": "test_user",
    "analysis_content": "这是题解分析内容"
  }' | jq .
echo ""

# 测试6: 获取热门解析
echo "测试6: 获取热门解析"
curl -s "$BASE_URL/api/analysis/popular" | jq .
echo ""

echo "=========================================="
echo "测试完成！"
echo "=========================================="
