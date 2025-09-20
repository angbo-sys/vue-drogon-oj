#!/bin/bash

echo "🚀 启动OJ前端项目..."

# 检查Node.js版本
node_version=$(node -v 2>/dev/null)
if [ $? -ne 0 ]; then
    echo "❌ 错误: 未安装Node.js，请先安装Node.js"
    exit 1
fi

echo "✅ Node.js版本: $node_version"

# 检查npm
npm_version=$(npm -v 2>/dev/null)
if [ $? -ne 0 ]; then
    echo "❌ 错误: 未安装npm，请先安装npm"
    exit 1
fi

echo "✅ npm版本: $npm_version"

# 安装依赖
echo "📦 安装依赖..."
npm install

if [ $? -ne 0 ]; then
    echo "❌ 依赖安装失败"
    exit 1
fi

echo "✅ 依赖安装完成"

# 启动开发服务器
echo "🌐 启动开发服务器..."
echo "前端地址: http://localhost:3000"
echo "后端地址: http://localhost:5555"
echo ""
echo "按 Ctrl+C 停止服务器"

npm run serve
