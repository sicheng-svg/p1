# Minesweeper - 在线扫雷游戏

## 版本记录

| 版本 | 日期 | 说明 |
|------|------|------|
| v1.0 | 2026-02-02 | 初始版本，9x9 棋盘，排行榜功能 |
| v1.1 | 2026-02-02 | 新增难度选择（初级 9×9/10雷、中级 16×16/40雷、高级 30×16/99雷），排行榜按难度独立显示 |

## 项目概述

基于 C++ 后端 + 原生前端的在线扫雷游戏，支持难度选择和排行榜功能。

## 技术栈

- **后端**: C++17
- **HTTP 服务器**: cpp-httplib (header-only，位于 `include/httplib.h`)
- **数据库**: MySQL
- **前端**: 原生 HTML + CSS + JavaScript
- **构建工具**: CMake 3.10+
- **依赖库**: mysqlclient, pthread

## 项目结构

```
minesweeper/
├── CMakeLists.txt          # CMake 构建配置
├── include/
│   └── httplib.h           # cpp-httplib 头文件
├── src/
│   ├── main.cpp            # HTTP 服务器、API 路由
│   ├── database.h          # Database 类定义
│   └── database.cpp        # MySQL 数据库操作实现
├── static/
│   ├── index.html          # 游戏页面（中文界面）
│   ├── script.js           # 游戏逻辑（支持多难度）
│   └── style.css           # 样式
└── build/                  # CMake 构建目录
```

## 数据库配置

- **数据库**: MySQL
- **数据库名**: `minesweeper`
- **用户名**: `xsc`
- **密码**: `Xsc.200411`
- **主机**: localhost
- **端口**: 3306

### 数据库初始化

```sql
CREATE DATABASE minesweeper;
CREATE USER 'xsc'@'localhost' IDENTIFIED BY 'Xsc.200411';
GRANT ALL PRIVILEGES ON minesweeper.* TO 'xsc'@'localhost';
FLUSH PRIVILEGES;
```

`leaderboard` 表由程序启动时自动创建：

```sql
CREATE TABLE IF NOT EXISTS leaderboard (
    id INT AUTO_INCREMENT PRIMARY KEY,
    player_name VARCHAR(50) NOT NULL,
    time_seconds INT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

## API 接口

| 方法 | 路径 | 说明 |
|------|------|------|
| GET  | `/api/leaderboard?difficulty=easy` | 获取指定难度排行榜前 20 名（easy/medium/hard） |
| POST | `/api/leaderboard` | 提交成绩 `{"player_name":"xxx","time_seconds":123,"difficulty":"easy"}` |
| GET  | `/` | 静态文件（游戏页面） |

## 编译与运行

```bash
cd minesweeper
./build/minesweeper
```

**注意**: 必须从项目根目录 `minesweeper/` 启动，不能在 `build/` 目录下运行，否则找不到 `static/` 静态文件目录。

编译命令：

```bash
cd minesweeper/build
cmake .. && make
```

服务启动后访问 `http://localhost:8080`。

## 已知问题与修复记录

### 1. 静态文件无法加载（页面空白）

- **原因**: `main.cpp` 中 `set_mount_point("/", "./static")` 使用相对路径，如果在 `build/` 目录下运行程序，会找不到 `static/` 目录。
- **解决**: 从项目根目录 `minesweeper/` 运行 `./build/minesweeper`，而非在 `build/` 下运行 `./minesweeper`。

### 2. 排行榜不更新，玩家名不显示，用时显示 `undefineds`

- **原因**: 前端 `script.js` 中 JSON 字段名与后端不一致。前端提交时使用 `name`/`time`，读取时使用 `entry.name`/`entry.time`，但后端期望和返回的是 `player_name`/`time_seconds`。
- **解决**: 将前端字段统一为 `player_name` 和 `time_seconds`。

## 游戏规则

- 三种难度：初级 (9×9, 10雷)、中级 (16×16, 40雷)、高级 (30×16, 99雷)
- 左键揭开格子，右键标记旗帜
- 揭开所有非雷格子即获胜，计时记录可提交至排行榜（按难度独立排名）
