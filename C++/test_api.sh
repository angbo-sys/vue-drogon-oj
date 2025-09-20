#!/bin/bash

echo "=== 测试刷题网站API ==="

# 启动服务器
echo "启动服务器..."
cd build
./your_project_name &
SERVER_PID=$!

# 等待服务器启动
echo "等待服务器启动..."
sleep 5

# 测试基本连接
echo -e "\n1. 测试基本连接..."
curl -s http://localhost:5555/test

# 测试用户列表
echo -e "\n\n2. 测试用户列表..."
curl -s http://localhost:5555/users

# 测试代码提交
echo -e "\n\n3. 测试代码提交..."
curl -X POST http://localhost:5555/api/submit \
  -H "Content-Type: application/json" \
  -H "user-id: 1" \
  -d '{
    "user_id": "1", 
    "question_id": 0, 
    "code": "#include <iostream>\nusing namespace std;\nint main() {\n    int a, b;\n    cin >> a >> b;\n    cout << a + b << endl;\n    return 0;\n}"
  }'

# 测试提交历史
echo -e "\n\n4. 测试提交历史..."
curl -s "http://localhost:5555/api/submit/history?user_id=1"

# 测试提交统计
echo -e "\n\n5. 测试提交统计..."
curl -s "http://localhost:5555/api/submit/statistics?user_id=1"

# 停止服务器
echo -e "\n\n停止服务器..."
kill $SERVER_PID

echo -e "\n=== 测试完成 ==="
