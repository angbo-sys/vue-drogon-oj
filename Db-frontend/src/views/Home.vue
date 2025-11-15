<template>
  <div class="home-page">
    <!-- 英雄区域 -->
    <section class="hero-section">
      <div class="container">
        <div class="hero-content">
          <h1 class="hero-title">欢迎来到OJ平台</h1>
          <p class="hero-subtitle">在线编程练习平台，提升你的编程技能</p>
          <div class="hero-actions">
            <el-button
              v-if="!userStore.isLoggedIn"
              type="primary"
              size="large"
              @click="$router.push('/login')"
            >
              开始练习
            </el-button>
            <el-button
              v-else
              type="primary"
              size="large"
              @click="$router.push('/problems')"
            >
              开始练习
            </el-button>
            <el-button
              v-if="!userStore.isLoggedIn"
              size="large"
              @click="$router.push('/register')"
            >
              注册账号
            </el-button>
          </div>
        </div>
      </div>
    </section>
    
    <!-- 功能特色 -->
    <section class="features-section">
      <div class="container">
        <h2 class="section-title">平台特色</h2>
        <div class="features-grid">
          <div class="feature-card" @click="navigateToProblems">
            <div class="feature-icon">
              <el-icon><Document /></el-icon>
            </div>
            <h3>在线编程</h3>
            <p>支持C++代码在线编写、编译和运行</p>
            <div class="card-arrow">
              <el-icon><ArrowRight /></el-icon>
            </div>
          </div>
          
          <div class="feature-card" @click="navigateToProblems">
            <div class="feature-icon">
              <el-icon><Trophy /></el-icon>
            </div>
            <h3>题目练习</h3>
            <p>丰富的算法题目，从入门到进阶</p>
            <div class="card-arrow">
              <el-icon><ArrowRight /></el-icon>
            </div>
          </div>
          
          <div class="feature-card" @click="navigateToSubmissions">
            <div class="feature-icon">
              <el-icon><DataAnalysis /></el-icon>
            </div>
            <h3>实时评测</h3>
            <p>即时反馈代码执行结果和性能分析</p>
            <div class="card-arrow">
              <el-icon><ArrowRight /></el-icon>
            </div>
          </div>
          
          <div class="feature-card" @click="navigateToProfile">
            <div class="feature-icon">
              <el-icon><User /></el-icon>
            </div>
            <h3>个人中心</h3>
            <p>记录学习进度，查看提交历史</p>
            <div class="card-arrow">
              <el-icon><ArrowRight /></el-icon>
            </div>
          </div>
        </div>
      </div>
    </section>
    
    <!-- 统计信息 -->
    <section v-if="userStore.isLoggedIn" class="stats-section">
      <div class="container">
        <h2 class="section-title">学习统计</h2>
        <div class="stats-grid">
          <div class="stat-card">
            <div class="stat-number">{{ statistics.total_submissions || 0 }}</div>
            <div class="stat-label">总提交次数</div>
          </div>
          
          <div class="stat-card">
            <div class="stat-number">{{ statistics.accepted_submissions || 0 }}</div>
            <div class="stat-label">通过题目</div>
          </div>
          
          <div class="stat-card">
            <div class="stat-number">{{ Math.round((statistics.acceptance_rate || 0) * 100) }}%</div>
            <div class="stat-label">通过率</div>
          </div>
        </div>
      </div>
    </section>
  </div>
</template>

<script>
import { ref, onMounted } from 'vue'
import { useRouter } from 'vue-router'
import { useUserStore } from '@/stores/user'
import { useProblemStore } from '@/stores/problem'
import { Document, Trophy, DataAnalysis, User, ArrowRight } from '@element-plus/icons-vue'

export default {
  name: 'HomePage',
  setup() {
    const router = useRouter()
    const userStore = useUserStore()
    const problemStore = useProblemStore()
    
    const statistics = ref({})
    
    // 路由跳转方法
    const navigateToProblems = () => {
      if (userStore.isLoggedIn) {
        router.push('/problems')
      } else {
        router.push('/login')
      }
    }
    
    const navigateToSubmissions = () => {
      if (userStore.isLoggedIn) {
        router.push('/submissions')
      } else {
        router.push('/login')
      }
    }
    
    const navigateToProfile = () => {
      if (userStore.isLoggedIn) {
        router.push('/profile')
      } else {
        router.push('/login')
      }
    }
    
    const loadStatistics = async () => {
      if (userStore.isLoggedIn) {
        const result = await problemStore.fetchStatistics()
        if (result.success) {
          statistics.value = result.data.statistics
        }
      }
    }
    
    onMounted(() => {
      loadStatistics()
    })
    
    return {
      userStore,
      statistics,
      navigateToProblems,
      navigateToSubmissions,
      navigateToProfile,
      Document,
      Trophy,
      DataAnalysis,
      User,
      ArrowRight
    }
  }
}
</script>

<style scoped>
.home-page {
  min-height: 100vh;
}

.hero-section {
  background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
  color: white;
  padding: 80px 0;
  text-align: center;
}

.hero-content {
  max-width: 600px;
  margin: 0 auto;
}

.hero-title {
  font-size: 48px;
  font-weight: 700;
  margin-bottom: 20px;
  line-height: 1.2;
}

.hero-subtitle {
  font-size: 20px;
  margin-bottom: 40px;
  opacity: 0.9;
  line-height: 1.5;
}

.hero-actions {
  display: flex;
  gap: 20px;
  justify-content: center;
  flex-wrap: wrap;
}

.features-section {
  padding: 80px 0;
  background: #f8f9fa;
}

.section-title {
  text-align: center;
  font-size: 36px;
  font-weight: 600;
  margin-bottom: 60px;
  color: #2c3e50;
}

.features-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(250px, 1fr));
  gap: 40px;
  max-width: 1000px;
  margin: 0 auto;
}

.feature-card {
  background: white;
  padding: 40px 30px;
  border-radius: 12px;
  text-align: center;
  box-shadow: 0 4px 20px rgba(0, 0, 0, 0.1);
  transition: all 0.3s ease;
  cursor: pointer;
  position: relative;
  overflow: hidden;
}

.feature-card:hover {
  transform: translateY(-5px);
  box-shadow: 0 8px 30px rgba(0, 0, 0, 0.15);
  background: linear-gradient(135deg, #f8f9fa 0%, #ffffff 100%);
}

.feature-card:active {
  transform: translateY(-2px);
}

.feature-icon {
  font-size: 48px;
  color: #409eff;
  margin-bottom: 20px;
}

.feature-card h3 {
  font-size: 24px;
  font-weight: 600;
  margin-bottom: 15px;
  color: #2c3e50;
}

.feature-card p {
  color: #7f8c8d;
  line-height: 1.6;
  font-size: 16px;
  margin-bottom: 20px;
}

.card-arrow {
  position: absolute;
  top: 20px;
  right: 20px;
  color: #409eff;
  font-size: 20px;
  opacity: 0;
  transition: all 0.3s ease;
  transform: translateX(10px);
}

.feature-card:hover .card-arrow {
  opacity: 1;
  transform: translateX(0);
}

.stats-section {
  padding: 80px 0;
  background: white;
}

.stats-grid {
  display: grid;
  grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
  gap: 40px;
  max-width: 800px;
  margin: 0 auto;
}

.stat-card {
  text-align: center;
  padding: 30px;
  background: #f8f9fa;
  border-radius: 12px;
  transition: transform 0.3s;
}

.stat-card:hover {
  transform: translateY(-3px);
}

.stat-number {
  font-size: 48px;
  font-weight: 700;
  color: #409eff;
  margin-bottom: 10px;
}

.stat-label {
  font-size: 16px;
  color: #7f8c8d;
  font-weight: 500;
}

/* 响应式设计 */
@media (max-width: 768px) {
  .hero-title {
    font-size: 36px;
  }
  
  .hero-subtitle {
    font-size: 18px;
  }
  
  .hero-actions {
    flex-direction: column;
    align-items: center;
  }
  
  .section-title {
    font-size: 28px;
  }
  
  .features-grid {
    grid-template-columns: 1fr;
    gap: 30px;
  }
  
  .stats-grid {
    grid-template-columns: 1fr;
    gap: 20px;
  }
}
</style>
