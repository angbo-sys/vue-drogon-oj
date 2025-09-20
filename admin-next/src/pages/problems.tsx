import { useState, useEffect } from 'react'
import { useRouter } from 'next/router'
import { 
  Layout, 
  Table, 
  Button, 
  Tag, 
  Modal, 
  Form, 
  Input, 
  Select, 
  message,
  Popconfirm,
  Space
} from 'antd'
import { PlusOutlined, EditOutlined, DeleteOutlined } from '@ant-design/icons'
import { useAdminStore } from '../stores/adminStore'

const { Header, Content } = Layout
const { Option } = Select

export default function Problems() {
  const router = useRouter()
  const { isLoggedIn, problems, fetchProblems, addProblem, updateProblem, deleteProblem } = useAdminStore()
  
  const [loading, setLoading] = useState(false)
  const [modalVisible, setModalVisible] = useState(false)
  const [editingProblem, setEditingProblem] = useState(null)
  const [form] = Form.useForm()

  useEffect(() => {
    if (!isLoggedIn) {
      router.push('/login')
      return
    }
    loadProblems()
  }, [isLoggedIn, router])

  const loadProblems = async () => {
    setLoading(true)
    try {
      await fetchProblems()
    } catch (error) {
      message.error('加载题目失败')
    } finally {
      setLoading(false)
    }
  }

  const handleAdd = () => {
    setEditingProblem(null)
    form.resetFields()
    setModalVisible(true)
  }

  const handleEdit = (record: any) => {
    setEditingProblem(record)
    form.setFieldsValue(record)
    setModalVisible(true)
  }

  const handleDelete = async (id: number) => {
    try {
      await deleteProblem(id)
      message.success('删除成功')
      loadProblems()
    } catch (error) {
      message.error('删除失败')
    }
  }

  const handleSubmit = async (values: any) => {
    try {
      if (editingProblem) {
        await updateProblem(editingProblem.id, values)
        message.success('更新成功')
      } else {
        await addProblem(values)
        message.success('添加成功')
      }
      setModalVisible(false)
      loadProblems()
    } catch (error) {
      message.error(editingProblem ? '更新失败' : '添加失败')
    }
  }

  const columns = [
    {
      title: '题号',
      dataIndex: 'id',
      key: 'id',
      width: 80,
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
      width: 100,
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
      width: 120,
      render: (type: number) => {
        const typeMap = { 0: '无类型', 1: '动态规划', 2: '前缀和', 3: '贪心' }
        return typeMap[type as keyof typeof typeMap]
      }
    },
    {
      title: '来源',
      dataIndex: 'source',
      key: 'source',
    },
    {
      title: '操作',
      key: 'action',
      width: 150,
      render: (record: any) => (
        <Space>
          <Button 
            type="link" 
            icon={<EditOutlined />} 
            onClick={() => handleEdit(record)}
          >
            编辑
          </Button>
          <Popconfirm
            title="确定要删除这道题目吗？"
            onConfirm={() => handleDelete(record.id)}
            okText="确定"
            cancelText="取消"
          >
            <Button type="link" danger icon={<DeleteOutlined />}>
              删除
            </Button>
          </Popconfirm>
        </Space>
      ),
    },
  ]

  if (!isLoggedIn) {
    return null
  }

  return (
    <Layout style={{ minHeight: '100vh' }}>
      <Header style={{ background: '#fff', padding: '0 24px', boxShadow: '0 2px 8px rgba(0,0,0,0.1)' }}>
        <div style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}>
          <h1 style={{ margin: 0, color: '#1890ff' }}>题目管理</h1>
          <Button type="primary" icon={<PlusOutlined />} onClick={handleAdd}>
            添加题目
          </Button>
        </div>
      </Header>
      
      <Content style={{ padding: '24px' }}>
        <Table
          columns={columns}
          dataSource={problems}
          loading={loading}
          rowKey="id"
          pagination={{
            pageSize: 10,
            showSizeChanger: true,
            showQuickJumper: true,
            showTotal: (total) => `共 ${total} 条记录`,
          }}
        />
      </Content>

      <Modal
        title={editingProblem ? '编辑题目' : '添加题目'}
        open={modalVisible}
        onCancel={() => setModalVisible(false)}
        footer={null}
        width={600}
      >
        <Form
          form={form}
          layout="vertical"
          onFinish={handleSubmit}
        >
          <Form.Item
            name="name"
            label="题目名称"
            rules={[{ required: true, message: '请输入题目名称!' }]}
          >
            <Input placeholder="请输入题目名称" />
          </Form.Item>

          <Form.Item
            name="difficulty"
            label="难度"
            rules={[{ required: true, message: '请选择难度!' }]}
          >
            <Select placeholder="请选择难度">
              <Option value={0}>入门</Option>
              <Option value={1}>简单</Option>
              <Option value={2}>中等</Option>
              <Option value={3}>困难</Option>
            </Select>
          </Form.Item>

          <Form.Item
            name="type"
            label="类型"
            rules={[{ required: true, message: '请选择类型!' }]}
          >
            <Select placeholder="请选择类型">
              <Option value={0}>无类型</Option>
              <Option value={1}>动态规划</Option>
              <Option value={2}>前缀和</Option>
              <Option value={3}>贪心</Option>
            </Select>
          </Form.Item>

          <Form.Item
            name="source"
            label="来源"
            rules={[{ required: true, message: '请输入来源!' }]}
          >
            <Input placeholder="请输入来源" />
          </Form.Item>

          <Form.Item
            name="description"
            label="题目描述"
          >
            <Input.TextArea rows={4} placeholder="请输入题目描述" />
          </Form.Item>

          <Form.Item>
            <Space>
              <Button type="primary" htmlType="submit">
                {editingProblem ? '更新' : '添加'}
              </Button>
              <Button onClick={() => setModalVisible(false)}>
                取消
              </Button>
            </Space>
          </Form.Item>
        </Form>
      </Modal>
    </Layout>
  )
}
