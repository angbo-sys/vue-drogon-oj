#!/bin/bash

# API测试脚本 - 评论区和题解功能
# 使用方法: ./test_api.sh

# 设置颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# API基础URL
BASE_URL="http://localhost:5555"

# 测试用的数据
PROBLEM_ID=1
USER_ID="test_user_123"
USERNAME="测试用户"
COMMENT_CONTENT="这是一条测试评论，用于验证评论功能是否正常工作。"
ANALYSIS_CONTENT="这是题解分析内容，包含详细的解题思路和代码实现。"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}    评论区和题解功能API测试脚本${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# 测试计数器
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# 测试函数
test_api() {
    local test_name="$1"
    local method="$2"
    local url="$3"
    local data="$4"
    local expected_status="$5"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${YELLOW}测试 $TOTAL_TESTS: $test_name${NC}"
    echo "请求: $method $url"
    
    if [ "$method" = "GET" ]; then
        response=$(curl -s -w "\n%{http_code}" "$BASE_URL$url")
    else
        response=$(curl -s -w "\n%{http_code}" -X "$method" \
            -H "Content-Type: application/json" \
            -d "$data" \
            "$BASE_URL$url")
    fi
    
    # 分离响应体和状态码
    http_code=$(echo "$response" | tail -n1)
    response_body=$(echo "$response" | head -n -1)
    
    echo "状态码: $http_code"
    echo "响应: $response_body"
    
    if [ "$http_code" = "$expected_status" ]; then
        echo -e "${GREEN}✓ 通过${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ 失败 (期望: $expected_status, 实际: $http_code)${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
    fi
    
    echo ""
}

# 等待用户确认
echo -e "${YELLOW}准备开始测试，请确保服务器已启动在端口5555${NC}"
echo -e "${YELLOW}按任意键继续，或Ctrl+C取消...${NC}"
read -n 1 -s

echo ""
echo -e "${BLUE}开始测试评论功能...${NC}"
echo ""

# 1. 测试获取题目评论列表
test_api "获取题目评论列表" "GET" "/api/comments/$PROBLEM_ID?page=1&page_size=10" "" "200"

# 2. 测试获取评论总数
test_api "获取评论总数" "GET" "/api/comments/count/$PROBLEM_ID" "" "200"

# 3. 测试创建评论
create_comment_data='{
    "problem_id": '$PROBLEM_ID',
    "user_id": "'$USER_ID'",
    "username": "'$USERNAME'",
    "content": "'$COMMENT_CONTENT'"
}'
test_api "创建评论" "POST" "/api/comments/create" "$create_comment_data" "200"

# 4. 再次获取评论列表，验证评论已创建
test_api "验证评论已创建" "GET" "/api/comments/$PROBLEM_ID?page=1&page_size=10" "" "200"

# 5. 测试获取评论详情
test_api "获取评论详情" "GET" "/api/comments/detail?comment_id=1" "" "200"

echo -e "${BLUE}开始测试题解分析功能...${NC}"
echo ""

# 6. 测试获取题目解析
test_api "获取题目解析" "GET" "/api/analysis/$PROBLEM_ID" "" "200"

# 7. 测试提交用户解析
submit_analysis_data='{
    "problem_id": '$PROBLEM_ID',
    "user_id": "'$USER_ID'",
    "analysis_content": "'$ANALYSIS_CONTENT'"
}'
test_api "提交用户解析" "POST" "/api/analysis/submit" "$submit_analysis_data" "200"

# 8. 测试获取解析点赞数
test_api "获取解析点赞数" "GET" "/api/analysis/likes/1" "" "200"

# 9. 测试点赞解析
like_analysis_data='{
    "analysis_id": 1,
    "user_id": "'$USER_ID'"
}'
test_api "点赞解析" "POST" "/api/analysis/like" "$like_analysis_data" "200"

# 10. 测试获取热门解析
test_api "获取热门解析" "GET" "/api/analysis/popular?limit=5" "" "200"

echo -e "${BLUE}开始测试错误处理...${NC}"
echo ""

# 11. 测试无效的题目ID
test_api "无效题目ID测试" "GET" "/api/comments/99999" "" "200"

# 12. 测试缺少参数的请求
test_api "缺少参数测试" "POST" "/api/comments/create" '{}' "400"

# 13. 测试无效的解析ID
test_api "无效解析ID测试" "GET" "/api/analysis/likes/99999" "" "200"

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}           测试结果汇总${NC}"
echo -e "${BLUE}========================================${NC}"
echo "总测试数: $TOTAL_TESTS"
echo -e "通过: ${GREEN}$PASSED_TESTS${NC}"
echo -e "失败: ${RED}$FAILED_TESTS${NC}"

if [ $FAILED_TESTS -eq 0 ]; then
    echo -e "${GREEN}🎉 所有测试通过！${NC}"
    exit 0
else
    echo -e "${RED}❌ 有 $FAILED_TESTS 个测试失败${NC}"
    exit 1
fi
