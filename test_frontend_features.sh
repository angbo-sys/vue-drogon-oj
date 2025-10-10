#!/bin/bash

# 前端功能测试脚本
# 使用方法: ./test_frontend_features.sh

echo "=========================================="
echo "    前端评论区和题解功能测试"
echo "=========================================="
echo ""

# 检查前端是否运行
echo "检查前端服务状态..."
if curl -s http://localhost:8080 > /dev/null; then
    echo "✅ 前端服务正在运行 (http://localhost:8080)"
else
    echo "❌ 前端服务未运行，请先启动前端服务"
    echo "   在 newVue/Db-frontend 目录下运行: npm run serve"
    exit 1
fi

echo ""
echo "检查后端API状态..."
if curl -s http://localhost:5555/ > /dev/null; then
    echo "✅ 后端服务正在运行 (http://localhost:5555)"
else
    echo "❌ 后端服务未运行，请先启动后端服务"
    exit 1
fi

echo ""
echo "=========================================="
echo "测试步骤："
echo "1. 打开浏览器访问: http://localhost:8080"
echo "2. 登录或注册一个账户"
echo "3. 进入任意题目详情页面"
echo "4. 测试以下功能："
echo ""
echo "   📝 评论区功能："
echo "   - 点击'评论区'标签页"
echo "   - 在评论框中输入评论内容"
echo "   - 点击'发表评论'按钮"
echo "   - 查看评论是否成功显示"
echo "   - 尝试删除自己的评论"
echo ""
echo "   💡 题解功能："
echo "   - 点击'题解'标签页"
echo "   - 点击'提交题解'按钮"
echo "   - 在对话框中输入题解内容（支持Markdown格式）"
echo "   - 点击'提交题解'按钮"
echo "   - 查看题解是否成功显示"
echo "   - 尝试点赞题解"
echo ""
echo "   🤖 AI助手功能："
echo "   - 点击'AI助手'按钮"
echo "   - 在聊天框中输入问题"
echo "   - 查看AI回复"
echo ""
echo "=========================================="
echo "如果遇到问题，请检查："
echo "1. 浏览器控制台是否有错误信息"
echo "2. 网络请求是否成功"
echo "3. 后端API是否正常响应"
echo "=========================================="
