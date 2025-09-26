import axios from 'axios'
import { ElMessage } from 'element-plus'
import Cookies from 'js-cookie'

// 创建axios实例
const api = axios.create({
  baseURL: process.env.VUE_APP_API_BASE_URL || 'http://localhost:5555',
  // 移除超时限制，让AI有足够时间思考
  headers: {
    'Content-Type': 'application/json'
  }
})

// 请求拦截器
api.interceptors.request.use(
  config => {
    // 添加token到请求头
    const token = Cookies.get('token')
    if (token) {
      config.headers.Authorization = `Bearer ${token}`
    }
    
    // 添加用户ID到请求头
    const userId = Cookies.get('userId')
    if (userId) {
      config.headers['user-id'] = userId
    }
    
    console.log('API Request:', config.method?.toUpperCase(), config.url, {
      headers: {
        'Authorization': config.headers.Authorization ? 'Bearer ***' : '未设置',
        'user-id': config.headers['user-id'] || '未设置'
      },
      data: config.data
    })
    return config
  },
  error => {
    console.error('Request Error:', error)
    return Promise.reject(error)
  }
)

// 响应拦截器
api.interceptors.response.use(
  response => {
    console.log('API Response:', response.status, response.data)
    return response
  },
  error => {
    console.error('Response Error:', error.response?.status, error.message)
    
    // 处理401未授权错误
    if (error.response?.status === 401) {
      // 清除token和用户信息
      Cookies.remove('token')
      Cookies.remove('userId')
      Cookies.remove('userInfo')
      
      // 跳转到登录页
      window.location.href = '/login'
      ElMessage.error('登录已过期，请重新登录')
    }
    
    // 处理其他错误
    if (error.response?.data?.message) {
      ElMessage.error(error.response.data.message)
    } else if (error.message) {
      ElMessage.error(error.message)
    }
    
    return Promise.reject(error)
  }
)

export default api
