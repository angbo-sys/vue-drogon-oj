<template>
  <div class="submission-history-page">
    <div class="container">
      <div class="page-header">
        <h1>提交记录</h1>
        <p>查看你的代码提交历史和评测结果</p>
      </div>
      
      <!-- 统计信息 -->
      <div class="stats-section">
        <el-row :gutter="20">
          <el-col :span="6">
            <el-card class="stat-card">
              <div class="stat-content">
                <div class="stat-number">{{ statistics.total_submissions || 0 }}</div>
                <div class="stat-label">总提交次数</div>
              </div>
            </el-card>
          </el-col>
          
          <el-col :span="6">
            <el-card class="stat-card">
              <div class="stat-content">
                <div class="stat-number">{{ statistics.accepted_submissions || 0 }}</div>
                <div class="stat-label">通过题目</div>
              </div>
            </el-card>
          </el-col>
          
          <el-col :span="6">
            <el-card class="stat-card">
              <div class="stat-content">
                <div class="stat-number">{{ Math.round((statistics.acceptance_rate || 0) * 100) }}%</div>
                <div class="stat-label">通过率</div>
              </div>
            </el-card>
          </el-col>
          
          <el-col :span="6">
            <el-card class="stat-card">
              <div class="stat-content">
                <div class="stat-number">{{ statistics.wrong_answer || 0 }}</div>
                <div class="stat-label">错误答案</div>
              </div>
            </el-card>
          </el-col>
        </el-row>
      </div>
      
      <!-- 筛选器 -->
      <div class="filters-section">
        <el-form :inline="true" class="filter-form">
          <el-form-item label="状态">
            <el-select v-model="statusFilter" placeholder="选择状态" clearable @change="handleFilter">
              <el-option label="全部" value="" />
              <el-option label="通过" value="AC" />
              <el-option label="错误答案" value="WA" />
              <el-option label="超时" value="TLE" />
              <el-option label="运行时错误" value="RE" />
              <el-option label="编译错误" value="CE" />
            </el-select>
          </el-form-item>
          
          <el-form-item label="题目">
            <el-input
              v-model="problemFilter"
              placeholder="题目ID"
              clearable
              @input="handleFilter"
            />
          </el-form-item>
        </el-form>
      </div>
      
      <!-- 提交记录列表 -->
      <div class="submissions-container">
        <el-card v-for="submission in filteredSubmissions" :key="submission.submission_id" class="submission-card">
          <div class="submission-content">
            <div class="submission-header">
              <div class="submission-info">
                <span class="submission-id">#{{ submission.submission_id }}</span>
                <span class="problem-name">题目 {{ submission.question_id }}</span>
                <span class="submission-time">{{ formatTime(submission.submitted_at) }}</span>
              </div>
              <el-tag :type="getStatusType(submission.status)">
                {{ submission.status }}
              </el-tag>
            </div>
            
            <div class="submission-details">
              <div class="detail-item">
                <el-icon><Clock /></el-icon>
                <span>{{ submission.execution_time?.toFixed(3) || 0 }}s</span>
              </div>
              
              <div class="detail-item">
                <el-icon><MemoryCard /></el-icon>
                <span>{{ submission.memory_usage || 0 }}KB</span>
              </div>
              
              <div class="detail-item">
                <el-icon><Code /></el-icon>
                <span>{{ submission.language?.toUpperCase() || 'CPP' }}</span>
              </div>
            </div>
            
            <div v-if="submission.message" class="submission-message">
              <p>{{ submission.message }}</p>
            </div>
            
            <div v-if="submission.error" class="submission-error">
              <h5>错误信息:</h5>
              <pre>{{ submission.error }}</pre>
            </div>
          </div>
        </el-card>
        
        <!-- 空状态 -->
        <div v-if="filteredSubmissions.length === 0 && !problemStore.loading" class="empty-state">
          <el-empty description="暂无提交记录" />
        </div>
        
        <!-- 加载状态 -->
        <div v-if="problemStore.loading" class="loading-container">
          <el-skeleton :rows="3" animated />
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { ref, computed, onMounted } from 'vue'
import { useProblemStore } from '@/stores/problem'
import { Clock, MemoryCard, Code } from '@element-plus/icons-vue'

export default {
  name: 'SubmissionHistory',
  setup() {
    const problemStore = useProblemStore()
    
    const statusFilter = ref('')
    const problemFilter = ref('')
    const statistics = ref({})
    
    // 计算过滤后的提交记录
    const filteredSubmissions = computed(() => {
      let submissions = problemStore.submissions || []
      
      // 状态过滤
      if (statusFilter.value) {
        submissions = submissions.filter(submission => 
          submission.status === statusFilter.value
        )
      }
      
      // 题目过滤
      if (problemFilter.value) {
        submissions = submissions.filter(submission => 
          submission.question_id.toString().includes(problemFilter.value)
        )
      }
      
      return submissions
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
    
    // 格式化时间
    const formatTime = (timestamp) => {
      if (!timestamp) return ''
      const date = new Date(parseInt(timestamp) * 1000)
      return date.toLocaleString('zh-CN')
    }
    
    // 筛选处理
    const handleFilter = () => {
      // 筛选逻辑已在computed中处理
    }
    
    // 加载数据
    const loadData = async () => {
      await Promise.all([
        problemStore.fetchSubmissions(),
        problemStore.fetchStatistics()
      ])
      statistics.value = problemStore.statistics
    }
    
    onMounted(() => {
      loadData()
    })
    
    return {
      problemStore,
      statusFilter,
      problemFilter,
      statistics,
      filteredSubmissions,
      getStatusType,
      formatTime,
      handleFilter,
      Clock,
      MemoryCard,
      Code
    }
  }
}
</script>

<style scoped>
.submission-history-page {
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

.stats-section {
  margin-bottom: 30px;
}

.stat-card {
  text-align: center;
}

.stat-content {
  padding: 20px;
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

.filters-section {
  margin-bottom: 30px;
}

.filter-form {
  background: white;
  padding: 20px;
  border-radius: 8px;
  box-shadow: 0 2px 8px rgba(0, 0, 0, 0.1);
}

.submissions-container {
  display: grid;
  gap: 20px;
}

.submission-card {
  transition: all 0.3s;
}

.submission-card:hover {
  transform: translateY(-2px);
  box-shadow: 0 8px 25px rgba(0, 0, 0, 0.1);
}

.submission-content {
  padding: 10px;
}

.submission-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 15px;
}

.submission-info {
  display: flex;
  gap: 20px;
  font-size: 14px;
  color: #7f8c8d;
}

.submission-details {
  display: flex;
  gap: 20px;
  margin-bottom: 15px;
}

.detail-item {
  display: flex;
  align-items: center;
  gap: 5px;
  font-size: 14px;
  color: #7f8c8d;
}

.submission-message {
  margin-bottom: 15px;
}

.submission-message p {
  margin: 0;
  color: #666;
  font-size: 14px;
}

.submission-error {
  background: #fef0f0;
  border: 1px solid #fbc4c4;
  border-radius: 4px;
  padding: 15px;
}

.submission-error h5 {
  margin: 0 0 10px 0;
  color: #f56c6c;
  font-size: 14px;
}

.submission-error pre {
  margin: 0;
  color: #f56c6c;
  font-size: 12px;
  white-space: pre-wrap;
  word-break: break-all;
}

.empty-state {
  text-align: center;
  padding: 60px 0;
}

.loading-container {
  padding: 20px 0;
}

/* 响应式设计 */
@media (max-width: 768px) {
  .submission-header {
    flex-direction: column;
    align-items: flex-start;
    gap: 10px;
  }
  
  .submission-info {
    flex-direction: column;
    gap: 5px;
  }
  
  .submission-details {
    flex-direction: column;
    gap: 10px;
  }
  
  .filter-form {
    padding: 15px;
  }
}
</style>
