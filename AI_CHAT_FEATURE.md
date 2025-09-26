# AI聊天功能实现说明

## 功能概述

在ProblemDetail.vue页面添加了一个AI聊天机器人功能，用户可以通过点击"AI助手"按钮打开聊天窗口，与AI讨论题目相关问题。

## 实现的功能

### 前端功能
1. **AI助手按钮**: 在题目详情页面的操作区域添加了绿色的"AI助手"按钮
2. **聊天窗口**: 点击按钮后弹出模态聊天窗口
3. **消息显示**: 支持用户和AI的消息显示，带有头像和时间戳
4. **实时交互**: 支持发送消息和接收AI回复
5. **加载状态**: 显示AI思考中的加载动画
6. **响应式设计**: 适配移动端和桌面端

### 后端功能
1. **API接口**: `/api/ai/chat` - 处理AI聊天请求
2. **AI服务**: `AIChatService` 类处理AI逻辑
3. **模拟响应**: 当前提供智能的模拟回复

## 文件修改

### 前端文件
- `newVue/Db-frontend/src/views/ProblemDetail.vue` - 添加聊天界面和逻辑
- `newVue/Db-frontend/src/api/problem.js` - 添加AI聊天API
- `newVue/Db-frontend/src/stores/problem.js` - 添加AI聊天状态管理

### 后端文件
- `C++ 2/main.cc` - 添加AI聊天API路由
- `C++ 2/services/AIChatService.h` - AI聊天服务头文件
- `C++ 2/services/AIChatService.cpp` - AI聊天服务实现

## API接口说明

### 请求格式
```json
POST /api/ai/chat
{
  "user_id": "用户ID",
  "question_id": "题目ID", 
  "message": "用户消息",
  "problem_info": {
    "name": "题目名称",
    "description": "题目描述",
    "difficulty": "难度等级",
    "tags": ["标签1", "标签2"]
  }
}
```

### 响应格式
```json
{
  "status": "success",
  "data": {
    "response": "AI回复内容",
    "timestamp": "时间戳"
  }
}
```

## 如何集成真实的AI服务

要集成真实的AI服务，你需要修改 `AIChatService.cpp` 中的 `callExternalAI` 方法：

### 1. OpenAI API 示例
```cpp
std::string AIChatService::callExternalAI(const std::string& prompt) {
    // 使用HTTP客户端调用OpenAI API
    // 需要实现HTTP请求和JSON解析
    return "真实的AI回复";
}
```

### 2. 本地模型示例
```cpp
std::string AIChatService::callExternalAI(const std::string& prompt) {
    // 调用本地部署的AI模型
    // 例如通过进程调用或其他方式
    return "本地AI回复";
}
```

## 使用方式

1. 启动后端服务：`cd "C++ 2" && make && ./your_project_name`
2. 启动前端服务：`cd newVue/Db-frontend && npm run serve`
3. 访问题目详情页面，点击"AI助手"按钮开始聊天

## 注意事项

1. 当前实现的是模拟AI回复，需要你根据实际需求集成真实的AI服务
2. AI服务需要处理题目相关的上下文信息
3. 建议添加错误处理和重试机制
4. 可以考虑添加聊天历史记录功能
5. 建议添加用户反馈机制来改进AI回复质量

## 扩展功能建议

1. **聊天历史**: 保存用户的聊天记录
2. **多轮对话**: 支持上下文相关的连续对话
3. **代码分析**: AI可以分析用户提交的代码
4. **个性化**: 根据用户水平提供个性化建议
5. **多语言支持**: 支持不同编程语言的题目讨论
