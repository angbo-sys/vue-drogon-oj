import { create } from 'zustand'
import { api } from '../services/api'

interface Admin {
  username: string
  password: string
}

interface Statistics {
  total_users: number
  total_problems: number
  total_submissions: number
  acceptance_rate: number
}

interface Problem {
  id: number
  name: string
  difficulty: number
  type: number
  source: string
  description?: string
}

interface AdminState {
  isLoggedIn: boolean
  admin: Admin | null
  statistics: Statistics
  problems: Problem[]
  
  // Actions
  login: (username: string, password: string) => Promise<{ success: boolean; message: string }>
  logout: () => void
  fetchStatistics: () => Promise<void>
  fetchProblems: () => Promise<void>
  addProblem: (problem: Omit<Problem, 'id'>) => Promise<void>
  updateProblem: (id: number, problem: Partial<Problem>) => Promise<void>
  deleteProblem: (id: number) => Promise<void>
}

export const useAdminStore = create<AdminState>((set, get) => ({
  isLoggedIn: false,
  admin: null,
  statistics: {
    total_users: 0,
    total_problems: 0,
    total_submissions: 0,
    acceptance_rate: 0
  },
  problems: [],

  login: async (username: string, password: string) => {
    try {
      // 简单的管理员验证（实际应用中应该调用API）
      if (username === 'admin' && password === 'admin123') {
        set({ 
          isLoggedIn: true, 
          admin: { username, password } 
        })
        return { success: true, message: '登录成功' }
      } else {
        return { success: false, message: '用户名或密码错误' }
      }
    } catch (error) {
      return { success: false, message: '登录失败' }
    }
  },

  logout: () => {
    set({ 
      isLoggedIn: false, 
      admin: null 
    })
  },

  fetchStatistics: async () => {
    try {
      const response = await api.get('/submit/statistics')
      if (response.data.status === 'success') {
        set({ statistics: response.data.statistics })
      }
    } catch (error) {
      console.error('获取统计信息失败:', error)
    }
  },

  fetchProblems: async () => {
    try {
      // 模拟数据，实际应用中应该调用API
      const mockProblems: Problem[] = [
        { id: 1, name: 'A+B Problem', difficulty: 0, type: 0, source: 'Luogu P1001' },
        { id: 2, name: '两数之和', difficulty: 1, type: 1, source: 'LeetCode 1' },
        { id: 3, name: '最长公共子序列', difficulty: 2, type: 1, source: 'LeetCode 1143' },
      ]
      set({ problems: mockProblems })
    } catch (error) {
      console.error('获取题目列表失败:', error)
    }
  },

  addProblem: async (problem: Omit<Problem, 'id'>) => {
    try {
      // 模拟添加题目
      const newProblem: Problem = {
        ...problem,
        id: Date.now() // 简单的ID生成
      }
      set(state => ({ 
        problems: [...state.problems, newProblem] 
      }))
    } catch (error) {
      throw new Error('添加题目失败')
    }
  },

  updateProblem: async (id: number, updates: Partial<Problem>) => {
    try {
      set(state => ({
        problems: state.problems.map(problem =>
          problem.id === id ? { ...problem, ...updates } : problem
        )
      }))
    } catch (error) {
      throw new Error('更新题目失败')
    }
  },

  deleteProblem: async (id: number) => {
    try {
      set(state => ({
        problems: state.problems.filter(problem => problem.id !== id)
      }))
    } catch (error) {
      throw new Error('删除题目失败')
    }
  }
}))
