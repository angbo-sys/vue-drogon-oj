import api from './index'

// 用户登录
export const login = (data) => {
  return api.post('/api/auth/login', data)
}

// 用户注册
export const register = (data) => {
  return api.post('/api/auth/register', data)
}

// 获取用户信息
export const getUserInfo = () => {
  return api.get('/api/user/profile')
}

// 更新用户信息
export const updateUserInfo = (data) => {
  return api.put('/api/user/profile', data)
}

// 修改密码
export const changePassword = (data) => {
  return api.put('/api/user/password', data)
}

// 用户登出
export const logout = () => {
  return api.post('/api/auth/logout')
}
