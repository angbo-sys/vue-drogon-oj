import { createRouter, createWebHistory } from 'vue-router'
import { useUserStore } from '@/stores/user'

// 路由组件懒加载
const Home = () => import('@/views/Home.vue')
const Login = () => import('@/views/Login.vue')
const Register = () => import('@/views/Register.vue')
const ProblemList = () => import('@/views/ProblemList.vue')
const ProblemDetail = () => import('@/views/ProblemDetail.vue')
const SubmissionHistory = () => import('@/views/SubmissionHistory.vue')
const Profile = () => import('@/views/Profile.vue')
const NotFound = () => import('@/views/NotFound.vue')

const routes = [
  {
    path: '/',
    name: 'Home',
    component: Home,
    meta: { requiresAuth: false }
  },
  {
    path: '/login',
    name: 'Login',
    component: Login,
    meta: { requiresAuth: false, hideForAuth: true }
  },
  {
    path: '/register',
    name: 'Register',
    component: Register,
    meta: { requiresAuth: false, hideForAuth: true }
  },
  {
    path: '/problems',
    name: 'ProblemList',
    component: ProblemList,
    meta: { requiresAuth: true }
  },
  {
    path: '/problem/:id',
    name: 'ProblemDetail',
    component: ProblemDetail,
    meta: { requiresAuth: true }
  },
  {
    path: '/submissions',
    name: 'SubmissionHistory',
    component: SubmissionHistory,
    meta: { requiresAuth: true }
  },
  {
    path: '/profile',
    name: 'Profile',
    component: Profile,
    meta: { requiresAuth: true }
  },
  {
    path: '/:pathMatch(.*)*',
    name: 'NotFound',
    component: NotFound,
    meta: { requiresAuth: false }
  }
]

const router = createRouter({
  history: createWebHistory(process.env.BASE_URL),
  routes
})

// 路由守卫
router.beforeEach((to, from, next) => {
  const userStore = useUserStore()
  
  // 检查登录状态
  const isLoggedIn = userStore.checkAuth()
  
  // 如果路由需要认证但用户未登录
  if (to.meta.requiresAuth && !isLoggedIn) {
    next('/login')
    return
  }
  
  // 如果用户已登录但访问登录/注册页面
  if (to.meta.hideForAuth && isLoggedIn) {
    next('/')
    return
  }
  
  next()
})

export default router
