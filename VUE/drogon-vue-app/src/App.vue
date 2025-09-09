<template>
  <div id="app">
    <nav class="navbar">
      <div class="nav-brand">
        <h1>Drogon Vue App</h1>
      </div>
      <div class="nav-links">
        <a href="#" @click="currentView = 'home'" :class="{ active: currentView === 'home' }">首页</a>
        <a href="#" @click="currentView = 'api-test'" :class="{ active: currentView === 'api-test' }">API测试</a>
        <a href="#" @click="currentView = 'about'" :class="{ active: currentView === 'about' }">关于</a>
      </div>
    </nav>
    
    <main class="main-content">
      <!-- 首页 -->
      <div v-if="currentView === 'home'" class="home-view">
        <div class="hero">
          <h2>欢迎使用 Drogon + Vue.js 应用</h2>
          <p>这是一个使用 C++ Drogon 框架作为后端，Vue.js 作为前端的全栈应用示例。</p>
        </div>
        
        <div class="features">
          <div class="feature-card">
            <h3>🚀 高性能后端</h3>
            <p>使用 C++ Drogon 框架提供高性能的 HTTP 服务</p>
          </div>
          <div class="feature-card">
            <h3>⚡ 现代前端</h3>
            <p>使用 Vue.js 3 构建响应式用户界面</p>
          </div>
          <div class="feature-card">
            <h3>🔗 无缝连接</h3>
            <p>前后端通过 RESTful API 进行通信</p>
          </div>
        </div>
      </div>

      <!-- API测试页面 -->
      <div v-if="currentView === 'api-test'" class="api-test-view">
        <h2>API 连接测试</h2>
        
        <div class="test-section">
          <h3>服务器状态测试</h3>
          <button @click="testServerConnection" :disabled="loading" class="btn">
            {{ loading ? '测试中...' : '测试服务器连接' }}
          </button>
          <div v-if="serverResponse" class="response">
            <h4>服务器响应:</h4>
            <pre>{{ serverResponse }}</pre>
          </div>
          <div v-if="error" class="error">
            <h4>错误信息:</h4>
            <p>{{ error }}</p>
          </div>
        </div>

        <div class="test-section">
          <h3>POST 请求测试</h3>
          <div class="form-group">
            <label>测试数据:</label>
            <input v-model="testData" type="text" placeholder="输入测试数据" class="input">
          </div>
          <button @click="testPostRequest" :disabled="loading" class="btn">
            {{ loading ? '发送中...' : '发送 POST 请求' }}
          </button>
          <div v-if="postResponse" class="response">
            <h4>POST 响应:</h4>
            <pre>{{ postResponse }}</pre>
          </div>
        </div>
      </div>

      <!-- 关于页面 -->
      <div v-if="currentView === 'about'" class="about-view">
        <h2>关于这个应用</h2>
        <div class="about-content">
          <h3>技术栈</h3>
          <ul>
            <li><strong>后端:</strong> C++ Drogon 框架</li>
            <li><strong>前端:</strong> Vue.js 3 + Vue CLI</li>
            <li><strong>HTTP 客户端:</strong> Axios</li>
            <li><strong>构建工具:</strong> Webpack (Vue CLI)</li>
          </ul>
          
          <h3>功能特性</h3>
          <ul>
            <li>前后端分离架构</li>
            <li>RESTful API 通信</li>
            <li>响应式用户界面</li>
            <li>实时错误处理</li>
          </ul>
        </div>
      </div>
    </main>
  </div>
</template>

<script>
import { apiService } from './services/api'

export default {
  name: 'App',
  data() {
    return {
      currentView: 'home',
      loading: false,
      serverResponse: '',
      postResponse: '',
      testData: 'Hello from Vue!',
      error: ''
    }
  },
  methods: {
    async testServerConnection() {
      this.loading = true
      this.error = ''
      this.serverResponse = ''
      
      try {
        const response = await apiService.getServerInfo()
        this.serverResponse = response
      } catch (error) {
        this.error = error.message
      } finally {
        this.loading = false
      }
    },
    
    async testPostRequest() {
      this.loading = true
      this.error = ''
      this.postResponse = ''
      
      try {
        const response = await apiService.testPost({ message: this.testData })
        this.postResponse = response
      } catch (error) {
        this.error = error.message
      } finally {
        this.loading = false
      }
    }
  }
}
</script>

<style>
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body {
  font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
  background-color: #f5f5f5;
}

#app {
  min-height: 100vh;
  display: flex;
  flex-direction: column;
}

.navbar {
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  padding: 1rem 2rem;
  display: flex;
  justify-content: space-between;
  align-items: center;
  box-shadow: 0 2px 10px rgba(0,0,0,0.1);
}

.nav-brand h1 {
  color: white;
  margin: 0;
  font-size: 1.5rem;
}

.nav-links {
  display: flex;
  gap: 1.5rem;
}

.nav-links a {
  color: white;
  text-decoration: none;
  padding: 0.5rem 1rem;
  border-radius: 5px;
  transition: background-color 0.3s;
}

.nav-links a:hover,
.nav-links a.active {
  background-color: rgba(255,255,255,0.2);
}

.main-content {
  flex: 1;
  padding: 2rem;
  max-width: 1200px;
  margin: 0 auto;
  width: 100%;
}

.hero {
  text-align: center;
  margin-bottom: 3rem;
}

.hero h2 {
  color: #333;
  margin-bottom: 1rem;
  font-size: 2.5rem;
}

.hero p {
  color: #666;
  font-size: 1.2rem;
  max-width: 600px;
  margin: 0 auto;
}

.features {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
  gap: 2rem;
  margin-top: 2rem;
}

.feature-card {
  background: white;
  padding: 2rem;
  border-radius: 10px;
  box-shadow: 0 4px 6px rgba(0,0,0,0.1);
  text-align: center;
}

.feature-card h3 {
  color: #333;
  margin-bottom: 1rem;
  font-size: 1.3rem;
}

.feature-card p {
  color: #666;
  line-height: 1.6;
}

.test-section {
  background: white;
  padding: 2rem;
  border-radius: 10px;
  box-shadow: 0 4px 6px rgba(0,0,0,0.1);
  margin-bottom: 2rem;
}

.test-section h3 {
  color: #333;
  margin-bottom: 1rem;
}

.btn {
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  color: white;
  border: none;
  padding: 0.75rem 1.5rem;
  border-radius: 5px;
  cursor: pointer;
  font-size: 1rem;
  transition: transform 0.2s, box-shadow 0.2s;
  margin-right: 1rem;
}

.btn:hover:not(:disabled) {
  transform: translateY(-2px);
  box-shadow: 0 4px 12px rgba(0,0,0,0.2);
}

.btn:disabled {
  opacity: 0.6;
  cursor: not-allowed;
  transform: none;
}

.input {
  width: 100%;
  padding: 0.75rem;
  border: 2px solid #e1e5e9;
  border-radius: 5px;
  font-size: 1rem;
  margin: 0.5rem 0 1rem 0;
}

.input:focus {
  outline: none;
  border-color: #667eea;
}

.form-group {
  margin-bottom: 1rem;
}

.form-group label {
  display: block;
  margin-bottom: 0.5rem;
  font-weight: bold;
  color: #333;
}

.response {
  background: #f8f9fa;
  border: 1px solid #e9ecef;
  border-radius: 5px;
  padding: 1rem;
  margin-top: 1rem;
}

.response h4 {
  color: #333;
  margin-bottom: 0.5rem;
}

.response pre {
  background: #fff;
  padding: 1rem;
  border-radius: 3px;
  overflow-x: auto;
  white-space: pre-wrap;
}

.error {
  background: #fdf2f2;
  border: 1px solid #fecaca;
  color: #e74c3c;
  padding: 1rem;
  border-radius: 5px;
  margin-top: 1rem;
}

.about-content {
  background: white;
  padding: 2rem;
  border-radius: 10px;
  box-shadow: 0 4px 6px rgba(0,0,0,0.1);
}

.about-content h3 {
  color: #333;
  margin: 1.5rem 0 1rem 0;
}

.about-content ul {
  margin-left: 2rem;
  margin-bottom: 1.5rem;
}

.about-content li {
  margin-bottom: 0.5rem;
  line-height: 1.6;
}
</style>
