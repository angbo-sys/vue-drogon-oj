#!/bin/bash

# DeepSeek API集成测试脚本

echo "测试DeepSeek API集成..."

# 检查后端服务是否运行
echo "1. 检查后端服务状态..."
if curl -s http://localhost:5555/ > /dev/null; then
    echo "✓ 后端服务正在运行"
else
    echo "✗ 后端服务未运行，请先启动后端服务"
    echo "  启动命令: cd 'C++ 2' && make && ./your_project_name"
    exit 1
fi

echo ""
echo "2. 测试AI聊天API..."

# 测试基本AI聊天功能
echo "发送测试消息: '这道题怎么理解？'"
response=$(curl -s -X POST http://localhost:5555/api/ai/chat \
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
  }')

echo "API响应:"
echo "$response" | jq '.'

# 检查响应状态
status=$(echo "$response" | jq -r '.status')
if [ "$status" = "success" ]; then
    echo "✓ AI聊天API调用成功"
    
    # 检查是否使用了DeepSeek API
    note=$(echo "$response" | jq -r '.data.note // empty')
    if [ -n "$note" ]; then
        echo "⚠ 使用了模拟响应: $note"
        echo "  这可能表示DeepSeek API调用失败，请检查API密钥和网络连接"
    else
        echo "✓ 成功调用DeepSeek API"
    fi
else
    echo "✗ AI聊天API调用失败"
    echo "错误信息: $(echo "$response" | jq -r '.message')"
fi

echo ""
echo "3. 测试不同的问题类型..."

# 测试算法问题
echo "发送算法问题: '这道题用什么算法比较好？'"
response2=$(curl -s -X POST http://localhost:5555/api/ai/chat \
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
  }')

echo "算法问题响应:"
echo "$response2" | jq '.data.response'

echo ""
echo "4. 测试代码实现问题..."

# 测试代码实现问题
echo "发送代码实现问题: '如何实现这个算法？'"
response3=$(curl -s -X POST http://localhost:5555/api/ai/chat \
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
  }')

echo "代码实现问题响应:"
echo "$response3" | jq '.data.response'

echo ""
echo "测试完成！"
echo ""
echo "如果看到详细的AI回复，说明DeepSeek API集成成功。"
echo "如果看到'使用模拟响应'的提示，请检查："
echo "1. API密钥是否正确"
echo "2. 网络连接是否正常"
echo "3. DeepSeek服务是否可用"
echo ""
echo "现在可以在前端界面中测试AI聊天功能了！"
