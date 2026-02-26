# MongoDB 从 0 到精通学习手册

> **定位说明**：本手册默认你已有 MySQL 和 Redis 基础，因此会大量用"对比"视角来讲解 MongoDB，帮你快速建立认知。重点放在文档模型设计、索引优化、聚合管道、事务机制和高可用架构。

---

## 目录

1. [MongoDB 简介与核心概念](#1-mongodb-简介与核心概念)
2. [安装与基础配置](#2-安装与基础配置)
3. [CRUD 操作详解](#3-crud-操作详解)
4. [文档模型设计（核心重点）](#4-文档模型设计核心重点)
5. [索引原理与实战](#5-索引原理与实战)
6. [聚合管道（Aggregation Pipeline）](#6-聚合管道aggregation-pipeline)
7. [事务机制](#7-事务机制)
8. [高可用架构](#8-高可用架构)
9. [性能优化实战](#9-性能优化实战)
10. [MongoDB vs MySQL vs Redis 选型指南](#10-mongodb-vs-mysql-vs-redis-选型指南)
11. [MongoDB 面试题精选](#11-mongodb-面试题精选)

---

## 1. MongoDB 简介与核心概念

### 1.1 什么是 MongoDB？

MongoDB 是一个**文档型 NoSQL 数据库**，数据以 **BSON（Binary JSON）** 格式存储，每条记录是一个文档（Document），文档组成集合（Collection），集合存在于数据库（Database）中。

```
MySQL 概念          MongoDB 概念
─────────────────────────────────
Database         →  Database（数据库）
Table            →  Collection（集合）
Row              →  Document（文档）
Column           →  Field（字段）
Primary Key      →  _id（文档唯一标识）
JOIN             →  $lookup / 嵌入文档
Schema           →  Schema-less（无固定结构）
```

### 1.2 一个文档长什么样

```javascript
// MySQL 中的一行 user 记录：
// id=1, name="Alice", age=25, city="Beijing"

// MongoDB 中的一个 user 文档：
{
  "_id": ObjectId("64f1a2b3c4d5e6f7a8b9c0d1"),  // 自动生成的唯一ID
  "name": "Alice",
  "age": 25,
  "address": {                    // 嵌套对象（MySQL需要单独一张表）
    "city": "Beijing",
    "district": "Chaoyang",
    "zip": "100020"
  },
  "tags": ["developer", "gamer"], // 数组（MySQL需要关联表）
  "orders": [                     // 嵌入子文档数组
    { "orderId": "O001", "amount": 299, "status": "paid" },
    { "orderId": "O002", "amount": 599, "status": "pending" }
  ],
  "createdAt": ISODate("2024-01-15T08:30:00Z")
}
```

> **关键区别**：MySQL 把数据拆分到多张表，MongoDB 把相关数据聚合到一个文档里。这带来了读取快（无需 JOIN）但更新复杂的权衡。

### 1.3 MongoDB 的优势与适用场景

**优势**：
- **灵活的 Schema**：文档结构可以随时变化，无需 ALTER TABLE
- **天然的层级结构**：嵌套对象和数组，完美映射业务对象
- **水平扩展（Sharding）**：原生支持，MySQL 的分库分表需要中间件
- **高写入吞吐**：适合日志、埋点等高频写入场景
- **地理空间查询**：原生支持 2dsphere 索引

**适用场景**：
- 内容管理系统（文章、评论，结构多变）
- 用户行为日志（高频写入，宽表）
- 游戏用户数据（背包、角色属性，嵌套复杂）
- 产品目录（不同品类属性完全不同）
- 实时分析（配合聚合管道）

**不适合场景**：
- 复杂多表 JOIN 查询（用 MySQL）
- 强事务一致性（转账等金融场景用 MySQL）
- 简单高频的键值读写（用 Redis）

---

## 2. 安装与基础配置

### 2.1 安装

```bash
# Ubuntu 22.04 安装 MongoDB 7.0
wget -qO - https://www.mongodb.org/static/pgp/server-7.0.asc | sudo apt-key add -
echo "deb [ arch=amd64,arm64 ] https://repo.mongodb.org/apt/ubuntu jammy/mongodb-org/7.0 multiverse" | \
  sudo tee /etc/apt/sources.list.d/mongodb-org-7.0.list
sudo apt update
sudo apt install -y mongodb-org

# 启动
sudo systemctl start mongod
sudo systemctl enable mongod

# 查看状态
sudo systemctl status mongod
```

### 2.2 连接与基础操作

```bash
# 连接 MongoDB（新版本用 mongosh）
mongosh

# 指定地址和端口
mongosh "mongodb://127.0.0.1:27017"

# 带用户名密码
mongosh "mongodb://admin:password@127.0.0.1:27017/admin"
```

```javascript
// 基础数据库操作
show dbs                    // 显示所有数据库
use myapp                   // 切换/创建数据库（有数据才真正创建）
db                          // 显示当前数据库
show collections            // 显示当前库的所有集合
db.dropDatabase()           // 删除当前数据库（危险！）
```

### 2.3 核心配置 `/etc/mongod.conf`

```yaml
# 网络配置
net:
  port: 27017
  bindIp: 127.0.0.1         # 生产环境不要用 0.0.0.0

# 存储配置
storage:
  dbPath: /var/lib/mongodb
  journal:
    enabled: true           # 开启 journal（崩溃恢复）
  wiredTiger:
    engineConfig:
      cacheSizeGB: 2        # WiredTiger 缓存（建议物理内存的50%）

# 日志配置
systemLog:
  destination: file
  logAppend: true
  path: /var/log/mongodb/mongod.log

# 操作日志（主从复制需要）
replication:
  replSetName: "rs0"        # 副本集名称

# 安全配置
security:
  authorization: enabled    # 开启认证
```

### 2.4 创建用户

```javascript
// 切换到 admin 库创建管理员
use admin
db.createUser({
  user: "admin",
  pwd: "StrongPassword123!",
  roles: [{ role: "userAdminAnyDatabase", db: "admin" }]
})

// 为具体业务库创建用户
use myapp
db.createUser({
  user: "appuser",
  pwd: "AppPassword456!",
  roles: [
    { role: "readWrite", db: "myapp" }
  ]
})

// 查看用户
db.getUsers()
```

---

## 3. CRUD 操作详解

> MongoDB 使用 JavaScript 风格的 API，所有操作都是方法调用。

### 3.1 Insert（插入）

```javascript
// 插入单条文档（自动生成 _id）
db.users.insertOne({
  name: "Alice",
  age: 25,
  email: "alice@example.com",
  tags: ["developer", "gamer"],
  createdAt: new Date()
})
// 返回：{ acknowledged: true, insertedId: ObjectId("...") }

// 插入多条文档
db.users.insertMany([
  { name: "Bob",     age: 30, email: "bob@example.com" },
  { name: "Charlie", age: 22, email: "charlie@example.com" },
  { name: "Diana",   age: 28, email: "diana@example.com" }
])
// 返回：{ acknowledged: true, insertedIds: { 0: ObjectId(...), 1: ..., 2: ... } }

// 指定 _id（自定义主键）
db.users.insertOne({
  _id: "user_1001",           // 字符串作为_id
  name: "Eve",
  age: 26
})

// 插入嵌套文档
db.orders.insertOne({
  userId: ObjectId("64f1a2b3c4d5e6f7a8b9c0d1"),
  items: [
    { productId: "P001", name: "iPhone", qty: 1, price: 7999 },
    { productId: "P002", name: "AirPods", qty: 2, price: 899 }
  ],
  totalAmount: 9797,
  status: "pending",
  address: {
    city: "Shanghai",
    street: "Nanjing Road 100"
  },
  createdAt: new Date()
})
```

### 3.2 Find（查询）

```javascript
// ==================== 基础查询 ====================

// 查询所有文档
db.users.find()

// 条件查询（等值）
db.users.find({ name: "Alice" })

// 指定返回字段（1=返回，0=排除），类似 SQL 的 SELECT 字段
db.users.find(
  { age: 25 },
  { name: 1, email: 1, _id: 0 }   // 返回name和email，排除_id
)

// 查询单条
db.users.findOne({ name: "Alice" })

// ==================== 比较操作符 ====================
// $eq  $ne  $gt  $gte  $lt  $lte  $in  $nin

db.users.find({ age: { $gt: 20 } })            // age > 20
db.users.find({ age: { $gte: 20, $lte: 30 } }) // 20 <= age <= 30
db.users.find({ age: { $ne: 25 } })            // age != 25
db.users.find({ status: { $in: ["active", "vip"] } })   // IN
db.users.find({ status: { $nin: ["banned", "deleted"] } }) // NOT IN

// ==================== 逻辑操作符 ====================
// $and  $or  $not  $nor

// AND（默认多条件就是AND）
db.users.find({ age: { $gte: 20 }, status: "active" })
// 等价于
db.users.find({ $and: [{ age: { $gte: 20 } }, { status: "active" }] })

// OR
db.users.find({
  $or: [{ age: { $lt: 18 } }, { age: { $gt: 60 } }]
})

// NOT
db.users.find({ age: { $not: { $gt: 30 } } })

// ==================== 元素操作符 ====================

// 字段存在性判断
db.users.find({ phone: { $exists: true } })   // 有 phone 字段
db.users.find({ phone: { $exists: false } })  // 没有 phone 字段

// 类型判断
db.users.find({ age: { $type: "int" } })
db.users.find({ age: { $type: ["int", "long"] } })

// ==================== 数组查询 ====================

// 查询数组中包含某个值
db.users.find({ tags: "developer" })           // tags数组包含"developer"

// 数组包含所有指定值（顺序无关）
db.users.find({ tags: { $all: ["developer", "gamer"] } })

// 数组元素满足条件
db.users.find({ scores: { $elemMatch: { $gte: 90 } } })

// 数组长度
db.users.find({ tags: { $size: 2 } })          // tags数组长度为2

// ==================== 嵌套文档查询 ====================

// 点语法查询嵌套字段
db.users.find({ "address.city": "Beijing" })

// 嵌套数组中的对象字段
db.orders.find({ "items.productId": "P001" })  // 订单中包含P001商品

// ==================== 正则表达式 ====================
db.users.find({ name: /^Ali/i })               // name以Ali开头（不区分大小写）
db.users.find({ name: { $regex: "^Ali", $options: "i" } })

// ==================== 结果处理 ====================

// 排序（1=升序，-1=降序）
db.users.find().sort({ age: -1, name: 1 })

// 分页
db.users.find().skip(20).limit(10)             // 第3页，每页10条

// 统计
db.users.countDocuments({ status: "active" })  // 满足条件的文档数
db.users.estimatedDocumentCount()              // 快速估算总数（不精确）

// 去重
db.users.distinct("city")                      // 获取所有不同的city值
db.users.distinct("city", { age: { $gte: 18 } }) // 带条件的去重
```

### 3.3 Update（更新）

```javascript
// ==================== 更新操作符 ====================
// $set $unset $inc $push $pull $addToSet $pop $rename

// 更新单条（默认只更新第一条匹配的）
db.users.updateOne(
  { name: "Alice" },          // 过滤条件
  { $set: { age: 26, "address.city": "Shanghai" } }  // 更新操作
)

// 更新多条
db.users.updateMany(
  { status: "inactive" },
  { $set: { status: "active", updatedAt: new Date() } }
)

// $set：设置字段值（字段不存在则创建）
db.users.updateOne({ _id: id }, {
  $set: { name: "Alice New", "profile.bio": "Developer" }
})

// $unset：删除字段
db.users.updateOne({ _id: id }, {
  $unset: { phone: "", oldField: "" }
})

// $inc：数值递增（可以是负数）
db.users.updateOne({ _id: id }, { $inc: { loginCount: 1 } })
db.products.updateOne({ _id: id }, { $inc: { stock: -1 } })   // 库存-1

// $push：向数组末尾添加元素
db.users.updateOne({ _id: id }, {
  $push: { tags: "vip" }
})

// $push 结合 $each 批量添加
db.users.updateOne({ _id: id }, {
  $push: { tags: { $each: ["vip", "senior"] } }
})

// $push 结合 $slice 限制数组长度（保留最新N个）
db.users.updateOne({ _id: id }, {
  $push: {
    recentViewed: {
      $each: [{ productId: "P100", viewedAt: new Date() }],
      $slice: -10   // 只保留最新10个
    }
  }
})

// $addToSet：添加到数组（自动去重，已存在则不添加）
db.users.updateOne({ _id: id }, {
  $addToSet: { tags: "developer" }
})

// $pull：从数组中删除满足条件的元素
db.users.updateOne({ _id: id }, {
  $pull: { tags: "inactive" }
})
db.orders.updateOne({ _id: id }, {
  $pull: { items: { productId: "P001" } }  // 删除items中productId为P001的元素
})

// $pop：删除数组第一个（-1）或最后一个（1）
db.users.updateOne({ _id: id }, { $pop: { tags: 1 } })   // 删最后一个

// upsert：不存在则插入，存在则更新
db.users.updateOne(
  { email: "new@example.com" },
  { $set: { name: "New User", createdAt: new Date() } },
  { upsert: true }
)

// findOneAndUpdate：更新并返回文档
const updated = db.users.findOneAndUpdate(
  { _id: id },
  { $inc: { loginCount: 1 } },
  {
    returnDocument: "after",   // 返回更新后的文档（"before"返回更新前）
    projection: { name: 1, loginCount: 1 }
  }
)
```

### 3.4 Delete（删除）

```javascript
// 删除单条
db.users.deleteOne({ name: "Alice" })

// 删除多条
db.users.deleteMany({ status: "deleted" })

// 删除所有文档（保留集合）
db.users.deleteMany({})

// 删除集合
db.users.drop()

// findOneAndDelete：删除并返回被删除的文档
const deleted = db.users.findOneAndDelete({ _id: id })
```

---

## 4. 文档模型设计（核心重点）

> 这是 MongoDB 和 MySQL 最大的思维差异，也是 MongoDB 用好用坏的关键。

### 4.1 嵌入 vs 引用

MongoDB 存储关联数据有两种方式：**嵌入（Embedding）** 和 **引用（Reference）**。

#### 嵌入文档（Embedding）

将相关数据直接嵌入到父文档中，类似 MySQL 的反范式化。

```javascript
// 嵌入方式存储用户和地址
{
  _id: ObjectId("..."),
  name: "Alice",
  addresses: [
    {
      type: "home",
      city: "Beijing",
      street: "Chang'an Ave 1"
    },
    {
      type: "work",
      city: "Shanghai",
      street: "Nanjing Road 100"
    }
  ]
}
```

**✅ 适合嵌入的情况**：
- 子文档只属于这一个父文档（1对1 或 1对少量）
- 子文档总是随父文档一起读取
- 子文档不需要被独立查询
- 子文档数量有限且不会无限增长（建议 < 100）

**❌ 不适合嵌入的情况**：
- 子文档数量无限增长（会导致文档越来越大）
- 子文档需要被多个父文档引用（多对多）
- 子文档需要频繁独立查询

#### 引用（Reference）

存储关联文档的 `_id`，类似 MySQL 的外键。

```javascript
// 用户文档
{
  _id: ObjectId("user001"),
  name: "Alice"
}

// 订单文档（引用用户）
{
  _id: ObjectId("order001"),
  userId: ObjectId("user001"),   // 引用用户ID
  amount: 299
}

// 查询时需要手动 $lookup（类似JOIN）
db.orders.aggregate([
  { $match: { _id: ObjectId("order001") } },
  { $lookup: {
    from: "users",
    localField: "userId",
    foreignField: "_id",
    as: "user"
  }}
])
```

---

### 4.2 实战场景的模型设计

#### 场景一：博客系统（文章 + 评论）

```javascript
// ❌ 错误设计：无限嵌入评论
{
  _id: ObjectId("..."),
  title: "Redis学习笔记",
  content: "...",
  comments: [              // 评论可能有几千条，文档会无限膨胀！
    { user: "Bob", text: "写得好", createdAt: ... },
    // ... 无限增长
  ]
}

// ✅ 正确设计：热评嵌入 + 完整评论引用
// 文章文档（嵌入最新3条评论，用于列表展示）
{
  _id: ObjectId("article001"),
  title: "Redis学习笔记",
  content: "...",
  author: "Alice",
  tags: ["redis", "nosql"],
  viewCount: 1520,
  likeCount: 88,
  commentCount: 156,        // 总评论数（用于显示，不存评论本身）
  topComments: [            // 只嵌入少量精选评论（预加载优化）
    { user: "Bob", text: "写得好", likeCount: 12 }
  ],
  createdAt: ISODate("2024-01-15")
}

// 评论集合（独立存储，支持分页）
{
  _id: ObjectId("comment001"),
  articleId: ObjectId("article001"),
  userId: ObjectId("user001"),
  text: "写得很好，学到了很多！",
  likeCount: 12,
  createdAt: ISODate("2024-01-16")
}
```

#### 场景二：电商商品目录（不同品类，属性完全不同）

```javascript
// MySQL 的做法：用 EAV 模型（attribute-value 表），查询极其复杂
// MongoDB 的做法：Schema-less，每个品类直接存自己的属性

// 手机商品
{
  _id: ObjectId("..."),
  category: "phone",
  name: "iPhone 15 Pro",
  price: 8999,
  brand: "Apple",
  specs: {             // 手机专属属性
    screen: "6.1inch OLED",
    chip: "A17 Pro",
    ram: "8GB",
    storage: ["128GB", "256GB", "512GB"],
    camera: "48MP + 12MP + 12MP",
    battery: "3274mAh"
  }
}

// 衣服商品（属性完全不同）
{
  _id: ObjectId("..."),
  category: "clothing",
  name: "纯棉T恤",
  price: 89,
  brand: "优衣库",
  specs: {             // 衣服专属属性
    material: "100%纯棉",
    sizes: ["S", "M", "L", "XL", "XXL"],
    colors: ["白色", "黑色", "灰色"],
    washingGuide: "机洗30度"
  }
}
```

#### 场景三：游戏用户背包系统

```javascript
// 游戏用户完整数据（嵌入背包，因为背包属于且仅属于该用户）
{
  _id: ObjectId("player001"),
  username: "DragonSlayer",
  level: 85,
  experience: 450000,
  gold: 9800,

  // 属性（强类型）
  attributes: {
    hp: 5000, maxHp: 5000,
    mp: 2000, maxMp: 2000,
    attack: 850, defense: 620
  },

  // 背包（嵌入，上限200格）
  inventory: [
    { slotId: 1, itemId: "sword_001", name: "龙牙剑", qty: 1, attributes: { atk: 500 } },
    { slotId: 2, itemId: "potion_hp", name: "HP药水", qty: 50 }
  ],

  // 已装备的物品（嵌入）
  equipment: {
    weapon: { itemId: "sword_001", name: "龙牙剑" },
    armor:  { itemId: "armor_003", name: "神圣铠甲" }
  },

  // 好友列表（引用，可能很多）
  friendIds: [ObjectId("player002"), ObjectId("player003")],  // 只存ID

  // 最近登录记录（限制长度）
  loginHistory: [
    { loginAt: ISODate("2024-01-20"), ip: "1.2.3.4" }
    // $push + $slice 保留最近30条
  ],

  lastLoginAt: ISODate("2024-01-20T10:30:00Z"),
  createdAt: ISODate("2023-06-01")
}
```

---

### 4.3 Schema 版本管理

MongoDB 虽然无 Schema，但生产中需要做版本管理，避免混乱。

```javascript
// 在文档中加入 schema 版本号
{
  _id: ObjectId("..."),
  schemaVersion: 2,          // 记录文档结构版本
  name: "Alice",
  // v2 新增字段
  profile: {
    bio: "Developer"
  }
}

// 查询旧版本文档并做迁移
db.users.find({ schemaVersion: { $lt: 2 } }).forEach(doc => {
  db.users.updateOne(
    { _id: doc._id },
    {
      $set: {
        schemaVersion: 2,
        profile: { bio: "" }   // 为旧文档补充新字段的默认值
      }
    }
  )
})
```

---

## 5. 索引原理与实战

### 5.1 索引基础

```javascript
// 查看集合上的所有索引
db.users.getIndexes()

// 创建单字段索引
db.users.createIndex({ name: 1 })          // 升序
db.users.createIndex({ createdAt: -1 })    // 降序

// 创建唯一索引
db.users.createIndex({ email: 1 }, { unique: true })

// 创建复合索引
db.users.createIndex({ status: 1, createdAt: -1 })

// 创建部分索引（只对满足条件的文档建索引，减小索引体积）
db.users.createIndex(
  { email: 1 },
  { partialFilterExpression: { status: "active" } }  // 只索引活跃用户的email
)

// 创建稀疏索引（只对有该字段的文档建索引）
db.users.createIndex({ phone: 1 }, { sparse: true })

// 创建 TTL 索引（自动过期删除）
db.sessions.createIndex(
  { createdAt: 1 },
  { expireAfterSeconds: 86400 }   // 24小时后自动删除文档
)

// 在后台创建索引（不阻塞读写，生产环境必须加）
db.users.createIndex({ name: 1 }, { background: true })

// 删除索引
db.users.dropIndex("name_1")       // 用索引名
db.users.dropIndex({ name: 1 })    // 用索引定义

// 删除所有索引（_id索引除外）
db.users.dropIndexes()
```

### 5.2 explain() 分析查询计划

类似 MySQL 的 EXPLAIN，是优化查询的核心工具。

```javascript
// 基础查询计划
db.users.find({ name: "Alice" }).explain()

// 详细执行统计（最常用）
db.users.find({ name: "Alice" }).explain("executionStats")

// 所有候选执行计划
db.users.find({ name: "Alice" }).explain("allPlansExecution")
```

**explain 输出关键字段解读**：

```javascript
{
  "queryPlanner": {
    "winningPlan": {
      "stage": "FETCH",          // 执行阶段
      "inputStage": {
        "stage": "IXSCAN",       // IXSCAN=索引扫描（好），COLLSCAN=全集合扫描（差）
        "indexName": "name_1",   // 使用的索引名
        "direction": "forward"
      }
    }
  },
  "executionStats": {
    "nReturned": 1,              // 返回文档数
    "totalKeysExamined": 1,      // 扫描的索引键数量（越小越好）
    "totalDocsExamined": 1,      // 扫描的文档数量（越小越好）
    "executionTimeMillis": 0     // 执行时间（毫秒）
  }
}
```

**关键判断标准**：
- `stage: "COLLSCAN"` → ❌ 全集合扫描，需要加索引
- `stage: "IXSCAN"` → ✅ 使用了索引
- `totalDocsExamined >> nReturned` → 索引效率低，需要优化
- `totalKeysExamined == nReturned` → 完美，索引精准命中

### 5.3 索引类型详解

#### 复合索引与最左前缀

和 MySQL 一样，MongoDB 复合索引也遵循最左前缀原则：

```javascript
// 复合索引 { status: 1, age: -1, name: 1 }
db.users.createIndex({ status: 1, age: -1, name: 1 })

// ✅ 用到索引
db.users.find({ status: "active" })
db.users.find({ status: "active", age: 25 })
db.users.find({ status: "active", age: 25, name: "Alice" })

// ❌ 跳过了status，无法使用索引
db.users.find({ age: 25 })
db.users.find({ name: "Alice" })
```

#### 多键索引（Multikey Index）

当被索引的字段是**数组**时，MongoDB 自动创建多键索引，为数组的每个元素建立索引条目。

```javascript
db.users.createIndex({ tags: 1 })  // tags 是数组，自动成为多键索引

// 以下查询都可以走索引
db.users.find({ tags: "developer" })
db.users.find({ tags: { $all: ["developer", "gamer"] } })
db.users.find({ tags: { $in: ["developer", "designer"] } })
```

#### 文本索引（Text Index）

```javascript
// 每个集合只能有一个文本索引
db.articles.createIndex({ title: "text", content: "text" })

// 带权重的文本索引（title匹配权重是content的10倍）
db.articles.createIndex(
  { title: "text", content: "text" },
  { weights: { title: 10, content: 1 } }
)

// 全文搜索
db.articles.find({ $text: { $search: "mongodb index" } })

// 按相关度排序
db.articles.find(
  { $text: { $search: "mongodb" } },
  { score: { $meta: "textScore" } }
).sort({ score: { $meta: "textScore" } })
```

#### 地理空间索引

```javascript
// 2dsphere 索引（支持球面地理计算）
db.places.createIndex({ location: "2dsphere" })

// 存储 GeoJSON 格式
db.places.insertOne({
  name: "故宫博物院",
  location: {
    type: "Point",
    coordinates: [116.3974, 39.9163]   // [经度, 纬度]
  }
})

// 查询附近500米内的地点
db.places.find({
  location: {
    $near: {
      $geometry: { type: "Point", coordinates: [116.4, 39.92] },
      $maxDistance: 500   // 单位：米
    }
  }
})

// 查询某个区域内的地点（多边形）
db.places.find({
  location: {
    $geoWithin: {
      $geometry: {
        type: "Polygon",
        coordinates: [[
          [116.3, 39.8], [116.5, 39.8],
          [116.5, 40.0], [116.3, 40.0],
          [116.3, 39.8]
        ]]
      }
    }
  }
})
```

### 5.4 索引优化策略

```javascript
// 1. 使用 hint 强制指定索引（测试/调试用）
db.users.find({ name: "Alice", age: 25 }).hint({ name: 1 })

// 2. 覆盖查询（查询字段全在索引中，无需取原文档）
// 索引 { name: 1, age: 1, _id: 0 }
db.users.createIndex({ name: 1, age: 1 })
// 这个查询的 explain 中 totalDocsExamined=0，纯走索引
db.users.find(
  { name: "Alice" },
  { name: 1, age: 1, _id: 0 }  // projection 字段是索引字段的子集
).explain("executionStats")

// 3. 查看索引使用情况（哪些索引没被使用，可以删除）
db.users.aggregate([{ $indexStats: {} }])
```

---

## 6. 聚合管道（Aggregation Pipeline）

> 聚合管道是 MongoDB 最强大的功能，类似 SQL 的 GROUP BY + JOIN + 子查询的集合体，是 MongoDB 的核心竞争力。

### 6.1 管道概念

```
文档流 → [Stage1] → [Stage2] → [Stage3] → 结果

就像 Linux 的管道：
cat file.txt | grep "error" | sort | uniq -c
```

### 6.2 核心 Stage 详解

#### $match（过滤，等同于 WHERE）

```javascript
db.orders.aggregate([
  { $match: {
    status: "paid",
    createdAt: {
      $gte: ISODate("2024-01-01"),
      $lt: ISODate("2024-02-01")
    },
    totalAmount: { $gte: 100 }
  }}
])
// ⚠️ $match 要放管道最前面，利用索引减少后续处理的数据量
```

#### $project（字段投影，等同于 SELECT 字段）

```javascript
db.users.aggregate([
  { $project: {
    _id: 0,
    name: 1,
    email: 1,
    // 计算新字段
    fullName: { $concat: ["$firstName", " ", "$lastName"] },
    ageGroup: {
      $cond: {
        if: { $gte: ["$age", 18] },
        then: "adult",
        else: "minor"
      }
    },
    // 字符串截取
    emailDomain: { $substr: ["$email", { $indexOfBytes: ["$email", "@"] }, -1] }
  }}
])
```

#### $group（分组聚合，等同于 GROUP BY）

```javascript
// 按 status 分组，统计各状态的订单数和总金额
db.orders.aggregate([
  { $group: {
    _id: "$status",                          // 分组字段
    orderCount: { $sum: 1 },                 // 计数
    totalAmount: { $sum: "$totalAmount" },   // 求和
    avgAmount: { $avg: "$totalAmount" },     // 平均值
    maxAmount: { $max: "$totalAmount" },     // 最大值
    minAmount: { $min: "$totalAmount" },     // 最小值
    firstOrder: { $first: "$createdAt" },    // 第一个值
    lastOrder: { $last: "$createdAt" },      // 最后一个值
    userIds: { $addToSet: "$userId" }        // 收集不重复的值到数组
  }}
])

// 多字段分组
db.orders.aggregate([
  { $group: {
    _id: { status: "$status", month: { $month: "$createdAt" } },
    count: { $sum: 1 }
  }}
])

// 统计全局（_id: null）
db.orders.aggregate([
  { $group: {
    _id: null,
    total: { $sum: "$totalAmount" },
    count: { $sum: 1 }
  }}
])
```

#### $sort、$limit、$skip

```javascript
db.orders.aggregate([
  { $match: { status: "paid" } },
  { $sort: { totalAmount: -1, createdAt: -1 } },  // 先按金额降序，再按时间降序
  { $skip: 20 },                                   // 跳过前20
  { $limit: 10 }                                   // 取10条
])
```

#### $lookup（关联查询，等同于 LEFT JOIN）

```javascript
// 查询订单时关联用户信息
db.orders.aggregate([
  { $lookup: {
    from: "users",           // 关联的集合名
    localField: "userId",    // orders 中的关联字段
    foreignField: "_id",     // users 中的关联字段
    as: "userInfo"           // 结果存在哪个字段（数组）
  }},
  // userInfo 是数组，取第一个元素
  { $unwind: "$userInfo" },  // 展开数组
  { $project: {
    orderId: 1,
    totalAmount: 1,
    "userInfo.name": 1,
    "userInfo.email": 1
  }}
])

// 带条件的 $lookup（MongoDB 3.6+）
db.orders.aggregate([
  { $lookup: {
    from: "products",
    let: { productIds: "$items.productId" },  // 定义变量
    pipeline: [
      { $match: {
        $expr: { $in: ["$_id", "$$productIds"] }  // 使用变量（$$ 双美元符）
      }},
      { $project: { name: 1, price: 1 } }
    ],
    as: "productDetails"
  }}
])
```

#### $unwind（展开数组）

```javascript
// 将数组字段展开为多条文档
// 原文档：{ name: "Alice", tags: ["a", "b", "c"] }
// 展开后：三条文档，每条 tags 为一个值

db.users.aggregate([
  { $unwind: "$tags" }
])
// 结果：
// { name: "Alice", tags: "a" }
// { name: "Alice", tags: "b" }
// { name: "Alice", tags: "c" }

// 保留空数组文档（默认会丢弃）
db.users.aggregate([
  { $unwind: { path: "$tags", preserveNullAndEmptyArrays: true } }
])
```

#### $addFields（添加新字段）

```javascript
db.orders.aggregate([
  { $addFields: {
    // 根据条件添加字段
    discountAmount: {
      $cond: {
        if: { $gte: ["$totalAmount", 1000] },
        then: { $multiply: ["$totalAmount", 0.1] },  // 打9折
        else: 0
      }
    },
    // 计算字段
    itemCount: { $size: "$items" }
  }}
])
```

#### $facet（多维度并行聚合）

```javascript
// 一次查询同时得到多个维度的统计结果（搜索页常用）
db.products.aggregate([
  { $match: { category: "electronics" } },
  { $facet: {
    // 维度1：价格区间分布
    "priceRanges": [
      { $bucket: {
        groupBy: "$price",
        boundaries: [0, 100, 500, 1000, 5000, 999999],
        default: "Other",
        output: { count: { $sum: 1 } }
      }}
    ],
    // 维度2：品牌分布
    "brands": [
      { $group: { _id: "$brand", count: { $sum: 1 } } },
      { $sort: { count: -1 } },
      { $limit: 10 }
    ],
    // 维度3：总数和平均价格
    "summary": [
      { $group: {
        _id: null,
        total: { $sum: 1 },
        avgPrice: { $avg: "$price" }
      }}
    ]
  }}
])
```

### 6.3 综合聚合案例

```javascript
// 案例：统计 2024年1月 各用户的消费情况，只显示消费超过1000的用户，按消费金额降序

db.orders.aggregate([
  // 第1步：筛选2024年1月的已支付订单
  { $match: {
    status: "paid",
    createdAt: {
      $gte: ISODate("2024-01-01T00:00:00Z"),
      $lt:  ISODate("2024-02-01T00:00:00Z")
    }
  }},

  // 第2步：按用户分组统计
  { $group: {
    _id: "$userId",
    totalSpent: { $sum: "$totalAmount" },
    orderCount: { $sum: 1 },
    avgOrderAmount: { $avg: "$totalAmount" }
  }},

  // 第3步：筛选消费超过1000的用户
  { $match: { totalSpent: { $gte: 1000 } } },

  // 第4步：关联用户信息
  { $lookup: {
    from: "users",
    localField: "_id",
    foreignField: "_id",
    as: "userInfo"
  }},
  { $unwind: "$userInfo" },

  // 第5步：格式化输出
  { $project: {
    _id: 0,
    userId: "$_id",
    userName: "$userInfo.name",
    email: "$userInfo.email",
    totalSpent: { $round: ["$totalSpent", 2] },
    orderCount: 1,
    avgOrderAmount: { $round: ["$avgOrderAmount", 2] }
  }},

  // 第6步：排序
  { $sort: { totalSpent: -1 } },

  // 第7步：取前20名
  { $limit: 20 }
])
```

---

## 7. 事务机制

### 7.1 单文档原子性

MongoDB **单个文档**的操作天然是原子的，无需手动开启事务：

```javascript
// 这个更新操作是原子的：要么成功更新所有字段，要么全部不更新
db.players.updateOne(
  { _id: playerId },
  {
    $inc: { gold: -100 },          // 扣100金币
    $push: { inventory: newItem }  // 加入物品
  }
)
```

> **设计启示**：如果能将需要原子化的数据放在同一个文档中，就不需要多文档事务。这是 MongoDB 模型设计的重要原则。

### 7.2 多文档事务（MongoDB 4.0+）

当需要跨文档/跨集合的原子操作时，使用事务。

```javascript
// 转账场景：从账户A转账100到账户B
const session = db.getMongo().startSession()

session.startTransaction({
  readConcern: { level: "snapshot" },   // 隔离级别
  writeConcern: { w: "majority" }       // 写入确认级别
})

try {
  const accounts = session.getDatabase("bank").accounts

  // 扣款
  const debit = accounts.updateOne(
    { _id: "accountA", balance: { $gte: 100 } },  // 余额充足才扣
    { $inc: { balance: -100 } },
    { session }
  )

  if (debit.modifiedCount === 0) {
    throw new Error("余额不足")
  }

  // 入账
  accounts.updateOne(
    { _id: "accountB" },
    { $inc: { balance: 100 } },
    { session }
  )

  // 提交
  session.commitTransaction()
  print("转账成功")

} catch (error) {
  // 回滚
  session.abortTransaction()
  print("转账失败，已回滚：", error.message)

} finally {
  session.endSession()
}
```

### 7.3 事务注意事项

- 事务在**副本集**或**分片集群**上才能使用，单机模式不支持
- 事务最长持续时间默认 **60秒**（`transactionLifetimeLimitSeconds`）
- 事务内的操作过多会影响性能，**尽量短小**
- MongoDB 事务性能不如 MySQL，能用单文档原子操作就不用事务

---

## 8. 高可用架构

### 8.1 副本集（Replica Set）

副本集是 MongoDB 高可用的基础，类似 MySQL 的主从复制 + 哨兵。

```
Primary（主节点）
    │ oplog 复制
    ├──→ Secondary1（从节点）
    └──→ Secondary2（从节点）

当 Primary 宕机时，剩余节点通过 Raft 协议选举新的 Primary
```

**搭建副本集**：

```javascript
// 1. 启动三个 mongod 实例（三个不同端口）
// 配置文件都加上 replication: replSetName: "rs0"

// 2. 连接任意一个，初始化副本集
rs.initiate({
  _id: "rs0",
  members: [
    { _id: 0, host: "192.168.1.101:27017", priority: 2 },  // 优先级高，优先选主
    { _id: 1, host: "192.168.1.102:27017", priority: 1 },
    { _id: 2, host: "192.168.1.103:27017", priority: 1 }
  ]
})

// 查看副本集状态
rs.status()
rs.conf()

// 添加/移除成员
rs.add("192.168.1.104:27017")
rs.remove("192.168.1.104:27017")

// 步进（手动触发主节点卸任）
rs.stepDown()
```

**读写分离配置**：

```javascript
// 连接字符串指定读偏好
const client = new MongoClient(
  "mongodb://host1:27017,host2:27017,host3:27017/?replicaSet=rs0&readPreference=secondaryPreferred"
)

// 代码中设置读偏好
db.users.find({}).readPref("secondaryPreferred")

// 读偏好选项：
// primary：只读主节点（默认，强一致性）
// primaryPreferred：优先主节点，不可用则读从节点
// secondary：只读从节点（可能读到旧数据）
// secondaryPreferred：优先从节点，不可用则读主节点（最常用的读写分离配置）
// nearest：读延迟最低的节点
```

---

### 8.2 分片集群（Sharded Cluster）

当数据量或并发超过单副本集能力时，使用分片集群水平扩展。

```
客户端
  │
  ↓
mongos（路由节点，可多个）
  │
  ├──→ Config Server副本集（存储分片元数据）
  │
  ├──→ Shard1 副本集（存储部分数据）
  ├──→ Shard2 副本集（存储部分数据）
  └──→ Shard3 副本集（存储部分数据）
```

**分片策略**：

```javascript
// 1. 启用分片
sh.enableSharding("myapp")

// 2. 选择分片键，对集合分片

// 哈希分片（数据均匀分布，适合高写入）
sh.shardCollection("myapp.orders", { userId: "hashed" })

// 范围分片（支持范围查询，但可能热点）
sh.shardCollection("myapp.logs", { createdAt: 1 })

// 复合分片键
sh.shardCollection("myapp.events", { userId: 1, createdAt: 1 })

// 查看分片状态
sh.status()
```

**分片键选择原则**：
- **基数高**（不同值多）：避免所有数据集中到少数分片
- **写分散**：写请求均匀分布到各分片
- **查询包含分片键**：否则需要广播查询（scatter-gather），性能差
- **不能修改**：分片键一旦选定无法更改

---

## 9. 性能优化实战

### 9.1 常见性能问题排查

```javascript
// 1. 查找慢查询（超过100ms）
db.setProfilingLevel(1, { slowms: 100 })

// 查看慢查询日志
db.system.profile.find().sort({ ts: -1 }).limit(10).pretty()

// 2. 查看当前正在执行的操作
db.currentOp()
db.currentOp({ "active": true, "secs_running": { "$gt": 5 } })  // 执行超过5秒的

// 3. 强制终止某个操作
db.killOp(opid)

// 4. 服务器状态统计
db.serverStatus()
db.stats()                     // 数据库统计
db.users.stats()               // 集合统计（包含索引大小）
```

### 9.2 写入优化

```javascript
// 1. 批量写入（BulkWrite）
const bulkOps = []
for (let i = 0; i < 10000; i++) {
  bulkOps.push({
    insertOne: { document: { name: `User${i}`, age: 20 + (i % 50) } }
  })
}
db.users.bulkWrite(bulkOps, { ordered: false })  // ordered:false 并行执行更快

// 2. 混合批量操作
db.users.bulkWrite([
  { insertOne: { document: { name: "Alice" } } },
  { updateOne: { filter: { name: "Bob" }, update: { $inc: { age: 1 } } } },
  { deleteOne: { filter: { status: "deleted" } } }
])

// 3. 降低写入关注级别（允许少量丢失，换取吞吐量）
db.logs.insertMany(logDocs, { writeConcern: { w: 0 } })  // 写入后不等确认
```

### 9.3 读取优化

```javascript
// 1. 投影（只取需要的字段，减少网络传输）
db.users.find({}, { name: 1, email: 1, _id: 0 })  // 避免 find({})

// 2. 合理使用 limit
db.users.find().limit(100)     // 永远不要无限制查询大集合

// 3. 大批量数据使用游标
const cursor = db.users.find({ status: "active" })
cursor.forEach(doc => {
  // 逐批处理，不一次性加载全部到内存
})

// 4. 使用 allowDiskUse 处理大型聚合（超过100MB内存限制）
db.orders.aggregate(
  [ /* ... pipeline ... */ ],
  { allowDiskUse: true }
)
```

### 9.4 连接池配置

```javascript
// Node.js 连接池配置示例
const client = new MongoClient(uri, {
  maxPoolSize: 50,        // 最大连接数（默认100）
  minPoolSize: 5,         // 最小连接数
  maxIdleTimeMS: 30000,   // 空闲连接超时
  waitQueueTimeoutMS: 5000  // 等待连接超时
})
```

---

## 10. MongoDB vs MySQL vs Redis 选型指南

```
需求分析树：

是否需要亿级 KV 读写，且延迟要求 <1ms？
    └─ 是 → Redis

是否需要强事务（资金、库存、订单状态）？
    └─ 是 → MySQL

数据结构是否高度嵌套、Schema 是否经常变化？
    └─ 是 → MongoDB

是否需要复杂的多表 JOIN 查询？
    └─ 是 → MySQL

是否是高频写入的日志、埋点数据？
    └─ 是 → MongoDB

是否需要地理位置查询？
    └─ 是 → MongoDB（或 MySQL 的空间索引）

是否是固定 Schema 的结构化业务数据？
    └─ 是 → MySQL
```

| 维度 | Redis | MySQL | MongoDB |
|------|-------|-------|---------|
| 数据模型 | KV + 多种数据结构 | 关系型表 | 文档（BSON） |
| Schema | 无 | 强 Schema | 弱 Schema |
| 事务 | 弱（Lua脚本） | 强ACID | 单文档原子/多文档事务 |
| JOIN | ❌ | ✅ 强项 | $lookup（性能较差） |
| 水平扩展 | ✅ Cluster | ⚠️ 需中间件 | ✅ 原生Sharding |
| 持久化 | RDB+AOF | ✅ | ✅ |
| 查询语言 | 命令 | SQL | 文档查询/聚合管道 |
| 典型用途 | 缓存/队列/锁 | 业务核心数据 | 内容/日志/游戏数据 |

---

## 11. MongoDB 面试题精选

---

**Q1：MongoDB 和 MySQL 最核心的区别是什么？各适合什么场景？**

> MongoDB 是文档型 NoSQL，以 BSON 格式存储，Schema 灵活，天然支持嵌套结构和数组，水平扩展能力强，适合内容管理、游戏数据、日志、产品目录等 Schema 变化频繁或数据结构复杂的场景。
>
> MySQL 是关系型数据库，强 Schema，支持复杂 JOIN 和强 ACID 事务，适合结构固定、需要强一致性的核心业务数据（订单、账户、库存）。两者通常在同一个系统中协作使用，而不是互相替代。

---

**Q2：MongoDB 文档中 _id 字段有什么特点？ObjectId 是怎么组成的？**

> `_id` 是每个文档的唯一标识符，相当于主键，默认类型是 ObjectId。ObjectId 由 12 字节组成：前4字节是 Unix 时间戳（秒），接3字节是机器ID，再2字节是进程ID，最后3字节是随机递增计数器。因此 ObjectId 天然有序，可以通过 `_id` 排序来替代 `createdAt` 字段排序，且能保证分布式环境下全局唯一。

---

**Q3：嵌入文档和引用各在什么时候使用？**

> 使用嵌入：子文档只属于一个父文档（一对一、一对少量多）；总是随父文档一起读取；子文档数量有上限，不会无限增长（建议小于100）；比如用户地址、订单的商品列表。
>
> 使用引用：子文档会被多个父文档引用；子文档数量无限增长；子文档需要频繁独立查询；比如文章与评论（评论可能几千条）、用户与好友（好友可能很多）。设计原则：优先考虑嵌入，在有充分理由时才用引用。

---

**Q4：MongoDB 的聚合管道是什么？和 SQL 的对应关系是什么？**

> 聚合管道是一系列处理文档的阶段，文档流过每个阶段被逐步处理和转换，最终得到结果。对应关系：`$match` 对应 WHERE、`$group` 对应 GROUP BY、`$project` 对应 SELECT、`$sort` 对应 ORDER BY、`$limit/$skip` 对应 LIMIT/OFFSET、`$lookup` 对应 JOIN、`$unwind` 用于展开数组（SQL无直接对应）。
>
> 优化关键：`$match` 和 `$sort` 要放最前面以利用索引，减少后续阶段处理的数据量。

---

**Q5：MongoDB 副本集的选举机制是怎样的？**

> 副本集使用 Raft 协议变种进行选举。当 Primary 宕机后：Secondary 发现心跳超时（默认10秒），发起选举；候选节点向其他节点发起投票请求；节点只投给 oplog 比自己新或一样新的候选节点；获得超过半数节点投票的节点成为新的 Primary。因此副本集成员数量应为**奇数**（3、5、7），避免出现平票。如果只有2台机器，可加一个仲裁节点（Arbiter），只参与投票不存数据。

---

**Q6：分片集群中如何选择分片键？选错了会有什么问题？**

> 分片键选择原则：高基数（值的种类多，保证数据均匀分布）、写分散（写操作均匀落到各分片）、查询包含分片键（避免广播查询）。
>
> 选错的后果：基数低（如 status 只有3个值）→ 数据都集中到少数分片，其他分片空闲，形成热点；查询不含分片键 → 需要询问所有分片（scatter-gather），延迟高；选了单调递增字段（如时间戳）且做范围分片 → 所有新数据写入最后一个分片，形成写热点。分片键一旦确定不可更改，必须慎重设计。

---

**Q7：MongoDB 的 TTL 索引是什么？如何实现自动过期？**

> TTL 索引是一种特殊的单字段索引，建在 Date 类型字段上，MongoDB 后台线程每60秒扫描一次，自动删除过期文档：`db.sessions.createIndex({ createdAt: 1 }, { expireAfterSeconds: 86400 })`。这是实现 Session 过期、日志自动清理等功能的常用方式，无需应用层定时任务。注意：TTL 索引不适用于副本集的从节点（删除只在主节点执行），且删除是异步的，有最多60秒的误差。

---

**Q8：explain 中看到 COLLSCAN 说明什么？如何优化？**

> COLLSCAN（Collection Scan）表示全集合扫描，需要遍历每一条文档来找到匹配数据，类似 MySQL 的 ALL 全表扫描，在大集合上性能极差。优化步骤：1) 查看查询条件字段是否有索引，没有则创建；2) 如果有索引但仍然 COLLSCAN，可能是索引字段被函数包裹或类型不匹配；3) 用 `hint()` 强制指定索引验证；4) 检查联合索引是否遵循最左前缀。

---

## 📚 学习路线总结

```
第1周：入门阶段
  ✅ 理解文档模型 vs 关系模型
  ✅ 掌握 CRUD 所有操作和操作符
  ✅ 理解嵌入 vs 引用的选择原则
  ✅ 在本地用 mongosh 完整操作一遍

第2周：进阶阶段
  ✅ 索引类型和 explain() 的使用
  ✅ 聚合管道：$match/$group/$project/$lookup/$unwind
  ✅ 副本集的搭建和基本原理
  ✅ 写一个完整的游戏用户数据模型设计

第3周：深入阶段
  ✅ 事务机制和单文档原子性的权衡
  ✅ 分片集群原理和分片键选择
  ✅ 性能优化和慢查询分析
  ✅ 结合实习经历（你用过MongoDB做点赞系统）深挖原理

持续提升：
  📖 官方文档 https://www.mongodb.com/docs/
  📖 《MongoDB权威指南》第3版
  💻 结合你的游戏服务器实习经验，对比实际使用场景
```

> 💡 **结合你的实习经历**：你在妙世界科技用 MongoDB 存冷数据（热数据在Redis），这就是典型的 Redis + MongoDB 分层架构。面试时要能详细说出：点赞数据什么时候从 Redis 迁到 MongoDB？迁移的触发条件是什么？MongoDB 里的点赞数据是什么结构？查询时怎么聚合？这些细节能让你的面试回答非常有说服力。💪