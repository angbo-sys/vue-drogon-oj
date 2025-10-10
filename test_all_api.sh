#!/bin/bash

# 综合API测试脚本
# 使用方法: ./test_all_api.sh

BASE_URL="http://localhost:5555"
PROBLEM_ID=1
USER_ID="test_user_$(date +%s)"

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}        综合API测试脚本${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# 测试函数
test_endpoint() {
    local name="$1"
    local method="$2"
    local url="$3"
    local data="$4"
    
    echo -e "${YELLOW}测试: $name${NC}"
    
    if [ "$method" = "GET" ]; then
        response=$(curl -s "$BASE_URL$url")
    else
        response=$(curl -s -X "$method" \
            -H "Content-Type: application/json" \
            -d "$data" \
            "$BASE_URL$url")
    fi
    
    # 检查响应是否包含错误
    if echo "$response" | grep -q '"status":"error"'; then
        echo -e "${RED}❌ 失败${NC}"
        echo "$response" | jq -r '.message' 2>/dev/null || echo "$response"
    else
        echo -e "${GREEN}✅ 成功${NC}"
        echo "$response" | jq -r '.status' 2>/dev/null || echo "响应格式异常"
    fi
    echo ""
}

echo -e "${BLUE}1. 测试现有功能...${NC}"

# 现有功能测试
test_endpoint "获取题目列表" "GET" "/api/problems"
test_endpoint "获取题目详情" "GET" "/api/problems/$PROBLEM_ID"
test_endpoint "用户注册" "POST" "/api/auth/register" '{
    "username": "'$USER_ID'",
    "email": "'$USER_ID'@example.com",
    "password": "testpass123"
}'
test_endpoint "用户登录" "POST" "/api/auth/login" '{
    "username": "'$USER_ID'",
    "password": "testpass123"
}'

echo -e "${BLUE}2. 测试代码提交功能...${NC}"

# 代码提交测试
test_endpoint "提交代码" "POST" "/api/submit" '{
    "user_id": "'$USER_ID'",
    "question_id": '$PROBLEM_ID',
    "code": "#include <iostream>\nusing namespace std;\nint main() { int a, b; cin >> a >> b; cout << a + b << endl; return 0; }",
    "language": "cpp"
}'

echo -e "${BLUE}3. 测试评论功能...${NC}"

# 评论功能测试
test_endpoint "获取评论列表" "GET" "/api/comments/$PROBLEM_ID"
test_endpoint "创建评论" "POST" "/api/comments/create" '{
    "problem_id": '$PROBLEM_ID',
    "user_id": "'$USER_ID'",
    "username": "测试用户",
    "content": "这是一条测试评论，用于验证评论功能。"
}'
test_endpoint "获取评论总数" "GET" "/api/comments/count/$PROBLEM_ID"

echo -e "${BLUE}4. 测试题解分析功能...${NC}"

# 题解分析功能测试
test_endpoint "获取题目解析" "GET" "/api/analysis/$PROBLEM_ID"
test_endpoint "提交用户解析" "POST" "/api/analysis/submit" '{
    "problem_id": '$PROBLEM_ID',
    "user_id": "'$USER_ID'",
    "analysis_content": "这是题解分析内容，包含详细的解题思路和代码实现。"
}'
test_endpoint "获取热门解析" "GET" "/api/analysis/popular"

echo -e "${BLUE}5. 测试AI聊天功能...${NC}"

# AI聊天功能测试
test_endpoint "AI聊天" "POST" "/api/ai/chat" '{
    "user_id": "'$USER_ID'",
    "question_id": "'$PROBLEM_ID'",
    "message": "这道题怎么做？",
    "problem_info": {"name": "A+B Problem"}
}'

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}           测试完成${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""
echo -e "${GREEN}所有API端点测试完成！${NC}"
echo -e "${YELLOW}如果看到 ✅ 成功，说明API工作正常${NC}"
echo -e "${YELLOW}如果看到 ❌ 失败，请检查服务器日志${NC}"
