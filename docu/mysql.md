# MySQL 从入门到精通学习指南

> **阅读说明**：本手册默认你已学过 MySQL 基础，因此基础语法一笔带过，重点放在**索引原理、事务机制、SQL 优化、锁机制、高可用架构**等进阶内容。每个重要章节都配有原理图示和实战案例。

---

## 目录

1. [基础速览（一笔带过）](#1-基础速览一笔带过)
2. [存储引擎深度解析](#2-存储引擎深度解析)
3. [索引原理与实战](#3-索引原理与实战)
4. [SQL 优化（核心重点）](#4-sql-优化核心重点)
5. [事务与 MVCC](#5-事务与-mvcc)
6. [锁机制详解](#6-锁机制详解)
7. [日志系统](#7-日志系统)
8. [主从复制与高可用](#8-主从复制与高可用)
9. [分库分表](#9-分库分表)
10. [MySQL 面试题精选](#10-mysql-面试题精选)

---

## 1. 基础速览（一笔带过）

已学过，简单列举备忘。

```sql
-- DDL
CREATE TABLE / ALTER TABLE / DROP TABLE / TRUNCATE TABLE

-- DML
INSERT INTO / UPDATE ... SET / DELETE FROM

-- DQL
SELECT ... FROM ... WHERE ... GROUP BY ... HAVING ... ORDER BY ... LIMIT

-- 连接
INNER JOIN / LEFT JOIN / RIGHT JOIN / CROSS JOIN

-- 常用函数
COUNT / SUM / AVG / MAX / MIN
IFNULL / COALESCE / CASE WHEN
DATE_FORMAT / NOW / TIMESTAMPDIFF
CONCAT / SUBSTRING / LENGTH
```

**数据类型选择原则**（这个很多人忽略，面试常问）：

| 场景 | 推荐类型 | 原因 |
|------|----------|------|
| 整数 ID | `BIGINT UNSIGNED` | 自增 ID 不用负数，范围更大 |
| 金额 | `DECIMAL(10,2)` | 浮点数有精度损失，财务必用 DECIMAL |
| 短字符串（固定长） | `CHAR(N)` | 定长，查询更快 |
| 可变字符串 | `VARCHAR(N)` | 按实际长度存储 |
| 大文本 | `TEXT` | 不要放在主表，影响行扫描性能 |
| 时间 | `DATETIME` | 存完整日期时间，不受时区影响 |
| 布尔值 | `TINYINT(1)` | MySQL 没有真正的 BOOL 类型 |
| IP 地址 | `INT UNSIGNED` | 用 `INET_ATON/INET_NTOA` 转换，比字符串省空间快4倍 |

---

## 2. 存储引擎深度解析

### 2.1 InnoDB vs MyISAM

面试必考，不要只背结论，要理解**为什么**。

| 对比项 | InnoDB | MyISAM |
|--------|--------|--------|
| 事务 | ✅ 支持 | ❌ 不支持 |
| 外键 | ✅ 支持 | ❌ 不支持 |
| 行级锁 | ✅ 支持 | ❌ 只有表锁 |
| 崩溃恢复 | ✅ redo log 保证 | ❌ 可能损坏 |
| 全文索引 | ✅ MySQL 5.6+ | ✅ 支持 |
| 表存储 | `.ibd` 文件 | `.MYD + .MYI` 文件 |
| 主键索引 | 聚簇索引 | 非聚簇索引 |
| `COUNT(*)` | 慢（全表扫） | 快（存了行数） |
| 适用场景 | **99% 的业务场景** | 只读、无事务需求 |

> **结论**：现代业务一律用 InnoDB，MyISAM 几乎是历史遗留。

### 2.2 InnoDB 的内存结构

理解这个，才能看懂后面的索引、事务、优化。

```
┌─────────────────────────────────────────────────────┐
│                  InnoDB Buffer Pool                  │
│  ┌─────────────┐  ┌──────────┐  ┌───────────────┐  │
│  │  数据页      │  │  索引页  │  │  Change Buffer│  │
│  │ (16KB/页)   │  │          │  │  (写缓冲)     │  │
│  └─────────────┘  └──────────┘  └───────────────┘  │
│  ┌─────────────┐  ┌──────────┐                      │
│  │  Undo Log   │  │  自适应  │                      │
│  │  页          │  │  哈希索引 │                      │
│  └─────────────┘  └──────────┘                      │
└─────────────────────────────────────────────────────┘
         ↕ 脏页刷盘（后台线程）
┌─────────────────────────────────────────────────────┐
│                     磁盘文件                         │
│   .ibd（数据+索引）    redo log     undo log         │
└─────────────────────────────────────────────────────┘
```

**Buffer Pool** 是 InnoDB 性能的核心，生产环境建议设置为物理内存的 **60%~80%**：

```ini
# my.cnf
innodb_buffer_pool_size = 8G   # 16G内存的机器设8G
innodb_buffer_pool_instances = 8  # 多实例减少锁竞争
```

**数据页（Page）**：InnoDB 读写的最小单位是 **16KB 的页**，不是一行一行读的。这是理解索引的基础。

---

## 3. 索引原理与实战

> 索引是 MySQL 优化的核心，面试最高频的考点。必须从底层理解，而不只是会写 `CREATE INDEX`。

### 3.1 B+ 树结构详解

InnoDB 的索引底层是 **B+ 树**，理解它是理解一切索引问题的前提。

```
                    [30 | 60]              ← 非叶子节点（只存键值，不存数据）
                   /    |    \
           [10|20]   [40|50]   [70|80]
          /  |  \   /  |  \   /  |  \
         叶  叶  叶 叶  叶  叶 叶  叶  叶  ← 叶子节点（存完整数据行）
         ↔   ↔   ↔  ↔   ↔   ↔  ↔   ↔   ↔  ← 双向链表连接
```

**B+ 树的关键特性**：
- 非叶子节点只存键值，不存数据，所以**一个节点（16KB）能存更多键值**，树更矮（通常3层即可存亿级数据）
- 所有数据都在**叶子节点**，且叶子节点通过**双向链表**相连，范围查询极高效
- 树高 = IO 次数。一棵高度为3的B+树，最多需要 **3次磁盘IO** 就能找到任意一条数据

**为什么是3层能存亿级数据？**

```
假设：
  - 非叶子节点的键值大小 = 8B (bigint) + 6B (指针) = 14B
  - 每个节点16KB = 16384B
  - 每个叶子节点存16行数据（每行1KB）

第1层（根节点）：1个节点，可以有 16384/14 ≈ 1170 个子节点指针
第2层：1170 个节点
第3层（叶子层）：1170 × 1170 = 136.89万 个叶子节点
存储行数：136.89万 × 16 ≈ 2190万 行

高度为4时可达约 256亿行，完全够用。
```

---

### 3.2 聚簇索引 vs 非聚簇索引

这是 InnoDB 索引体系最核心的概念，必须彻底理解。

**聚簇索引（主键索引）**：

```
主键 B+ 树的叶子节点直接存储完整的数据行

叶子节点：
┌──────┬──────────┬─────┬──────────────────────────┐
│ id=1 │ name=Alice│age=25│ ... 所有列的完整数据 ... │
├──────┼──────────┼─────┼──────────────────────────┤
│ id=2 │ name=Bob │age=30│ ... 所有列的完整数据 ... │
└──────┴──────────┴─────┴──────────────────────────┘
```

**非聚簇索引（二级索引）**：

```
name 字段的 B+ 树叶子节点存的是：name值 + 主键ID

叶子节点：
┌───────────┬────────┐
│ name=Alice│ id=1   │  ← 只存name和主键，不存完整数据
├───────────┼────────┤
│ name=Bob  │ id=2   │
└───────────┴────────┘
      │ 找到主键后
      ↓ 再去聚簇索引查完整数据（回表）
```

**回表（Table Lookup）**：通过二级索引找到主键，再去主键索引找完整数据的过程。每次回表 = 一次额外 B+ 树查找，代价较高。这就是为什么要有**覆盖索引**的原因。

---

### 3.3 索引类型

```sql
-- 普通索引
CREATE INDEX idx_name ON users(name);

-- 唯一索引（额外保证唯一性约束）
CREATE UNIQUE INDEX idx_email ON users(email);

-- 主键索引（特殊的唯一聚簇索引）
PRIMARY KEY (id)

-- 联合索引（复合索引）
CREATE INDEX idx_name_age ON users(name, age);

-- 前缀索引（对长字符串取前N个字符建索引）
CREATE INDEX idx_title ON articles(title(20));

-- 全文索引
CREATE FULLTEXT INDEX idx_content ON articles(content);
-- 使用全文索引（不能用LIKE）
SELECT * FROM articles WHERE MATCH(content) AGAINST('redis');
```

---

### 3.4 联合索引与最左前缀原则

**联合索引 `(a, b, c)` 的存储结构**：先按 a 排序，a 相同再按 b，b 相同再按 c。

```
联合索引 (name, age, city) 的叶子节点：
Alice  25  Beijing  → id=1
Alice  28  Shanghai → id=5
Bob    22  Guangzhou→ id=3
Bob    30  Beijing  → id=2
...
```

**最左前缀原则**：查询条件必须从索引的最左列开始，不能跳过。

```sql
-- 联合索引 (name, age, city)

-- ✅ 用到索引 (name)
SELECT * FROM users WHERE name = 'Alice';

-- ✅ 用到索引 (name, age)
SELECT * FROM users WHERE name = 'Alice' AND age = 25;

-- ✅ 用到索引 (name, age, city)
SELECT * FROM users WHERE name = 'Alice' AND age = 25 AND city = 'Beijing';

-- ✅ 顺序无关，优化器会调整
SELECT * FROM users WHERE age = 25 AND name = 'Alice';

-- ❌ 跳过了 name，无法用索引
SELECT * FROM users WHERE age = 25;

-- ❌ 跳过了 age，只能用到 name 部分
SELECT * FROM users WHERE name = 'Alice' AND city = 'Beijing';

-- ⚠️ 范围查询后的字段无法用索引
-- name用索引，age用索引，但 city 无法用索引（age是范围查询，断了）
SELECT * FROM users WHERE name = 'Alice' AND age > 25 AND city = 'Beijing';
```

**索引下推（ICP，Index Condition Pushdown）**：MySQL 5.6+ 的优化，即使联合索引后面的字段无法用于范围缩小，也能在索引层面过滤，减少回表次数。

---

### 3.5 覆盖索引（重要！）

**定义**：查询所需的所有列都在索引中，不需要回表。

```sql
-- 表结构
CREATE TABLE users (
    id BIGINT PRIMARY KEY,
    name VARCHAR(50),
    age INT,
    email VARCHAR(100),
    INDEX idx_name_age (name, age)
);

-- ✅ 覆盖索引：SELECT的列(name,age)都在联合索引里，无需回表
SELECT name, age FROM users WHERE name = 'Alice';

-- ❌ 非覆盖索引：email不在索引里，需要回表
SELECT name, age, email FROM users WHERE name = 'Alice';

-- EXPLAIN 验证：覆盖索引时 Extra 列显示 "Using index"
EXPLAIN SELECT name, age FROM users WHERE name = 'Alice';
```

**业务中如何利用覆盖索引**：
- 高频查询只 SELECT 需要的字段，避免 `SELECT *`
- 根据查询字段设计联合索引，将查询字段全部包含进索引

---

### 3.6 索引失效场景（面试必考）

```sql
-- 假设 name 字段有索引

-- ❌ 1. 对索引列使用函数
WHERE YEAR(create_time) = 2024          -- 失效
WHERE create_time >= '2024-01-01'       -- ✅ 改成范围

-- ❌ 2. 对索引列进行运算
WHERE age + 1 = 26                      -- 失效
WHERE age = 25                          -- ✅

-- ❌ 3. 隐式类型转换（字符串索引传入数字）
-- name 是 VARCHAR，但传入数字，MySQL会做类型转换
WHERE name = 123                        -- 失效（name被转换了）
WHERE name = '123'                      -- ✅

-- ❌ 4. LIKE 左模糊或全模糊
WHERE name LIKE '%Alice%'               -- 失效
WHERE name LIKE '%Alice'                -- 失效
WHERE name LIKE 'Alice%'               -- ✅ 右模糊可以用索引

-- ❌ 5. OR 连接时，OR 两侧都必须有索引，否则全表扫描
WHERE name = 'Alice' OR email = 'a@b.com'  -- 如果email无索引则失效

-- ❌ 6. NOT IN / NOT EXISTS / != / <>
WHERE age != 25                         -- 可能失效（取决于选择性）

-- ❌ 7. IS NULL / IS NOT NULL（取决于NULL值的占比）
WHERE name IS NULL

-- ❌ 8. 联合索引违反最左前缀
WHERE age = 25                          -- 联合索引(name,age)，跳过name，失效
```

---

### 3.7 索引设计原则

**该建索引的情况**：
- 频繁出现在 `WHERE`、`JOIN ON`、`ORDER BY`、`GROUP BY` 的字段
- 区分度高的字段（`SELECT COUNT(DISTINCT col)/COUNT(*)` 越接近1越好）
- 外键字段

**不该建索引的情况**：
- 数据量小的表（<1万行，全表扫比走索引更快）
- 频繁更新的字段（维护索引有开销）
- 区分度低的字段（性别、状态等，走索引还不如全表扫）

**索引数量控制**：单表不超过 **5~6个**，写多读少的表更要克制。

---

## 4. SQL 优化（核心重点）

> 这是整个手册最重要的章节。生产中 90% 的性能问题都能通过 SQL 优化解决。

### 4.1 EXPLAIN 详解

优化 SQL 的第一步永远是 `EXPLAIN`，看懂它是核心技能。

```sql
EXPLAIN SELECT u.name, o.amount
FROM users u
JOIN orders o ON u.id = o.user_id
WHERE u.name = 'Alice'
AND o.amount > 100;
```

**EXPLAIN 输出字段详解**：

| 字段 | 说明 | 关注点 |
|------|------|--------|
| `id` | 查询的序列号，id 越大越先执行 | 子查询嵌套层数 |
| `select_type` | 查询类型 | SIMPLE/PRIMARY/SUBQUERY/DERIVED |
| `table` | 当前行分析的是哪张表 | |
| `type` | **访问类型（最重要！）** | 见下表 |
| `possible_keys` | 可能用到的索引 | |
| `key` | **实际用到的索引** | NULL=全表扫描 |
| `key_len` | 索引使用的字节数 | 判断联合索引用了几列 |
| `ref` | 索引等值匹配的列或常量 | |
| `rows` | **预估扫描行数** | 越小越好 |
| `filtered` | 表示过滤后剩余行的百分比 | 越大越好 |
| `Extra` | 额外信息 | 见下表 |

**`type` 访问类型（性能从好到差）**：

```
system > const > eq_ref > ref > range > index > ALL

system：只有一行（系统表）
const：主键或唯一索引等值查询，最多匹配一行
eq_ref：JOIN时使用主键/唯一索引，每次最多一行
ref：普通索引等值查询，可能多行
range：索引范围查询（BETWEEN / > / < / IN）
index：全索引扫描（比ALL快，但仍是全扫）
ALL：全表扫描 ← 生产中必须避免！
```

> **优化目标**：`type` 至少达到 `range`，最好是 `ref` 以上；`key` 不为 NULL；`rows` 尽量小。

**`Extra` 常见值解读**：

| Extra 值 | 含义 | 好/坏 |
|----------|------|-------|
| `Using index` | 覆盖索引，无需回表 | ✅ 很好 |
| `Using where` | 在存储引擎取出数据后，Server层再过滤 | 中性 |
| `Using index condition` | 索引下推（ICP） | ✅ 好 |
| `Using filesort` | 文件排序，无法用索引排序 | ❌ 需优化 |
| `Using temporary` | 用了临时表（GROUP BY / DISTINCT） | ❌ 需优化 |
| `Using join buffer` | JOIN 时没用到索引，用了缓冲区 | ❌ 需优化 |

---

### 4.2 慢查询日志

定位慢 SQL 的第一步，生产环境必须开启。

```ini
# my.cnf
slow_query_log = ON
slow_query_log_file = /var/log/mysql/slow.log
long_query_time = 2          # 超过2秒记录（生产建议1秒）
log_queries_not_using_indexes = ON  # 未使用索引的查询也记录
```

```sql
-- 运行时开启（不重启）
SET GLOBAL slow_query_log = ON;
SET GLOBAL long_query_time = 1;

-- 查看慢查询状态
SHOW VARIABLES LIKE 'slow_query%';
SHOW VARIABLES LIKE 'long_query_time';
```

**使用 mysqldumpslow 分析慢日志**：

```bash
# 按查询时间排序，显示前10
mysqldumpslow -s t -t 10 /var/log/mysql/slow.log

# 按出现次数排序
mysqldumpslow -s c -t 10 /var/log/mysql/slow.log

# 找含有LEFT JOIN的慢查询
mysqldumpslow -g "LEFT JOIN" /var/log/mysql/slow.log
```

---

### 4.3 索引优化实战

#### 案例一：联合索引字段顺序

```sql
-- 需求：查询 status=1 AND type=2 的用户，按 create_time 排序
SELECT * FROM orders WHERE status = 1 AND type = 2 ORDER BY create_time;

-- ❌ 错误索引设计
CREATE INDEX idx_bad ON orders(create_time, status, type);
-- 原因：先按create_time排序，status和type的范围查询无法高效过滤

-- ✅ 正确索引设计：等值查询字段在前，范围/排序字段在后
CREATE INDEX idx_good ON orders(status, type, create_time);
-- status等值，type等值，create_time排序 → 索引完全命中，无filesort
```

#### 案例二：ORDER BY 优化

```sql
-- 表有联合索引 (status, create_time)

-- ✅ 索引排序（无 filesort）
SELECT * FROM orders WHERE status = 1 ORDER BY create_time ASC;

-- ❌ 产生 filesort（排序方向不一致）
SELECT * FROM orders WHERE status = 1 ORDER BY create_time DESC;
-- 解决：将索引改为 (status, create_time DESC)  MySQL 8.0 支持

-- ❌ 产生 filesort（ORDER BY 字段不在索引中）
SELECT * FROM orders WHERE status = 1 ORDER BY amount;

-- ❌ 产生 filesort（多字段排序方向不一致）
SELECT * FROM orders ORDER BY status ASC, create_time DESC;
```

**filesort 的两种模式**：
- **单路排序**：将所有需要的列都加载到 sort buffer，直接排序，内存换 IO
- **双路排序**：只取排序字段+主键到 sort buffer 排序，再回表取其他列

> 通过增大 `sort_buffer_size` 可以减少磁盘排序，但根本解决方案是让排序走索引。

#### 案例三：GROUP BY 优化

```sql
-- GROUP BY 默认会排序，可能产生 filesort + temporary

-- ❌ 会产生 Using temporary; Using filesort
SELECT status, COUNT(*) FROM orders GROUP BY status;

-- ✅ 有 status 索引时，避免排序
SELECT status, COUNT(*) FROM orders GROUP BY status ORDER BY NULL;

-- ✅ 更好：覆盖索引
CREATE INDEX idx_status ON orders(status);
SELECT status, COUNT(*) FROM orders GROUP BY status;  -- Using index
```

---

### 4.4 JOIN 优化

#### 驱动表与被驱动表

```
Nested Loop Join（嵌套循环连接）：

for each row in 驱动表（小表）:
    for each row in 被驱动表（大表）:
        if 满足 ON 条件: output
```

**核心原则**：小表驱动大表，被驱动表的连接字段必须有索引。

```sql
-- 假设 users 1万行，orders 100万行

-- ✅ 小表驱动大表（MySQL优化器通常会自动处理）
SELECT * FROM users u JOIN orders o ON u.id = o.user_id WHERE u.vip = 1;
-- MySQL会选users作为驱动表（因为有WHERE过滤，结果集更小）

-- ✅ 被驱动表的 join 字段必须有索引
-- orders.user_id 必须有索引，否则每次都全表扫描 orders
CREATE INDEX idx_user_id ON orders(user_id);
```

**JOIN Buffer**：当被驱动表无索引时，MySQL 使用 `join_buffer_size` 缓存驱动表数据做批量匹配。增大此值可减少 IO，但不如加索引根本。

```ini
join_buffer_size = 4M
```

#### 多表 JOIN 的优化策略

```sql
-- ❌ 超过3张表的 JOIN 慎用，性能差且难维护
SELECT a.*, b.name, c.amount, d.status
FROM a JOIN b ON ...
JOIN c ON ...
JOIN d ON ...

-- ✅ 方案一：拆分为多次查询，在应用层合并
-- ✅ 方案二：冗余字段（反范式化），减少 JOIN
-- ✅ 方案三：建立中间宽表（数仓思路）
```

---

### 4.5 子查询优化

子查询（Subquery）通常比 JOIN 慢，因为子查询可能产生临时表。

```sql
-- ❌ 相关子查询（对外表每一行都执行一次子查询）
SELECT * FROM orders o
WHERE o.amount > (
    SELECT AVG(amount) FROM orders WHERE user_id = o.user_id
);

-- ✅ 改为 JOIN
SELECT o.*
FROM orders o
JOIN (
    SELECT user_id, AVG(amount) AS avg_amount
    FROM orders GROUP BY user_id
) t ON o.user_id = t.user_id
WHERE o.amount > t.avg_amount;
```

```sql
-- ❌ IN 子查询（子查询结果集大时很慢）
SELECT * FROM users WHERE id IN (
    SELECT user_id FROM orders WHERE amount > 1000
);

-- ✅ 改为 EXISTS（更高效，找到即停止）
SELECT * FROM users u WHERE EXISTS (
    SELECT 1 FROM orders o
    WHERE o.user_id = u.id AND o.amount > 1000
);

-- ✅ 或改为 JOIN（通常最快）
SELECT DISTINCT u.* FROM users u
JOIN orders o ON u.id = o.user_id
WHERE o.amount > 1000;
```

---

### 4.6 分页查询优化（深分页问题）

这是一个被严重低估的性能问题。

```sql
-- ❌ 深分页：LIMIT 1000000, 10
-- 需要扫描并丢弃 100万条记录，只返回10条，极其低效！
SELECT * FROM orders ORDER BY id LIMIT 1000000, 10;

-- ✅ 方案一：游标分页（记住上次的最大ID）
-- 前端记住上次返回的最后一条ID
SELECT * FROM orders WHERE id > 1000000 ORDER BY id LIMIT 10;
-- 利用主键索引，直接从 id=1000000 开始，无需扫描之前的数据

-- ✅ 方案二：子查询先定位
SELECT * FROM orders o
JOIN (
    SELECT id FROM orders ORDER BY id LIMIT 1000000, 10
) t ON o.id = t.id;
-- 子查询走覆盖索引（只查id），不回表，速度快；再用id join回去取完整数据

-- ✅ 方案三：业务层限制翻页深度（最实用）
-- 禁止跳页，只能点下一页；最大翻页深度限制在100页
```

**性能对比**（1000万条数据）：

| 方案 | 耗时 |
|------|------|
| `LIMIT 1000000, 10` | ~3秒 |
| 游标分页（id > 上次最大ID） | ~1ms |
| 子查询定位 | ~100ms |

---

### 4.7 COUNT 优化

```sql
-- COUNT(*) vs COUNT(1) vs COUNT(主键) vs COUNT(普通列)

-- MySQL 8.0 的官方说法：COUNT(*) 和 COUNT(1) 效果等同，推荐 COUNT(*)
-- 区别在于：
-- COUNT(*) 统计所有行（包括NULL）
-- COUNT(列名) 不统计该列为NULL的行

-- ❌ 低效
SELECT COUNT(*) FROM orders;  -- 大表慎用，全表扫

-- ✅ 有 WHERE 条件时，确保 WHERE 字段有索引
SELECT COUNT(*) FROM orders WHERE status = 1;  -- status有索引时走索引

-- ✅ 用 Redis 维护计数器（最佳方案）
-- 对于需要实时总数的场景，写入时 INCR redis_counter
-- 查询直接读 Redis，不查 MySQL
```

**MyISAM 的 COUNT 为什么快？**

MyISAM 存储了表的行数（元数据），`COUNT(*)` 直接读元数据返回，O(1)。但 InnoDB 要扫描索引（最短路径是扫主键/最小的索引）。

---

### 4.8 写操作优化

```sql
-- ✅ 批量插入替代循环单条插入
-- ❌ 慢（1000次网络往返）
for item in items:
    INSERT INTO orders VALUES (...)

-- ✅ 快（1次往返）
INSERT INTO orders VALUES (...), (...), (...), ...;  -- 一次最多1000条

-- ✅ 使用 LOAD DATA INFILE 导入大量数据（比INSERT快10-20倍）
LOAD DATA INFILE '/tmp/orders.csv'
INTO TABLE orders
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n';

-- ✅ 更新时精确定位，避免全表更新
UPDATE orders SET status = 2 WHERE id = 1001;  -- 走主键索引
-- ❌ 避免
UPDATE orders SET status = 2;  -- 全表！
```

---

### 4.9 表设计层面的优化

这些是从源头优化，比事后加索引更重要。

**1. 选择合适的数据类型**

```sql
-- ❌ 过度设计
name VARCHAR(1000)     -- 用不到1000，浪费
id VARCHAR(36)         -- 用UUID字符串当主键（见下条）

-- ✅ 合适即可
name VARCHAR(50)
id BIGINT AUTO_INCREMENT  -- 整数主键，索引效率远高于UUID
```

**为什么不推荐 UUID 做主键？**

UUID 是随机字符串，新插入的数据在 B+ 树中位置随机，会频繁导致**页分裂**（Page Split），产生大量碎片，性能下降严重。而自增整数永远在树的最右边追加，没有页分裂问题。

**2. 垂直分表（大列拆分）**

```sql
-- ❌ 把大字段和频繁查询字段放一起
CREATE TABLE articles (
    id BIGINT PRIMARY KEY,
    title VARCHAR(200),        -- 频繁查询
    author VARCHAR(50),        -- 频繁查询
    content LONGTEXT           -- 很少查询，但很大
);

-- ✅ 垂直拆分
CREATE TABLE articles (
    id BIGINT PRIMARY KEY,
    title VARCHAR(200),
    author VARCHAR(50)
);

CREATE TABLE article_content (
    article_id BIGINT PRIMARY KEY,
    content LONGTEXT
);
-- 列表页只查 articles，详情页才 JOIN article_content
-- 行数据变小，一个数据页能放更多行，Buffer Pool 命中率更高
```

---

### 4.10 SQL 优化总结清单

```
写 SQL 时的自检清单：

☑ 是否用了 SELECT *？换成只查需要的字段
☑ WHERE 条件的字段是否有索引？
☑ 索引列是否被函数/运算/隐式转换包裹（会失效）？
☑ LIKE 是否以 % 开头（会失效）？
☑ JOIN 的连接字段是否都有索引？
☑ 子查询是否可以改为 JOIN 或 EXISTS？
☑ 分页是否有深分页问题？
☑ ORDER BY 是否能走索引，还是产生了 filesort？
☑ GROUP BY 是否产生了 temporary table？
☑ 大批量写入是否用了批量 INSERT？
☑ 用 EXPLAIN 验证执行计划了吗？
```

---

## 5. 事务与 MVCC

### 5.1 事务四大特性（ACID）

| 特性 | 说明 | InnoDB 如何实现 |
|------|------|----------------|
| **原子性（A）** | 要么全成功，要么全失败 | **undo log**（回滚日志） |
| **一致性（C）** | 事务前后数据逻辑一致 | 由其他三个特性共同保证 |
| **隔离性（I）** | 事务间互不干扰 | **MVCC + 锁** |
| **持久性（D）** | 提交后数据永久保存 | **redo log**（重做日志） |

### 5.2 四种隔离级别与并发问题

```sql
-- 查看/设置隔离级别
SELECT @@transaction_isolation;
SET SESSION TRANSACTION ISOLATION LEVEL READ COMMITTED;
```

| 隔离级别 | 脏读 | 不可重复读 | 幻读 | 性能 |
|----------|------|-----------|------|------|
| `READ UNCOMMITTED` | ❌可能 | ❌可能 | ❌可能 | 最高 |
| `READ COMMITTED` | ✅解决 | ❌可能 | ❌可能 | 高 |
| **`REPEATABLE READ`** | ✅解决 | ✅解决 | ⚠️基本解决 | 中（**InnoDB默认**） |
| `SERIALIZABLE` | ✅解决 | ✅解决 | ✅解决 | 最低 |

**三种并发问题的定义**：

- **脏读**：读到其他事务**未提交**的数据
- **不可重复读**：同一事务内，两次读同一行数据**结果不同**（被其他事务修改并提交了）
- **幻读**：同一事务内，两次查询**行数不同**（其他事务插入/删除了数据）

---

### 5.3 MVCC（多版本并发控制）详解

MVCC 是 InnoDB 实现高并发的核心机制，让读操作不加锁，极大提升并发性能。

**实现 MVCC 的三个关键组件**：

**1. 隐藏列**

InnoDB 每行数据有两个隐藏列：
- `trx_id`：最近一次修改该行的事务 ID
- `roll_pointer`：指向 undo log 中上一个版本的指针

**2. Undo Log（版本链）**

每次修改一行数据，旧版本都会写入 undo log，通过 `roll_pointer` 串成一条版本链：

```
当前行：trx_id=100, name="Bob"
    │ roll_pointer
    ↓
undo: trx_id=80,  name="Alice"
    │ roll_pointer
    ↓
undo: trx_id=50,  name="Tom"
    │
   NULL（最初版本）
```

**3. ReadView（读视图）**

事务开始读数据时，生成一个 ReadView，包含：
- `m_ids`：当前活跃（未提交）的事务 ID 列表
- `min_trx_id`：`m_ids` 中的最小值
- `max_trx_id`：下一个将分配的事务 ID
- `creator_trx_id`：创建此 ReadView 的事务 ID

**可见性判断规则**：

```
对于版本链中的某个版本，其 trx_id 为 X，判断当前事务是否可见：

1. X == creator_trx_id → 可见（自己修改的）
2. X < min_trx_id → 可见（在ReadView创建前已提交）
3. X >= max_trx_id → 不可见（在ReadView创建后才开始的事务）
4. min_trx_id <= X < max_trx_id：
   - X 在 m_ids 中 → 不可见（该事务还未提交）
   - X 不在 m_ids 中 → 可见（该事务已提交）

沿着版本链向下找，直到找到可见的版本为止。
```

**RC 和 RR 的区别就在于 ReadView 的创建时机**：
- `READ COMMITTED`：**每次 SELECT 都创建新的 ReadView**（所以能读到其他事务最新提交）
- `REPEATABLE READ`：**整个事务只在第一次 SELECT 时创建 ReadView**（所以多次读结果一致）

---

### 5.4 InnoDB 如何解决幻读

RR 级别下，InnoDB 通过 **MVCC + Gap Lock（间隙锁）** 解决幻读：

- **快照读**（普通 SELECT）：通过 MVCC 的 ReadView 机制，看不到其他事务新插入的行，无幻读
- **当前读**（SELECT FOR UPDATE / INSERT / UPDATE / DELETE）：通过 **间隙锁** 锁住范围，防止其他事务插入

```sql
-- 事务A执行，会加间隙锁锁住 age IN (25, 30) 之间的间隙
SELECT * FROM users WHERE age > 25 FOR UPDATE;

-- 事务B此时插入 age=27 的行 → 会被阻塞！
INSERT INTO users (age) VALUES (27);  -- 阻塞
```

---

## 6. 锁机制详解

### 6.1 锁的分类

```
按粒度：
  全局锁（FTWRL）
  表级锁（表锁、元数据锁MDL）
  行级锁（记录锁、间隙锁、临键锁）

按模式：
  共享锁（S锁，读锁）：SELECT ... LOCK IN SHARE MODE
  排他锁（X锁，写锁）：SELECT ... FOR UPDATE / INSERT / UPDATE / DELETE
```

**兼容性矩阵**：

| | S锁 | X锁 |
|--|-----|-----|
| **S锁** | ✅ 兼容 | ❌ 冲突 |
| **X锁** | ❌ 冲突 | ❌ 冲突 |

---

### 6.2 行级锁的三种形式

**1. Record Lock（记录锁）**：锁住某一条具体记录

```sql
SELECT * FROM users WHERE id = 1 FOR UPDATE;  -- 锁住id=1这一行
```

**2. Gap Lock（间隙锁）**：锁住两条记录之间的间隙，防止插入

```
假设 id 列有值：1, 5, 10, 20

SELECT * FROM users WHERE id BETWEEN 5 AND 10 FOR UPDATE;
会加间隙锁：锁住 (1,5) 和 (10,20) 之间的间隙
其他事务无法插入 id=3, 7, 15 等值
```

**3. Next-Key Lock（临键锁）**：Record Lock + Gap Lock 的组合，是 InnoDB **行锁的默认形式**

```
临键锁锁住的是一个左开右闭区间，如 (5, 10]
锁住记录10本身，同时锁住(5,10)的间隙
```

---

### 6.3 死锁

**死锁示例**：

```
事务A：
  1. UPDATE orders SET status=1 WHERE id=1;  → 锁住id=1
  2. UPDATE orders SET status=1 WHERE id=2;  → 等待id=2的锁

事务B：
  1. UPDATE orders SET status=1 WHERE id=2;  → 锁住id=2
  2. UPDATE orders SET status=1 WHERE id=1;  → 等待id=1的锁

→ 互相等待，死锁！
```

**InnoDB 死锁处理**：检测到死锁后，自动选择"代价最小"的事务回滚（通常是undo log更少的）。

**避免死锁的策略**：
- 多个事务按**相同顺序**访问资源（破坏循环等待条件）
- 事务尽量短小，减少锁的持有时间
- 避免大事务
- 用 `SHOW ENGINE INNODB STATUS` 分析死锁原因

```sql
-- 查看最近一次死锁信息
SHOW ENGINE INNODB STATUS\G
-- 找 LATEST DETECTED DEADLOCK 部分
```

---

### 6.4 锁等待与分析

```sql
-- 查看当前正在等待锁的事务
SELECT * FROM information_schema.INNODB_TRX;

-- 查看锁等待关系（MySQL 8.0+）
SELECT * FROM performance_schema.data_lock_waits;

-- 查看当前持有的锁
SELECT * FROM performance_schema.data_locks;

-- 查看锁等待超时时间（默认50秒）
SHOW VARIABLES LIKE 'innodb_lock_wait_timeout';
SET innodb_lock_wait_timeout = 10;  -- 改为10秒
```

---

## 7. 日志系统

### 7.1 三大核心日志

| 日志 | 所属层 | 作用 |
|------|--------|------|
| **undo log** | InnoDB 引擎层 | 事务回滚 + MVCC |
| **redo log** | InnoDB 引擎层 | 崩溃恢复，保证持久性 |
| **binlog** | MySQL Server 层 | 主从复制 + 数据恢复 |

---

### 7.2 redo log（WAL 机制）

**为什么需要 redo log？**

直接修改数据时，不能每次都立刻把数据页刷到磁盘（随机IO，太慢）。而是先写 redo log（顺序IO，极快），数据页延迟刷盘。即使崩溃，也能通过 redo log 重放恢复。

这就是 **WAL（Write-Ahead Log，预写日志）** 机制。

```
写操作流程：
1. 修改 Buffer Pool 中的数据页（内存操作）
2. 将修改记录写入 redo log buffer（内存）
3. 根据 innodb_flush_log_at_trx_commit 策略刷盘
4. 后台线程异步将脏页刷到磁盘（checkpoint）
```

**`innodb_flush_log_at_trx_commit` 三个值**：

| 值 | 行为 | 性能 | 安全性 |
|----|------|------|--------|
| `0` | 每秒刷盘，提交时不刷 | 最高 | 最低（崩溃丢1秒数据） |
| `1` | 每次提交都刷盘 | 最低 | 最高（**默认，推荐**） |
| `2` | 提交时写OS缓存，每秒fsync | 高 | 中（OS崩溃可能丢数据） |

---

### 7.3 binlog

binlog 记录的是所有 DDL 和 DML 操作，用于主从复制和数据恢复。

**三种格式**：

| 格式 | 记录内容 | 优点 | 缺点 |
|------|----------|------|------|
| `STATEMENT` | SQL 语句原文 | 日志量少 | 某些函数（NOW()）主从不一致 |
| `ROW` | 每行数据的变化 | 精确，无歧义 | 日志量大 |
| `MIXED` | 混合模式 | 折中 | 复杂 |

**生产推荐** `ROW` 格式，精确可靠。

```sql
SHOW VARIABLES LIKE 'binlog_format';
SHOW BINARY LOGS;          -- 查看所有binlog文件
SHOW BINLOG EVENTS IN 'mysql-bin.000001';  -- 查看binlog内容
```

---

### 7.4 两阶段提交（保证 redo log 和 binlog 一致性）

```
事务提交流程（两阶段提交）：

阶段一（prepare）：
  写入 redo log，标记为 prepare 状态

阶段二（commit）：
  写入 binlog
  ↓
  redo log 标记为 commit 状态

崩溃恢复：
  - redo log prepare + binlog 完整 → 提交事务
  - redo log prepare + binlog 不完整 → 回滚事务
  - redo log commit → 提交事务
```

---

## 8. 主从复制与高可用

### 8.1 主从复制原理

```
主库（Master）                         从库（Slave）
    │                                      │
    │ 写操作                                │
    ↓                                      │
  binlog ──→ binlog dump thread ──→ IO Thread ──→ relay log
                                                       │
                                                    SQL Thread
                                                       │
                                                    执行SQL
                                                       ↓
                                                    从库数据
```

**配置主从复制**：

```sql
-- 主库配置（my.cnf）
server-id = 1
log-bin = mysql-bin
binlog_format = ROW
sync_binlog = 1  -- 每次提交都刷盘binlog

-- 从库配置（my.cnf）
server-id = 2
relay-log = relay-bin
read_only = ON  -- 从库只读

-- 主库创建复制账号
CREATE USER 'repl'@'%' IDENTIFIED BY 'password';
GRANT REPLICATION SLAVE ON *.* TO 'repl'@'%';

-- 从库配置主库信息
CHANGE MASTER TO
    MASTER_HOST='192.168.1.100',
    MASTER_PORT=3306,
    MASTER_USER='repl',
    MASTER_PASSWORD='password',
    MASTER_AUTO_POSITION=1;  -- GTID模式（推荐）

START SLAVE;
SHOW SLAVE STATUS\G  -- 查看复制状态
```

**主从延迟的原因和解决**：

| 原因 | 解决方案 |
|------|---------|
| 从库 SQL Thread 单线程 | 开启并行复制（MySQL 5.7+）`slave_parallel_workers=4` |
| 主库大事务（如一次更新100万行） | 拆分大事务为小事务 |
| 网络延迟 | 优化网络，主从同机房 |
| 从库负载高 | 单独分离只读从库，专库专用 |

---

### 8.2 常见高可用架构

**1. 一主一从（基础）**：主库宕机需手动切换，有停机时间

**2. 双主架构（互为主从）**：两台都可写，通过 `auto_increment_offset` 避免 ID 冲突，但需要处理双写冲突

**3. MHA（Master High Availability）**：自动故障切换，30秒内完成切换，业界常用

**4. InnoDB Cluster（MySQL 8.0 官方方案）**：基于 Paxos 协议，强一致，自动故障切换

**5. 云数据库（RDS）**：在云上直接用 RDS，高可用由云厂商保证，省心

---

## 9. 分库分表

> 当单表超过 **500万行** 或单库超过 **1TB**，需要考虑分库分表。

### 9.1 垂直分割

**垂直分库**：按业务拆分数据库（订单库、用户库、商品库）

```
monolith DB
    ├── users    →  user_db（用户服务）
    ├── orders   →  order_db（订单服务）
    └── products →  product_db（商品服务）
```

**垂直分表**：将大列拆分到独立表（前面已讲过）

---

### 9.2 水平分表

将同一张表的数据按某个规则分散到多张结构相同的表中。

**常见分片规则**：

```sql
-- 按范围分片（Range）
order_2024_01：create_time 在 2024年1月
order_2024_02：create_time 在 2024年2月
-- 优点：便于归档；缺点：数据不均匀，热点在最新分片

-- 按哈希分片（Hash）
分片key = user_id % 4
user_id=1 → table_1
user_id=2 → table_2
user_id=3 → table_3
user_id=4 → table_0
-- 优点：数据均匀；缺点：扩容需要数据迁移

-- 按哈希分片（一致性哈希）
-- 优点：扩容时只需迁移少量数据
```

**分表带来的新问题**：

| 问题 | 说明 | 解决方案 |
|------|------|---------|
| 跨分片查询 | `SELECT * FROM order WHERE status=1` 需查所有分片 | 在分片键上查询；ES做跨分片搜索 |
| 跨分片 JOIN | 无法直接 JOIN | 在应用层做 JOIN；冗余字段 |
| 分布式事务 | 跨库事务 | Seata 等分布式事务框架 |
| 全局唯一 ID | 不能用自增主键 | 雪花算法、Leaf、UUID |
| 分页排序 | 跨分片排序代价大 | 业务上避免深分页；用 ES |

---

### 9.3 分库分表中间件

| 工具 | 特点 | 适用场景 |
|------|------|---------|
| **ShardingSphere** | 功能全面，社区活跃，支持JDBC和Proxy模式 | 生产首选 |
| **MyCat** | 老牌，配置复杂 | 存量项目 |
| **TDDL（阿里）** | 阿里内部，非开源 | 阿里体系 |

---

## 10. MySQL 面试题精选

---

**Q1：MyISAM 和 InnoDB 的区别是什么？**

> InnoDB 支持事务、外键、行级锁，使用聚簇索引，崩溃后可通过 redo log 恢复，适合绝大多数 OLTP 业务。MyISAM 不支持事务和外键，只有表锁，但 `COUNT(*)` 很快（存了行数元数据），适合只读统计场景。生产环境应一律使用 InnoDB。

---

**Q2：什么是聚簇索引？InnoDB 必须有主键吗？**

> 聚簇索引是指索引和数据存储在一起，B+树的叶子节点直接存放完整数据行。InnoDB 每张表有且只有一个聚簇索引。
>
> InnoDB 必须有主键。如果不显式定义，MySQL 会：先找是否有唯一非空索引作为主键，若也没有，则自动生成一个隐藏的 6 字节 `rowid` 作为主键。建议永远显式定义自增整数主键。

---

**Q3：为什么建议用自增整数作为主键，而不是 UUID？**

> UUID 是无序字符串，每次插入的主键在 B+ 树中位置随机，导致频繁的**页分裂**：当某个数据页满了，新数据要插入其中间位置时，必须将该页一分为二，产生大量碎片，写性能下降，索引文件膨胀。
>
> 自增整数永远追加在 B+ 树最右侧，没有页分裂，写入效率高，索引紧凑，且整数比字符串占用空间小，比较速度更快。

---

**Q4：EXPLAIN 中的 type 字段哪些值是需要优化的信号？**

> `ALL`（全表扫描）和 `index`（全索引扫描）是必须优化的信号，说明查询没有有效利用索引。`range` 是可接受的下限，`ref` 以上说明索引使用良好，`const` 和 `eq_ref` 是最优状态。
>
> 看到 Extra 中出现 `Using filesort` 或 `Using temporary` 也需要重点优化。

---

**Q5：什么是覆盖索引？为什么它能提升性能？**

> 覆盖索引是指查询涉及的所有列都包含在某个索引中，查询时无需回表（不需要再去聚簇索引找完整行数据），直接从索引中返回结果。EXPLAIN 中 Extra 显示 `Using index`。
>
> 它能提升性能是因为避免了回表操作，减少了 B+ 树的查找次数，特别是在大数据量下效果显著。设计高频查询时，可以将查询字段和条件字段一起放入联合索引来实现覆盖索引。

---

**Q6：MySQL 默认的事务隔离级别是什么？为什么不是最高级别？**

> MySQL InnoDB 默认是 `REPEATABLE READ（可重复读）`。
>
> 不选 `SERIALIZABLE` 是因为串行化意味着所有事务串行执行，彻底失去并发能力，性能极差。而 RR 级别通过 MVCC 解决了脏读和不可重复读，通过间隙锁基本解决了幻读，在安全性和性能之间取得了很好的平衡，适合大多数 OLTP 业务场景。

---

**Q7：MVCC 是如何实现的？**

> MVCC 通过三个机制实现：**隐藏列**、**undo log 版本链**、**ReadView**。
>
> 每行数据有 `trx_id`（最后修改的事务ID）和 `roll_pointer`（指向旧版本的指针）两个隐藏列。修改数据时，旧版本写入 undo log，通过 roll_pointer 串成版本链。
>
> 读操作时，创建 ReadView 快照，里面记录当前活跃事务列表。对版本链中的每个版本，根据其 trx_id 与 ReadView 的关系判断是否可见，找到最近可见的版本返回。RC 隔离级别每次 SELECT 都创建新 ReadView，RR 只在事务第一次 SELECT 时创建，这就是两者行为差异的根本原因。

---

**Q8：redo log 和 binlog 的区别是什么？为什么需要两阶段提交？**

> redo log 是 InnoDB 引擎层的日志，记录物理页的修改，用于崩溃恢复，保证持久性，大小固定循环写入。binlog 是 Server 层的逻辑日志，记录所有 DDL/DML 操作，用于主从复制和数据恢复，追加写入不限大小。
>
> 需要两阶段提交是为了保证 redo log 和 binlog 的一致性。如果先写 redo log 再写 binlog，redo log 写完后崩溃，binlog 没有这条记录，主从就会不一致。两阶段提交通过 prepare → binlog → commit 的顺序，配合崩溃恢复逻辑，保证二者要么都有要么都没有。

---

**Q9：什么是死锁？InnoDB 如何处理？如何避免？**

> 死锁是多个事务互相等待对方持有的锁，形成循环等待，永远无法继续执行。
>
> InnoDB 通过死锁检测（等待图算法）发现死锁后，自动选择代价最小的事务回滚（undo log 最少的），另一个事务继续执行。
>
> 避免策略：多个事务按相同顺序访问资源，破坏循环等待条件；保持事务短小减少锁持有时间；避免在事务中与用户交互；合理设置超时时间 `innodb_lock_wait_timeout`。

---

**Q10：深分页 LIMIT 1000000,10 为什么慢？如何优化？**

> `LIMIT 1000000,10` 需要扫描并丢弃前 100 万条记录，只返回第 100 万零一到第 100 万零十条，IO 和 CPU 开销巨大。
>
> 优化方案：
> 1. **游标分页**：记住上次最后一条的 id，用 `WHERE id > last_id LIMIT 10`，直接从断点继续，无需扫描历史数据
> 2. **子查询定位**：先用子查询走覆盖索引找到目标 id 范围，再 JOIN 取完整数据
> 3. **业务限制**：限制最大翻页深度，禁止跳页，只允许点下一页

---

**Q11：什么情况下索引会失效？**

> 常见失效场景：对索引列使用函数或运算；隐式类型转换（如 VARCHAR 字段传入数字）；LIKE 左模糊；OR 连接的字段不都有索引；联合索引违反最左前缀原则；区分度低的字段（MySQL 优化器认为全表扫更快会放弃索引）。
>
> 记住一句话：**索引列不能做变换**，凡是让 MySQL 无法直接拿索引值去比较的操作，都可能导致失效。

---

**Q12：count(*) 和 count(1) 和 count(列名) 有什么区别？**

> `COUNT(*)` 和 `COUNT(1)` 统计所有行（包括 NULL），MySQL 优化器对这两者的处理是等价的，性能相同，推荐用 `COUNT(*)`。
>
> `COUNT(列名)` 不统计该列为 NULL 的行，如果列有 NULL 值，结果会比 `COUNT(*)` 小。
>
> 对于 InnoDB，大表的 `COUNT(*)` 都需要扫描全表（或最小的索引），速度慢。生产中对实时总数有需求时，应维护 Redis 计数器。

---

## 📚 学习路线总结

```
第1阶段：查漏补缺（1周）
  ✅ 回顾基础语法，重点：数据类型选择
  ✅ 彻底搞懂 B+树结构
  ✅ 理解聚簇索引 vs 非聚簇索引

第2阶段：索引精通（1周）
  ✅ 联合索引最左前缀 + 索引下推
  ✅ 覆盖索引实战
  ✅ 学会用 EXPLAIN 分析所有查询

第3阶段：SQL 优化实战（2周）
  ✅ 开启慢查询日志，找出项目中的慢SQL
  ✅ 深分页、ORDER BY、GROUP BY 优化
  ✅ JOIN 优化原则
  ✅ 对项目中所有核心SQL做 EXPLAIN 分析

第4阶段：原理深挖（1周）
  ✅ 彻底理解 MVCC + ReadView
  ✅ redo log + binlog + 两阶段提交
  ✅ 行锁、间隙锁、临键锁

第5阶段：架构扩展（持续学习）
  ✅ 主从复制，了解延迟问题
  ✅ 分库分表基本概念
  📖 《高性能MySQL》第4版（必读）
  📖 《MySQL技术内幕：InnoDB存储引擎》（进阶必读）
```

---

> 💡 **写给你的话**：MySQL 的优化没有捷径，核心在于**理解原理**。知道 B+ 树是什么，才知道为什么最左前缀有效；知道 MVCC 怎么工作，才能判断什么场景用什么隔离级别。建议边看手册边在本地 MySQL 里实验，每一个 EXPLAIN 的结论都亲手验证一遍。这些知识，是你和只会写 CRUD 的人真正拉开差距的地方。💪