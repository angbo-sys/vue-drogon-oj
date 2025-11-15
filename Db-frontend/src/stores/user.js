import { defineStore } from 'pinia'
import { ref, computed } from 'vue'
import { login, register, getUserInfo, logout } from '@/api/user'
import Cookies from 'js-cookie'

export const useUserStore = defineStore('user', () => {
  // 状态
  const token = ref(Cookies.get('token') || '')
  const userInfo = ref(JSON.parse(Cookies.get('userInfo') || '{}'))
  const loading = ref(false)

  // 计算属性
  const isLoggedIn = computed(() => !!token.value)
  const userId = computed(() => userInfo.value.id || '')

  // 登录
  const userLogin = async (loginData) => {
    try {
      loading.value = true
      const response = await login(loginData)
      
      if (response.data.status === 'success') {
        const { token: newToken, user } = response.data
        
        // 保存token和用户信息
        token.value = newToken
        userInfo.value = user
        
        // 设置cookie（1天过期）
        const expires = new Date()
        expires.setDate(expires.getDate() + 1)
        
        Cookies.set('token', newToken, { expires })
        Cookies.set('userId', user.id, { expires })
        Cookies.set('userInfo', JSON.stringify(user), { expires })
        
        return { success: true, data: response.data }
      } else {
        return { success: false, message: response.data.message }
      }
    } catch (error) {
      console.error('Login error:', error)
      return { success: false, message: error.response?.data?.message || '登录失败' }
    } finally {
      loading.value = false
    }
  }

  // 注册
  const userRegister = async (registerData) => {
    try {
      loading.value = true
      const response = await register(registerData)
      
      if (response.data.status === 'success') {
        const { token: newToken, user } = response.data
        
        // 保存token和用户信息
        token.value = newToken
        userInfo.value = user
        
        // 设置cookie（1天过期）
        const expires = new Date()
        expires.setDate(expires.getDate() + 1)
        
        Cookies.set('token', newToken, { expires })
        Cookies.set('userId', user.id, { expires })
        Cookies.set('userInfo', JSON.stringify(user), { expires })
        
        return { success: true, data: response.data }
      } else {
        return { success: false, message: response.data.message }
      }
    } catch (error) {
      console.error('Register error:', error)
      return { success: false, message: error.response?.data?.message || '注册失败' }
    } finally {
      loading.value = false
    }
  }

  // 获取用户信息
  const fetchUserInfo = async () => {
    try {
      const response = await getUserInfo()
      if (response.data.status === 'success') {
        userInfo.value = response.data.user
        Cookies.set('userInfo', JSON.stringify(response.data.user), { expires: 1 })
      }
    } catch (error) {
      console.error('Fetch user info error:', error)
    }
  }

  // 登出
  const userLogout = async () => {
    try {
      await logout()
    } catch (error) {
      console.error('Logout error:', error)
    } finally {
      // 清除本地状态和cookie
      token.value = ''
      userInfo.value = {}
      Cookies.remove('token')
      Cookies.remove('userId')
      Cookies.remove('userInfo')
    }
  }

  // 检查登录状态
  const checkAuth = () => {
    const tokenFromCookie = Cookies.get('token')
    const userInfoFromCookie = Cookies.get('userInfo')
    
    if (tokenFromCookie && userInfoFromCookie) {
      token.value = tokenFromCookie
      userInfo.value = JSON.parse(userInfoFromCookie)
      return true
    }
    return false
  }

  return {
    // 状态
    token,
    userInfo,
    loading,
    // 计算属性
    isLoggedIn,
    userId,
    // 方法
    userLogin,
    userRegister,
    fetchUserInfo,
    userLogout,
    checkAuth
  }
})
