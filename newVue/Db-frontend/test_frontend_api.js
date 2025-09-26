// 测试前端API修复
// 这个脚本模拟前端登录和API调用流程

const axios = require('axios');
const Cookies = require('js-cookie');

// 模拟前端API配置
const api = axios.create({
  baseURL: 'http://localhost:5555',
  timeout: 10000,
  headers: {
    'Content-Type': 'application/json'
  }
});

// 模拟请求拦截器
api.interceptors.request.use(
  config => {
    // 添加token到请求头
    const token = 'sample_token_123'; // 模拟登录后的token
    if (token) {
      config.headers.Authorization = `Bearer ${token}`;
    }
    
    // 添加用户ID到请求头
    const userId = '1'; // 模拟用户ID
    if (userId) {
      config.headers['user-id'] = userId;
    }
    
    console.log('API Request:', config.method?.toUpperCase(), config.url, config.headers);
    return config;
  },
  error => {
    console.error('Request Error:', error);
    return Promise.reject(error);
  }
);

// 模拟响应拦截器
api.interceptors.response.use(
  response => {
    console.log('API Response:', response.status, response.data);
    return response;
  },
  error => {
    console.error('Response Error:', error.response?.status, error.message);
    return Promise.reject(error);
  }
);

// 测试函数
async function testFrontendAPI() {
  console.log('=== 测试前端API修复 ===\n');
  
  try {
    // 测试1: 登录API
    console.log('1. 测试登录API...');
    const loginResponse = await api.post('/api/auth/login', {
      username: 'testuser',
      password: '123456'
    });
    console.log('登录成功:', loginResponse.data);
    console.log('用户ID类型:', typeof loginResponse.data.user.id);
    console.log('用户ID值:', loginResponse.data.user.id);
    console.log('');
    
    // 测试2: 获取用户信息API
    console.log('2. 测试用户信息API...');
    const profileResponse = await api.get('/api/user/profile');
    console.log('用户信息:', profileResponse.data);
    console.log('用户ID类型:', typeof profileResponse.data.user.id);
    console.log('用户ID值:', profileResponse.data.user.id);
    console.log('');
    
    // 测试3: 获取统计信息API（这是关键测试）
    console.log('3. 测试统计信息API...');
    const statsResponse = await api.get('/api/submit/statistics');
    console.log('统计信息:', statsResponse.data);
    console.log('统计数据类型:', typeof statsResponse.data.statistics);
    console.log('总提交次数:', statsResponse.data.statistics.total_submissions);
    console.log('通过次数:', statsResponse.data.statistics.accepted_submissions);
    console.log('通过率:', statsResponse.data.statistics.acceptance_rate);
    console.log('');
    
    // 测试4: 获取提交历史API
    console.log('4. 测试提交历史API...');
    const historyResponse = await api.get('/api/submit/history');
    console.log('提交历史:', historyResponse.data);
    console.log('');
    
    console.log('=== 所有测试通过！前端API修复成功 ===');
    
  } catch (error) {
    console.error('测试失败:', error.response?.data || error.message);
  }
}

// 运行测试
testFrontendAPI();
