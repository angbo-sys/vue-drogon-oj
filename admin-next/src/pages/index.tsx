import { useEffect } from 'react'
import { useRouter } from 'next/router'
import { Layout, Card, Row, Col, Statistic, Table, Button, Tag } from 'antd'
import { 
  UserOutlined, 
  FileTextOutlined, 
  TrophyOutlined, 
  BarChartOutlined 
} from '@ant-design/icons'
import { useAdminStore } from '../stores/adminStore'

const { Header, Content } = Layout

export default function Dashboard() {
  const router = useRouter()
  const { isLoggedIn, statistics, fetchStatistics } = useAdminStore()

  useEffect(() => {
    if (!isLoggedIn) {
      router.push('/login')
      return
    }
    fetchStatistics()
  }, [isLoggedIn, router, fetchStatistics])

  const columns = [
    {
      title: '题号',
      dataIndex: 'id',
      key: 'id',
    },
    {
      title: '题目名称',
      dataIndex: 'name',
      key: 'name',
    },
    {
      title: '难度',
      dataIndex: 'difficulty',
      key: 'difficulty',
      render: (difficulty: number) => {
        const difficultyMap = { 0: '入门', 1: '简单', 2: '中等', 3: '困难' }
        const colorMap = { 0: 'blue', 1: 'green', 2: 'orange', 3: 'red' }
        return <Tag color={colorMap[difficulty as keyof typeof colorMap]}>{difficultyMap[difficulty as keyof typeof difficultyMap]}</Tag>
      }
    },
    {
      title: '类型',
      dataIndex: 'type',
      key: 'type',
      render: (type: number) => {
        const typeMap = { 0: '无类型', 1: '动态规划', 2: '前缀和', 3: '贪心' }
        return typeMap[type as keyof typeof typeMap]
      }
    },
    {
      title: '操作',
      key: 'action',
      render: () => (
        <Button type="link" size="small">
          编辑
        </Button>
      ),
    },
  ]

  const recentProblems = [
    { id: 1, name: 'A+B Problem', difficulty: 0, type: 0 },
    { id: 2, name: '两数之和', difficulty: 1, type: 1 },
  ]

  if (!isLoggedIn) {
    return null
  }

  return (
    <Layout style={{ minHeight: '100vh' }}>
      <Header style={{ background: '#fff', padding: '0 24px', boxShadow: '0 2px 8px rgba(0,0,0,0.1)' }}>
        <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
          <h1 style={{ margin: 0, color: '#1890ff' }}>在线刷题平台 - 管理后台</h1>
          <Button type="primary" onClick={() => router.push('/problems')}>
            题目管理
          </Button>
        </div>
      </Header>
      
      <Content style={{ padding: '24px' }}>
        <Row gutter={[16, 16]} style={{ marginBottom: '24px' }}>
          <Col span={6}>
            <Card>
              <Statistic
                title="总用户数"
                value={statistics.total_users || 0}
                prefix={<UserOutlined />}
                valueStyle={{ color: '#3f8600' }}
              />
            </Card>
          </Col>
          <Col span={6}>
            <Card>
              <Statistic
                title="题目总数"
                value={statistics.total_problems || 0}
                prefix={<FileTextOutlined />}
                valueStyle={{ color: '#1890ff' }}
              />
            </Card>
          </Col>
          <Col span={6}>
            <Card>
              <Statistic
                title="总提交数"
                value={statistics.total_submissions || 0}
                prefix={<TrophyOutlined />}
                valueStyle={{ color: '#cf1322' }}
              />
            </Card>
          </Col>
          <Col span={6}>
            <Card>
              <Statistic
                title="通过率"
                value={statistics.acceptance_rate || 0}
                suffix="%"
                prefix={<BarChartOutlined />}
                valueStyle={{ color: '#722ed1' }}
                precision={1}
              />
            </Card>
          </Col>
        </Row>

        <Row gutter={[16, 16]}>
          <Col span={24}>
            <Card title="最新题目" extra={<Button type="link">查看全部</Button>}>
              <Table 
                columns={columns} 
                dataSource={recentProblems} 
                pagination={false}
                rowKey="id"
              />
            </Card>
          </Col>
        </Row>
      </Content>
    </Layout>
  )
}
