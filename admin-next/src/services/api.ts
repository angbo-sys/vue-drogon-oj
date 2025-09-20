import axios from 'axios'

const api = axios.create({
  baseURL: '/api',
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

export { api }
