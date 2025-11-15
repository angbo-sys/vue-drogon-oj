#!/bin/bash

echo "==========================================="
echo "🚀 Starting DrogonDb Frontend Server"
echo "==========================================="

# 检查是否在正确的目录
if [ ! -f "package.json" ]; then
    echo "❌ 请在Db-frontend目录下运行此脚本"
    exit 1
fi

# 检查是否安装了依赖
if [ ! -d "node_modules" ]; then
    echo "📦 Installing dependencies..."
    npm install
    if [ $? -ne 0 ]; then
        echo "❌ 依赖安装失败！"
        exit 1
    fi
    echo "✅ 依赖安装完成！"
fi

# 获取本机IP地址
LOCAL_IP=$(ifconfig | grep "inet " | grep -v 127.0.0.1 | head -1 | awk '{print $2}')
echo "🌐 Local IP: $LOCAL_IP"
echo "📱 Frontend will be available at:"
echo "   - Local: http://localhost:8080"
echo "   - Network: http://$LOCAL_IP:8080"
echo "==========================================="

# 启动开发服务器
echo "🚀 Starting Vue development server on port 8080..."
npm run serve -- --host 0.0.0.0 --port 8080
