#!/bin/bash

echo "=== 前端功能测试 ==="

# 检查后端服务器
echo "1. 检查后端服务器状态..."
if curl -s http://localhost:5555/test > /dev/null; then
    echo "✅ 后端服务器运行正常"
else
    echo "❌ 后端服务器未运行，请先启动后端"
    exit 1
fi

# 检查用户端前端
echo "2. 检查用户端前端..."
if curl -s http://localhost:3000 > /dev/null; then
    echo "✅ 用户端前端运行正常 (http://localhost:3000)"
else
    echo "❌ 用户端前端未运行，请运行: cd VUE/drogon-vue-app && npm run serve"
fi

# 检查管理员端前端
echo "3. 检查管理员端前端..."
if curl -s http://localhost:3001 > /dev/null; then
    echo "✅ 管理员端前端运行正常 (http://localhost:3001)"
else
    echo "❌ 管理员端前端未运行，请运行: cd admin-next && npm run dev"
fi

# 测试API接口
echo "4. 测试API接口..."

echo "测试用户列表API..."
curl -s http://localhost:5555/users | jq .

echo "测试代码提交API..."
curl -X POST http://localhost:5555/api/submit \
  -H "Content-Type: application/json" \
  -d '{
    "user_id": "1",
    "question_id": 0,
    "code": "#include <iostream>\nusing namespace std;\nint main() {\n    int a, b;\n    cin >> a >> b;\n    cout << a + b << endl;\n    return 0;\n}",
    "language": "cpp"
  }' | jq .

echo "测试提交历史API..."
curl -s "http://localhost:5555/api/submit/history?user_id=1" | jq .

echo "测试统计信息API..."
curl -s "http://localhost:5555/api/submit/statistics?user_id=1" | jq .

echo "=== 测试完成 ==="
echo ""
echo "访问地址："
echo "用户端: http://localhost:3000"
echo "管理员端: http://localhost:3001"
echo "后端API: http://localhost:5555"
