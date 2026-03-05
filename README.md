# vue-drogon-oj（大学生 C++ 课程设计）

一款基于 Vue + Drogon 的轻量级 OJ 平台。

当前实现可用于演示题库管理、代码提交/编译、在线评测、评论与简单 AI 辅助（保留 DeepSeek API 接口）。项目仍有若干bug

## 主要特性

- 用户注册 / 登录
- 在线代码提交、编译与结果展示（后端使用 Drogon + C++）
- 题库管理（基于 JSON 的轻量存储，可扩展为 DB）
- 评论与题解发表功能
- 集成简单 AI 接口（DeepSeek 占位实现，可接入实际 API）

## 技术栈

- 前端：Vue.js
- 后端：Drogon（C++）
- 构建：CMake / Make
- 语言：C++17、JavaScript

## 运行前准备（依赖）

确保系统已安装以下工具：

- g++（支持 C++17）
- cmake
- make
- node.js（用于前端）
- npm / yarn（根据前端脚本选择）

在 Linux 环境下，典型安装命令（以 Ubuntu 为例）：

```bash
sudo apt update
sudo apt install -y build-essential cmake g++ nodejs npm
```

## 后端（Drogon）编译与运行

1. 进入后端构建目录：

```bash
cd c++/build
```

2. 清理并构建：

```bash
rm -rf ./*
cmake ..
make -j$(nproc)
```

3. 运行后端：

```bash
./c_project
```

注意：项目使用了 Drogon 子模块或第三方库，请先确保这些依赖已安装或在 CMake 配置中正确指向。

## 前端（Vue）启动

前端位于 `newVue/Db-frontend`（或仓库中相应前端目录），启动步骤：

```bash
cd newVue/Db-frontend
./start.sh
```

如果 `start.sh` 使用 npm/yarn，请先安装依赖：

```bash
npm install
# 或
yarn install
```
