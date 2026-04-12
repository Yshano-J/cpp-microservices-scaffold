# C++ Microservice Scaffold

A C++-based microservice development scaffold that provides integrated封装 of commonly used third-party components.

## Project Overview

This project aims to provide a comprehensive set of foundational components for C++ microservice development, encapsulating middleware and utility libraries frequently used in microservice development, allowing developers to focus more on business logic implementation.

## Functional Modules

### Core Components

| Module | Description |
|--------|-------------|
| AriesBrpc | Integration of Baidu RPC framework, supporting synchronous/asynchronous RPC calls |
| AriesAMQP | Integration of RabbitMQ message queue, supporting DLX dead-letter queues |
| AriesRedis | Encapsulated Redis client with connection pool support |
| AriesElastic | Integrated Elasticsearch client |
| AriesEtcd | Integration of service registration and discovery |
| AriesFastdfs | FastDFS distributed file storage client |
| AriesFFmpeg | Video processing encapsulation, supporting HLS streaming conversion |
| AriesODB | Integrated database ORM framework |
| AriesMail | Email sending client |
| AriesLog | Log encapsulation based on spdlog |
| AriesUtil | Utility library including JSON processing |

### Example Programs

The project provides rich example code under the `example/` directory:

- **brpc** - RPC server/client examples
- **amqpcpp** - Message producer/consumer examples
- **redis** - Examples for various data structure operations
- **elasticsearch** - Index and document operation examples
- **fastdfs** - File upload/download examples
- **ffmpeg** - Video transcoding example
- **gtest** - Unit test examples
- **websocketpp** - WebSocket example

## Dependencies

The project depends on the following open-source libraries:

- brpc - Baidu RPC framework
- protobuf - Protocol Buffers
- rabbitmq-c - AMQP client
- Boost.Asio - Asynchronous I/O
- hiredis/sworedis - Redis client
- elasticlient - Elasticsearch C++ client
- libfastdfs - FastDFS client
- ffmpeg - Video processing
- jsoncpp - JSON processing
- libodb - Database ORM
- spdlog - High-performance logging library
- websocketpp - WebSocket library

## Build Requirements

- C++17 or higher
- CMake 3.10+
- Linux environment

## Quick Start

```bash
# Create build directory
mkdir build && cd build

# Configure project
cmake ..

# Compile
make

# Run example programs
./example/brpc/server &
./example/brpc/client
```

## Directory Structure

```
├── source/          # Core library source code
├── example/         # Example code
├── test/            # Unit tests
├── cmake/           # CMake configuration files
└── README.md        # Project documentation
```

## License

This project is open-sourced under the MIT License. See the LICENSE file for details.