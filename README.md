

# C++ 微服务脚手架

一个基于 C++ 的微服务开发脚手架，提供常用第三方组件的集成封装。

## 项目简介

本项目致力于为 C++ 微服务开发提供一套完整的基础组件库，封装了微服务开发中常用的中间件和工具库，让开发者可以更专注于业务逻辑的实现。

## 功能模块

### 核心组件

| 模块 | 说明 |
|------|------|
| AriesBrpc | Baidu RPC 框架集成，支持同步/异步 RPC 调用 |
| AriesAMQP | RabbitMQ 消息队列集成，支持 DLX 死信队列 |
| AriesRedis | Redis 客户端封装，支持连接池 |
| AriesElastic | Elasticsearch 客户端集成 |
| AriesEtcd | 服务注册与发现集成 |
| AriesFastdfs | FastDFS 分布式文件存储客户端 |
| AriesFFmpeg | 视频处理封装，支持 HLS 流媒体转换 |
| AriesODB | 数据库 ORM 框架集成 |
| AriesMail | 邮件发送客户端 |
| AriesLog | 基于 spdlog 的日志封装 |
| AriesUtil | 常用工具集，包括 JSON 处理 |

### 示例程序

项目在 `example/` 目录下提供了丰富的示例代码：

- **brpc** - RPC 服务端/客户端示例
- **amqpcpp** - 消息生产/消费示例
- **redis** - 各类数据结构操作示例
- **elasticsearch** - 索引和文档操作示例
- **fastdfs** - 文件上传/下载示例
- **ffmpeg** - 视频转码示例
- **gtest** - 单元测试示例
- **websocketpp** - WebSocket 示例

## 依赖库

项目依赖于以下开源库：

- brpc - 百度 RPC 框架
- protobuf - Protocol Buffers
- rabbitmq-c - AMQP 客户端
- Boost.Asio - 异步 IO
- hiredis/sworedis - Redis 客户端
- elasticlient - Elasticsearch C++ 客户端
- libfastdfs - FastDFS 客户端
- ffmpeg - 视频处理
- jsoncpp - JSON 处理
- libodb - 数据库 ORM
- spdlog - 高性能日志库
- websocketpp - WebSocket 库

## 编译要求

- C++17 或更高版本
- CMake 3.10+
- Linux 环境

## 快速开始

```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake ..

# 编译
make

# 运行示例程序
./example/brpc/server &
./example/brpc/client
```

## 目录结构

```
├── source/          # 核心库源码
├── example/         # 示例代码
├── test/            # 单元测试
├── cmake/           # CMake 配置文件
└── README.md        # 项目说明
```

## License

本项目基于 MIT 许可证开源，详见 LICENSE 文件。