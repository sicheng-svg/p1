# 数据库实战案例手册
## Redis + MySQL + MongoDB 综合实践

> **使用说明**：本手册以"业务场景"为主线，每个场景都包含完整的建表/建索引语句、核心 SQL/命令，以及背后的设计思路。**建议一条一条手敲**，不要复制粘贴，真正理解每行语句的含义。

---

## 目录

- [场景一：用户注册与登录系统](#场景一用户注册与登录系统)
- [场景二：商品与订单系统](#场景二商品与订单系统)
- [场景三：缓存架构（Cache-Aside 模式）](#场景三缓存架构cache-aside-模式)
- [场景四：分布式限流](#场景四分布式限流)
- [场景五：排行榜系统](#场景五排行榜系统)
- [场景六：社交关系（好友/关注）](#场景六社交关系好友关注)
- [场景七：消息队列与异步任务](#场景七消息队列与异步任务)
- [场景八：点赞/踩系统（你的实习项目）](#场景八点赞踩系统你的实习项目)
- [场景九：内容管理系统（文章+评论）](#场景九内容管理系统文章评论)
- [场景十：分布式锁](#场景十分布式锁)
- [场景十一：延迟任务队列](#场景十一延迟任务队列)
- [场景十二：数据统计与分析](#场景十二数据统计与分析)

---

## 场景一：用户注册与登录系统

### 1.1 MySQL 建表

```sql
-- 创建数据库
CREATE DATABASE IF NOT EXISTS practice CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE practice;

-- 用户表
CREATE TABLE users (
    id          BIGINT UNSIGNED NOT NULL AUTO_INCREMENT COMMENT '用户ID',
    username    VARCHAR(50)     NOT NULL                COMMENT '用户名',
    email       VARCHAR(100)    NOT NULL                COMMENT '邮箱',
    phone       VARCHAR(20)     DEFAULT NULL            COMMENT '手机号',
    password    CHAR(60)        NOT NULL                COMMENT 'bcrypt加密密码',
    avatar      VARCHAR(255)    DEFAULT NULL            COMMENT '头像URL',
    status      TINYINT(1)      NOT NULL DEFAULT 1      COMMENT '状态:1正常,0禁用',
    created_at  DATETIME        NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at  DATETIME        NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    last_login  DATETIME        DEFAULT NULL            COMMENT '最后登录时间',
    PRIMARY KEY (id),
    UNIQUE KEY uk_username (username),
    UNIQUE KEY uk_email    (email),
    UNIQUE KEY uk_phone    (phone),
    KEY idx_status_created (status, created_at)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户表';

-- 用户详情表（垂直分表：把不常用的大字段拆出来）
CREATE TABLE user_profiles (
    user_id     BIGINT UNSIGNED NOT NULL COMMENT '关联用户ID',
    nickname    VARCHAR(50)     DEFAULT NULL,
    gender      TINYINT(1)      DEFAULT 0 COMMENT '0未知,1男,2女',
    birthday    DATE            DEFAULT NULL,
    bio         VARCHAR(500)    DEFAULT NULL COMMENT '个人简介',
    city        VARCHAR(50)     DEFAULT NULL,
    PRIMARY KEY (user_id),
    CONSTRAINT fk_profile_user FOREIGN KEY (user_id) REFERENCES users(id) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='用户详情表';

-- 验证：查看表结构
DESC users;
SHOW CREATE TABLE users\G
SHOW INDEX FROM users;
```

### 1.2 MySQL 基础数据操作

```sql
-- 插入测试用户
INSERT INTO users (username, email, phone, password) VALUES
('alice', 'alice@example.com', '13800138001', '$2b$12$hashed_password_alice'),
('bob',   'bob@example.com',   '13800138002', '$2b$12$hashed_password_bob'),
('charlie','charlie@example.com','13800138003','$2b$12$hashed_password_charlie');

-- 插入用户详情
INSERT INTO user_profiles (user_id, nickname, gender, city) VALUES
(1, 'Alice大佬', 1, 'Beijing'),
(2, 'BobCoder',  1, 'Shanghai'),
(3, 'Charlie',   0, 'Guangzhou');

-- 查询：用户登录（通过邮箱查用户）
SELECT id, username, email, password, status
FROM users
WHERE email = 'alice@example.com';

-- 查询：联合查用户信息和详情
SELECT u.id, u.username, u.email, u.phone, u.last_login,
       p.nickname, p.gender, p.city
FROM users u
LEFT JOIN user_profiles p ON u.id = p.user_id
WHERE u.id = 1;

-- 更新：登录成功后更新最后登录时间
UPDATE users SET last_login = NOW() WHERE id = 1;

-- 更新：禁用用户
UPDATE users SET status = 0, updated_at = NOW() WHERE id = 3;

-- 查询：分页查询活跃用户（按注册时间倒序）
SELECT id, username, email, created_at
FROM users
WHERE status = 1
ORDER BY created_at DESC
LIMIT 0, 10;    -- 第1页，每页10条

-- EXPLAIN 验证索引使用
EXPLAIN SELECT id, username, email, created_at
FROM users
WHERE status = 1
ORDER BY created_at DESC
LIMIT 0, 10;
-- 应该看到 key: idx_status_created，type: ref

-- 统计：各状态用户数量
SELECT status,
       COUNT(*) AS total,
       COUNT(CASE WHEN last_login >= DATE_SUB(NOW(), INTERVAL 30 DAY) THEN 1 END) AS active_30days
FROM users
GROUP BY status;
```

### 1.3 Redis 存储 Session

```bash
# 用户登录成功后，生成 SessionID 存入 Redis
# key 格式：session:{sessionId}，value 是用户信息JSON，过期时间7天

# 登录：设置 Session
SET session:abc123def456 '{"userId":1,"username":"alice","email":"alice@example.com","loginAt":"2024-01-15T10:00:00Z"}' EX 604800

# 验证 Session（每次请求都调用）
GET session:abc123def456

# 刷新 Session 过期时间（活跃用户滑动过期）
EXPIRE session:abc123def456 604800

# 查询 Session 剩余时间
TTL session:abc123def456

# 登出：删除 Session
DEL session:abc123def456

# 实战：使用 Hash 存储 Session（方便单独更新字段）
HSET session:abc123def456 userId 1 username "alice" email "alice@example.com" loginAt "2024-01-15"
EXPIRE session:abc123def456 604800

HGET session:abc123def456 userId
HGETALL session:abc123def456

# 强制下线某用户（后台封号操作）
# 需要维护 userId -> sessionId 的映射
SET user_session:1 "abc123def456" EX 604800
# 找到 session 并删除
GET user_session:1           -- 得到 sessionId
DEL session:abc123def456     -- 删除 session
DEL user_session:1           -- 删除映射

# 统计在线用户数（用 SET 存储在线 userId）
SADD online_users 1 2 3
SCARD online_users           -- 在线用户数
SISMEMBER online_users 1     -- 某用户是否在线
SREM online_users 3          -- 用户下线
```

---

## 场景二：商品与订单系统

### 2.1 MySQL 建表

```sql
-- 商品分类表
CREATE TABLE categories (
    id          INT UNSIGNED    NOT NULL AUTO_INCREMENT,
    parent_id   INT UNSIGNED    NOT NULL DEFAULT 0    COMMENT '父分类ID，0为顶级',
    name        VARCHAR(50)     NOT NULL,
    sort_order  INT             NOT NULL DEFAULT 0,
    PRIMARY KEY (id),
    KEY idx_parent (parent_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 商品表
CREATE TABLE products (
    id          BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
    category_id INT UNSIGNED    NOT NULL,
    name        VARCHAR(200)    NOT NULL,
    price       DECIMAL(10,2)   NOT NULL COMMENT '价格（单位：元）',
    stock       INT             NOT NULL DEFAULT 0   COMMENT '库存',
    status      TINYINT(1)      NOT NULL DEFAULT 1   COMMENT '1上架,0下架',
    description TEXT,
    sales_count INT             NOT NULL DEFAULT 0   COMMENT '销量',
    created_at  DATETIME        NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id),
    KEY idx_category_status (category_id, status),
    KEY idx_price (price),
    KEY idx_sales (sales_count DESC)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 订单表
CREATE TABLE orders (
    id          BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
    order_no    CHAR(20)        NOT NULL                COMMENT '订单号（业务唯一键）',
    user_id     BIGINT UNSIGNED NOT NULL,
    total_amount DECIMAL(10,2)  NOT NULL,
    pay_amount  DECIMAL(10,2)   NOT NULL                COMMENT '实付金额',
    discount    DECIMAL(10,2)   NOT NULL DEFAULT 0,
    status      TINYINT(1)      NOT NULL DEFAULT 0
                                COMMENT '0待支付,1已支付,2已发货,3已完成,4已取消',
    pay_time    DATETIME        DEFAULT NULL,
    created_at  DATETIME        NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at  DATETIME        NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY (id),
    UNIQUE KEY uk_order_no (order_no),
    KEY idx_user_status (user_id, status),
    KEY idx_status_created (status, created_at)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- 订单商品表（快照，防止商品信息修改后订单历史变化）
CREATE TABLE order_items (
    id          BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
    order_id    BIGINT UNSIGNED NOT NULL,
    product_id  BIGINT UNSIGNED NOT NULL,
    product_name VARCHAR(200)   NOT NULL    COMMENT '下单时的商品名（快照）',
    product_price DECIMAL(10,2) NOT NULL    COMMENT '下单时的价格（快照）',
    quantity    INT             NOT NULL,
    subtotal    DECIMAL(10,2)   NOT NULL    COMMENT '小计 = price * quantity',
    PRIMARY KEY (id),
    KEY idx_order_id (order_id),
    KEY idx_product_id (product_id)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
```

### 2.2 下单流程 SQL（事务）

```sql
-- 插入测试商品
INSERT INTO categories (name, parent_id) VALUES ('手机数码', 0), ('电脑外设', 0);
INSERT INTO products (category_id, name, price, stock) VALUES
(1, 'iPhone 15', 8999.00, 100),
(1, '小米14',    3999.00, 200),
(2, '机械键盘',   599.00, 500);

-- ==================== 下单事务 ====================
-- 模拟用户购买 iPhone15 × 1 + 机械键盘 × 2

-- 第一步：查询商品信息和库存（加锁防止超卖）
SELECT id, name, price, stock, status
FROM products
WHERE id IN (1, 3)
FOR UPDATE;    -- 悲观锁，锁住这两行

-- 检查库存是否充足（应用层判断）
-- iPhone15: stock=100 >= 1 ✅
-- 机械键盘: stock=500 >= 2 ✅

-- 开始事务
START TRANSACTION;

-- 扣减库存
UPDATE products SET stock = stock - 1 WHERE id = 1 AND stock >= 1;
UPDATE products SET stock = stock - 2 WHERE id = 3 AND stock >= 2;

-- 插入订单主表
INSERT INTO orders (order_no, user_id, total_amount, pay_amount, status)
VALUES ('ORD20240115001', 1, 10197.00, 10197.00, 0);

-- 获取刚插入的订单ID
-- SET @order_id = LAST_INSERT_ID();

-- 插入订单商品（价格快照，记录下单时的价格）
INSERT INTO order_items (order_id, product_id, product_name, product_price, quantity, subtotal)
VALUES
(LAST_INSERT_ID(), 1, 'iPhone 15', 8999.00, 1, 8999.00),
(LAST_INSERT_ID(), 3, '机械键盘',   599.00, 2, 1198.00);

-- 提交事务
COMMIT;

-- 回滚示例（如果中间某步失败）
-- ROLLBACK;

-- ==================== 支付成功更新订单状态 ====================
START TRANSACTION;

UPDATE orders
SET status = 1, pay_time = NOW(), updated_at = NOW()
WHERE order_no = 'ORD20240115001' AND status = 0;  -- 只更新待支付状态的订单

-- 检查影响行数，=1才算成功（防止重复支付）
-- ROW_COUNT() = 1 则提交，否则回滚

COMMIT;
```

### 2.3 订单查询

```sql
-- 查询用户订单列表（分页）
SELECT o.id, o.order_no, o.total_amount, o.status, o.created_at,
       COUNT(oi.id) AS item_count
FROM orders o
LEFT JOIN order_items oi ON o.id = oi.order_id
WHERE o.user_id = 1
GROUP BY o.id
ORDER BY o.created_at DESC
LIMIT 0, 10;

-- 查询订单详情（关联商品）
SELECT o.order_no, o.total_amount, o.status, o.pay_time,
       oi.product_name, oi.product_price, oi.quantity, oi.subtotal
FROM orders o
JOIN order_items oi ON o.id = oi.order_id
WHERE o.order_no = 'ORD20240115001';

-- 查询热销商品 TOP10（按销量）
SELECT id, name, price, sales_count
FROM products
WHERE status = 1
ORDER BY sales_count DESC
LIMIT 10;

-- 统计今日各状态订单数和金额
SELECT status,
       COUNT(*)          AS order_count,
       SUM(total_amount) AS total_amount,
       AVG(pay_amount)   AS avg_amount
FROM orders
WHERE DATE(created_at) = CURDATE()
GROUP BY status
ORDER BY status;

-- 深分页优化（游标分页）
-- ❌ 深分页（慢）
SELECT * FROM orders WHERE user_id = 1 ORDER BY id DESC LIMIT 10000, 10;

-- ✅ 游标分页（快），前端保存上次最后一条的 id
SELECT * FROM orders
WHERE user_id = 1 AND id < 99990    -- 99990 是上页最后一条的 id
ORDER BY id DESC
LIMIT 10;

-- EXPLAIN 验证
EXPLAIN SELECT * FROM orders
WHERE user_id = 1 AND id < 99990
ORDER BY id DESC LIMIT 10;
```

### 2.4 Redis 缓存商品信息

```bash
# 商品详情缓存（Hash结构，方便单独更新字段）
HSET product:1 name "iPhone 15" price "8999.00" stock 100 status 1
EXPIRE product:1 3600

# 读取商品信息
HGETALL product:1
HGET product:1 price

# 库存相关（用 String 的原子操作防超卖）
SET stock:1 100        # 初始化库存到 Redis
SET stock:3 500

# 扣减库存（原子操作）
DECR stock:1           # 返回新值，<0 说明超卖
DECRBY stock:3 2       # 减2

# 如果返回值 < 0，说明超卖，需要回滚
INCR stock:1           # 回滚库存

# 商品列表缓存（整个列表序列化为 JSON 存 String）
SET products:category:1:page:1 '[{"id":1,"name":"iPhone15","price":8999}]' EX 300

# 热门商品列表（ZSet，score=销量）
ZADD hot_products 15000 "1"   # 商品1，销量15000
ZADD hot_products 8000  "2"   # 商品2，销量8000
ZADD hot_products 5000  "3"   # 商品3，销量5000

# 获取销量最高的 TOP10 商品ID
ZREVRANGE hot_products 0 9 WITHSCORES

# 销量增加时更新
ZINCRBY hot_products 1 "1"    # 商品1销量+1
```

---

## 场景三：缓存架构（Cache-Aside 模式）

```bash
# ============================================================
# 读操作：先查 Redis，未命中再查 MySQL，结果写入 Redis
# ============================================================

# 1. 查 Redis
GET user:1
# 结果是 nil（未命中）

# 2. 查 MySQL（应用层执行）
# SELECT id,username,email FROM users WHERE id=1;
# 得到：{"id":1,"username":"alice","email":"alice@example.com"}

# 3. 写入 Redis（设置过期时间，防止冷数据永久占内存）
SET user:1 '{"id":1,"username":"alice","email":"alice@example.com"}' EX 3600

# 4. 再次查询，命中缓存
GET user:1

# ============================================================
# 写操作：先更新 MySQL，再删除 Redis（不是更新！）
# ============================================================

# MySQL 更新用户信息（应用层执行）
# UPDATE users SET username='alice_new' WHERE id=1;

# 删除缓存（而不是更新，避免并发写入导致不一致）
DEL user:1

# 验证缓存已删除
EXISTS user:1     # 返回 0

# ============================================================
# 缓存穿透防护：查询不存在的数据，缓存空值
# ============================================================

# 查询 id=99999（不存在）
GET user:99999    # nil

# MySQL 查询也没有，缓存空值（短期，防止持续穿透）
SET user:99999 "" EX 300   # 空字符串，5分钟过期

# 下次查询
GET user:99999    # 返回空字符串，应用层判断为不存在，不查DB

# ============================================================
# 缓存雪崩防护：过期时间加随机数，避免大量key同时过期
# ============================================================

# 基础过期时间 3600 秒，加 0~600 的随机数
# 实际 TTL 在 3600~4200 之间随机（应用层计算后设置）
SET user:1 '{"id":1}' EX 3756   # 3600 + random(600)
SET user:2 '{"id":2}' EX 3821
SET user:3 '{"id":3}' EX 3612
```

---

## 场景四：分布式限流

```bash
# ============================================================
# 方案一：固定窗口限流（每分钟最多100次请求）
# ============================================================

# key 格式：rate:{userId}:{时间窗口}
# 时间窗口 = 当前Unix时间戳 / 60（每分钟一个key）

# 用 INCR + EXPIRE 实现
INCR rate:user1:23567    # 返回当前计数
# 如果返回1（第一次请求），设置过期时间
EXPIRE rate:user1:23567 60

# 模拟多次请求
INCR rate:user1:23567    # 2
INCR rate:user1:23567    # 3
TTL rate:user1:23567     # 查剩余时间

# ============================================================
# 方案二：滑动窗口限流（用 ZSet 实现，更精确）
# ============================================================
# key：rate_window:{userId}
# score：请求时间戳（毫秒）
# value：唯一请求ID（UUID 或时间戳+随机数）

# 模拟请求到来（时间戳：1705286400000 ms）
ZADD rate_window:user1 1705286400000 "req_001"
ZADD rate_window:user1 1705286401000 "req_002"
ZADD rate_window:user1 1705286402000 "req_003"

# 删除窗口之外的旧请求（保留最近60秒内的）
# 当前时间 - 60000ms = 窗口起点
ZREMRANGEBYSCORE rate_window:user1 0 1705286340000

# 统计窗口内的请求数
ZCARD rate_window:user1

# 如果 > 100，则限流拒绝
# 如果 <= 100，则允许，并设置过期时间避免内存泄漏
EXPIRE rate_window:user1 60

# ============================================================
# 方案三：令牌桶限流（用 Lua 脚本保证原子性）
# ============================================================
# 每秒补充10个令牌，桶容量100

EVAL "
local key = KEYS[1]
local capacity = tonumber(ARGV[1])     -- 桶容量：100
local rate = tonumber(ARGV[2])          -- 每秒补充：10
local now = tonumber(ARGV[3])           -- 当前时间戳（秒）
local requested = tonumber(ARGV[4])     -- 本次需要令牌数：1

local last_time = tonumber(redis.call('HGET', key, 'last_time') or now)
local tokens = tonumber(redis.call('HGET', key, 'tokens') or capacity)

-- 计算新增令牌数
local elapsed = now - last_time
local new_tokens = math.min(capacity, tokens + elapsed * rate)

-- 判断令牌是否足够
if new_tokens >= requested then
    redis.call('HSET', key, 'tokens', new_tokens - requested, 'last_time', now)
    redis.call('EXPIRE', key, 3600)
    return 1    -- 允许
else
    redis.call('HSET', key, 'tokens', new_tokens, 'last_time', now)
    redis.call('EXPIRE', key, 3600)
    return 0    -- 限流
end
" 1 token_bucket:user1 100 10 1705286400 1
```

---

## 场景五：排行榜系统

```bash
# ============================================================
# 游戏积分排行榜（ZSet）
# ============================================================

# 添加玩家积分
ZADD game_rank 9500 "player:alice"
ZADD game_rank 8800 "player:bob"
ZADD game_rank 9200 "player:charlie"
ZADD game_rank 7600 "player:diana"
ZADD game_rank 9900 "player:eve"

# 查询排行榜 TOP 10（降序）
ZREVRANGE game_rank 0 9 WITHSCORES

# 查询某玩家排名（0-based）
ZREVRANK game_rank "player:alice"   # 返回1（第2名，eve是第1名）

# 查询某玩家积分
ZSCORE game_rank "player:alice"     # 9500

# 玩家积分增加（游戏中获得积分）
ZINCRBY game_rank 200 "player:alice"   # alice积分变为9700

# 查询积分在8000-9000之间的玩家
ZRANGEBYSCORE game_rank 8000 9000 WITHSCORES

# 查询某玩家周围的排名（前后各2名）
# 先获取alice的排名
ZREVRANK game_rank "player:alice"     # 假设返回2（第3名）
# 获取第1名到第5名
ZREVRANGE game_rank 0 4 WITHSCORES

# ============================================================
# 每日/每周/每月排行榜（多榜单设计）
# ============================================================

# 每日榜：key 带日期
ZADD rank:daily:20240115 9500 "player:alice"
ZADD rank:daily:20240115 8800 "player:bob"
EXPIRE rank:daily:20240115 86400      # 每日榜保留1天

# 每周榜：合并7天数据
ZUNIONSTORE rank:weekly:2024W03 7 \
  rank:daily:20240115 \
  rank:daily:20240116 \
  rank:daily:20240117 \
  rank:daily:20240118 \
  rank:daily:20240119 \
  rank:daily:20240120 \
  rank:daily:20240121
EXPIRE rank:weekly:2024W03 604800     # 每周榜保留7天

ZREVRANGE rank:weekly:2024W03 0 9 WITHSCORES

# ============================================================
# MySQL 存储历史排行榜（持久化）
# ============================================================

-- 排行榜历史表
CREATE TABLE leaderboard_history (
    id          BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
    period_type TINYINT(1)      NOT NULL COMMENT '1日榜,2周榜,3月榜',
    period_key  VARCHAR(20)     NOT NULL COMMENT '如:20240115,2024W03',
    user_id     BIGINT UNSIGNED NOT NULL,
    score       BIGINT          NOT NULL,
    rank_num    INT             NOT NULL,
    created_at  DATETIME        NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id),
    KEY idx_period_rank (period_type, period_key, rank_num),
    KEY idx_user_period (user_id, period_type)
) ENGINE=InnoDB;

-- 每日定时任务：将 Redis 排行榜数据持久化到 MySQL
-- （应用层从Redis读数据，再批量插入MySQL）
INSERT INTO leaderboard_history (period_type, period_key, user_id, score, rank_num) VALUES
(1, '20240115', 1, 9700, 1),
(1, '20240115', 3, 9200, 2),
(1, '20240115', 2, 8800, 3);

-- 查询用户历史最高排名
SELECT period_key, score, rank_num
FROM leaderboard_history
WHERE user_id = 1 AND period_type = 1
ORDER BY rank_num ASC
LIMIT 10;
```

---

## 场景六：社交关系（好友/关注）

```bash
# ============================================================
# 关注/粉丝系统（Set）
# ============================================================

# alice 关注 bob 和 charlie
SADD following:alice bob charlie diana

# bob 关注 alice 和 charlie
SADD following:bob alice charlie eve

# 粉丝列表（alice的粉丝）
SADD followers:alice bob eve

# 查询 alice 的关注列表
SMEMBERS following:alice

# 查询 alice 的粉丝数
SCARD followers:alice

# alice 和 bob 的共同关注（交集）
SINTER following:alice following:bob
# 结果：charlie（alice和bob都关注了charlie）

# alice 可能认识的人（bob关注但alice没关注的）
SDIFF following:bob following:alice

# 是否关注（alice是否关注了bob）
SISMEMBER following:alice bob

# 取消关注（alice取消关注diana）
SREM following:alice diana
```

```sql
-- ============================================================
-- MySQL 持久化好友关系（大规模数据）
-- ============================================================

CREATE TABLE user_follows (
    id          BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
    follower_id BIGINT UNSIGNED NOT NULL COMMENT '关注者',
    followee_id BIGINT UNSIGNED NOT NULL COMMENT '被关注者',
    created_at  DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id),
    UNIQUE KEY uk_follow (follower_id, followee_id),  -- 防重复关注
    KEY idx_followee (followee_id)                     -- 查粉丝时用
) ENGINE=InnoDB;

-- 关注操作
INSERT IGNORE INTO user_follows (follower_id, followee_id) VALUES (1, 2);
-- IGNORE：已关注则忽略，不报错

-- 取消关注
DELETE FROM user_follows WHERE follower_id = 1 AND followee_id = 2;

-- 查询关注列表（带用户信息）
SELECT u.id, u.username, u.avatar, uf.created_at AS follow_time
FROM user_follows uf
JOIN users u ON uf.followee_id = u.id
WHERE uf.follower_id = 1
ORDER BY uf.created_at DESC
LIMIT 20;

-- 查询共同关注（alice=1 和 bob=2 共同关注的人）
SELECT followee_id
FROM user_follows
WHERE follower_id IN (1, 2)
GROUP BY followee_id
HAVING COUNT(*) = 2;

-- 统计关注数和粉丝数（冗余字段，避免每次COUNT）
ALTER TABLE users ADD COLUMN following_count INT NOT NULL DEFAULT 0;
ALTER TABLE users ADD COLUMN followers_count INT NOT NULL DEFAULT 0;

-- 关注时同时更新计数（用事务）
START TRANSACTION;
INSERT IGNORE INTO user_follows (follower_id, followee_id) VALUES (1, 2);
UPDATE users SET following_count = following_count + 1 WHERE id = 1;
UPDATE users SET followers_count = followers_count + 1 WHERE id = 2;
COMMIT;
```

---

## 场景七：消息队列与异步任务

```bash
# ============================================================
# 简单消息队列（List 实现）
# ============================================================

# 生产者：将任务推入队列（RPUSH 从右边加）
RPUSH task_queue '{"type":"send_email","to":"alice@example.com","subject":"验证码","code":"123456"}'
RPUSH task_queue '{"type":"send_sms","phone":"13800138001","content":"您的验证码是：654321"}'
RPUSH task_queue '{"type":"process_order","orderId":"ORD20240115001"}'

# 查看队列长度
LLEN task_queue

# 查看队列内容（不消费）
LRANGE task_queue 0 -1

# 消费者：BLPOP（阻塞弹出，消费者等待任务）
BLPOP task_queue 30    # 阻塞最多30秒，有数据立即返回
# 返回：1) "task_queue"  2) '{"type":"send_email",...}'

# ============================================================
# 延迟队列（ZSet 实现，score=执行时间戳）
# ============================================================

# 添加延迟任务（30分钟后执行）
# score = 当前时间戳 + 延迟秒数
ZADD delay_queue 1705288200 '{"taskId":"T001","type":"cancel_order","orderId":"ORD001"}'
ZADD delay_queue 1705288800 '{"taskId":"T002","type":"cancel_order","orderId":"ORD002"}'

# 消费者轮询（每秒执行一次）：取出到期的任务
ZRANGEBYSCORE delay_queue 0 1705286400   # 取出 score <= 当前时间戳的任务

# 原子获取并删除（用 Lua 保证原子性）
EVAL "
local tasks = redis.call('ZRANGEBYSCORE', KEYS[1], 0, ARGV[1], 'LIMIT', 0, 10)
if #tasks > 0 then
    redis.call('ZREM', KEYS[1], unpack(tasks))
end
return tasks
" 1 delay_queue 1705286400

# ============================================================
# Redis Stream（持久化消息队列，支持消费者组）
# ============================================================

# 生产消息
XADD order_stream * orderId "ORD001" userId "1" amount "9797"
XADD order_stream * orderId "ORD002" userId "2" amount "599"

# 查看消息
XLEN order_stream
XRANGE order_stream - +     # 查看所有消息

# 创建消费者组（从最新消息开始消费用 $，从头消费用 0）
XGROUP CREATE order_stream payment_group $ MKSTREAM

# 消费者1 消费消息
XREADGROUP GROUP payment_group consumer1 COUNT 5 BLOCK 0 STREAMS order_stream >

# 确认消费（ACK）
XACK order_stream payment_group 1705286400000-0

# 查看未确认消息
XPENDING order_stream payment_group - + 10
```

---

## 场景八：点赞/踩系统（你的实习项目）

```bash
# ============================================================
# Redis 热数据层（高频读写）
# ============================================================

# 数据结构设计：
# like:{targetType}:{targetId} → Hash
#   field: {userId}
#   value: 1（点赞）或 -1（踩）

# 用户1点赞文章100
HSET like:article:100 "user:1" 1
HSET like:article:100 "user:2" 1
HSET like:article:100 "user:3" -1   # 用户3踩了

# 点赞数 = 值为1的数量（应用层统计）
HGETALL like:article:100    # 获取所有数据后应用层统计

# 更好的设计：分开存点赞和踩（两个Set，方便统计）
SADD likes:article:100 "user:1" "user:2"     # 点赞用户集合
SADD dislikes:article:100 "user:3"            # 踩用户集合

# 获取点赞数
SCARD likes:article:100      # 2

# 判断用户是否点赞
SISMEMBER likes:article:100 "user:1"    # 1（已点赞）

# 取消点赞
SREM likes:article:100 "user:1"

# 点赞数缓存（单独存，快速读取）
INCR like_count:article:100    # 点赞+1
DECR like_count:article:100    # 取消点赞-1
GET like_count:article:100

# ============================================================
# 热点数据双写：写 Redis 同时标记"脏数据"
# 后台定时任务将脏数据同步到 MongoDB
# ============================================================

# 标记文章100的点赞数据需要同步
SADD dirty_likes "article:100"

# 定时任务读取脏数据列表
SMEMBERS dirty_likes
# 得到 article:100，从 Redis 读取最新点赞数据写入 MongoDB
# 写入成功后从脏数据集合中删除
SREM dirty_likes "article:100"
```

```javascript
// ============================================================
// MongoDB 冷数据层（持久化存储）
// ============================================================

// 使用 practice 数据库
use practice

// 点赞统计集合（存储汇总数据）
db.like_stats.insertOne({
    targetType: "article",
    targetId: 100,
    likeCount: 256,
    dislikeCount: 12,
    updatedAt: new Date()
})

// 用户点赞明细集合（可选，用于防刷和数据分析）
db.like_records.insertMany([
    {
        targetType: "article",
        targetId: 100,
        userId: 1,
        action: 1,          // 1=点赞，-1=踩
        createdAt: new Date("2024-01-15T10:00:00Z")
    },
    {
        targetType: "article",
        targetId: 100,
        userId: 2,
        action: 1,
        createdAt: new Date("2024-01-15T10:05:00Z")
    }
])

// 唯一索引防重复
db.like_records.createIndex(
    { targetType: 1, targetId: 1, userId: 1 },
    { unique: true }
)

// 查询某文章的点赞统计
db.like_stats.findOne({ targetType: "article", targetId: 100 })

// 从 Redis 同步时，upsert 更新
db.like_stats.updateOne(
    { targetType: "article", targetId: 100 },
    { $set: { likeCount: 258, dislikeCount: 12, updatedAt: new Date() } },
    { upsert: true }
)

// 查询某用户点赞过的所有文章（分页）
db.like_records.find(
    { userId: 1, action: 1 },
    { targetId: 1, createdAt: 1, _id: 0 }
).sort({ createdAt: -1 }).limit(20)

// 统计点赞数前10的文章
db.like_stats.find(
    { targetType: "article" },
    { targetId: 1, likeCount: 1, _id: 0 }
).sort({ likeCount: -1 }).limit(10)
```

---

## 场景九：内容管理系统（文章+评论）

```sql
-- ============================================================
-- MySQL：文章表（核心结构化数据）
-- ============================================================
CREATE TABLE articles (
    id          BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
    user_id     BIGINT UNSIGNED NOT NULL,
    title       VARCHAR(200)    NOT NULL,
    summary     VARCHAR(500)    DEFAULT NULL  COMMENT '摘要',
    cover_url   VARCHAR(255)    DEFAULT NULL  COMMENT '封面图',
    status      TINYINT(1)      NOT NULL DEFAULT 0  COMMENT '0草稿,1发布,2下线',
    view_count  INT             NOT NULL DEFAULT 0,
    like_count  INT             NOT NULL DEFAULT 0,
    comment_count INT           NOT NULL DEFAULT 0,
    published_at DATETIME       DEFAULT NULL,
    created_at  DATETIME        NOT NULL DEFAULT CURRENT_TIMESTAMP,
    updated_at  DATETIME        NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    PRIMARY KEY (id),
    KEY idx_user_status   (user_id, status),
    KEY idx_status_publish (status, published_at),
    KEY idx_view_count    (view_count DESC),
    FULLTEXT KEY ft_title (title)   -- 全文索引（标题搜索）
) ENGINE=InnoDB;

-- 文章标签关联表
CREATE TABLE article_tags (
    article_id  BIGINT UNSIGNED NOT NULL,
    tag_name    VARCHAR(50)     NOT NULL,
    PRIMARY KEY (article_id, tag_name),
    KEY idx_tag (tag_name)
) ENGINE=InnoDB;

-- 阅读量更新（高频写，可以先写Redis再批量同步MySQL）
UPDATE articles SET view_count = view_count + 1 WHERE id = 1;

-- 查询文章列表（分页，带作者信息）
SELECT a.id, a.title, a.summary, a.cover_url,
       a.view_count, a.like_count, a.comment_count, a.published_at,
       u.username AS author, u.avatar AS author_avatar
FROM articles a
JOIN users u ON a.user_id = u.id
WHERE a.status = 1
ORDER BY a.published_at DESC
LIMIT 0, 20;

-- 全文搜索文章（利用FULLTEXT索引）
SELECT id, title, view_count,
       MATCH(title) AGAINST('Redis学习' IN BOOLEAN MODE) AS score
FROM articles
WHERE MATCH(title) AGAINST('Redis学习' IN BOOLEAN MODE) > 0
ORDER BY score DESC
LIMIT 20;

-- 查询某标签下的文章
SELECT a.id, a.title, a.published_at
FROM articles a
JOIN article_tags t ON a.id = t.article_id
WHERE t.tag_name = 'redis' AND a.status = 1
ORDER BY a.published_at DESC
LIMIT 20;
```

```javascript
// ============================================================
// MongoDB：文章内容 + 评论（Schema灵活，嵌套友好）
// ============================================================

use practice

// 文章内容集合（正文用MongoDB，避免MySQL大字段）
db.article_contents.insertOne({
    articleId: 1,           // 对应 MySQL 的 articles.id
    content: "# Redis学习笔记\n\n## 1. 什么是Redis...",
    format: "markdown",
    wordCount: 5000,
    readingTime: 15,        // 预估阅读时间（分钟）
    revision: 3,            // 修订版本号
    updatedAt: new Date()
})

// 获取文章内容
db.article_contents.findOne({ articleId: 1 })

// 评论集合（独立存储，支持大量评论）
db.comments.insertMany([
    {
        articleId: 1,
        userId: 2,
        content: "写得非常好，Redis的底层原理讲得很清楚！",
        likeCount: 25,
        replyCount: 3,
        status: 1,          // 1正常，0已删除
        createdAt: new Date("2024-01-16T09:00:00Z"),
        // 嵌入前3条回复（预加载优化）
        topReplies: [
            {
                userId: 3,
                content: "同感，收藏了",
                createdAt: new Date("2024-01-16T09:30:00Z")
            }
        ]
    },
    {
        articleId: 1,
        userId: 3,
        content: "请问跳表和红黑树的区别是什么？",
        likeCount: 8,
        replyCount: 1,
        status: 1,
        createdAt: new Date("2024-01-16T10:00:00Z"),
        topReplies: []
    }
])

// 评论集合索引
db.comments.createIndex({ articleId: 1, createdAt: -1 })    // 按文章查评论（时间降序）
db.comments.createIndex({ userId: 1, createdAt: -1 })        // 查用户的所有评论
db.comments.createIndex({ likeCount: -1 })                   // 热门评论排序

// 查询文章的评论（分页，按时间倒序）
db.comments.find(
    { articleId: 1, status: 1 },
    { content: 1, userId: 1, likeCount: 1, createdAt: 1, topReplies: 1 }
).sort({ createdAt: -1 }).skip(0).limit(20)

// 查询热门评论（按点赞数）
db.comments.find(
    { articleId: 1, status: 1 },
    { content: 1, userId: 1, likeCount: 1, createdAt: 1 }
).sort({ likeCount: -1 }).limit(5)

// 评论获得点赞
db.comments.updateOne(
    { _id: ObjectId("...") },
    { $inc: { likeCount: 1 } }
)

// 统计文章的评论数（更新MySQL的comment_count）
db.comments.countDocuments({ articleId: 1, status: 1 })

// 聚合：按文章统计评论数（批量更新MySQL）
db.comments.aggregate([
    { $match: { status: 1 } },
    { $group: {
        _id: "$articleId",
        commentCount: { $sum: 1 }
    }},
    { $sort: { commentCount: -1 } },
    { $limit: 100 }
])
```

```bash
# ============================================================
# Redis：文章阅读量（高频写，避免直接写MySQL）
# ============================================================

# 阅读量写入 Redis（每次访问 +1）
INCR view_count:article:1    # 返回最新阅读量

# 批量查询多篇文章的阅读量
MGET view_count:article:1 view_count:article:2 view_count:article:3

# 定时任务（每5分钟）将 Redis 的阅读量同步到 MySQL
# 1. 用 SCAN 遍历所有 view_count:article:* 的 key
SCAN 0 MATCH "view_count:article:*" COUNT 100

# 2. 获取值后批量更新 MySQL（应用层执行）
# UPDATE articles SET view_count = ? WHERE id = ?

# 热门文章缓存（ZSet，score=阅读量）
ZADD hot_articles 15000 "1"
ZADD hot_articles 8000  "2"
ZADD hot_articles 5000  "3"
ZINCRBY hot_articles 1 "1"   # 每次阅读+1

# 获取今日热门文章 TOP20
ZREVRANGE hot_articles 0 19 WITHSCORES
```

---

## 场景十：分布式锁

```bash
# ============================================================
# Redis 分布式锁完整实现
# ============================================================

# ==================== 加锁 ====================
# key：lock:{业务名}:{资源ID}
# value：随机唯一值（UUID），用于验证锁归属
# NX：不存在才设置
# EX：过期时间（防死锁）

SET lock:order:ORD001 "uuid-abc123" NX EX 30
# 返回 OK：加锁成功
# 返回 nil：锁已被占用

# 查看锁
GET lock:order:ORD001         # uuid-abc123
TTL lock:order:ORD001         # 剩余秒数

# ==================== 解锁（Lua保证原子性）====================
EVAL "
if redis.call('get', KEYS[1]) == ARGV[1] then
    return redis.call('del', KEYS[1])
else
    return 0
end
" 1 lock:order:ORD001 "uuid-abc123"
# 返回 1：解锁成功（是自己的锁）
# 返回 0：解锁失败（锁不存在或已被其他持有者持有）

# ==================== 锁续期（看门狗）====================
# 如果业务还在执行但锁快到期，续期
EXPIRE lock:order:ORD001 30   # 重新设置30秒

# ==================== 模拟秒杀场景 ====================

# 商品库存
SET stock:P001 10

# 多个请求同时来，只有一个能成功加锁
SET lock:stock:P001 "req-001" NX EX 5    # 返回 OK（加锁成功）
SET lock:stock:P001 "req-002" NX EX 5    # 返回 nil（加锁失败）

# req-001 执行扣减库存
DECR stock:P001    # 返回 9

# req-001 解锁
EVAL "if redis.call('get',KEYS[1])==ARGV[1] then return redis.call('del',KEYS[1]) else return 0 end" \
  1 lock:stock:P001 "req-001"

# 现在 req-002 可以重试加锁了
SET lock:stock:P001 "req-002" NX EX 5    # 返回 OK

DECR stock:P001    # 返回 8
```

---

## 场景十一：延迟任务队列

```bash
# ============================================================
# 用 ZSet 实现延迟队列（订单30分钟未支付自动取消）
# ============================================================

# 下单时，添加延迟取消任务
# score = 当前时间戳 + 1800秒（30分钟）
# value = 任务JSON

# 假设当前时间戳是 1705286400
ZADD cancel_tasks 1705288200 '{"orderId":"ORD001","userId":1,"action":"cancel_if_unpaid"}'
ZADD cancel_tasks 1705288800 '{"orderId":"ORD002","userId":2,"action":"cancel_if_unpaid"}'
ZADD cancel_tasks 1705289400 '{"orderId":"ORD003","userId":3,"action":"cancel_if_unpaid"}'

# 查看所有待执行任务
ZRANGE cancel_tasks 0 -1 WITHSCORES

# ==================== 消费者（每秒轮询）====================
# 获取到期任务（score <= 当前时间戳）
ZRANGEBYSCORE cancel_tasks 0 1705288200

# 原子取出并删除（Lua脚本）
EVAL "
local tasks = redis.call('ZRANGEBYSCORE', KEYS[1], 0, ARGV[1], 'LIMIT', 0, 10)
if #tasks > 0 then
    redis.call('ZREM', KEYS[1], unpack(tasks))
    return tasks
end
return {}
" 1 cancel_tasks 1705288200

# ==================== 订单支付后取消延迟任务 ====================
# 用户在30分钟内支付了，需要把延迟任务从队列中删除
ZREM cancel_tasks '{"orderId":"ORD001","userId":1,"action":"cancel_if_unpaid"}'
```

```sql
-- MySQL：记录延迟任务状态（持久化，防止 Redis 重启丢失）
CREATE TABLE delay_tasks (
    id          BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
    task_type   VARCHAR(50)     NOT NULL  COMMENT '任务类型',
    task_data   JSON            NOT NULL  COMMENT '任务数据',
    execute_at  DATETIME        NOT NULL  COMMENT '执行时间',
    status      TINYINT(1)      NOT NULL DEFAULT 0 COMMENT '0待执行,1已执行,2已取消',
    created_at  DATETIME        NOT NULL DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (id),
    KEY idx_execute_status (execute_at, status)
) ENGINE=InnoDB;

-- 下单时写入延迟任务
INSERT INTO delay_tasks (task_type, task_data, execute_at)
VALUES (
    'cancel_if_unpaid',
    '{"orderId":"ORD001","userId":1}',
    DATE_ADD(NOW(), INTERVAL 30 MINUTE)
);

-- 服务重启后，从 MySQL 恢复未执行的任务到 Redis
SELECT id, task_type, task_data, UNIX_TIMESTAMP(execute_at) AS execute_ts
FROM delay_tasks
WHERE status = 0 AND execute_at > NOW()
ORDER BY execute_at ASC;

-- 任务执行后更新状态
UPDATE delay_tasks SET status = 1 WHERE id = 1;

-- 用户支付后取消任务
UPDATE delay_tasks
SET status = 2
WHERE task_type = 'cancel_if_unpaid'
  AND JSON_EXTRACT(task_data, '$.orderId') = 'ORD001'
  AND status = 0;
```

---

## 场景十二：数据统计与分析

```sql
-- ============================================================
-- MySQL：业务统计查询实战
-- ============================================================

-- 过去7天每天的注册用户数
SELECT DATE(created_at) AS reg_date, COUNT(*) AS reg_count
FROM users
WHERE created_at >= DATE_SUB(NOW(), INTERVAL 7 DAY)
GROUP BY DATE(created_at)
ORDER BY reg_date;

-- 过去30天的每日订单量和营收
SELECT DATE(created_at)          AS order_date,
       COUNT(*)                   AS order_count,
       SUM(pay_amount)            AS revenue,
       AVG(pay_amount)            AS avg_order_value,
       COUNT(DISTINCT user_id)    AS paying_users
FROM orders
WHERE status IN (1, 2, 3)    -- 已支付/已发货/已完成
  AND created_at >= DATE_SUB(NOW(), INTERVAL 30 DAY)
GROUP BY DATE(created_at)
ORDER BY order_date;

-- 用户消费金额分层（RFM 分析雏形）
SELECT
    CASE
        WHEN total_spent >= 10000 THEN 'VIP'
        WHEN total_spent >= 1000  THEN 'Regular'
        WHEN total_spent >= 100   THEN 'New'
        ELSE 'Inactive'
    END AS user_level,
    COUNT(*) AS user_count,
    AVG(total_spent) AS avg_spent
FROM (
    SELECT user_id, SUM(pay_amount) AS total_spent
    FROM orders
    WHERE status IN (1, 2, 3)
    GROUP BY user_id
) t
GROUP BY user_level
ORDER BY avg_spent DESC;

-- 商品销售排行（带同比增长）
SELECT
    p.id, p.name,
    COALESCE(this_month.sales, 0) AS this_month_sales,
    COALESCE(last_month.sales, 0) AS last_month_sales,
    CASE
        WHEN last_month.sales > 0
        THEN ROUND((this_month.sales - last_month.sales) / last_month.sales * 100, 2)
        ELSE NULL
    END AS growth_rate
FROM products p
LEFT JOIN (
    SELECT oi.product_id, SUM(oi.quantity) AS sales
    FROM order_items oi
    JOIN orders o ON oi.order_id = o.id
    WHERE o.status IN (1,2,3)
      AND YEAR(o.created_at)  = YEAR(NOW())
      AND MONTH(o.created_at) = MONTH(NOW())
    GROUP BY oi.product_id
) this_month ON p.id = this_month.product_id
LEFT JOIN (
    SELECT oi.product_id, SUM(oi.quantity) AS sales
    FROM order_items oi
    JOIN orders o ON oi.order_id = o.id
    WHERE o.status IN (1,2,3)
      AND o.created_at >= DATE_FORMAT(DATE_SUB(NOW(), INTERVAL 1 MONTH), '%Y-%m-01')
      AND o.created_at <  DATE_FORMAT(NOW(), '%Y-%m-01')
    GROUP BY oi.product_id
) last_month ON p.id = last_month.product_id
WHERE p.status = 1
ORDER BY this_month_sales DESC
LIMIT 20;
```

```bash
# ============================================================
# Redis：实时统计
# ============================================================

# UV（独立访客）统计——HyperLogLog（误差0.81%，内存极省）
PFADD uv:20240115 "user:1" "user:2" "user:3" "user:1"  # user:1重复不计
PFCOUNT uv:20240115    # 返回3（不重复用户数）

# 多天合并统计（本周UV）
PFADD uv:20240116 "user:1" "user:4" "user:5"
PFADD uv:20240117 "user:2" "user:6"
PFMERGE uv:week_20240115 uv:20240115 uv:20240116 uv:20240117
PFCOUNT uv:week_20240115    # 合并后的独立用户数

# PV（页面浏览量）——普通计数器
INCR pv:article:1:20240115    # 每次访问+1
GET pv:article:1:20240115     # 今日PV

# 签到统计——Bitmap（1亿用户只需12.5MB）
# 用户ID作为偏移量，1=签到
SETBIT sign:20240115 1 1     # 用户1今天签到
SETBIT sign:20240115 2 1     # 用户2今天签到
SETBIT sign:20240115 3 0     # 用户3今天未签到

GETBIT sign:20240115 1       # 查询用户1是否签到

BITCOUNT sign:20240115       # 今天总签到人数

# 统计连续签到（本月1号到15号都签到的用户）
BITOP AND sign:streak_20240101_15 \
  sign:20240101 sign:20240102 sign:20240103 \
  sign:20240104 sign:20240105 sign:20240106 \
  sign:20240107 sign:20240108 sign:20240109 \
  sign:20240110 sign:20240111 sign:20240112 \
  sign:20240113 sign:20240114 sign:20240115
BITCOUNT sign:streak_20240101_15    # 连续15天签到的用户数
```

```javascript
// ============================================================
// MongoDB：日志分析（聚合管道实战）
// ============================================================

use practice

// 插入操作日志
db.operation_logs.insertMany([
    { userId: 1, action: "view_article", targetId: 100, createdAt: new Date("2024-01-15T10:00:00Z") },
    { userId: 1, action: "like_article", targetId: 100, createdAt: new Date("2024-01-15T10:01:00Z") },
    { userId: 2, action: "view_article", targetId: 100, createdAt: new Date("2024-01-15T10:05:00Z") },
    { userId: 2, action: "view_article", targetId: 101, createdAt: new Date("2024-01-15T10:10:00Z") },
    { userId: 1, action: "create_order", targetId: 200, createdAt: new Date("2024-01-15T11:00:00Z") },
    { userId: 3, action: "view_article", targetId: 100, createdAt: new Date("2024-01-15T12:00:00Z") },
    { userId: 3, action: "create_order", targetId: 201, createdAt: new Date("2024-01-15T12:30:00Z") }
])

// 索引
db.operation_logs.createIndex({ userId: 1, createdAt: -1 })
db.operation_logs.createIndex({ action: 1, createdAt: -1 })
db.operation_logs.createIndex({ createdAt: 1 }, { expireAfterSeconds: 2592000 })  // 30天自动过期

// ── 分析1：今日各操作类型的次数 ──
db.operation_logs.aggregate([
    {
        $match: {
            createdAt: {
                $gte: new Date("2024-01-15T00:00:00Z"),
                $lt:  new Date("2024-01-16T00:00:00Z")
            }
        }
    },
    {
        $group: {
            _id: "$action",
            count: { $sum: 1 },
            uniqueUsers: { $addToSet: "$userId" }
        }
    },
    {
        $project: {
            action: "$_id",
            count: 1,
            uniqueUserCount: { $size: "$uniqueUsers" },
            _id: 0
        }
    },
    { $sort: { count: -1 } }
])

// ── 分析2：用户行为漏斗（浏览→点赞→下单的转化率）──
db.operation_logs.aggregate([
    {
        $match: {
            action: { $in: ["view_article", "like_article", "create_order"] },
            createdAt: { $gte: new Date("2024-01-15T00:00:00Z") }
        }
    },
    {
        $group: {
            _id: "$action",
            uniqueUsers: { $addToSet: "$userId" }
        }
    },
    {
        $project: {
            action: "$_id",
            userCount: { $size: "$uniqueUsers" },
            _id: 0
        }
    }
])
// 结果示例：
// view_article: 3人
// like_article: 1人（转化率33%）
// create_order: 2人（转化率67%）

// ── 分析3：每小时操作量趋势 ──
db.operation_logs.aggregate([
    {
        $group: {
            _id: {
                hour: { $hour: "$createdAt" },
                action: "$action"
            },
            count: { $sum: 1 }
        }
    },
    {
        $group: {
            _id: "$_id.hour",
            actions: {
                $push: { action: "$_id.action", count: "$count" }
            },
            total: { $sum: "$count" }
        }
    },
    { $sort: { "_id": 1 } }
])
```

---

## 综合练习：完整业务流程串联

> **最终练习**：把所有场景串起来，模拟一个完整的用户下单流程，体会三个数据库如何协作。

```
完整下单流程：

1. [Redis] 验证 Session → 获取用户身份
2. [Redis] 检查限流 → 防止刷单
3. [Redis] 查商品缓存 → 快速返回价格
4. [Redis] 分布式锁 → 防止并发超卖
5. [Redis] 原子扣减库存
6. [MySQL] 事务写订单 → 持久化
7. [Redis] 延迟队列 → 30分钟未支付自动取消
8. [Redis] 删商品缓存 → 保证一致性
9. [MongoDB] 写操作日志 → 行为分析
10. [Redis] 更新排行榜（销量榜）
```

```bash
# 步骤1：验证Session
HGETALL session:abc123def456          # 获取用户信息

# 步骤2：限流检查
INCR rate:user1:23600                  # 当前分钟计数+1
EXPIRE rate:user1:23600 60

# 步骤3：查商品缓存
HGETALL product:1                      # 命中：直接用
# 如未命中：查MySQL，写Redis，再用

# 步骤4：加分布式锁（防并发）
SET lock:stock:1 "req-abc" NX EX 5

# 步骤5：扣减Redis库存
DECR stock:1                           # 返回值 >= 0 才继续

# 步骤6：MySQL事务写订单（SQL见场景二）

# 步骤7：添加延迟取消任务
ZADD cancel_tasks 1705288200 '{"orderId":"ORD001","action":"cancel_if_unpaid"}'

# 步骤8：解锁 + 删缓存
EVAL "if redis.call('get',KEYS[1])==ARGV[1] then return redis.call('del',KEYS[1]) else return 0 end" \
  1 lock:stock:1 "req-abc"
DEL product:1                          # 删除商品缓存，下次读时从MySQL重建

# 步骤10：更新销量排行榜
ZINCRBY hot_products 1 "1"
```

```javascript
// 步骤9：MongoDB写操作日志
db.operation_logs.insertOne({
    userId: 1,
    action: "create_order",
    targetId: "ORD001",
    metadata: {
        productIds: [1, 3],
        totalAmount: 10197,
        deviceType: "mobile",
        ip: "1.2.3.4"
    },
    createdAt: new Date()
})
```

---

> 💡 **练习建议**：
> 1. 先在本地把 MySQL、Redis、MongoDB 全部启动
> 2. 按场景顺序，每个语句**手动敲一遍**，观察返回值
> 3. 遇到不理解的命令，用 `HELP SET`、`\h SELECT`、`db.help()` 查文档
> 4. 每个场景做完后，思考：**如果流量扩大100倍，哪里会成为瓶颈？**
> 5. 结合你的五子棋和日志系统项目，想想哪些地方可以用 Redis 优化
>
> 这份手册覆盖了面试中 90% 的数据库实战问题，反复敲完一遍，你对数据库的理解会有质的飞跃。💪