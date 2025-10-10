import api from './index'

// 评论相关API
export const commentAPI = {
  // 获取题目评论列表
  getProblemComments: (problemId, params = {}) => {
    return api.get(`/api/comments/${problemId}`, { params })
  },

  // 创建评论
  createComment: (data) => {
    return api.post('/api/comments/create', data)
  },

  // 删除评论
  deleteComment: (data) => {
    return api.post('/api/comments/delete', data)
  },

  // 获取评论总数
  getCommentCount: (problemId) => {
    return api.get(`/api/comments/count/${problemId}`)
  },

  // 获取评论详情
  getCommentDetail: (commentId) => {
    return api.get(`/api/comments/detail?comment_id=${commentId}`)
  }
}

// 题解分析相关API
export const analysisAPI = {
  // 获取题目解析
  getQuestionAnalysis: (problemId) => {
    return api.get(`/api/analysis/${problemId}`)
  },

  // 提交用户解析
  submitUserAnalysis: (data) => {
    return api.post('/api/analysis/submit', data)
  },

  // 获取解析点赞数
  getAnalysisLikes: (analysisId) => {
    return api.get(`/api/analysis/likes/${analysisId}`)
  },

  // 点赞解析
  likeAnalysis: (data) => {
    return api.post('/api/analysis/like', data)
  },

  // 获取热门解析
  getPopularAnalyses: (params = {}) => {
    return api.get('/api/analysis/popular', { params })
  }
}
