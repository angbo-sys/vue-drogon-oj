import { defineStore } from 'pinia'
import { ref } from 'vue'
import { 
  getProblemList, 
  getProblemDetail, 
  submitCode, 
  getSubmissionHistory, 
  getSubmissionStatistics 
} from '@/api/problem'

export const useProblemStore = defineStore('problem', () => {
  // 状态
  const problems = ref([])
  const currentProblem = ref(null)
  const submissions = ref([])
  const statistics = ref({})
  const loading = ref(false)

  // 获取题目列表
  const fetchProblems = async (params = {}) => {
    try {
      loading.value = true
      const response = await getProblemList(params)
      
      if (response.data.status === 'success') {
        problems.value = response.data.problems || []
        return { success: true, data: response.data }
      } else {
        return { success: false, message: response.data.message }
      }
    } catch (error) {
      console.error('Fetch problems error:', error)
      return { success: false, message: error.response?.data?.message || '获取题目列表失败' }
    } finally {
      loading.value = false
    }
  }

  // 获取题目详情
  const fetchProblemDetail = async (id) => {
    try {
      loading.value = true
      const response = await getProblemDetail(id)
      
      if (response.data.status === 'success') {
        currentProblem.value = response.data.problem
        return { success: true, data: response.data }
      } else {
        return { success: false, message: response.data.message }
      }
    } catch (error) {
      console.error('Fetch problem detail error:', error)
      return { success: false, message: error.response?.data?.message || '获取题目详情失败' }
    } finally {
      loading.value = false
    }
  }

  // 提交代码
  const submitCodeToJudge = async (data) => {
    try {
      loading.value = true
      const response = await submitCode(data)
      
      if (response.data.status === 'success') {
        // 刷新提交历史
        await fetchSubmissions()
        return { success: true, data: response.data }
      } else {
        return { success: false, message: response.data.message }
      }
    } catch (error) {
      console.error('Submit code error:', error)
      return { success: false, message: error.response?.data?.message || '代码提交失败' }
    } finally {
      loading.value = false
    }
  }

  // 获取提交历史
  const fetchSubmissions = async (params = {}) => {
    try {
      const response = await getSubmissionHistory(params)
      
      if (response.data.status === 'success') {
        submissions.value = response.data.items || []
        return { success: true, data: response.data }
      } else {
        return { success: false, message: response.data.message }
      }
    } catch (error) {
      console.error('Fetch submissions error:', error)
      return { success: false, message: error.response?.data?.message || '获取提交历史失败' }
    }
  }

  // 获取提交统计
  const fetchStatistics = async (params = {}) => {
    try {
      const response = await getSubmissionStatistics(params)
      
      if (response.data.status === 'success') {
        statistics.value = response.data.statistics || {}
        return { success: true, data: response.data }
      } else {
        return { success: false, message: response.data.message }
      }
    } catch (error) {
      console.error('Fetch statistics error:', error)
      return { success: false, message: error.response?.data?.message || '获取统计信息失败' }
    }
  }

  return {
    // 状态
    problems,
    currentProblem,
    submissions,
    statistics,
    loading,
    // 方法
    fetchProblems,
    fetchProblemDetail,
    submitCodeToJudge,
    fetchSubmissions,
    fetchStatistics
  }
})
