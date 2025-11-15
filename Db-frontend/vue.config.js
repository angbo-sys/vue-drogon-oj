const { defineConfig } = require('@vue/cli-service')

module.exports = defineConfig({
  transpileDependencies: true,

  devServer: {
    port: 8080,
    host: '0.0.0.0',
    allowedHosts: 'all',
    compress: true,
    hot: true,
    liveReload: true,
    client: {
      webSocketURL: 'auto://0.0.0.0:0/ws'
    },
    proxy: {
      '/api': {
        target: 'http://localhost:5555',
        changeOrigin: true,
        logLevel: 'debug'
      }
    }
  },

  configureWebpack: {
    resolve: {
      fallback: {
        "path": false,
        "fs": false
      }
    },
    optimization: {
      splitChunks: {
        chunks: 'all',
        cacheGroups: {
          vendor: {
            test: /[\\/]node_modules[\\/]/,
            name: 'vendors',
            chunks: 'all',
          }
        }
      }
    }
  }
})
