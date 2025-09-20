# 前端使用指南

## 🎯 项目概述

本项目包含两个前端应用：
- **用户端** (Vue 3): 面向普通用户的刷题平台
- **管理员端** (Next.js): 面向管理员的后台管理系统

## 🚀 快速开始

### 1. 启动后端服务器

```bash
cd build
./your_project_name &
```

### 2. 启动用户端前端

```bash
cd VUE/drogon-vue-app
npm install
npm run serve
```

访问地址: http://localhost:3000

### 3. 启动管理员端前端

```bash
cd admin-next
npm install
npm run dev
```

访问地址: http://localhost:3001

## 📱 用户端功能

### 主要页面

1. **首页** (`/`)
   - 平台介绍和特色展示
   - 统计信息展示
   - 最新题目列表

2. **题目列表** (`/problems`)
   - 题目筛选和搜索
   - 难度和类型筛选
   - 分页显示

3. **题目详情** (`/problem/:id`)
   - 题目描述和样例
   - Monaco 代码编辑器
   - 代码提交和结果展示

4. **提交记录** (`/submissions`)
   - 个人提交历史
   - 详细执行结果
   - 分页浏览

5. **个人中心** (`/profile`)
   - 个人信息管理
   - 学习统计
   - 密码修改

### 技术特性

- **Vue 3 + Composition API**: 现代化的响应式框架
- **Element Plus**: 丰富的UI组件库
- **Monaco Editor**: 强大的代码编辑器
- **Pinia**: 状态管理
- **Vue Router**: 路由管理
- **Axios**: HTTP客户端

## 🔧 管理员端功能

### 主要页面

1. **仪表板** (`/`)
   - 系统统计信息
   - 数据概览
   - 快速操作

2. **题目管理** (`/problems`)
   - 题目CRUD操作
   - 批量管理
   - 分类筛选

3. **用户管理** (待实现)
   - 用户列表
   - 权限管理
   - 数据统计

4. **系统设置** (待实现)
   - 系统配置
   - 参数调整

### 技术特性

- **Next.js 14**: React全栈框架
- **Ant Design**: 企业级UI组件库
- **TypeScript**: 类型安全
- **Zustand**: 轻量级状态管理
- **React Hook Form**: 表单管理

## 🔌 API接口

### 用户端API

| 接口 | 方法 | 描述 |
|------|------|------|
| `/api/submit` | POST | 提交代码 |
| `/api/submit/history` | GET | 获取提交历史 |
| `/api/submit/statistics` | GET | 获取统计信息 |
| `/users` | GET | 获取用户列表 |

### 管理员端API

| 接口 | 方法 | 描述 |
|------|------|------|
| `/api/problems` | GET/POST | 题目管理 |
| `/api/users` | GET | 用户管理 |
| `/api/statistics` | GET | 系统统计 |

## 🎨 界面设计

### 用户端设计

- **色彩方案**: 蓝色主题 (#409EFF)
- **布局**: 响应式设计，支持移动端
- **组件**: Element Plus组件库
- **图标**: Element Plus Icons

### 管理员端设计

- **色彩方案**: Ant Design默认主题
- **布局**: 后台管理系统布局
- **组件**: Ant Design组件库
- **图标**: Ant Design Icons

## 📝 开发说明

### 用户端开发

```bash
# 安装依赖
npm install

# 开发模式
npm run serve

# 构建生产版本
npm run build

# 预览生产版本
npm run preview
```

### 管理员端开发

```bash
# 安装依赖
npm install

# 开发模式
npm run dev

# 构建生产版本
npm run build

# 启动生产版本
npm start
```

## 🔧 配置说明

### 用户端配置

- **代理配置**: `vue.config.js` 中配置API代理
- **路由配置**: `src/router/index.js`
- **状态管理**: `src/stores/`
- **API服务**: `src/services/api.js`

### 管理员端配置

- **代理配置**: `next.config.js` 中配置API重写
- **页面路由**: `src/pages/` 目录结构
- **状态管理**: `src/stores/adminStore.ts`
- **API服务**: `src/services/api.ts`

## 🐛 常见问题

### 1. 跨域问题

确保后端服务器配置了CORS头：
```cpp
resp->addHeader("Access-Control-Allow-Origin", "*");
```

### 2. 代理配置

用户端使用Vue CLI代理，管理员端使用Next.js重写规则。

### 3. 端口冲突

- 用户端: 3000
- 管理员端: 3001
- 后端: 5555

### 4. 依赖安装

如果遇到依赖安装问题，可以尝试：
```bash
npm cache clean --force
rm -rf node_modules package-lock.json
npm install
```

## 📊 性能优化

### 用户端优化

- 代码分割和懒加载
- 图片懒加载
- 组件缓存
- API请求缓存

### 管理员端优化

- Next.js自动优化
- 静态生成
- 图片优化
- 代码分割

## 🔒 安全考虑

- 输入验证和过滤
- XSS防护
- CSRF防护
- 敏感信息保护

## 📈 监控和日志

- 控制台日志
- 网络请求监控
- 错误边界处理
- 性能监控

## 🚀 部署说明

### 用户端部署

```bash
npm run build
# 将dist目录部署到Web服务器
```

### 管理员端部署

```bash
npm run build
npm start
# 或使用PM2等进程管理器
```

## 📞 技术支持

如有问题，请检查：
1. 后端服务器是否正常运行
2. 端口是否被占用
3. 依赖是否正确安装
4. 网络连接是否正常

---

**注意**: 这是一个开发版本，生产环境部署需要额外的安全配置和优化。
