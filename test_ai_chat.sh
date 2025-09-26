#!/bin/bash

# AI聊天功能测试脚本

echo "测试AI聊天功能..."

# 测试AI聊天API
echo "1. 测试AI聊天API..."
curl -X POST http://localhost:5555/api/ai/chat \
  -H "Content-Type: application/json" \
  -d '{
    "user_id": "123",
    "question_id": "1",
    "message": "这道题怎么理解？",
    "problem_info": {
      "name": "两数之和",
      "description": "给定一个整数数组和一个目标值，找出数组中和为目标值的两个数。",
      "difficulty": "1",
      "tags": ["数组", "哈希表"]
    }
  }' | jq '.'

echo ""
echo "2. 测试不同的问题类型..."

# 测试算法问题
curl -X POST http://localhost:5555/api/ai/chat \
  -H "Content-Type: application/json" \
  -d '{
    "user_id": "123",
    "question_id": "1", 
    "message": "这道题用什么算法比较好？",
    "problem_info": {
      "name": "两数之和",
      "description": "给定一个整数数组和一个目标值，找出数组中和为目标值的两个数。",
      "difficulty": "1",
      "tags": ["数组", "哈希表"]
    }
  }' | jq '.'

echo ""
echo "3. 测试代码实现问题..."

curl -X POST http://localhost:5555/api/ai/chat \
  -H "Content-Type: application/json" \
  -d '{
    "user_id": "123",
    "question_id": "1",
    "message": "如何实现这个算法？",
    "problem_info": {
      "name": "两数之和", 
      "description": "给定一个整数数组和一个目标值，找出数组中和为目标值的两个数。",
      "difficulty": "1",
      "tags": ["数组", "哈希表"]
    }
  }' | jq '.'

echo ""
echo "测试完成！"
echo ""
echo "如果看到JSON响应，说明AI聊天功能正常工作。"
echo "现在可以在前端界面中测试聊天功能了。"
