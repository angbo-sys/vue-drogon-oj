<template>
  <div id="app">
    <AppHeader v-if="showHeader" />
    <main class="main-content">
      <router-view />
    </main>
    <AppFooter v-if="showFooter" />
  </div>
</template>

<script>
import { computed } from 'vue'
import { useRoute } from 'vue-router'
import AppHeader from '@/components/AppHeader.vue'
import AppFooter from '@/components/AppFooter.vue'

export default {
  name: 'App',
  components: {
    AppHeader,
    AppFooter
  },
  setup() {
    const route = useRoute()
    
    // 根据路由决定是否显示头部和底部
    const showHeader = computed(() => {
      return !['Login', 'Register'].includes(route.name)
    })
    
    const showFooter = computed(() => {
      return !['Login', 'Register'].includes(route.name)
    })
    
    return {
      showHeader,
      showFooter
    }
  }
}
</script>

<style>
* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

#app {
  min-height: 100vh;
  display: flex;
  flex-direction: column;
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', 'Roboto', 'Oxygen',
    'Ubuntu', 'Cantarell', 'Fira Sans', 'Droid Sans', 'Helvetica Neue',
    sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
}

.main-content {
  flex: 1;
  min-height: calc(100vh - 120px);
}

/* 全局样式 */
.container {
  max-width: 1200px;
  margin: 0 auto;
  padding: 0 20px;
}

.page-container {
  padding: 20px 0;
}

.card {
  background: #fff;
  border-radius: 8px;
  box-shadow: 0 2px 12px 0 rgba(0, 0, 0, 0.1);
  padding: 20px;
  margin-bottom: 20px;
}

.text-center {
  text-align: center;
}

.mb-20 {
  margin-bottom: 20px;
}

.mt-20 {
  margin-top: 20px;
}

/* 响应式设计 */
@media (max-width: 768px) {
  .container {
    padding: 0 15px;
  }
  
  .page-container {
    padding: 15px 0;
  }
}
</style>