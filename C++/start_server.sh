#!/bin/bash

echo "==========================================="
echo "🚀 Starting DrogonOJ Backend Server"
echo "==========================================="

# 检查是否存在构建目录
if [ ! -d "build" ]; then
    echo "📁 Creating build directory..."
    mkdir -p build
fi

# 进入构建目录
cd build

# 检查是否需要重新编译
if [ ! -f "DorgonOJ" ] || [ "../main.cc" -nt "DorgonOJ" ] || [ "../CMakeLists.txt" -nt "DorgonOJ" ]; then
    echo "🔨 Compiling server..."
    cmake ..
    make -j$(nproc)

    if [ $? -ne 0 ]; then
        echo "❌ Compilation failed!"
        exit 1
    fi

    echo "✅ Compilation successful!"
else
    echo "✅ Using existing binary"
fi

# 启动服务器
echo "🚀 Starting server..."
echo "📱 Server will be available at: http://localhost:5555"
echo "🛑 Press Ctrl+C to stop the server"
echo "==========================================="

./DorgonOJ
