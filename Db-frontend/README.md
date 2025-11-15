# Db Platform Frontend

基于Vue 3 + Element Plus的在线编程练习平台前端应用。

## 功能特性

- 🔐 **用户认证**: 支持用户注册、登录，使用token和cookie管理会话
- 📝 **题目管理**: 题目列表、详情查看、搜索和筛选
- 💻 **代码编辑**: 集成Monaco Editor，支持C++代码编写
- ⚡ **实时评测**: 代码提交、编译、运行和结果反馈
- 📊 **学习统计**: 提交历史、通过率统计、个人中心
- 🎨 **响应式设计**: 适配桌面和移动设备
- 🛡️ **路由守卫**: 自动登录状态检查和页面权限控制

## 技术栈

- **Vue 3** - 渐进式JavaScript框架
- **Vue Router 4** - 官方路由管理器
- **Pinia** - 状态管理库
- **Element Plus** - Vue 3组件库
- **Axios** - HTTP客户端
- **Monaco Editor** - 代码编辑器
- **js-cookie** - Cookie管理

## 项目结构

```
src/
├── api/                    # API接口管理
│   ├── index.js           # Axios配置和拦截器
│   ├── user.js            # 用户相关API
│   └── problem.js         # 题目相关API
├── components/            # 公共组件
│   ├── AppHeader.vue      # 头部导航
│   └── AppFooter.vue      # 底部信息
├── router/                # 路由配置
│   └── index.js           # 路由定义和守卫
├── stores/                # 状态管理
│   ├── user.js            # 用户状态
│   └── problem.js         # 题目状态
├── views/                 # 页面组件
│   ├── Home.vue           # 首页
│   ├── Login.vue          # 登录页
│   ├── Register.vue       # 注册页
│   ├── ProblemList.vue    # 题目列表
│   ├── ProblemDetail.vue  # 题目详情
│   ├── SubmissionHistory.vue # 提交历史
│   ├── Profile.vue        # 个人中心
│   └── NotFound.vue       # 404页面
├── App.vue                # 根组件
└── main.js                # 入口文件
```

## 快速开始

### 环境要求

- Node.js >= 16.0.0
- npm >= 8.0.0

### 安装依赖

```bash
npm install
```

### 启动开发服务器

```bash
npm run serve
```

或者使用启动脚本：

```bash
./start.sh
```

### 构建生产版本

```bash
npm run build
```

## 配置说明

### 环境变量

在项目根目录创建 `.env` 文件：

```env
VUE_APP_API_BASE_URL=http://localhost:5555
```

### 代理配置

开发环境下，API请求会自动代理到后端服务器：

```javascript
// vue.config.js
devServer: {
  proxy: {
    '/api': {
      target: 'http://localhost:5555',
      changeOrigin: true,
      secure: false
    }
  }
}
```

## API接口

### 用户认证

- `POST /api/auth/login` - 用户登录
- `POST /api/auth/register` - 用户注册
- `GET /api/user/profile` - 获取用户信息
- `POST /api/auth/logout` - 用户登出

### 题目管理

- `GET /api/problems` - 获取题目列表
- `GET /api/problems/:id` - 获取题目详情
- `POST /api/submit` - 提交代码
- `GET /api/submit/history` - 获取提交历史
- `GET /api/submit/statistics` - 获取提交统计

## 功能说明

### 认证系统

- 使用JWT token进行身份验证
- Cookie自动管理token过期时间（1天）
- 路由守卫自动检查登录状态
- 未登录用户自动跳转到登录页

### 代码编辑器

- 集成Monaco Editor
- 支持C++语法高亮
- 自动保存代码内容
- 支持代码折叠和搜索

### 状态管理

- 使用Pinia进行状态管理
- 用户信息和登录状态全局共享
- 题目数据和提交记录缓存

### 响应式设计

- 移动端适配
- 弹性布局
- 触摸友好的交互

## 开发指南

### 添加新页面

1. 在 `src/views/` 创建Vue组件
2. 在 `src/router/index.js` 添加路由配置
3. 更新导航菜单（如需要）

### 添加新API

1. 在 `src/api/` 对应文件中添加API方法
2. 在 `src/stores/` 中添加状态管理
3. 在组件中调用API

### 样式规范

- 使用Element Plus组件库
- 遵循BEM命名规范
- 响应式设计优先
- 保持一致的视觉风格

## 部署说明

### 构建

```bash
npm run build
```

构建产物在 `dist/` 目录。

### 部署到Nginx

```nginx
server {
    listen 80;
    server_name your-domain.com;

    location / {
        root /path/to/dist;
        try_files $uri $uri/ /index.html;
    }

    location /api {
        proxy_pass http://localhost:5555;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
    }
}
```

## 常见问题

### 1. 后端API连接失败

检查后端服务器是否启动，确认API地址配置正确。

### 2. Monaco编辑器加载失败

确保网络连接正常，Monaco Editor需要从CDN加载资源。

### 3. 登录状态丢失

检查Cookie设置，确保域名和路径配置正确。

## 贡献指南

1. Fork项目
2. 创建功能分支
3. 提交更改
4. 推送到分支
5. 创建Pull Request

## 许可证

MIT License
