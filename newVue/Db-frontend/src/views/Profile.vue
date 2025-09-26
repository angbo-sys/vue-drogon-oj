<template>
  <div class="profile-page">
    <div class="container">
      <div class="page-header">
        <h1>个人中心</h1>
        <p>管理你的个人信息和学习统计</p>
      </div>
      
      <el-row :gutter="20">
        <!-- 个人信息 -->
        <el-col :span="8">
          <el-card class="profile-card">
            <template #header>
              <h3>个人信息</h3>
            </template>
            
            <div class="profile-info">
              <div class="avatar-section">
                <el-avatar :size="80" :src="userStore.userInfo.avatar">
                  {{ userStore.userInfo.username?.charAt(0)?.toUpperCase() }}
                </el-avatar>
                <h4>{{ userStore.userInfo.username }}</h4>
                <p>{{ userStore.userInfo.email }}</p>
              </div>
              
              <div class="user-stats">
                <div class="stat-item">
                  <span class="stat-label">注册时间:</span>
                  <span class="stat-value">{{ formatDate(userStore.userInfo.created_at) }}</span>
                </div>
                <div class="stat-item">
                  <span class="stat-label">最后登录:</span>
                  <span class="stat-value">{{ formatDate(userStore.userInfo.last_login) }}</span>
                </div>
              </div>
            </div>
          </el-card>
        </el-col>
        
        <!-- 学习统计 -->
        <el-col :span="16">
          <el-card class="stats-card">
            <template #header>
              <h3>学习统计</h3>
            </template>
            
            <div v-if="problemStore.loading" class="loading-container">
              <el-skeleton :rows="4" animated />
            </div>
            
            <div v-else class="stats-content">
              <el-row :gutter="20">
                <el-col :span="6">
                  <div class="stat-box">
                    <div class="stat-number">{{ statistics.total_submissions || 0 }}</div>
                    <div class="stat-label">总提交次数</div>
                  </div>
                </el-col>
                
                <el-col :span="6">
                  <div class="stat-box">
                    <div class="stat-number">{{ statistics.accepted_submissions || 0 }}</div>
                    <div class="stat-label">通过题目</div>
                  </div>
                </el-col>
                
                <el-col :span="6">
                  <div class="stat-box">
                    <div class="stat-number">{{ Math.round((statistics.acceptance_rate || 0) * 100) }}%</div>
                    <div class="stat-label">通过率</div>
                  </div>
                </el-col>
                
                <el-col :span="6">
                  <div class="stat-box">
                    <div class="stat-number">{{ statistics.wrong_answer || 0 }}</div>
                    <div class="stat-label">错误答案</div>
                  </div>
                </el-col>
              </el-row>
              
              <div class="detailed-stats">
                <h4>详细统计</h4>
                <el-row :gutter="20">
                  <el-col :span="8">
                    <div class="detail-stat">
                      <span class="detail-label">超时次数:</span>
                      <span class="detail-value">{{ statistics.time_limit_exceeded || 0 }}</span>
                    </div>
                  </el-col>
                  
                  <el-col :span="8">
                    <div class="detail-stat">
                      <span class="detail-label">运行时错误:</span>
                      <span class="detail-value">{{ statistics.runtime_error || 0 }}</span>
                    </div>
                  </el-col>
                  
                  <el-col :span="8">
                    <div class="detail-stat">
                      <span class="detail-label">编译错误:</span>
                      <span class="detail-value">{{ statistics.compilation_error || 0 }}</span>
                    </div>
                  </el-col>
                </el-row>
              </div>
            </div>
          </el-card>
        </el-col>
      </el-row>
      
      <!-- 最近提交 -->
      <el-card class="recent-submissions">
        <template #header>
          <h3>最近提交</h3>
        </template>
        
        <div v-if="problemStore.loading" class="loading-container">
          <el-skeleton :rows="3" animated />
        </div>
        
        <div v-else-if="recentSubmissions.length === 0" class="empty-state">
          <el-empty description="暂无提交记录" />
        </div>
        
        <div v-else class="submissions-list">
          <div v-for="submission in recentSubmissions" :key="submission.submission_id" class="submission-item">
            <div class="submission-info">
              <span class="submission-id">#{{ submission.submission_id }}</span>
              <span class="problem-name">题目 {{ submission.question_id }}</span>
              <span class="submission-time">{{ formatTime(submission.submitted_at) }}</span>
            </div>
            <el-tag :type="getStatusType(submission.status)">
              {{ submission.status }}
            </el-tag>
          </div>
        </div>
      </el-card>
    </div>
  </div>
</template>

<script>
import { ref, computed, onMounted } from 'vue'
import { useUserStore } from '@/stores/user'
import { useProblemStore } from '@/stores/problem'

export default {
  name: 'ProfilePage',
  setup() {
    const userStore = useUserStore()
    const problemStore = useProblemStore()
    
    const statistics = ref({})
    
    // 最近提交记录（取前5条）
    const recentSubmissions = computed(() => {
      return (problemStore.submissions || []).slice(0, 5)
    })
    
    // 获取状态类型
    const getStatusType = (status) => {
      const types = {
        'AC': 'success',
        'WA': 'error',
        'TLE': 'warning',
        'RE': 'error',
        'CE': 'error',
        'MLE': 'error',
        'SE': 'error'
      }
      return types[status] || 'info'
    }
    
    // 格式化日期
    const formatDate = (timestamp) => {
      if (!timestamp) return '未知'
      const date = new Date(parseInt(timestamp) * 1000)
      return date.toLocaleDateString('zh-CN')
    }
    
    // 格式化时间
    const formatTime = (timestamp) => {
      if (!timestamp) return ''
      const date = new Date(parseInt(timestamp) * 1000)
      return date.toLocaleString('zh-CN')
    }
    
    // 加载数据
    const loadData = async () => {
      // 检查用户是否已登录
      if (!userStore.isLoggedIn) {
        console.warn('用户未登录，无法获取个人统计数据')
        return
      }
      
      const userId = userStore.userId
      console.log('加载用户数据，用户ID:', userId)
      
      try {
        await Promise.all([
          problemStore.fetchSubmissions(userId),  // 传递用户ID
          problemStore.fetchStatistics(userId)    // 传递用户ID
        ])
        statistics.value = problemStore.statistics
        console.log('统计数据加载完成:', statistics.value)
      } catch (error) {
        console.error('加载数据失败:', error)
      }
    }
    
    onMounted(() => {
      // 确保用户已登录后再加载数据
      if (userStore.checkAuth()) {
        loadData()
      } else {
        console.warn('用户认证失败，跳转到登录页')
        // 可以在这里添加跳转到登录页的逻辑
      }
    })
    
    return {
      userStore,
      problemStore,
      statistics,
      recentSubmissions,
      getStatusType,
      formatDate,
      formatTime
    }
  }
}
</script>

<style scoped>
.profile-page {
  min-height: 100vh;
  background: #f8f9fa;
  padding: 20px 0;
}

.page-header {
  text-align: center;
  margin-bottom: 40px;
}

.page-header h1 {
  font-size: 36px;
  font-weight: 600;
  color: #2c3e50;
  margin-bottom: 10px;
}

.page-header p {
  font-size: 16px;
  color: #7f8c8d;
}

.profile-card {
  margin-bottom: 20px;
}

.profile-info {
  text-align: center;
}

.avatar-section {
  margin-bottom: 30px;
}

.avatar-section h4 {
  margin: 15px 0 5px 0;
  color: #2c3e50;
  font-size: 20px;
}

.avatar-section p {
  margin: 0;
  color: #7f8c8d;
  font-size: 14px;
}

.user-stats {
  text-align: left;
}

.stat-item {
  display: flex;
  justify-content: space-between;
  margin-bottom: 10px;
  padding: 8px 0;
  border-bottom: 1px solid #f0f0f0;
}

.stat-label {
  color: #7f8c8d;
  font-size: 14px;
}

.stat-value {
  color: #2c3e50;
  font-size: 14px;
  font-weight: 500;
}

.stats-card {
  margin-bottom: 20px;
}

.stats-content {
  padding: 10px 0;
}

.stat-box {
  text-align: center;
  padding: 20px;
  background: #f8f9fa;
  border-radius: 8px;
  margin-bottom: 20px;
}

.stat-number {
  font-size: 32px;
  font-weight: 700;
  color: #409eff;
  margin-bottom: 8px;
}

.stat-label {
  font-size: 14px;
  color: #7f8c8d;
}

.detailed-stats {
  margin-top: 30px;
  padding-top: 20px;
  border-top: 1px solid #f0f0f0;
}

.detailed-stats h4 {
  margin: 0 0 20px 0;
  color: #2c3e50;
  font-size: 18px;
}

.detail-stat {
  display: flex;
  justify-content: space-between;
  padding: 10px 0;
  border-bottom: 1px solid #f0f0f0;
}

.detail-label {
  color: #7f8c8d;
  font-size: 14px;
}

.detail-value {
  color: #2c3e50;
  font-size: 14px;
  font-weight: 500;
}

.recent-submissions {
  margin-top: 20px;
}

.submissions-list {
  display: grid;
  gap: 15px;
}

.submission-item {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 15px;
  background: #f8f9fa;
  border-radius: 8px;
  transition: all 0.3s;
}

.submission-item:hover {
  background: #e9ecef;
}

.submission-info {
  display: flex;
  gap: 20px;
  font-size: 14px;
  color: #7f8c8d;
}

.loading-container {
  padding: 20px 0;
}

.empty-state {
  text-align: center;
  padding: 40px 0;
}

/* 响应式设计 */
@media (max-width: 768px) {
  .el-col {
    margin-bottom: 20px;
  }
  
  .submission-info {
    flex-direction: column;
    gap: 5px;
  }
  
  .submission-item {
    flex-direction: column;
    align-items: flex-start;
    gap: 10px;
  }
}
</style>
