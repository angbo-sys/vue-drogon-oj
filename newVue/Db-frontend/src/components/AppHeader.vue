<template>
  <header class="app-header">
    <div class="container">
      <div class="header-content">
        <!-- Logo -->
        <div class="logo">
          <router-link to="/" class="logo-link">
            <el-icon class="logo-icon"><Trophy /></el-icon>
            <span class="logo-text">Db Platform</span>
          </router-link>
        </div>
        
        <!-- 导航菜单 -->
        <nav class="nav-menu">
          <router-link to="/" class="nav-item">首页</router-link>
          <router-link to="/problems" class="nav-item">题目</router-link>
          <router-link to="/submissions" class="nav-item">提交记录</router-link>
        </nav>
        
        <!-- 用户菜单 -->
        <div class="user-menu">
          <template v-if="userStore.isLoggedIn">
            <el-dropdown @command="handleUserCommand">
              <span class="user-info">
                <el-avatar :size="32" :src="userStore.userInfo.avatar">
                  {{ userStore.userInfo.username?.charAt(0)?.toUpperCase() }}
                </el-avatar>
                <span class="username">{{ userStore.userInfo.username }}</span>
                <el-icon><ArrowDown /></el-icon>
              </span>
              <template #dropdown>
                <el-dropdown-menu>
                  <el-dropdown-item command="profile">
                    <el-icon><User /></el-icon>
                    个人中心
                  </el-dropdown-item>
                  <el-dropdown-item command="logout" divided>
                    <el-icon><SwitchButton /></el-icon>
                    退出登录
                  </el-dropdown-item>
                </el-dropdown-menu>
              </template>
            </el-dropdown>
          </template>
          <template v-else>
            <el-button type="primary" @click="$router.push('/login')">
              登录
            </el-button>
            <el-button @click="$router.push('/register')">
              注册
            </el-button>
          </template>
        </div>
      </div>
    </div>
  </header>
</template>

<script>
import { useUserStore } from '@/stores/user'
import { useRouter } from 'vue-router'
import { ElMessage, ElMessageBox } from 'element-plus'

export default {
  name: 'AppHeader',
  setup() {
    const userStore = useUserStore()
    const router = useRouter()
    
    const handleUserCommand = async (command) => {
      switch (command) {
        case 'profile':
          // 跳转到个人中心
          router.push('/profile')
          break
        case 'logout':
          try {
            await ElMessageBox.confirm('确定要退出登录吗？', '提示', {
              confirmButtonText: '确定',
              cancelButtonText: '取消',
              type: 'warning'
            })
            await userStore.userLogout()
            ElMessage.success('退出登录成功')
            // 跳转到首页
            router.push('/')
          } catch (error) {
            // 用户取消
          }
          break
      }
    }
    
    return {
      userStore,
      handleUserCommand
    }
  }
}
</script>

<style scoped>
.app-header {
  background: #fff;
  border-bottom: 1px solid #e4e7ed;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
  position: sticky;
  top: 0;
  z-index: 1000;
}

.header-content {
  display: flex;
  align-items: center;
  justify-content: space-between;
  height: 60px;
}

.logo {
  display: flex;
  align-items: center;
}

.logo-link {
  display: flex;
  align-items: center;
  text-decoration: none;
  color: #409eff;
  font-size: 20px;
  font-weight: bold;
}

.logo-icon {
  font-size: 24px;
  margin-right: 8px;
}

.logo-text {
  font-size: 20px;
}

.nav-menu {
  display: flex;
  align-items: center;
  gap: 30px;
}

.nav-item {
  text-decoration: none;
  color: #606266;
  font-size: 16px;
  padding: 8px 12px;
  border-radius: 4px;
  transition: all 0.3s;
}

.nav-item:hover,
.nav-item.router-link-active {
  color: #409eff;
  background-color: #ecf5ff;
}

.user-menu {
  display: flex;
  align-items: center;
  gap: 12px;
}

.user-info {
  display: flex;
  align-items: center;
  gap: 8px;
  cursor: pointer;
  padding: 8px 12px;
  border-radius: 4px;
  transition: background-color 0.3s;
}

.user-info:hover {
  background-color: #f5f7fa;
}

.username {
  font-size: 14px;
  color: #606266;
}

/* 响应式设计 */
@media (max-width: 768px) {
  .nav-menu {
    display: none;
  }
  
  .logo-text {
    display: none;
  }
  
  .username {
    display: none;
  }
}
</style>
