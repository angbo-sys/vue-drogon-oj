# 刷题网站完整实现方案

## 📋 项目概述

本项目是一个基于C++后端、Vue3用户端和Next.js管理员端的在线刷题平台，支持C/C++代码提交和自动判题功能。

### 技术栈
- **后端**: C++ + Drogon框架 + JSON文件存储
- **用户端**: Vue 3 + Element Plus + Monaco Editor
- **管理员端**: Next.js 14 + Ant Design
- **代码执行**: 直接在服务器上执行C++代码

## 🏗️ 系统架构

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   用户端 (Vue)   │    │  管理员端 (Next) │    │   后端 (C++)    │
│                 │    │                 │    │                 │
│ • 用户注册/登录  │    │ • 管理员登录    │    │ • Drogon框架    │
│ • 题目浏览      │    │ • 题目管理      │    │ • JSON数据存储  │
│ • 代码提交      │    │ • 用户管理      │    │ • 代码执行引擎  │
│ • 进度跟踪      │    │ • 统计分析      │    │ • 文件管理      │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

## 📁 数据存储结构

```
SQL/
├── user/
│   └── user.json                    # 用户数据
├── title/
│   ├── question.json               # 题目列表
│   └── [题目ID]/
│       ├── question.json           # 题目详情
│       └── text.md                 # 题目描述(Markdown)
├── submissions/
│   └── submissions.json            # 提交记录
├── admin/
│   └── admin.json                  # 管理员账号
└── system/
    └── config.json                 # 系统配置
```

### 数据格式定义

#### 用户数据 (user.json)
```json
{
  "userNumber": 1,
  "userInfo": [
    {
      "user_id": "1",
      "user_name": "admin",
      "user_password": "123456",
      "created_at": "2024-01-01T00:00:00Z",
      "last_login": "2024-01-01T00:00:00Z",
      "user_question_down_info": {
        "question_finish_number": 1,
        "question_hasDoes_number": 0,
        "question_finish_info": [
          {"type_id": 1, "question_id": 1, "is_finish": true, "completed_at": "2024-01-01T00:00:00Z", "attempts": 1}
        ],
        "question_unfinish_info": []
      }
    }
  ]
}
```

#### 题目数据 (question.json)
```json
{
  "subjectNumber": 1,
  "difficultyCalculate": [
    {"difficulty_id": 0, "difficultyName": "入门", "count": 1},
    {"difficulty_id": 1, "difficultyName": "简单", "count": 0},
    {"difficulty_id": 2, "difficultyName": "中等", "count": 0},
    {"difficulty_id": 3, "difficultyName": "困难", "count": 0}
  ],
  "typeCalculate": [
    {"type_id": 0, "typeName": "无类型", "count": 1},
    {"type_id": 1, "typeName": "动态规划", "count": 0},
    {"type_id": 2, "typeName": "前缀和", "count": 0},
    {"type_id": 3, "typeName": "贪心", "count": 0}
  ],
  "subjects": [
    {
      "question_id": 0,
      "name": "P1001 A+B Problem",
      "path": "./0001/",
      "source": "Luogu P1001",
      "difficulty_id": 0,
      "type_id": 0
    }
  ]
}
```

#### 提交记录 (submissions.json)
```json
{
  "submissionNumber": 0,
  "submissions": [
    {
      "submission_id": "1",
      "user_id": "1",
      "question_id": 1,
      "code": "#include <iostream>\nusing namespace std;\nint main() { ... }",
      "language": "cpp",
      "status": "AC",
      "execution_time": 0.001,
      "memory_usage": 1024,
      "submitted_at": "2024-01-01T00:00:00Z",
      "test_cases": [
        {"input": "1 2", "expected_output": "3", "actual_output": "3", "result": "AC"}
      ]
    }
  ]
}
```

#### 管理员数据 (admin.json)
```json
{
  "admin": {
    "username": "admin",
    "password": "admin123",
    "created_at": "2024-01-01T00:00:00Z"
  }
}
```

## 🔧 后端API设计

### 用户相关API
```
POST   /api/auth/register           # 用户注册
POST   /api/auth/login              # 用户登录
POST   /api/auth/logout             # 用户登出
GET    /api/user/profile            # 获取用户信息
PUT    /api/user/profile            # 更新用户信息
GET    /api/user/submissions        # 获取用户提交历史
```

### 题目相关API
```
GET    /api/problems                # 获取题目列表
GET    /api/problems/:id            # 获取题目详情
GET    /api/problems/:id/content    # 获取题目内容(Markdown)
GET    /api/problems/types          # 获取题目类型
GET    /api/problems/difficulties   # 获取难度等级
```

### 提交相关API
```
POST   /api/submit                  # 提交代码
GET    /api/submit/history          # 获取提交历史
GET    /api/submit/:id              # 获取提交详情
```

### 管理员API
```
POST   /api/admin/login             # 管理员登录
GET    /api/admin/problems          # 获取所有题目
POST   /api/admin/problems          # 添加题目
PUT    /api/admin/problems/:id      # 更新题目
DELETE /api/admin/problems/:id      # 删除题目
GET    /api/admin/users             # 获取用户列表
GET    /api/admin/statistics        # 获取统计数据
```

## 💻 代码执行引擎设计

### 执行限制
```cpp
const int TIME_LIMIT = 2;        // 2秒时间限制
const int MEMORY_LIMIT = 256;    // 256MB内存限制
const int OUTPUT_LIMIT = 1024;   // 1MB输出限制
```

### 执行流程
1. 接收用户代码和题目ID
2. 生成临时文件 (main.cpp)
3. 编译C++代码 (g++ -std=c++17 -O2)
4. 运行并限制资源 (timeout + ulimit)
5. 比较输出结果
6. 返回执行结果
7. 清理临时文件

### 执行结果状态
- **AC**: Accepted (通过)
- **WA**: Wrong Answer (错误答案)
- **TLE**: Time Limit Exceeded (超时)
- **RE**: Runtime Error (运行时错误)
- **CE**: Compilation Error (编译错误)
- **MLE**: Memory Limit Exceeded (内存超限)

## 🎨 前端页面设计

### 用户端 (Vue 3)

#### 页面结构
```
/ (首页 - 题目列表)
├── /login (登录页)
├── /register (注册页)
├── /problem/:id (题目详情页)
│   └── 代码编辑器 + 提交按钮
├── /profile (个人中心)
│   ├── 个人信息
│   └── 提交历史
└── /submission/:id (提交详情页)
```

#### 核心组件
- **ProblemList**: 题目列表组件
- **ProblemDetail**: 题目详情组件
- **CodeEditor**: 代码编辑器组件 (Monaco Editor)
- **SubmissionHistory**: 提交历史组件
- **UserProfile**: 用户信息组件

### 管理员端 (Next.js)

#### 页面结构
```
/admin/login (管理员登录)
/admin (仪表板)
├── /admin/problems (题目管理)
│   ├── 题目列表
│   ├── 添加题目
│   └── 编辑题目
├── /admin/users (用户管理)
└── /admin/statistics (数据统计)
```

#### 核心组件
- **ProblemManagement**: 题目管理组件
- **UserManagement**: 用户管理组件
- **Statistics**: 数据统计组件
- **AdminDashboard**: 管理员仪表板

## 🔐 认证系统设计

### JWT Token管理
```cpp
// Token结构
{
  "user_id": "1",
  "username": "testuser",
  "role": "user", // user | admin
  "exp": 1640995200, // 过期时间
  "iat": 1640908800  // 签发时间
}
```

### 权限控制
- **用户端**: 需要用户Token，有效期1天
- **管理员端**: 需要管理员Token，有效期1天
- **API访问**: 基于Token验证身份

## 📊 数据统计设计

### 用户统计
```json
{
  "user_stats": {
    "total_submissions": 0,
    "accepted_submissions": 0,
    "problems_solved": 0,
    "success_rate": 0.0,
    "average_execution_time": 0.0
  }
}
```

### 系统统计
```json
{
  "system_stats": {
    "total_users": 0,
    "total_problems": 0,
    "total_submissions": 0,
    "daily_active_users": 0,
    "popular_problems": []
  }
}
```

## 🚀 实现优先级

### 第一阶段：后端核心功能 (当前阶段)
1. ✅ 用户模型和服务 (已完成)
2. ✅ 题目模型和服务 (已完成)
3. 🔄 代码执行引擎
4. 🔄 提交记录管理
5. 🔄 管理员功能
6. 🔄 API接口实现

### 第二阶段：前端开发
1. 用户端基础页面
2. 管理员端基础页面
3. 代码编辑器集成
4. 数据展示优化

### 第三阶段：功能完善
1. 错误处理优化
2. 性能优化
3. 用户体验优化
4. 测试和调试

## 📝 开发规范

### 代码规范
- **C++**: 遵循Google C++ Style Guide
- **Vue**: 使用Composition API，遵循Vue 3最佳实践
- **Next.js**: 使用App Router，遵循React最佳实践

### 文件命名
- **C++**: 使用snake_case (如: user_service.cpp)
- **Vue**: 使用PascalCase (如: ProblemList.vue)
- **Next.js**: 使用kebab-case (如: problem-list.tsx)

### 注释规范
- **C++**: 使用Doxygen格式
- **Vue/Next.js**: 使用JSDoc格式

## 🔧 开发环境配置

### 后端环境
```bash
# 安装依赖
sudo apt-get install g++ cmake libjsoncpp-dev

# 编译项目
mkdir build && cd build
cmake ..
make
```

### 前端环境
```bash
# 用户端
cd VUE/drogon-vue-app
npm install
npm run serve

# 管理员端
cd admin-react
npm install
npm run dev
```

## 📋 测试计划

### 单元测试
- 模型类功能测试
- 服务层功能测试
- API接口测试

### 集成测试
- 用户注册登录流程
- 题目提交判题流程
- 管理员功能流程

### 性能测试
- 代码执行性能
- 并发用户处理
- 内存使用监控

## 🚀 部署方案

### 本地部署
1. 编译后端程序
2. 启动Drogon服务器
3. 构建前端项目
4. 配置Nginx反向代理

### 文件结构
```
deploy/
├── backend/           # 后端程序
├── frontend/          # 用户端静态文件
├── admin/            # 管理员端静态文件
├── data/             # 数据文件
└── nginx.conf        # Nginx配置
```

## 📚 文档计划

1. **API文档**: 使用Swagger生成
2. **用户手册**: 前端使用说明
3. **管理员手册**: 后台管理说明
4. **开发文档**: 代码结构和开发指南

## 🎯 项目里程碑

- **Week 1**: 后端核心功能完成
- **Week 2**: API接口实现完成
- **Week 3**: 用户端基础功能完成
- **Week 4**: 管理员端基础功能完成
- **Week 5**: 功能测试和优化
- **Week 6**: 部署和文档完善

---

*本文档将随着项目进展持续更新*
