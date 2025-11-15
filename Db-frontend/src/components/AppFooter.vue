<template>
  <footer class="app-footer">
    <div class="container">
      <div class="footer-content">
        <div class="footer-info">
          <h3>Db Platform</h3>
          <p>在线编程练习平台</p>
          <p>支持C++代码在线编译和运行</p>
        </div>
        
        <div class="footer-links">
          <h4>快速链接</h4>
          <ul>
            <li><router-link to="/">首页</router-link></li>
            <li><router-link to="/problems">题目列表</router-link></li>
            <li><router-link to="/submissions">提交记录</router-link></li>
          </ul>
        </div>
        
        <div class="footer-contact">
          <h4>联系我们</h4>
          <p>邮箱: jiex2573@gmail.com</p>
          <p>电话: 157-1358-8491</p>
        </div>
      </div>
      
      <div class="footer-bottom">
        <p>&copy; 2025 Db Platform. All rights reserved.</p>
      </div>
    </div>
  </footer>
</template>

<script>
export default {
  name: 'AppFooter'
}
</script>

<style scoped>
.app-footer {
  background: #2c3e50;
  color: #ecf0f1;
  margin-top: auto;
}

.footer-content {
  display: grid;
  grid-template-columns: 1fr 1fr 1fr;
  gap: 40px;
  padding: 40px 0;
}

.footer-info h3 {
  color: #3498db;
  margin-bottom: 15px;
  font-size: 24px;
}

.footer-info p {
  margin-bottom: 8px;
  color: #bdc3c7;
  line-height: 1.6;
}

.footer-links h4,
.footer-contact h4 {
  color: #3498db;
  margin-bottom: 15px;
  font-size: 18px;
}

.footer-links ul {
  list-style: none;
}

.footer-links li {
  margin-bottom: 8px;
}

.footer-links a {
  color: #bdc3c7;
  text-decoration: none;
  transition: color 0.3s;
}

.footer-links a:hover {
  color: #3498db;
}

.footer-contact p {
  margin-bottom: 8px;
  color: #bdc3c7;
  line-height: 1.6;
}

.footer-bottom {
  border-top: 1px solid #34495e;
  padding: 20px 0;
  text-align: center;
}

.footer-bottom p {
  color: #95a5a6;
  margin: 0;
}

/* 响应式设计 */
@media (max-width: 768px) {
  .footer-content {
    grid-template-columns: 1fr;
    gap: 30px;
    text-align: center;
  }
}
</style>
