const { defineConfig } = require('@vue/cli-service')

module.exports = defineConfig({
  transpileDependencies: true,
  devServer: {
    port: 3000,
    proxy: {
      '/api': {
        target: 'http://localhost:5555',
        changeOrigin: true,
        secure: false
      }
    }
  },
  configureWebpack: {
    resolve: {
      fallback: {
        "path": false,
        "fs": false
      }
    }
  }
})