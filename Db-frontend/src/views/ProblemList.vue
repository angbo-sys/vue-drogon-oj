<template>
  <div class="problem-list-page">
    <div class="container">
      <div class="page-header">
        <h1>题目列表</h1>
        <p>选择题目开始编程练习</p>
      </div>
      
      <!-- 搜索和筛选 -->
      <div class="filters-section">
        <div class="search-box">
          <el-input
            v-model="searchQuery"
            placeholder="搜索题目..."
            :prefix-icon="Search"
            clearable
            @input="handleSearch"
          />
        </div>
        
        <div class="filter-options">
          <el-select v-model="difficultyFilter" placeholder="难度" clearable @change="handleFilter">
            <el-option label="入门" value="0" />
            <el-option label="简单" value="1" />
            <el-option label="中等" value="2" />
            <el-option label="困难" value="3" />
          </el-select>
          
          <el-select v-model="typeFilter" placeholder="类型" clearable @change="handleFilter">
            <el-option label="动态规划" value="1" />
            <el-option label="前缀和" value="2" />
            <el-option label="贪心" value="3" />
          </el-select>
        </div>
      </div>
      
      <!-- 题目列表 -->
      <div class="problems-container">
        <el-card
          v-for="problem in filteredProblems"
          :key="problem.id"
          class="problem-card"
          @click="goToProblem(problem.id)"
        >
          <div class="problem-content">
            <div class="problem-header">
              <h3 class="problem-title">{{ problem.name }}</h3>
              <el-tag :type="getDifficultyType(problem.difficulty)">
                {{ getDifficultyText(problem.difficulty) }}
              </el-tag>
            </div>
            
            <div class="problem-meta">
              <span class="problem-id">#{{ problem.id }}</span>
              <span class="problem-source">{{ problem.source }}</span>
              <span class="problem-type">{{ getTypeText(problem.type) }}</span>
            </div>
            
            <div class="problem-stats">
              <span class="stat-item">
                <el-icon><User /></el-icon>
                {{ problem.solved_count || 0 }} 人通过
              </span>
              <span class="stat-item">
                <el-icon><Clock /></el-icon>
                {{ problem.time_limit || 1000 }}ms
              </span>
              <span class="stat-item">
                <el-icon><Cpu /></el-icon>
                {{ problem.memory_limit || 256 }}MB
              </span>
            </div>
          </div>
        </el-card>
        
        <!-- 空状态 -->
        <div v-if="filteredProblems.length === 0" class="empty-state">
          <el-empty description="暂无题目" />
        </div>
      </div>
      
      <!-- 加载状态 -->
      <div v-if="problemStore.loading" class="loading-container">
        <el-skeleton :rows="5" animated />
      </div>
    </div>
  </div>
</template>

<script>
import { ref, computed, onMounted } from 'vue'
import { useRouter } from 'vue-router'
import { useProblemStore } from '@/stores/problem'
import { Search, User, Clock, Cpu } from '@element-plus/icons-vue'

export default {
  name: 'ProblemList',
  setup() {
    const router = useRouter()
    const problemStore = useProblemStore()
    
    const searchQuery = ref('')
    const difficultyFilter = ref('')
    const typeFilter = ref('')
    
    // 计算过滤后的题目列表
    const filteredProblems = computed(() => {
      let problems = problemStore.problems || []
      
      // 搜索过滤
      if (searchQuery.value) {
        const query = searchQuery.value.toLowerCase()
        problems = problems.filter(problem => 
          problem.name.toLowerCase().includes(query) ||
          problem.description?.toLowerCase().includes(query)
        )
      }
      
      // 难度过滤
      if (difficultyFilter.value !== '') {
        problems = problems.filter(problem => 
          problem.difficulty === parseInt(difficultyFilter.value)
        )
      }
      
      // 类型过滤
      if (typeFilter.value !== '') {
        problems = problems.filter(problem => 
          problem.type === parseInt(typeFilter.value)
        )
      }
      
      return problems
    })
    
    // 获取难度类型
    const getDifficultyType = (difficulty) => {
      const types = { 0: 'info', 1: 'success', 2: 'warning', 3: 'danger' }
      return types[difficulty] || 'info'
    }
    
    // 获取难度文本
    const getDifficultyText = (difficulty) => {
      const texts = { 0: '入门', 1: '简单', 2: '中等', 3: '困难' }
      return texts[difficulty] || '未知'
    }
    
    // 获取类型文本
    const getTypeText = (type) => {
      const texts = { 0: '无类型', 1: '动态规划', 2: '前缀和', 3: '贪心' }
      return texts[type] || '未知'
    }
    
    // 跳转到题目详情
    const goToProblem = (id) => {
      router.push(`/problem/${id}`)
    }
    
    // 搜索处理
    const handleSearch = () => {
      // 搜索逻辑已在computed中处理
    }
    
    // 筛选处理
    const handleFilter = () => {
      // 筛选逻辑已在computed中处理
    }
    
    // 加载题目列表
    const loadProblems = async () => {
      await problemStore.fetchProblems()
    }
    
    onMounted(() => {
      loadProblems()
    })
    
    return {
      problemStore,
      searchQuery,
      difficultyFilter,
      typeFilter,
      filteredProblems,
      getDifficultyType,
      getDifficultyText,
      getTypeText,
      goToProblem,
      handleSearch,
      handleFilter,
      Search,
      User,
      Clock,
      Cpu
    }
  }
}
</script>

<style scoped>
.problem-list-page {
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

.filters-section {
  display: flex;
  gap: 20px;
  margin-bottom: 30px;
  flex-wrap: wrap;
  align-items: center;
}

.search-box {
  flex: 1;
  min-width: 300px;
}

.filter-options {
  display: flex;
  gap: 15px;
}

.problems-container {
  display: grid;
  gap: 20px;
}

.problem-card {
  cursor: pointer;
  transition: all 0.3s;
  border: 1px solid #e4e7ed;
}

.problem-card:hover {
  transform: translateY(-2px);
  box-shadow: 0 8px 25px rgba(0, 0, 0, 0.1);
  border-color: #409eff;
}

.problem-content {
  padding: 10px;
}

.problem-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 15px;
}

.problem-title {
  font-size: 20px;
  font-weight: 600;
  color: #2c3e50;
  margin: 0;
}

.problem-meta {
  display: flex;
  gap: 20px;
  margin-bottom: 15px;
  font-size: 14px;
  color: #7f8c8d;
}

.problem-stats {
  display: flex;
  gap: 20px;
  font-size: 14px;
  color: #7f8c8d;
}

.stat-item {
  display: flex;
  align-items: center;
  gap: 5px;
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
  .filters-section {
    flex-direction: column;
    align-items: stretch;
  }
  
  .search-box {
    min-width: auto;
  }
  
  .filter-options {
    justify-content: center;
  }
  
  .problem-header {
    flex-direction: column;
    align-items: flex-start;
    gap: 10px;
  }
  
  .problem-meta,
  .problem-stats {
    flex-direction: column;
    gap: 10px;
  }
}
</style>
