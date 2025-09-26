import api from './index'

// 获取题目列表
export const getProblemList = (params = {}) => {
  return api.get('/api/problems', { params })
}

// 获取题目详情
export const getProblemDetail = (id) => {
  return api.get(`/api/problems/${id}`)
}

// 提交代码
export const submitCode = (data) => {
  return api.post('/api/submit', data)
}

// 获取提交历史
export const getSubmissionHistory = (userId = null) => {
  if (userId) {
    return api.get(`/api/submit/history?user_id=${userId}`)
  } else {
    return api.get('/api/submit/history')
  }
}

// 获取提交详情
export const getSubmissionDetail = (id) => {
  return api.get(`/api/submit/${id}`)
}

// 获取提交统计
export const getSubmissionStatistics = (userId = null) => {
  if (userId) {
    return api.get(`/api/submit/statistics?user_id=${userId}`)
  } else {
    return api.get('/api/submit/statistics')
  }
}

// AI聊天接口
export const chatWithAI = (data) => {
  return api.post('/api/ai/chat', data)
}
