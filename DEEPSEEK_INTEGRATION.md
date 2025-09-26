# DeepSeek API集成说明

## 概述

已成功将DeepSeek API集成到AI聊天机器人功能中，现在可以使用真实的AI服务来回答用户关于算法题目的问题。

## 集成的功能

### 1. DeepSeek API集成
- **API端点**: `https://api.deepseek.com/v1/chat/completions`
- **模型**: `deepseek-chat`
- **API密钥**: `自行补全`
- **超时设置**: 30秒
- **最大令牌数**: 1000
- **温度参数**: 0.7

### 2. 配置管理
- 支持通过配置文件管理API设置
- 配置文件位置: `C++ 2/config/ai_config.json`
- 支持运行时配置加载
- 提供默认配置作为备用

### 3. 错误处理和备用机制
- API调用失败时自动使用模拟响应
- 详细的错误日志记录
- 优雅的降级处理

## 文件结构

```
C++ 2/
├── services/
│   ├── AIChatService.h          # AI聊天服务头文件
│   └── AIChatService.cpp        # AI聊天服务实现
├── config/
│   └── ai_config.json           # AI配置文件
└── main.cc                      # 主程序（已更新）

newVue/Db-frontend/src/
├── views/
│   └── ProblemDetail.vue        # 题目详情页面（已更新）
├── api/
│   └── problem.js               # API接口（已更新）
└── stores/
    └── problem.js               # 状态管理（已更新）
```

## 配置文件说明

`config/ai_config.json`:
```json
{
  "deepseek": {
    "api_url": "https://api.deepseek.com/v1/chat/completions",
    "api_key": "自行补全",
    "model": "deepseek-chat",
    "max_tokens": 1000,
    "temperature": 0.7,
    "timeout": 30
  },
  "fallback": {
    "use_mock_response": true,
    "mock_response_enabled": true
  }
}
```

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
    "timestamp": "时间戳",
    "note": "使用模拟响应" // 可选，仅在API失败时出现
  }
}
```

## 使用方法

### 1. 编译和启动
```bash
# 编译项目
cd "C++ 2"
make

# 启动后端服务
./your_project_name

# 启动前端服务（新终端）
cd newVue/Db-frontend
npm run serve
```

### 2. 测试功能
```bash
# 运行完整测试
./build_and_test.sh

# 或单独测试AI功能
./test_deepseek_api.sh
```

### 3. 使用界面
1. 打开浏览器访问前端应用
2. 进入任意题目详情页面
3. 点击绿色的"AI助手"按钮
4. 在弹出的聊天窗口中输入问题
5. AI会基于题目信息提供专业回答

## AI助手能力

### 支持的问题类型
1. **题目理解**: "这道题怎么理解？"
2. **算法选择**: "这道题用什么算法比较好？"
3. **代码实现**: "如何实现这个算法？"
4. **复杂度分析**: "这个算法的时间复杂度是多少？"
5. **优化建议**: "如何优化这个解法？"

### AI回答特点
- 基于具体题目信息提供针对性回答
- 提供专业的算法分析和建议
- 支持代码示例和实现指导
- 中文回答，易于理解

## 故障排除

### 常见问题

1. **API调用失败**
   - 检查API密钥是否正确
   - 确认网络连接正常
   - 查看服务器日志中的错误信息

2. **使用模拟响应**
   - 这是正常的备用机制
   - 检查DeepSeek服务状态
   - 验证API配额是否充足

3. **编译错误**
   - 确保安装了所有依赖
   - 检查CMakeLists.txt配置
   - 确认Drogon框架正确安装

### 日志查看
```bash
# 查看服务器日志
cd "C++ 2"
./your_project_name

# 日志会显示：
# - AI配置加载状态
# - API调用结果
# - 错误信息
```

## 扩展功能

### 可以添加的功能
1. **聊天历史记录**: 保存用户对话历史
2. **多轮对话**: 支持上下文相关的连续对话
3. **代码分析**: AI分析用户提交的代码
4. **个性化推荐**: 根据用户水平推荐题目
5. **多语言支持**: 支持不同编程语言

### 配置优化
1. **调整参数**: 修改temperature、max_tokens等参数
2. **添加模型**: 支持其他AI模型
3. **缓存机制**: 添加响应缓存提高性能
4. **限流控制**: 添加API调用频率限制

## 安全注意事项

1. **API密钥保护**: 不要将API密钥提交到版本控制系统
2. **输入验证**: 验证用户输入防止注入攻击
3. **错误处理**: 不要暴露敏感的错误信息
4. **访问控制**: 考虑添加用户认证和权限控制

## 性能优化

1. **异步处理**: 考虑使用异步HTTP客户端
2. **连接池**: 复用HTTP连接
3. **缓存策略**: 缓存常见问题的回答
4. **负载均衡**: 在多个API密钥间轮询

现在你的AI聊天机器人已经完全集成了DeepSeek API，可以提供专业、准确的算法题目解答服务！
