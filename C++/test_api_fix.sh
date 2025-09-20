#!/bin/bash

echo "=== 测试API修复 ==="

# 检查后端服务器
echo "1. 检查后端服务器状态..."
if curl -s http://localhost:5555/test > /dev/null; then
    echo "✅ 后端服务器运行正常"
else
    echo "❌ 后端服务器未运行"
    exit 1
fi

# 检查用户端前端
echo "2. 检查用户端前端..."
if curl -s http://localhost:3000 > /dev/null; then
    echo "✅ 用户端前端运行正常 (http://localhost:3000)"
else
    echo "❌ 用户端前端未运行"
fi

# 检查管理员端前端
echo "3. 检查管理员端前端..."
if curl -s http://localhost:3001 > /dev/null; then
    echo "✅ 管理员端前端运行正常 (http://localhost:3001)"
else
    echo "❌ 管理员端前端未运行"
fi

# 测试API接口
echo "4. 测试API接口..."

echo "测试提交历史API..."
curl -s "http://localhost:5555/api/submit/history?user_id=1" | jq '.status'

echo "测试统计信息API..."
curl -s "http://localhost:5555/api/submit/statistics?user_id=1" | jq '.status'

echo "测试代码提交API..."
curl -X POST http://localhost:5555/api/submit \
  -H "Content-Type: application/json" \
  -d '{
    "user_id": "1",
    "question_id": 0,
    "code": "#include <iostream>\nusing namespace std;\nint main() {\n    int a, b;\n    cin >> a >> b;\n    cout << a + b << endl;\n    return 0;\n}",
    "language": "cpp"
  }' | jq '.status'

echo ""
echo "=== 测试完成 ==="
echo "现在可以访问："
echo "用户端: http://localhost:3000"
echo "管理员端: http://localhost:3001"
echo "后端API: http://localhost:5555"
