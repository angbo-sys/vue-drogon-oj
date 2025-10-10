<template>
  <div class="problem-detail-page">
    <div class="container">
      <el-row :gutter="20">
        <!-- 题目描述区域 -->
        <el-col :span="14">
          <el-card class="problem-card" shadow="never">
            <template #header>
              <div class="card-header">
                <div class="problem-title">
                  <h2>{{ problem?.name || '加载中...' }}</h2>
                  <div class="problem-meta">
                    <el-tag :type="getDifficultyType(problem?.difficulty_id)" size="large">
                      {{ getDifficultyText(problem?.difficulty_id) }}
                    </el-tag>
                    <el-tag type="info" size="large">{{ problem?.source || '未知来源' }}</el-tag>
                  </div>
                </div>
                <div class="problem-actions">
                  <el-button type="primary" @click="toggleFavorite" :icon="isFavorited ? 'StarFilled' : 'Star'">
                    {{ isFavorited ? '已收藏' : '收藏' }}
                  </el-button>
                  <el-button @click="shareProblem" icon="Share">分享</el-button>
                  <el-button type="success" @click="openAIChat" icon="ChatDotRound">
                    AI助手
                  </el-button>
                </div>
              </div>
            </template>
            
            <!-- 标签页切换 -->
            <el-tabs v-model="activeTab" class="problem-tabs">
              <!-- 题目描述标签页 -->
              <el-tab-pane label="题目描述" name="description">
                <!-- 加载状态 -->
                <div v-if="problemStore.loading" class="loading-container">
                  <el-skeleton :rows="15" animated />
                </div>
                
                <!-- 题目内容 -->
                <div v-else-if="problem" class="problem-content">
                  <!-- 题目统计信息 -->
                  <div class="problem-stats">
                    <div class="stat-item">
                      <span class="stat-label">通过率:</span>
                      <span class="stat-value">{{ problem.acceptance_rate || '--' }}%</span>
                    </div>
                    <div class="stat-item">
                      <span class="stat-label">提交次数:</span>
                      <span class="stat-value">{{ problem.submit_count || 0 }}</span>
                    </div>
                    <div class="stat-item">
                      <span class="stat-label">通过次数:</span>
                      <span class="stat-value">{{ problem.accept_count || 0 }}</span>
                    </div>
                    <div class="stat-item">
                      <span class="stat-label">时间限制:</span>
                      <span class="stat-value">{{ problem.time_limit || 1000 }}ms</span>
                    </div>
                    <div class="stat-item">
                      <span class="stat-label">内存限制:</span>
                      <span class="stat-value">{{ problem.memory_limit || 256 }}MB</span>
                    </div>
                  </div>

                  <!-- 题目描述 -->
                  <div class="problem-description" v-html="formattedDescription"></div>
                  
                  <!-- 题目标签 -->
                  <div class="problem-tags" v-if="problem.tags && problem.tags.length">
                    <el-tag 
                      v-for="tag in problem.tags" 
                      :key="tag" 
                      type="info" 
                      effect="plain"
                      class="tag-item"
                    >
                      {{ tag }}
                    </el-tag>
                  </div>
                </div>
              </el-tab-pane>

              <!-- 评论区标签页 -->
              <el-tab-pane label="评论区" name="comments">
                <div class="comments-section">
                  <!-- 评论统计 -->
                  <div class="comments-header">
                    <h3>评论区</h3>
                    <div class="comments-stats">
                      <span class="comment-count">共 {{ commentCount }} 条评论</span>
                    </div>
                  </div>

                  <!-- 发表评论 -->
                  <div class="comment-form">
                    <el-input
                      v-model="newComment"
                      type="textarea"
                      :rows="3"
                      placeholder="写下你的评论..."
                      maxlength="500"
                      show-word-limit
                    />
                    <div class="comment-actions">
                      <el-button 
                        type="primary" 
                        @click="submitComment"
                        :loading="submittingComment"
                        :disabled="!newComment.trim()"
                      >
                        发表评论
                      </el-button>
                    </div>
                  </div>

                  <!-- 评论列表 -->
                  <div class="comments-list">
                    <div v-if="commentsLoading" class="loading-comments">
                      <el-skeleton :rows="3" animated />
                    </div>
                    <div v-else-if="comments.length === 0" class="no-comments">
                      <el-empty description="暂无评论，快来抢沙发吧！" />
                    </div>
                    <div v-else>
                      <div 
                        v-for="comment in comments" 
                        :key="comment.id"
                        class="comment-item"
                      >
                        <div class="comment-header">
                          <div class="comment-user">
                            <el-avatar :size="32" :src="comment.avatar">
                              {{ comment.username?.charAt(0) }}
                            </el-avatar>
                            <div class="user-info">
                              <span class="username">{{ comment.username }}</span>
                              <span class="comment-time">{{ formatCommentTime(comment.created_at) }}</span>
                            </div>
                          </div>
                          <div class="comment-actions" v-if="comment.user_id === userStore.userId">
                            <el-button 
                              size="small" 
                              type="danger" 
                              text
                              @click="deleteComment(comment.id)"
                            >
                              删除
                            </el-button>
                          </div>
                        </div>
                        <div class="comment-content">
                          {{ comment.content }}
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
              </el-tab-pane>

              <!-- 题解标签页 -->
              <el-tab-pane label="题解" name="solutions">
                <div class="solutions-section">
                  <!-- 题解统计 -->
                  <div class="solutions-header">
                    <h3>题解</h3>
                    <el-button type="primary" @click="showSubmitSolution = true">
                      提交题解
                    </el-button>
                  </div>

                  <!-- 题解列表 -->
                  <div class="solutions-list">
                    <div v-if="solutionsLoading" class="loading-solutions">
                      <el-skeleton :rows="3" animated />
                    </div>
                    <div v-else-if="solutions.length === 0" class="no-solutions">
                      <el-empty description="暂无题解，快来分享你的解题思路吧！" />
                    </div>
                    <div v-else>
                      <div 
                        v-for="solution in solutions" 
                        :key="solution.analysis_id"
                        class="solution-item"
                      >
                        <div class="solution-header">
                          <div class="solution-user">
                            <el-avatar :size="32" :src="solution.avatar">
                              {{ solution.username?.charAt(0) }}
                            </el-avatar>
                            <div class="user-info">
                              <span class="username">{{ solution.username }}</span>
                              <span class="solution-time">{{ formatCommentTime(solution.created_at) }}</span>
                            </div>
                          </div>
                          <div class="solution-stats">
                            <button 
                              :class="solution.isLiked ? 'like-button liked' : 'like-button'"
                              @click="toggleLikeSolution(solution.analysis_id)"
                            >
                              <HeartSolidIconFilled v-if="solution.isLiked" class="heart-icon" />
                              <HeartIcon v-else class="heart-icon" />
                              {{ solution.like_count || 0 }}
                            </button>
                          </div>
                        </div>
                        <div class="solution-content" v-html="renderMarkdown(solution.analysis_content)"></div>
                      </div>
                    </div>
                  </div>
                </div>
              </el-tab-pane>
            </el-tabs>
          </el-card>
        </el-col>
        
        <!-- 代码编辑和提交区域 -->
        <el-col :span="10">
          <el-card class="code-card" shadow="never">
            <template #header>
              <h3>代码编辑器</h3>
              <div class="code-header">
                <div class="editor-controls">
                  <el-select v-model="selectedLanguage" @change="changeLanguage" style="width: 120px; margin-right: 10px;">
                    <el-option label="C++" value="cpp" />
                    <el-option label="Java" value="java" />
                    <el-option label="Python" value="python" />
                    <el-option label="JavaScript" value="javascript" />
                  </el-select>
                  <el-button 
                    type="primary" 
                    @click="submitCode" 
                    :loading="submitting"
                    :disabled="!code.trim()"
                    size="large"
                  >
                    <el-icon><Upload /></el-icon>
                    提交代码
                  </el-button>
                </div>
              </div>
            </template>
            
            <!-- 代码编辑器容器 -->
            <div class="code-editor-container">
              <el-input
                v-model="code"
                type="textarea"
                :rows="20"
                placeholder="请输入代码..."
                class="code-textarea"
                @input="onCodeChange"
              />
            </div>
            
            <!-- 提交结果 -->
            <div v-if="submissionResult" class="submission-result">
              <div class="result-header">
                <h4>提交结果</h4>
                <el-button @click="clearResult" size="small" text>清除</el-button>
              </div>
              
              <el-alert
                :title="submissionResult.message"
                :type="getResultType(submissionResult.status)"
                :description="submissionResult.error || submissionResult.output"
                show-icon
                :closable="false"
                class="result-alert"
              />
              
              <!-- 测试用例结果 -->
              <div v-if="submissionResult.test_case_results?.length" class="test-results">
                <h5>测试用例结果:</h5>
                <div class="test-case-list">
                  <div 
                    v-for="(result, index) in submissionResult.test_case_results" 
                    :key="index"
                    class="test-case-item"
                    :class="getTestCaseClass(result)"
                  >
                    <span class="test-case-number">用例 {{ index + 1 }}</span>
                    <span class="test-case-status">{{ result }}</span>
                  </div>
                </div>
              </div>
              
              <!-- 执行信息 -->
              <div v-if="submissionResult.execution_time" class="execution-info">
                <div class="execution-item">
                  <span class="execution-label">执行时间:</span>
                  <span class="execution-value">{{ submissionResult.execution_time.toFixed(3) }}s</span>
                </div>
                <div class="execution-item">
                  <span class="execution-label">内存使用:</span>
                  <span class="execution-value">{{ submissionResult.memory_usage }}KB</span>
                </div>
              </div>
            </div>
          </el-card>
        </el-col>
      </el-row>
    </div>

    <!-- AI聊天窗口 -->
    <div v-if="showAIChat" class="ai-chat-overlay" @click="closeAIChat">
      <div class="ai-chat-window" @click.stop>
        <div class="ai-chat-header">
          <div class="ai-chat-title">
            <el-icon><ChatDotRound /></el-icon>
            <span>AI题目助手</span>
          </div>
          <el-button @click="closeAIChat" circle size="small" text>
            <el-icon><Close /></el-icon>
          </el-button>
        </div>
        
        <div class="ai-chat-messages" ref="chatMessagesRef">
          <div 
            v-for="(message, index) in chatMessages" 
            :key="index"
            class="chat-message"
            :class="message.type"
          >
            <div class="message-avatar">
              <el-icon v-if="message.type === 'user'"><User /></el-icon>
              <el-icon v-else><Robot /></el-icon>
            </div>
            <div class="message-content">
              <div class="message-text" v-html="renderMarkdown(message.content)"></div>
              <div class="message-time">{{ formatTime(message.timestamp) }}</div>
            </div>
          </div>
          
          <!-- 加载状态 -->
          <div v-if="aiLoading" class="chat-message ai">
            <div class="message-avatar">
              <el-icon><Robot /></el-icon>
            </div>
            <div class="message-content">
              <div class="message-text">
                <el-icon class="loading-icon"><Loading /></el-icon>
                AI正在思考中...
              </div>
            </div>
          </div>
        </div>
        
        <div class="ai-chat-input">
          <el-input
            v-model="chatInput"
            placeholder="询问关于这道题的任何问题..."
            @keyup.enter="sendMessage"
            :disabled="aiLoading"
            class="chat-input-field"
          >
            <template #append>
              <el-button 
                @click="sendMessage" 
                :loading="aiLoading"
                :disabled="!chatInput.trim()"
                type="primary"
              >
                发送
              </el-button>
            </template>
          </el-input>
        </div>
      </div>
    </div>

    <!-- 题解提交对话框 -->
    <el-dialog
      v-model="showSubmitSolution"
      title="提交题解"
      width="600px"
      :before-close="handleCloseSolutionDialog"
    >
      <el-form :model="solutionForm" label-width="80px">
        <el-form-item label="题解内容">
          <el-input
            v-model="solutionForm.content"
            type="textarea"
            :rows="8"
            placeholder="请详细描述你的解题思路、算法分析、代码实现等..."
            maxlength="2000"
            show-word-limit
          />
        </el-form-item>
      </el-form>
      
      <template #footer>
        <div class="dialog-footer">
          <el-button @click="showSubmitSolution = false">取消</el-button>
          <el-button 
            type="primary" 
            @click="submitSolution"
            :loading="submittingSolution"
            :disabled="!solutionForm.content.trim()"
          >
            提交题解
          </el-button>
        </div>
      </template>
    </el-dialog>
  </div>
</template>

<script>
import { ref, onMounted, computed, nextTick, watch } from 'vue'
import { useRoute } from 'vue-router'
import { useProblemStore } from '@/stores/problem'
import { useUserStore } from '@/stores/user'
import { ElMessage } from 'element-plus'
import MarkdownIt from 'markdown-it'
import { ChatDotRound, Close, User, Robot, Loading, Star } from '@element-plus/icons-vue'
import { HeartIcon, HeartSolidIcon } from '@heroicons/vue/24/outline'
import { HeartIcon as HeartSolidIconFilled } from '@heroicons/vue/24/solid'
import { commentAPI, analysisAPI } from '@/api/comments'

export default {
  name: 'ProblemDetail',
  components: {
    ChatDotRound,
    Close,
    User,
    Robot,
    Loading,
    Star,
    HeartIcon,
    HeartSolidIcon,
    HeartSolidIconFilled
  },
  setup() {
    const route = useRoute()
    const problemStore = useProblemStore()
    const userStore = useUserStore()
    
    const submitting = ref(false)
    const problem = ref(null)
    const selectedLanguage = ref('cpp')
    const submissionResult = ref(null)
    const isFavorited = ref(false)
    
    // 标签页状态
    const activeTab = ref('description')
    
    // AI聊天相关状态
    const showAIChat = ref(false)
    const chatMessages = ref([])
    const chatInput = ref('')
    const aiLoading = ref(false)
    const chatMessagesRef = ref(null)
    
    // 评论相关状态
    const comments = ref([])
    const commentCount = ref(0)
    const commentsLoading = ref(false)
    const submittingComment = ref(false)
    const newComment = ref('')
    
    // 题解相关状态
    const solutions = ref([])
    const solutionsLoading = ref(false)
    const submittingSolution = ref(false)
    const showSubmitSolution = ref(false)
    const solutionForm = ref({
      content: ''
    })
    
    // 初始化markdown渲染器
    const md = new MarkdownIt({
      html: true,
      linkify: true,
      typographer: true
    })
    
    // 默认代码模板
    const codeTemplates = {
      cpp: `#include <iostream>
using namespace std;

int main() {
    int a, b;
    cin >> a >> b;
    cout << a + b << endl;
    return 0;
}`,
      java: `import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int a = scanner.nextInt();
        int b = scanner.nextInt();
        System.out.println(a + b);
    }
}`,
      python: `a, b = map(int, input().split())
print(a + b)`,
      javascript: `const readline = require('readline');
const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

rl.on('line', (line) => {
    const [a, b] = line.split(' ').map(Number);
    console.log(a + b);
    rl.close();
});`
    }
    
    const code = ref(codeTemplates.cpp)
    
    // Markdown渲染
    const formattedDescription = computed(() => {
      if (!problem.value?.description) {
        return '<p>题目描述加载中...</p>'
      }
      return md.render(problem.value.description)
    })
    
    // 移除复杂的Markdown渲染，使用简单的文本显示提高性能
    
    const getDifficultyType = (difficulty) => {
      const types = { 0: 'info', 1: 'success', 2: 'warning', 3: 'danger' }
      return types[difficulty] || 'info'
    }
    
    const getDifficultyText = (difficulty) => {
      const texts = { 0: '入门', 1: '简单', 2: '中等', 3: '困难' }
      return texts[difficulty] || '未知'
    }
    
    const getResultType = (status) => {
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
    
    const getTestCaseClass = (result) => {
      if (result.includes('通过') || result.includes('AC')) return 'test-case-pass'
      if (result.includes('错误') || result.includes('WA')) return 'test-case-fail'
      if (result.includes('超时') || result.includes('TLE')) return 'test-case-timeout'
      return 'test-case-unknown'
    }
    
    // 代码变化处理
    const onCodeChange = (value) => {
      code.value = value
    }
    
    // 语言切换处理
    const changeLanguage = (newLanguage) => {
      selectedLanguage.value = newLanguage
      code.value = codeTemplates[newLanguage] || codeTemplates.cpp
      ElMessage.success(`已切换到${newLanguage.toUpperCase()}语言`)
    }
    
    const submitCode = async () => {
      console.log('submitCode called')
      
      if (!code.value.trim()) {
        ElMessage.warning('请输入代码')
        return
      }
      
      submitting.value = true
      submissionResult.value = null
      
      try {
        console.log('Submitting code...')
        
        const result = await problemStore.submitCodeToJudge({
          user_id: userStore.userId,
          question_id: parseInt(route.params.id),
          code: code.value,
          language: selectedLanguage.value
        })
        
        console.log('Submit result:', result)
        
        if (result.success) {
          submissionResult.value = result.data.execution_result
          ElMessage.success('代码提交成功')
        } else {
          ElMessage.error(result.message)
        }
      } catch (error) {
        console.error('Submit error:', error)
        ElMessage.error('提交失败，请重试: ' + error.message)
      } finally {
        submitting.value = false
      }
    }
    
    const loadProblem = async () => {
      try {
        const problemId = route.params.id
        console.log('Loading problem with ID:', problemId)
        
        // 验证ID是否为有效数字
        if (!problemId || isNaN(parseInt(problemId))) {
          ElMessage.error('无效的题目ID')
          return
        }
        
        const result = await problemStore.fetchProblemDetail(parseInt(problemId))
        console.log('Problem load result:', result)
        
        if (result.success) {
          problem.value = problemStore.currentProblem
          console.log('Problem loaded:', problem.value)
        } else {
          ElMessage.error(result.message)
        }
      } catch (error) {
        console.error('Load problem error:', error)
        ElMessage.error('加载题目失败: ' + error.message)
      }
    }
    
    const toggleFavorite = () => {
      isFavorited.value = !isFavorited.value
      ElMessage.success(isFavorited.value ? '已添加到收藏' : '已取消收藏')
    }
    
    const shareProblem = () => {
      const url = window.location.href
      navigator.clipboard.writeText(url).then(() => {
        ElMessage.success('题目链接已复制到剪贴板')
      }).catch(() => {
        ElMessage.error('复制失败，请手动复制链接')
      })
    }
    
    const clearResult = () => {
      submissionResult.value = null
    }
    
    // AI聊天相关方法
    const openAIChat = () => {
      showAIChat.value = true
      // 添加欢迎消息
      if (!chatMessages.value || chatMessages.value.length === 0) {
        addMessage('ai', `你好！我是AI题目助手，可以帮你解读题目"${problem.value?.name || '这道题'}"。你可以问我关于题目理解、解题思路、算法分析等问题。`)
      }
    }
    
    const closeAIChat = () => {
      showAIChat.value = false
    }
    
    const addMessage = (type, content) => {
      console.log('添加消息:', { type, content })
      if (!chatMessages.value) {
        chatMessages.value = []
      }
      const messageObj = {
        type,
        content,
        timestamp: new Date()
      }
      console.log('消息对象:', messageObj)
      chatMessages.value.push(messageObj)
      console.log('当前消息列表:', chatMessages.value)
      // 滚动到底部
      nextTick(() => {
        if (chatMessagesRef.value) {
          chatMessagesRef.value.scrollTop = chatMessagesRef.value.scrollHeight
        }
      })
    }
    
    const formatTime = (timestamp) => {
      return timestamp.toLocaleTimeString('zh-CN', { 
        hour: '2-digit', 
        minute: '2-digit' 
      })
    }
    
    // Markdown渲染函数
    const renderMarkdown = (content) => {
      if (!content) return ''
      
      // 创建MarkdownIt实例
      const md = new MarkdownIt({
        html: true,
        linkify: true,
        typographer: true,
        breaks: true
      })
      
      // 渲染Markdown为HTML
      return md.render(content)
    }
    
    const sendMessage = async () => {
      if (!chatInput.value.trim() || aiLoading.value) return
      
      const userMessage = chatInput.value.trim()
      chatInput.value = ''
      
      // 添加用户消息
      addMessage('user', userMessage)
      
      // 设置加载状态
      aiLoading.value = true
      
      try {
        // 调用AI聊天API - 移除超时限制，让AI有足够时间思考
        const result = await problemStore.chatWithAI({
          user_id: userStore.userId,
          question_id: parseInt(route.params.id),
          message: userMessage,
          problem_info: {
            name: problem.value?.name,
            description: problem.value?.description,
            difficulty: problem.value?.difficulty_id,
            tags: problem.value?.tags
          }
        })
        
        console.log('AI聊天结果:', result)
        if (result.success) {
          console.log('AI回复内容:', result.data.response)
          addMessage('ai', result.data.response)
        } else {
          console.log('AI聊天失败:', result.message)
          addMessage('ai', '抱歉，我暂时无法回答这个问题。请稍后再试。')
        }
      } catch (error) {
        console.error('AI chat error:', error)
        addMessage('ai', '抱歉，发生了错误。请稍后再试。')
      } finally {
        aiLoading.value = false
      }
    }
    
    // 评论相关方法
    const loadComments = async () => {
      if (!problem.value?.id) return
      
      commentsLoading.value = true
      try {
        const response = await commentAPI.getProblemComments(problem.value.id, {
          page: 1,
          page_size: 20
        })
        
        if (response.data.status === 'success') {
          comments.value = response.data.comments || []
          commentCount.value = response.data.total || 0
        }
      } catch (error) {
        console.error('Load comments error:', error)
        ElMessage.error('加载评论失败')
      } finally {
        commentsLoading.value = false
      }
    }
    
    const submitComment = async () => {
      if (!newComment.value.trim()) {
        ElMessage.warning('请输入评论内容')
        return
      }
      
      if (!userStore.userId) {
        ElMessage.warning('请先登录')
        return
      }
      
      submittingComment.value = true
      try {
        const response = await commentAPI.createComment({
          problem_id: problem.value.id,
          user_id: userStore.userId,
          username: userStore.userInfo?.username || '匿名用户',
          content: newComment.value.trim()
        })
        
        if (response.data.status === 'success') {
          ElMessage.success('评论发表成功')
          newComment.value = ''
          await loadComments()
        } else {
          ElMessage.error(response.data.message || '发表评论失败')
        }
      } catch (error) {
        console.error('Submit comment error:', error)
        ElMessage.error('发表评论失败')
      } finally {
        submittingComment.value = false
      }
    }
    
    const deleteComment = async (commentId) => {
      try {
        const response = await commentAPI.deleteComment({
          comment_id: commentId,
          user_id: userStore.userId
        })
        
        if (response.data.status === 'success') {
          ElMessage.success('评论删除成功')
          await loadComments()
        } else {
          ElMessage.error(response.data.message || '删除评论失败')
        }
      } catch (error) {
        console.error('Delete comment error:', error)
        ElMessage.error('删除评论失败')
      }
    }
    
    const formatCommentTime = (timestamp) => {
      if (!timestamp) return ''
      
      // 处理Unix时间戳（秒）
      let date
      if (typeof timestamp === 'number') {
        // 如果是10位数字，说明是秒级时间戳
        if (timestamp.toString().length === 10) {
          date = new Date(timestamp * 1000)
        } else {
          date = new Date(timestamp)
        }
      } else {
        date = new Date(timestamp)
      }
      
      const now = new Date()
      const diff = now - date
      
      if (diff < 60000) return '刚刚'
      if (diff < 3600000) return `${Math.floor(diff / 60000)}分钟前`
      if (diff < 86400000) return `${Math.floor(diff / 3600000)}小时前`
      if (diff < 604800000) return `${Math.floor(diff / 86400000)}天前`
      
      return date.toLocaleDateString('zh-CN')
    }
    
    // 题解相关方法
    const loadSolutions = async () => {
      if (!problem.value?.id) return
      
      solutionsLoading.value = true
      try {
        const response = await analysisAPI.getQuestionAnalysis(problem.value.id)
        
        if (response.data.status === 'success') {
          solutions.value = response.data.analyses || []
          // 为每个题解添加用户名和头像信息
          solutions.value.forEach(solution => {
            solution.username = solution.username || '匿名用户'
            solution.avatar = solution.avatar || ''
            solution.isLiked = false // 默认未点赞
          })
        }
      } catch (error) {
        console.error('Load solutions error:', error)
        ElMessage.error('加载题解失败')
      } finally {
        solutionsLoading.value = false
      }
    }
    
    const submitSolution = async () => {
      if (!solutionForm.value.content.trim()) {
        ElMessage.warning('请输入题解内容')
        return
      }
      
      if (!userStore.userId) {
        ElMessage.warning('请先登录')
        return
      }
      
      submittingSolution.value = true
      try {
        const response = await analysisAPI.submitUserAnalysis({
          question_id: problem.value.id,
          user_id: userStore.userId,
          analysis_content: solutionForm.value.content.trim()
        })
        
        if (response.data.status === 'success') {
          ElMessage.success('题解提交成功')
          showSubmitSolution.value = false
          solutionForm.value.content = ''
          await loadSolutions()
        } else {
          ElMessage.error(response.data.message || '提交题解失败')
        }
      } catch (error) {
        console.error('Submit solution error:', error)
        ElMessage.error('提交题解失败')
      } finally {
        submittingSolution.value = false
      }
    }
    
    const toggleLikeSolution = async (solutionId) => {
      if (!userStore.userId) {
        ElMessage.warning('请先登录')
        return
      }
      
      try {
        const response = await analysisAPI.likeAnalysis({
          analysis_id: solutionId,
          user_id: userStore.userId
        })
        
        if (response.data.status === 'success') {
          // 更新本地状态
          const solution = solutions.value.find(s => s.analysis_id === solutionId)
          if (solution) {
            solution.isLiked = !solution.isLiked
            solution.like_count = (solution.like_count || 0) + (solution.isLiked ? 1 : -1)
          }
        } else {
          ElMessage.error(response.data.message || '操作失败')
        }
      } catch (error) {
        console.error('Toggle like error:', error)
        ElMessage.error('操作失败')
      }
    }
    
    const handleCloseSolutionDialog = () => {
      showSubmitSolution.value = false
      solutionForm.value.content = ''
    }
    
    // 监听标签页切换
    watch(activeTab, (newTab) => {
      if (newTab === 'comments' && comments.value.length === 0) {
        loadComments()
      } else if (newTab === 'solutions' && solutions.value.length === 0) {
        loadSolutions()
      }
    })
    
    
    onMounted(async () => {
      console.log('ProblemDetail mounted, route params:', route.params)
      // 只加载题目内容，不需要初始化编辑器
      await loadProblem()
    })
    
    return {
      problemStore,
      userStore,
      submitting,
      problem,
      selectedLanguage,
      submissionResult,
      code,
      isFavorited,
      formattedDescription,
      getDifficultyType,
      getDifficultyText,
      getResultType,
      getTestCaseClass,
      submitCode,
      toggleFavorite,
      shareProblem,
      clearResult,
      changeLanguage,
      onCodeChange,
      // 标签页相关
      activeTab,
      // AI聊天相关
      showAIChat,
      chatMessages,
      chatInput,
      aiLoading,
      chatMessagesRef,
      openAIChat,
      closeAIChat,
      sendMessage,
      formatTime,
      renderMarkdown,
      // 评论相关
      comments,
      commentCount,
      commentsLoading,
      submittingComment,
      newComment,
      loadComments,
      submitComment,
      deleteComment,
      formatCommentTime,
      // 题解相关
      solutions,
      solutionsLoading,
      submittingSolution,
      showSubmitSolution,
      solutionForm,
      loadSolutions,
      submitSolution,
      toggleLikeSolution,
      handleCloseSolutionDialog
    }
  }
}
</script>

<style scoped>
.problem-detail-page {
  min-height: 100vh;
  background: #f8f9fa;
  padding: 20px 0;
}

.container {
  max-width: 1200px;
  margin: 0 auto;
  padding: 0 20px;
}

.card-header {
  display: flex;
  justify-content: space-between;
  align-items: flex-start;
  gap: 20px;
}

.problem-title h2 {
  margin: 0 0 10px 0;
  color: #2c3e50;
  font-size: 1.5rem;
  font-weight: 600;
}

.problem-meta {
  display: flex;
  gap: 10px;
  align-items: center;
}

.problem-actions {
  display: flex;
  gap: 10px;
  flex-shrink: 0;
}

.problem-stats {
  display: flex;
  flex-wrap: wrap;
  gap: 20px;
  margin-bottom: 20px;
  padding: 15px;
  background: #f8f9fa;
  border-radius: 8px;
  border: 1px solid #e9ecef;
}

.stat-item {
  display: flex;
  align-items: center;
  gap: 5px;
}

.stat-label {
  color: #6c757d;
  font-size: 0.9rem;
}

.stat-value {
  color: #2c3e50;
  font-weight: 600;
  font-size: 0.9rem;
}

.problem-description {
  line-height: 1.8;
  color: #2c3e50;
  font-size: 1rem;
  background: #ffffff;
  padding: 20px;
  border-radius: 8px;
  border: 1px solid #e9ecef;
  max-height: 600px;
  overflow-y: auto;
}

.problem-description :deep(p) {
  color: #2c3e50;
  margin: 15px 0;
}

.problem-description :deep(h1),
.problem-description :deep(h2),
.problem-description :deep(h3),
.problem-description :deep(h4) {
  color: #2c3e50;
  margin: 20px 0 15px 0;
}

.problem-description :deep(h1) {
  font-size: 1.4rem;
  border-bottom: 2px solid #e9ecef;
  padding-bottom: 10px;
}

.problem-description :deep(h2) {
  font-size: 1.2rem;
}

.problem-description :deep(h3) {
  font-size: 1.1rem;
}

.problem-description :deep(h4) {
  font-size: 1rem;
}

.problem-description :deep(pre) {
  background: #f8f9fa;
  padding: 15px;
  border-radius: 6px;
  border: 1px solid #e9ecef;
  font-family: 'Monaco', 'Menlo', 'Ubuntu Mono', monospace;
  margin: 15px 0;
  overflow-x: auto;
  color: #2c3e50;
}

.problem-description :deep(code) {
  background: #f1f3f4;
  padding: 2px 6px;
  border-radius: 3px;
  font-family: 'Monaco', 'Menlo', 'Ubuntu Mono', monospace;
  font-size: 0.9rem;
  color: #2c3e50;
}

.problem-description :deep(blockquote) {
  border-left: 4px solid #007bff;
  padding-left: 15px;
  margin: 15px 0;
  color: #6c757d;
  font-style: italic;
}

.problem-description :deep(ul),
.problem-description :deep(ol) {
  padding-left: 20px;
  margin: 15px 0;
}

.problem-description :deep(li) {
  margin: 5px 0;
  color: #2c3e50;
}

.problem-description :deep(table) {
  width: 100%;
  border-collapse: collapse;
  margin: 15px 0;
}

.problem-description :deep(th),
.problem-description :deep(td) {
  border: 1px solid #dee2e6;
  padding: 8px 12px;
  text-align: left;
}

.problem-description :deep(th) {
  background-color: #f8f9fa;
  font-weight: bold;
}

.problem-tags {
  margin-top: 20px;
  display: flex;
  flex-wrap: wrap;
  gap: 8px;
}

.tag-item {
  margin: 0;
}

.code-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.code-header h3 {
  margin: 0;
  color: #2c3e50;
}

.editor-controls {
  display: flex;
  align-items: center;
  gap: 10px;
}

.code-editor-container {
  height: 500px;
  border: 1px solid #dcdfe6;
  border-radius: 6px;
  overflow: hidden;
  position: relative;
  margin-bottom: 20px;
}

.code-textarea {
  height: 100%;
}

.code-textarea :deep(.el-textarea__inner) {
  height: 100% !important;
  font-family: 'Monaco', 'Menlo', 'Ubuntu Mono', monospace;
  font-size: 14px;
  line-height: 1.5;
  resize: none;
  border: none;
  border-radius: 4px;
  background: #1e1e1e;
  color: #d4d4d4;
  padding: 15px;
}

.editor-loading p {
  margin-top: 15px;
  color: #6c757d;
  font-size: 0.9rem;
}

.submission-result {
  margin-top: 20px;
  padding: 20px;
  background: #f8f9fa;
  border-radius: 8px;
  border: 1px solid #e9ecef;
}

.result-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 15px;
}

.result-header h4 {
  margin: 0;
  color: #2c3e50;
}

.result-alert {
  margin-bottom: 15px;
}

.test-results {
  margin-top: 15px;
}

.test-results h5 {
  margin: 0 0 10px 0;
  color: #2c3e50;
  font-size: 1rem;
}

.test-case-list {
  display: flex;
  flex-direction: column;
  gap: 8px;
}

.test-case-item {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 8px 12px;
  border-radius: 4px;
  font-size: 0.9rem;
}

.test-case-pass {
  background: #d4edda;
  color: #155724;
  border: 1px solid #c3e6cb;
}

.test-case-fail {
  background: #f8d7da;
  color: #721c24;
  border: 1px solid #f5c6cb;
}

.test-case-timeout {
  background: #fff3cd;
  color: #856404;
  border: 1px solid #ffeaa7;
}

.test-case-unknown {
  background: #e2e3e5;
  color: #6c757d;
  border: 1px solid #d6d8db;
}

.test-case-number {
  font-weight: 500;
}

.execution-info {
  margin-top: 15px;
  display: flex;
  gap: 20px;
  flex-wrap: wrap;
}

.execution-item {
  display: flex;
  align-items: center;
  gap: 5px;
}

.execution-label {
  color: #6c757d;
  font-size: 0.9rem;
}

.execution-value {
  color: #2c3e50;
  font-weight: 600;
  font-size: 0.9rem;
}

.loading-container {
  padding: 20px;
}

/* 响应式设计 */
@media (max-width: 768px) {
  .container {
    padding: 0 10px;
  }
  
  .el-col {
    margin-bottom: 20px;
  }
  
  .card-header {
    flex-direction: column;
    align-items: stretch;
    gap: 15px;
  }
  
  .problem-actions {
    justify-content: center;
  }
  
  .problem-stats {
    flex-direction: column;
    gap: 10px;
  }
  
  .code-editor-container {
    height: 400px;
  }
  
  .execution-info {
    flex-direction: column;
    gap: 10px;
  }
}

/* 深色主题支持 */
@media (prefers-color-scheme: dark) {
  .problem-detail-page {
    background: #1a1a1a;
  }
  
  .problem-title h2 {
    color: #ffffff;
  }
  
  .problem-description {
    color: #e9ecef;
  }
  
  .problem-description h1,
  .problem-description h2,
  .problem-description h3,
  .problem-description h4 {
    color: #ffffff;
  }
  
  .problem-stats {
    background: #2d2d2d;
    border-color: #404040;
  }
  
  .stat-label {
    color: #adb5bd;
  }
  
  .stat-value {
    color: #ffffff;
  }
}

/* AI聊天窗口样式 */
.ai-chat-overlay {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(0, 0, 0, 0.5);
  z-index: 1000;
  display: flex;
  align-items: center;
  justify-content: center;
}

.ai-chat-window {
  width: 500px;
  height: 600px;
  background: #ffffff;
  border-radius: 12px;
  box-shadow: 0 8px 32px rgba(0, 0, 0, 0.2);
  display: flex;
  flex-direction: column;
  overflow: hidden;
}

.ai-chat-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 16px 20px;
  background: #f8f9fa;
  border-bottom: 1px solid #e9ecef;
}

.ai-chat-title {
  display: flex;
  align-items: center;
  gap: 8px;
  font-weight: 600;
  color: #2c3e50;
  font-size: 1.1rem;
}

.ai-chat-messages {
  flex: 1;
  padding: 20px;
  overflow-y: auto;
  display: flex;
  flex-direction: column;
  gap: 16px;
}

.chat-message {
  display: flex;
  gap: 12px;
  align-items: flex-start;
}

.chat-message.user {
  flex-direction: row-reverse;
}

.message-avatar {
  width: 36px;
  height: 36px;
  border-radius: 50%;
  display: flex;
  align-items: center;
  justify-content: center;
  flex-shrink: 0;
}

.chat-message.user .message-avatar {
  background: #007bff;
  color: white;
}

.chat-message.ai .message-avatar {
  background: #28a745;
  color: white;
}

.message-content {
  max-width: 70%;
  display: flex;
  flex-direction: column;
  gap: 4px;
}

.chat-message.user .message-content {
  align-items: flex-end;
}

.message-text {
  padding: 12px 16px;
  border-radius: 18px;
  font-size: 0.95rem;
  line-height: 1.4;
  word-wrap: break-word;
}

.chat-message.user .message-text {
  background: #007bff;
  color: white;
  border-bottom-right-radius: 4px;
}

.chat-message.ai .message-text {
  background: #f1f3f4;
  color: #2c3e50;
  border-bottom-left-radius: 4px;
}

/* Markdown样式 */
.message-text h1,
.message-text h2,
.message-text h3,
.message-text h4,
.message-text h5,
.message-text h6 {
  margin: 0.5em 0;
  font-weight: bold;
  line-height: 1.3;
}

.message-text h1 { font-size: 1.2em; }
.message-text h2 { font-size: 1.1em; }
.message-text h3 { font-size: 1.05em; }

.message-text p {
  margin: 0.5em 0;
  line-height: 1.5;
}

.message-text ul,
.message-text ol {
  margin: 0.5em 0;
  padding-left: 1.5em;
}

.message-text li {
  margin: 0.2em 0;
  line-height: 1.4;
}

.message-text code {
  background: rgba(0, 0, 0, 0.1);
  padding: 0.2em 0.4em;
  border-radius: 3px;
  font-family: 'Courier New', monospace;
  font-size: 0.9em;
}

.message-text pre {
  background: rgba(0, 0, 0, 0.05);
  padding: 1em;
  border-radius: 6px;
  overflow-x: auto;
  margin: 0.5em 0;
}

.message-text pre code {
  background: none;
  padding: 0;
}

.message-text blockquote {
  border-left: 3px solid #ddd;
  padding-left: 1em;
  margin: 0.5em 0;
  color: #666;
}

.message-text strong {
  font-weight: bold;
}

.message-text em {
  font-style: italic;
}

.message-text a {
  color: #007bff;
  text-decoration: none;
}

.message-text a:hover {
  text-decoration: underline;
}

.message-text hr {
  border: none;
  border-top: 1px solid #ddd;
  margin: 1em 0;
}

.message-time {
  font-size: 0.75rem;
  color: #6c757d;
  padding: 0 4px;
}

.loading-icon {
  animation: spin 1s linear infinite;
}

@keyframes spin {
  from { transform: rotate(0deg); }
  to { transform: rotate(360deg); }
}

.ai-chat-input {
  padding: 16px 20px;
  border-top: 1px solid #e9ecef;
  background: #f8f9fa;
}

.chat-input-field {
  width: 100%;
}

.chat-input-field :deep(.el-input__inner) {
  border-radius: 20px;
  border: 1px solid #dcdfe6;
}

.chat-input-field :deep(.el-input-group__append) {
  border-radius: 0 20px 20px 0;
  border-left: none;
}

/* 响应式设计 */
@media (max-width: 768px) {
  .ai-chat-window {
    width: 90vw;
    height: 80vh;
    margin: 20px;
  }
  
  .message-content {
    max-width: 85%;
  }
}

/* 标签页样式 */
.problem-tabs {
  margin-top: 20px;
}

.problem-tabs :deep(.el-tabs__header) {
  margin-bottom: 20px;
}

.problem-tabs :deep(.el-tabs__nav-wrap) {
  padding: 0 20px;
}

/* 评论区样式 */
.comments-section {
  padding: 20px;
}

.comments-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
  padding-bottom: 15px;
  border-bottom: 1px solid #e9ecef;
}

.comments-header h3 {
  margin: 0;
  color: #2c3e50;
  font-size: 1.2rem;
}

.comment-count {
  color: #6c757d;
  font-size: 0.9rem;
}

.comment-form {
  margin-bottom: 30px;
  padding: 20px;
  background: #f8f9fa;
  border-radius: 8px;
  border: 1px solid #e9ecef;
}

.comment-actions {
  margin-top: 15px;
  display: flex;
  justify-content: flex-end;
}

.comments-list {
  display: flex;
  flex-direction: column;
  gap: 20px;
}

.comment-item {
  padding: 20px;
  background: #ffffff;
  border-radius: 8px;
  border: 1px solid #e9ecef;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.05);
}

.comment-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 15px;
}

.comment-user {
  display: flex;
  align-items: center;
  gap: 12px;
}

.user-info {
  display: flex;
  flex-direction: column;
  gap: 2px;
}

.username {
  font-weight: 600;
  color: #2c3e50;
  font-size: 0.95rem;
}

.comment-time {
  color: #6c757d;
  font-size: 0.8rem;
}

.comment-content {
  color: #2c3e50;
  line-height: 1.6;
  font-size: 0.95rem;
}

.loading-comments {
  padding: 20px;
}

.no-comments {
  padding: 40px 20px;
  text-align: center;
}

/* 题解区样式 */
.solutions-section {
  padding: 20px;
}

.solutions-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 20px;
  padding-bottom: 15px;
  border-bottom: 1px solid #e9ecef;
}

.solutions-header h3 {
  margin: 0;
  color: #2c3e50;
  font-size: 1.2rem;
}

.solutions-list {
  display: flex;
  flex-direction: column;
  gap: 20px;
}

.solution-item {
  padding: 20px;
  background: #ffffff;
  border-radius: 8px;
  border: 1px solid #e9ecef;
  box-shadow: 0 2px 4px rgba(0, 0, 0, 0.05);
}

.solution-header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  margin-bottom: 15px;
}

.solution-user {
  display: flex;
  align-items: center;
  gap: 12px;
}

.solution-stats {
  display: flex !important;
  align-items: center !important;
  gap: 10px !important;
  min-width: 60px !important;
  justify-content: flex-end !important;
}

/* 爱心点赞按钮样式 */
.solution-item .like-button {
  border: 1px solid #dcdfe6 !important;
  background: #ffffff !important;
  color: #606266 !important;
  border-radius: 20px !important;
  padding: 6px 12px !important;
  font-size: 12px !important;
  transition: all 0.3s ease !important;
  display: flex !important;
  align-items: center !important;
  gap: 4px !important;
  min-width: auto !important;
  height: auto !important;
  visibility: visible !important;
  opacity: 1 !important;
  position: relative !important;
  z-index: 1 !important;
  cursor: pointer !important;
  outline: none !important;
  font-family: inherit !important;
}

.solution-item .like-button:hover {
  border-color: #f56c6c !important;
  color: #f56c6c !important;
  background: #fef0f0 !important;
}

.solution-item .like-button.liked {
  border-color: #f56c6c !important;
  background: #f56c6c !important;
  color: #ffffff !important;
}

.solution-item .like-button.liked:hover {
  background: #f78989 !important;
  border-color: #f78989 !important;
}

.solution-item .like-button .heart-icon {
  width: 16px !important;
  height: 16px !important;
  transition: transform 0.2s ease !important;
}

.solution-item .like-button:hover .heart-icon {
  transform: scale(1.1) !important;
}

.solution-item .like-button.liked .heart-icon {
  animation: heartbeat 0.6s ease-in-out !important;
}

@keyframes heartbeat {
  0% { transform: scale(1); }
  25% { transform: scale(1.2); }
  50% { transform: scale(1); }
  75% { transform: scale(1.1); }
  100% { transform: scale(1); }
}

.solution-content {
  color: #2c3e50;
  line-height: 1.6;
  font-size: 0.95rem;
}

.solution-content :deep(h1),
.solution-content :deep(h2),
.solution-content :deep(h3),
.solution-content :deep(h4) {
  color: #2c3e50;
  margin: 15px 0 10px 0;
}

.solution-content :deep(h1) { font-size: 1.3rem; }
.solution-content :deep(h2) { font-size: 1.2rem; }
.solution-content :deep(h3) { font-size: 1.1rem; }
.solution-content :deep(h4) { font-size: 1rem; }

.solution-content :deep(p) {
  margin: 10px 0;
  line-height: 1.6;
}

.solution-content :deep(pre) {
  background: #f8f9fa;
  padding: 15px;
  border-radius: 6px;
  border: 1px solid #e9ecef;
  font-family: 'Monaco', 'Menlo', 'Ubuntu Mono', monospace;
  margin: 15px 0;
  overflow-x: auto;
}

.solution-content :deep(code) {
  background: #f1f3f4;
  padding: 2px 6px;
  border-radius: 3px;
  font-family: 'Monaco', 'Menlo', 'Ubuntu Mono', monospace;
  font-size: 0.9rem;
}

.solution-content :deep(ul),
.solution-content :deep(ol) {
  padding-left: 20px;
  margin: 10px 0;
}

.solution-content :deep(li) {
  margin: 5px 0;
}

.loading-solutions {
  padding: 20px;
}

.no-solutions {
  padding: 40px 20px;
  text-align: center;
}

/* 对话框样式 */
.dialog-footer {
  display: flex;
  justify-content: flex-end;
  gap: 10px;
}

/* 响应式设计 */
@media (max-width: 768px) {
  .comments-header,
  .solutions-header {
    flex-direction: column;
    align-items: stretch;
    gap: 15px;
  }
  
  .comment-header,
  .solution-header {
    flex-direction: column;
    align-items: stretch;
    gap: 10px;
  }
  
  .comment-user,
  .solution-user {
    justify-content: center;
  }
  
  .solution-stats {
    justify-content: center;
  }
  
  .comment-form {
    padding: 15px;
  }
  
  .comment-item,
  .solution-item {
    padding: 15px;
  }
}
</style>
