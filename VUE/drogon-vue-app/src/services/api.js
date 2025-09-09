import axios from 'axios'

// 创建axios实例
const api = axios.create({
  baseURL: '/api', // 通过Vue CLI代理到C++后端
  timeout: 10000,
  headers: {
    'Content-Type': 'application/json'
  }
})

// 请求拦截器
api.interceptors.request.use(
  config => {
    console.log('发送请求:', config.method?.toUpperCase(), config.url)
    return config
  },
  error => {
    console.error('请求错误:', error)
    return Promise.reject(error)
  }
)

// 响应拦截器
api.interceptors.response.use(
  response => {
    console.log('收到响应:', response.status, response.data)
    return response
  },
  error => {
    console.error('响应错误:', error.response?.status, error.message)
    return Promise.reject(error)
  }
)

// API方法
export const apiService = {
  // 获取服务器信息
  async getServerInfo() {
    try {
      const response = await api.get('/')
      return response.data
    } catch (error) {
      throw new Error(`获取服务器信息失败: ${error.message}`)
    }
  },

  // 测试POST请求
  async testPost(data) {
    try {
      const response = await api.post('/test', data)
      return response.data
    } catch (error) {
      throw new Error(`POST测试失败: ${error.message}`)
    }
  },

  // 获取用户列表（示例）
  async getUsers() {
    try {
      const response = await api.get('/users')
      return response.data
    } catch (error) {
      throw new Error(`获取用户列表失败: ${error.message}`)
    }
  },

  // 创建用户（示例）
  async createUser(userData) {
    try {
      const response = await api.post('/users', userData)
      return response.data
    } catch (error) {
      throw new Error(`创建用户失败: ${error.message}`)
    }
  }
}

export default api
