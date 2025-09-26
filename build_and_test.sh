#!/bin/bash

# 编译和测试脚本

echo "开始编译和测试AI聊天功能..."

# 进入C++项目目录
cd "C++ 2"

echo "1. 清理之前的编译文件..."
make clean 2>/dev/null || true

echo "2. 编译项目..."
if make; then
    echo "✓ 编译成功"
else
    echo "✗ 编译失败"
    exit 1
fi

echo ""
echo "3. 启动后端服务..."
echo "正在启动服务器，请稍等..."

# 在后台启动服务器
./your_project_name &
SERVER_PID=$!

# 等待服务器启动
sleep 3

# 检查服务器是否启动成功
if curl -s http://localhost:5555/ > /dev/null; then
    echo "✓ 后端服务启动成功 (PID: $SERVER_PID)"
else
    echo "✗ 后端服务启动失败"
    kill $SERVER_PID 2>/dev/null
    exit 1
fi

echo ""
echo "4. 运行AI聊天测试..."
cd ..
chmod +x test_deepseek_api.sh
./test_deepseek_api.sh

echo ""
echo "5. 清理..."
echo "停止后端服务..."
kill $SERVER_PID 2>/dev/null

echo ""
echo "测试完成！"
echo ""
echo "如果测试成功，你可以："
echo "1. 启动后端: cd 'C++ 2' && ./your_project_name"
echo "2. 启动前端: cd newVue/Db-frontend && npm run serve"
echo "3. 在浏览器中测试AI聊天功能"
