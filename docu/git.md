# Git 从 0 到精通学习指南

> 作者定位：后端高级开发工程师视角，覆盖日常开发、团队协作、CI/CD 场景的完整 Git 知识体系。

---

## 目录

1. [Git 是什么 & 核心概念](#1-git-是什么--核心概念)
2. [安装与初始配置](#2-安装与初始配置)
3. [仓库初始化与基础操作](#3-仓库初始化与基础操作)
4. [暂存区与提交](#4-暂存区与提交)
5. [分支管理](#5-分支管理)
6. [远程仓库操作](#6-远程仓库操作)
7. [合并与变基](#7-合并与变基)
8. [撤销与回退](#8-撤销与回退)
9. [标签管理](#9-标签管理)
10. [储藏与清理](#10-储藏与清理)
11. [子模块 Submodule](#11-子模块-submodule)
12. [高级技巧](#12-高级技巧)
13. [Git 工作流规范](#13-git-工作流规范)
14. [.gitignore 配置指南](#14-gitignore-配置指南)
15. [Git Hooks 自动化](#15-git-hooks-自动化)
16. [故障排查与实战场景](#16-故障排查与实战场景)
17. [Git 内部原理](#17-git-内部原理)
18. [常用命令速查表](#18-常用命令速查表)

---

## 1. Git 是什么 & 核心概念

### 1.1 版本控制系统演进

| 类型 | 代表 | 特点 |
|------|------|------|
| 本地版本控制 | RCS | 只在本机，无协作能力 |
| 集中式版本控制 | SVN、CVS | 单一中央服务器，网络依赖强 |
| 分布式版本控制 | **Git**、Mercurial | 每个节点都是完整仓库，离线可用 |

### 1.2 Git 的三个区域（核心）

理解这三个区域是掌握 Git 的基础：

```
工作区 (Working Directory)
    ↓  git add
暂存区 (Staging Area / Index)
    ↓  git commit
本地仓库 (Local Repository / .git目录)
    ↓  git push
远程仓库 (Remote Repository)
```

- **工作区**：你实际编辑文件的目录，就是你看得见的文件夹
- **暂存区**：一个中间层，保存你准备提交的变更快照（存在 `.git/index` 文件中）
- **本地仓库**：所有提交历史和元数据存储的地方（`.git` 目录）
- **远程仓库**：GitHub、GitLab、Gitee 等托管平台

### 1.3 文件的四种状态

```
Untracked  →  git add  →  Staged  →  git commit  →  Unmodified
                                                          ↓ 修改文件
                                                       Modified  →  git add  →  Staged
```

| 状态 | 含义 |
|------|------|
| `Untracked` | 新文件，Git 还未追踪 |
| `Unmodified` | 已追踪，未修改 |
| `Modified` | 已追踪，已修改，未暂存 |
| `Staged` | 已暂存，等待提交 |

### 1.4 Git 对象模型

Git 底层存储了 4 种对象（SHA-1 哈希索引）：

| 对象 | 说明 |
|------|------|
| `blob` | 文件内容快照 |
| `tree` | 目录结构，指向 blob 和其他 tree |
| `commit` | 提交信息，指向一个 tree |
| `tag` | 标签对象，指向 commit |

---

## 2. 安装与初始配置

### 2.1 安装

```bash
# Ubuntu / Debian
sudo apt-get install git

# CentOS / RHEL
sudo yum install git

# macOS (Homebrew)
brew install git

# Windows
# 下载 Git for Windows: https://git-scm.com/download/win
```

验证安装：

```bash
git --version
# git version 2.43.0
```

### 2.2 全局配置（必做）

```bash
# 设置用户名和邮箱（会写入每次 commit 的元数据）
git config --global user.name "Sicheng"
git config --global user.email "sicheng@example.com"

# 设置默认编辑器（推荐 vim 或 vscode）
git config --global core.editor vim
git config --global core.editor "code --wait"

# 设置默认分支名为 main（与 GitHub 保持一致）
git config --global init.defaultBranch main

# Windows 用户处理换行符问题
git config --global core.autocrlf true   # Windows
git config --global core.autocrlf input  # Mac/Linux

# 开启命令行颜色高亮
git config --global color.ui auto

# 设置 pull 默认行为为 rebase（推荐，保持线性历史）
git config --global pull.rebase true
```

### 2.3 配置作用域

```bash
git config --system  # 系统级：/etc/gitconfig，影响所有用户
git config --global  # 用户级：~/.gitconfig，影响当前用户所有仓库
git config --local   # 仓库级：.git/config，只影响当前仓库（默认）
```

优先级：`local > global > system`

### 2.4 查看配置

```bash
git config --list              # 查看所有配置
git config --list --global     # 查看全局配置
git config user.name           # 查看单个配置项
git config --show-origin user.name  # 查看配置来源文件
```

### 2.5 配置 SSH 密钥（与远程仓库免密通信）

```bash
# 生成 ED25519 密钥（推荐，比 RSA 更安全）
ssh-keygen -t ed25519 -C "sicheng@example.com"

# 查看公钥并复制到 GitHub/GitLab
cat ~/.ssh/id_ed25519.pub

# 测试连接
ssh -T git@github.com
# Hi Sicheng! You've successfully authenticated...
```

---

## 3. 仓库初始化与基础操作

### 3.1 初始化仓库

```bash
# 方式一：在现有目录初始化
mkdir my-project && cd my-project
git init
# 会创建 .git 目录

# 方式二：克隆远程仓库
git clone https://github.com/user/repo.git
git clone git@github.com:user/repo.git        # SSH 方式（推荐）
git clone https://github.com/user/repo.git my-folder  # 克隆到指定目录
git clone --depth 1 https://github.com/user/repo.git  # 浅克隆，只拉最新一次提交（节省空间）
```

### 3.2 查看仓库状态

```bash
git status          # 详细状态
git status -s       # 简洁模式，每行两个字符表示状态
```

`-s` 模式下状态含义：

```
?? 新文件未追踪
A  已暂存新文件
M  已修改
D  已删除
R  已重命名
```

示例输出：

```
 M src/main.cpp     # 工作区修改，未暂存
M  src/utils.cpp    # 已暂存
?? config.json      # 未追踪新文件
```

### 3.3 查看文件差异

```bash
# 工作区 vs 暂存区（未暂存的修改）
git diff

# 暂存区 vs 最新提交（已暂存但未提交的修改）
git diff --staged
git diff --cached   # 同上，两种写法都可以

# 两个提交之间的差异
git diff abc123 def456

# 查看某个文件的差异
git diff src/main.cpp

# 只显示差异的文件名，不显示内容
git diff --name-only
git diff --stat
```

**案例：** 你修改了 `server.cpp`，在提交前想检查改了什么：

```bash
git diff server.cpp
# 输出：
# diff --git a/server.cpp b/server.cpp
# --- a/server.cpp
# +++ b/server.cpp
# @@ -10,6 +10,7 @@
#  int main() {
# +    Logger::init("logs/server.log");  // 新增这行
#      Server server(8080);
```

---

## 4. 暂存区与提交

### 4.1 添加到暂存区

```bash
git add filename.cpp           # 添加单个文件
git add src/                   # 添加整个目录
git add *.cpp                  # 通配符
git add .                      # 添加所有变更（新增+修改+删除）
git add -u                     # 只添加已追踪文件的修改和删除（不包含新文件）
git add -A                     # 等同于 git add .

# 交互式暂存（精细控制，只暂存部分修改）
git add -p filename.cpp
# 会逐个 hunk 询问是否暂存 (y/n/s/q/?)
```

**`git add -p` 是高级技巧的核心，强烈推荐！**

假设你在 `server.cpp` 里同时做了两件事（修复 bug + 添加新功能），你想分两次提交：

```bash
git add -p server.cpp
# Git 会展示每个修改块并询问：
# Stage this hunk [y,n,q,a,d,s,?]?
# y - 暂存这个块
# n - 不暂存
# s - 将这个块拆分成更小的块
```

### 4.2 提交

```bash
# 基础提交
git commit -m "feat: add Redis connection pool"

# 打开编辑器写详细提交信息
git commit

# 跳过暂存区直接提交已追踪文件的修改（不含新文件）
git commit -am "fix: handle null pointer exception"

# 修改最近一次提交（未 push 时使用）
git commit --amend -m "fix: handle null pointer exception in parser"

# 修改提交但不改消息（只更新内容）
git commit --amend --no-edit
```

### 4.3 提交信息规范（Conventional Commits）

良好的提交信息是团队协作的基础，也是写简历项目经历的加分项：

```
<type>(<scope>): <subject>

<body>

<footer>
```

**type 类型：**

| type | 含义 |
|------|------|
| `feat` | 新功能 |
| `fix` | Bug 修复 |
| `docs` | 文档更新 |
| `style` | 代码格式（不影响逻辑） |
| `refactor` | 重构（非 bug 非新功能） |
| `perf` | 性能优化 |
| `test` | 测试相关 |
| `chore` | 构建工具/依赖更新 |
| `revert` | 回滚提交 |

**示例：**

```
feat(auth): add JWT token refresh mechanism

Implement automatic token refresh to prevent session expiration.
The refresh token is stored in Redis with a 7-day TTL.

Closes #42
```

### 4.4 查看提交历史

```bash
git log                          # 完整日志
git log --oneline                # 单行简洁模式
git log --oneline --graph        # 图形化分支结构
git log --oneline --graph --all  # 包含所有分支

# 自定义格式（强烈推荐配置为别名）
git log --pretty=format:"%h %ad | %s%d [%an]" --date=short

# 过滤查询
git log --author="Sicheng"              # 按作者
git log --since="2024-01-01"            # 按时间
git log --grep="Redis"                  # 按提交信息关键词
git log -n 10                           # 最近 10 条
git log --follow src/server.cpp         # 追踪某文件的历史（含重命名）
git log -p src/server.cpp               # 查看某文件每次提交的具体改动

# 查看两个分支的差异提交
git log main..feature/login             # feature/login 有但 main 没有的提交
```

**配置实用别名：**

```bash
git config --global alias.lg "log --oneline --graph --all --decorate"
# 之后直接 git lg 就能看漂亮的树形日志
```

---

## 5. 分支管理

分支是 Git 最强大的特性，本质上是一个指向某个 commit 的可移动指针。

### 5.1 分支基础操作

```bash
# 查看分支
git branch          # 本地分支（* 表示当前分支）
git branch -r       # 远程追踪分支
git branch -a       # 所有分支

# 创建分支
git branch feature/user-auth

# 切换分支
git checkout feature/user-auth
git switch feature/user-auth    # Git 2.23+ 推荐新语法

# 创建并切换（最常用）
git checkout -b feature/user-auth
git switch -c feature/user-auth  # 新语法

# 基于远程分支创建本地分支
git checkout -b feature/login origin/feature/login
git switch -c feature/login --track origin/feature/login

# 删除分支
git branch -d feature/user-auth   # 安全删除（已合并才能删）
git branch -D feature/user-auth   # 强制删除

# 重命名分支
git branch -m old-name new-name
git branch -M main  # 强制重命名当前分支为 main
```

### 5.2 HEAD 指针理解

`HEAD` 始终指向当前所在位置：

```
main     →  commit C3
              ↑
feature  →  commit C2  →  C1  →  C0
              ↑
            HEAD（当前在 feature 分支）
```

```bash
# 查看 HEAD 指向
cat .git/HEAD
# ref: refs/heads/feature/user-auth

# 分离 HEAD（直接 checkout 到某个 commit）
git checkout abc1234
# 此时 HEAD 不指向任何分支，处于 detached HEAD 状态
# 从这里创建新分支可以保存工作
git switch -c new-branch-from-here
```

### 5.3 分支策略案例

**场景：** 你在开发 Gomoku 项目的重连机制，同时线上有个紧急 Bug 要修：

```bash
# 当前在 main 分支
git switch -c feature/reconnect    # 开发重连功能

# ... 开发了一半，收到紧急 Bug 通知

git stash                           # 保存当前工作（后面详解）
git switch main
git switch -c hotfix/null-pointer   # 创建 hotfix 分支

# 修复 Bug
git commit -am "fix: handle null pointer in game state"

git switch main
git merge hotfix/null-pointer       # 合并到 main
git branch -d hotfix/null-pointer   # 删除 hotfix 分支

git switch feature/reconnect        # 回到之前的工作
git stash pop                       # 恢复之前的工作
```

---

## 6. 远程仓库操作

### 6.1 远程仓库管理

```bash
# 查看远程仓库
git remote -v
# origin  git@github.com:sicheng/gomoku.git (fetch)
# origin  git@github.com:sicheng/gomoku.git (push)

# 添加远程仓库
git remote add origin git@github.com:sicheng/gomoku.git
git remote add upstream git@github.com:original/gomoku.git  # 添加上游

# 修改远程仓库 URL
git remote set-url origin git@github.com:sicheng/new-repo.git

# 删除远程仓库引用
git remote remove upstream

# 重命名
git remote rename origin github
```

### 6.2 fetch、pull、push

```bash
# fetch：下载远程数据但不合并（安全）
git fetch origin
git fetch --all    # 拉取所有远程

# pull：fetch + merge（或 fetch + rebase，取决于配置）
git pull origin main
git pull           # 追踪分支已配置时可省略参数

# push：推送到远程
git push origin main
git push           # 已配置追踪时可省略
git push -u origin feature/reconnect  # 推送并建立追踪关系
git push --force-with-lease           # 安全的强制推送（推荐替代 -f）
git push origin --delete feature/old  # 删除远程分支
git push origin --tags                # 推送所有标签
```

**`--force-with-lease` vs `--force` 的区别：**

`--force` 会无条件覆盖远程，可能覆盖他人提交。`--force-with-lease` 会检查远程是否有你未 fetch 的新提交，如果有则拒绝推送，更安全。

### 6.3 追踪分支配置

```bash
# 查看追踪关系
git branch -vv

# 设置追踪关系
git branch --set-upstream-to=origin/main main
git push -u origin feature/reconnect  # push 时同时设置
```

---

## 7. 合并与变基

这是 Git 中最重要也最容易出问题的部分。

### 7.1 Merge 合并

```bash
git switch main
git merge feature/user-auth
```

**Fast-forward 合并**（main 没有新提交时）：

```
Before:  main → A → B
                        feature → C → D

After:   main → A → B → C → D
                              ↑ feature
```

**三方合并**（main 有新提交时，会产生 merge commit）：

```
Before:  main → A → B → E
                    ↘
                     C → D ← feature

After:   main → A → B → E → M (merge commit)
                    ↘       ↗
                     C → D
```

```bash
# 禁止 fast-forward，强制生成 merge commit（保留完整历史）
git merge --no-ff feature/user-auth -m "Merge feature/user-auth"

# 只合并提交，不合并历史（squash merge）
git merge --squash feature/user-auth
git commit -m "feat: add user auth module"
```

### 7.2 解决冲突

当两个分支都修改了同一文件的同一区域时，会产生冲突：

```bash
git merge feature/user-auth
# Auto-merging src/server.cpp
# CONFLICT (content): Merge conflict in src/server.cpp
# Automatic merge failed; fix conflicts and then commit the result.
```

冲突文件内容：

```cpp
<<<<<<< HEAD (当前分支 main 的内容)
int port = 8080;
=======
int port = 9090;
>>>>>>> feature/user-auth (要合并进来的内容)
```

**解决步骤：**

```bash
# 1. 手动编辑冲突文件，删除标记，保留正确内容
# 2. 标记为已解决
git add src/server.cpp

# 3. 完成合并
git commit

# 如果想放弃合并
git merge --abort
```

**推荐使用 VSCode 或 merge tool：**

```bash
git mergetool    # 打开配置的 merge tool
```

### 7.3 Rebase 变基

Rebase 将当前分支的提交"重新嫁接"到目标分支的最新提交之后，产生线性历史：

```
Before:  main → A → B → E
                    ↘
                     C → D ← feature

After rebase feature onto main:
         main → A → B → E
                          ↘
                           C' → D' ← feature
```

```bash
git switch feature/user-auth
git rebase main             # 将 feature 变基到 main 的最新提交

# 变基过程中发生冲突
# 1. 解决冲突
# 2. git add .
# 3. git rebase --continue
# 或放弃：git rebase --abort
```

### 7.4 交互式变基（强大的历史整理工具）

```bash
git rebase -i HEAD~3   # 整理最近 3 个提交
git rebase -i abc1234  # 从某个 commit 开始整理
```

会打开编辑器，显示：

```
pick a1b2c3 feat: add login endpoint
pick d4e5f6 fix typo
pick g7h8i9 fix: handle token expiry

# Commands:
# p, pick = 保留此提交
# r, reword = 保留但修改提交信息
# e, edit = 保留并允许修改内容
# s, squash = 合并到上一个提交（保留提交信息）
# f, fixup = 合并到上一个提交（丢弃此提交信息）
# d, drop = 删除此提交
```

**案例：** 把 3 个零散提交合并为一个干净的提交：

```
pick a1b2c3 feat: add login endpoint
f    d4e5f6 fix typo          ← 改为 f（fixup）
f    g7h8i9 fix: handle token ← 改为 f（fixup）
```

保存退出后，3 个提交合并为 1 个，只保留第一个提交信息。

### 7.5 Merge vs Rebase 选择原则

| 场景 | 推荐方式 |
|------|------|
| 功能分支合入主干 | `--no-ff merge`（保留分支历史） |
| 更新功能分支（同步主干最新代码） | `rebase`（保持线性，便于 review） |
| 公共分支（main/develop） | **禁止** rebase |
| 个人私有分支整理历史 | `rebase -i` |

**黄金法则：永远不要对已推送到远程的公共分支执行 rebase！**

---

## 8. 撤销与回退

这部分非常重要，是救急的关键技能。

### 8.1 撤销工作区修改

```bash
# 丢弃工作区的修改（恢复到暂存区状态，或最后提交状态）
git checkout -- src/server.cpp      # 旧语法
git restore src/server.cpp          # 新语法（推荐）

# 丢弃所有工作区修改
git restore .
```

⚠️ 此操作不可恢复，谨慎使用！

### 8.2 撤销暂存区

```bash
# 取消暂存（文件仍保留修改，只是移出暂存区）
git reset HEAD src/server.cpp    # 旧语法
git restore --staged src/server.cpp  # 新语法（推荐）
```

### 8.3 回退提交（核心）

```bash
# git reset 三种模式
git reset --soft  HEAD~1   # 只移动 HEAD，暂存区和工作区保留
git reset --mixed HEAD~1   # 移动 HEAD，清空暂存区，工作区保留（默认）
git reset --hard  HEAD~1   # 移动 HEAD，暂存区和工作区全部清空（危险！）
```

三种模式的差异：

| 模式 | HEAD | 暂存区 | 工作区 |
|------|------|------|------|
| `--soft` | 移动 | 保留 | 保留 |
| `--mixed` | 移动 | 清空 | 保留 |
| `--hard` | 移动 | 清空 | 清空 |

**案例一：** 最近一次提交信息写错了，想重新写：

```bash
git reset --soft HEAD~1   # 撤销提交，代码回到暂存区
git commit -m "fix: correct error message for connection timeout"
```

**案例二：** 提交了不该提交的调试代码：

```bash
git reset --mixed HEAD~1   # 撤销提交，代码回到工作区
# 删除调试代码
git add .
git commit -m "feat: add connection pool"
```

**案例三：** 某个功能完全做错了，想彻底丢弃（未 push）：

```bash
git reset --hard HEAD~3    # 彻底回退 3 个提交
```

### 8.4 git revert（安全的公共分支回退）

`reset` 会修改历史，不适合公共分支。`revert` 创建一个新提交来抵消之前的变更：

```bash
# 撤销某个提交的变更（创建反向提交）
git revert abc1234
git revert HEAD        # 撤销最近一次提交
git revert HEAD~2..HEAD  # 撤销最近两次提交
git revert -n abc1234  # 只应用变更，不自动提交（批量 revert 时用）
```

**案例：** 上线后发现某个 commit 引入了 Bug，需要紧急回滚：

```bash
git log --oneline
# a1b2c3 (HEAD, main) fix: update connection config   ← 没问题
# d4e5f6 feat: add Redis cluster support              ← 这个有 Bug
# g7h8i9 feat: add user cache layer

git revert d4e5f6     # 创建新提交抵消 d4e5f6 的变更
git push origin main  # 推送到远程，安全可追溯
```

### 8.5 找回丢失的提交（reflog）

`git reflog` 记录了本地所有 HEAD 移动历史，是最后的救命稻草：

```bash
git reflog
# a1b2c3 HEAD@{0}: reset: moving to HEAD~1
# d4e5f6 HEAD@{1}: commit: feat: add Redis cluster
# g7h8i9 HEAD@{2}: commit: feat: add connection pool

# 找到想恢复的 commit，重置回去
git reset --hard d4e5f6

# 或者创建新分支恢复
git checkout -b recovery/redis-feature d4e5f6
```

⚠️ `reflog` 只保留本地记录，`git gc` 清理后可能消失，丢失代码后尽快操作！

---

## 9. 标签管理

标签通常用于标记发布版本，如 `v1.0.0`、`v2.3.1`。

```bash
# 创建轻量标签（只是指针）
git tag v1.0.0

# 创建附注标签（包含完整对象信息，推荐）
git tag -a v1.0.0 -m "Release version 1.0.0"

# 给指定 commit 打标签
git tag -a v0.9.0 abc1234 -m "Beta release"

# 查看标签
git tag
git tag -l "v1.*"         # 过滤
git show v1.0.0           # 查看标签详情

# 推送标签到远程
git push origin v1.0.0    # 推送单个
git push origin --tags    # 推送所有标签

# 删除标签
git tag -d v1.0.0                    # 删除本地
git push origin --delete tag v1.0.0  # 删除远程

# 检出标签（会进入 detached HEAD 状态）
git checkout v1.0.0
git checkout -b hotfix/v1.0.1 v1.0.0  # 基于标签创建分支
```

### 语义化版本规范（SemVer）

```
v主版本号.次版本号.修订号
  MAJOR.MINOR.PATCH

MAJOR：不兼容的 API 修改
MINOR：向下兼容的新功能
PATCH：向下兼容的 Bug 修复
```

---

## 10. 储藏与清理

### 10.1 git stash 储藏

当你需要临时切换工作，但当前修改还不想提交时使用：

```bash
# 储藏当前工作（包括已追踪文件的修改）
git stash
git stash push -m "WIP: reconnect mechanism"  # 带描述（推荐）

# 包含未追踪的新文件
git stash -u

# 包含被 .gitignore 忽略的文件
git stash -a

# 查看储藏列表
git stash list
# stash@{0}: On feature/reconnect: WIP: reconnect mechanism
# stash@{1}: On main: WIP: fix login bug

# 应用储藏（保留 stash 记录）
git stash apply stash@{0}

# 应用并删除储藏
git stash pop         # 应用最近一次
git stash pop stash@{1}

# 删除储藏
git stash drop stash@{0}
git stash clear       # 清空所有储藏

# 从储藏创建分支（当 stash 和当前代码冲突时很有用）
git stash branch new-feature stash@{0}
```

### 10.2 git clean 清理

```bash
# 查看会被清理的文件（dry run，不实际删除）
git clean -n
git clean -nd  # 包含目录

# 删除未追踪的文件
git clean -f
git clean -fd  # 包含目录
git clean -fdx  # 包含被 .gitignore 忽略的文件（慎用！）
```

---

## 11. 子模块 Submodule

当项目依赖另一个 Git 仓库时使用，常见于 C++ 项目引入第三方库：

```bash
# 添加子模块
git submodule add https://github.com/nlohmann/json.git third_party/json
git submodule add git@github.com:gabime/spdlog.git third_party/spdlog

# 克隆包含子模块的仓库
git clone --recurse-submodules git@github.com:user/project.git
# 或分步进行
git clone git@github.com:user/project.git
git submodule init
git submodule update

# 更新子模块到最新
git submodule update --remote

# 查看子模块状态
git submodule status

# 删除子模块（较繁琐）
git submodule deinit third_party/json
git rm third_party/json
rm -rf .git/modules/third_party/json
```

---

## 12. 高级技巧

### 12.1 cherry-pick 精选提交

将某个分支上的特定提交应用到当前分支：

```bash
# 应用单个提交
git cherry-pick abc1234

# 应用多个提交
git cherry-pick abc1234 def5678

# 应用一段范围的提交
git cherry-pick abc1234..def5678

# 应用但不自动提交
git cherry-pick -n abc1234
```

**案例：** 你在 `feature` 分支修复了一个 Bug（commit `abc1234`），需要把这个修复同步到 `main`：

```bash
git switch main
git cherry-pick abc1234
# 相当于把 abc1234 这个提交的变更复制一份在 main 上重新提交
```

### 12.2 bisect 二分查找 Bug

用二分法快速定位引入 Bug 的提交，特别适合"某个版本开始出 Bug 但不知道是哪个提交"的场景：

```bash
git bisect start
git bisect bad                  # 当前版本有 Bug
git bisect good v1.0.0          # v1.0.0 没问题

# Git 会自动 checkout 中间的 commit，你测试后告诉 Git 结果
git bisect good  # 没有 Bug
git bisect bad   # 有 Bug

# 不断二分，最终 Git 会告诉你第一个有 Bug 的 commit
# first bad commit: [abc1234] feat: add Redis cluster support

git bisect reset  # 结束 bisect，回到原始 HEAD
```

### 12.3 blame 追责

```bash
# 查看文件每一行最后是谁在哪次提交修改的
git blame src/server.cpp
git blame -L 10,20 src/server.cpp  # 只看 10-20 行
```

### 12.4 grep 搜索

```bash
# 在工作区中搜索
git grep "ConnectionPool"
git grep -n "TODO"          # 显示行号
git grep -l "Redis"         # 只显示文件名
git grep "Redis" -- "*.cpp" # 只搜索 cpp 文件

# 在某个提交中搜索
git grep "ConnectionPool" v1.0.0
```

### 12.5 worktree 多工作区

允许同时 checkout 多个分支到不同目录，无需频繁切换：

```bash
# 创建新工作区
git worktree add ../hotfix-branch hotfix/null-pointer

# 列出所有工作区
git worktree list

# 删除工作区
git worktree remove ../hotfix-branch
```

### 12.6 sparse-checkout 稀疏检出

只检出仓库的部分目录（适用于 monorepo）：

```bash
git clone --no-checkout git@github.com:user/monorepo.git
cd monorepo
git sparse-checkout init --cone
git sparse-checkout set services/auth services/user
git checkout main
```

### 12.7 配置实用别名

在 `~/.gitconfig` 中添加：

```ini
[alias]
    st = status -s
    co = checkout
    sw = switch
    br = branch
    cm = commit
    lg = log --oneline --graph --all --decorate
    unstage = restore --staged
    last = log -1 HEAD
    undo = reset --soft HEAD~1
    aliases = config --get-regexp alias
    pr = "!f() { git fetch origin pull/$1/head:pr/$1 && git switch pr/$1; }; f"
```

---

## 13. Git 工作流规范

### 13.1 Git Flow

适合有明确版本发布周期的项目：

```
main（只含正式发布版本）
  ↑ release
develop（集成分支）
  ↑ feature/*  hotfix/*
```

| 分支 | 作用 | 来源 | 合入 |
|------|------|------|------|
| `main` | 生产代码 | — | `release`、`hotfix` |
| `develop` | 开发集成 | `main` | `feature`、`release` |
| `feature/*` | 新功能开发 | `develop` | `develop` |
| `release/*` | 版本准备 | `develop` | `main` + `develop` |
| `hotfix/*` | 紧急修复 | `main` | `main` + `develop` |

### 13.2 GitHub Flow

适合持续部署的互联网产品，简单高效：

```
main（随时可部署）
  ↑
feature/* → Pull Request → Code Review → merge to main → deploy
```

### 13.3 Trunk Based Development

适合高频发布、有完善 CI/CD 的团队：

- 只有一个 `main` 分支
- 功能用 Feature Flags 控制
- 每次提交都触发 CI，随时可发布

### 13.4 分支命名规范

```
feature/issue-42-user-auth        # 新功能
bugfix/null-pointer-in-parser     # Bug 修复
hotfix/critical-redis-conn-leak   # 紧急修复
release/v2.3.0                    # 版本发布
chore/update-dependencies         # 杂项
refactor/connection-pool          # 重构
```

---

## 14. .gitignore 配置指南

### 14.1 语法规则

```gitignore
# 注释行

# 忽略所有 .o 文件
*.o

# 忽略 build 目录及其所有内容
build/

# 只忽略根目录下的 config.json，不忽略子目录中的
/config.json

# 忽略 doc 目录下所有 .txt，但不忽略子目录
doc/*.txt

# 忽略所有 .txt，包括子目录
**/*.txt

# 取消忽略某个文件（前面必须有规则先匹配）
!lib/important.txt
```

### 14.2 C++ 项目推荐 .gitignore

```gitignore
# 编译输出
build/
dist/
out/
*.o
*.a
*.so
*.so.*
*.dylib
*.dll
*.exe

# CMake
CMakeCache.txt
CMakeFiles/
cmake_install.cmake
Makefile
*.cmake
!CMakeLists.txt

# IDE 文件
.vscode/
.idea/
*.user
*.suo
.vs/
*.sln.docstates

# 调试符号
*.dSYM/
*.pdb

# 依赖（如果不用 submodule 的话）
third_party/
vendor/

# 日志
logs/
*.log

# 环境配置（含密码等敏感信息）
.env
.env.local
config/local.yaml

# 临时文件
*.tmp
*.bak
*.swp
*~
```

### 14.3 已追踪文件如何忽略

```bash
# 文件已经被 Git 追踪，再加到 .gitignore 无效
# 需要先从追踪中移除（不删除本地文件）
git rm --cached config/local.yaml
git rm --cached -r build/   # 目录

git commit -m "chore: stop tracking local config"
```

---

## 15. Git Hooks 自动化

Hooks 是在 Git 操作前后自动执行的脚本，存放在 `.git/hooks/` 目录。

### 15.1 常用 Hooks

| Hook | 触发时机 | 常见用途 |
|------|------|------|
| `pre-commit` | commit 之前 | 代码格式化、Lint 检查 |
| `commit-msg` | 提交信息写入后 | 验证提交信息格式 |
| `pre-push` | push 之前 | 运行测试 |
| `post-merge` | merge 完成后 | 更新依赖 |
| `prepare-commit-msg` | 打开编辑器前 | 自动填充提交信息模板 |

### 15.2 创建 pre-commit Hook

```bash
cat > .git/hooks/pre-commit << 'EOF'
#!/bin/bash

echo "Running pre-commit checks..."

# 检查是否有调试代码
if git diff --cached | grep -E "^\+" | grep -v "^+++" | grep -E "(TODO|FIXME|console\.log|debugger|std::cout)"; then
    echo "❌ Found debug code, please remove before committing!"
    exit 1
fi

# 运行 clang-format 检查（C++ 项目）
CHANGED_FILES=$(git diff --cached --name-only --diff-filter=ACM | grep -E '\.(cpp|cc|cxx|h|hpp)$')
if [ -n "$CHANGED_FILES" ]; then
    for file in $CHANGED_FILES; do
        if ! clang-format --dry-run --Werror "$file" 2>/dev/null; then
            echo "❌ $file not formatted. Run: clang-format -i $file"
            exit 1
        fi
    done
fi

echo "✅ Pre-commit checks passed!"
exit 0
EOF

chmod +x .git/hooks/pre-commit
```

### 15.3 使用 Husky（Node.js 项目）

```bash
npm install --save-dev husky
npx husky init

# .husky/pre-commit
echo "npm test" > .husky/pre-commit
```

### 15.4 commit-msg Hook（验证提交信息格式）

```bash
cat > .git/hooks/commit-msg << 'EOF'
#!/bin/bash

COMMIT_MSG=$(cat "$1")
PATTERN="^(feat|fix|docs|style|refactor|perf|test|chore|revert)(\(.+\))?: .{1,100}"

if ! echo "$COMMIT_MSG" | grep -qE "$PATTERN"; then
    echo "❌ Invalid commit message format!"
    echo "   Expected: type(scope): subject"
    echo "   Example:  feat(auth): add JWT refresh token"
    exit 1
fi
exit 0
EOF

chmod +x .git/hooks/commit-msg
```

---

## 16. 故障排查与实战场景

### 场景一：误 push 了敏感信息（密码/密钥）

```bash
# 1. 立即修改泄露的密码/密钥！

# 2. 使用 git filter-repo 彻底清除（推荐）
pip install git-filter-repo
git filter-repo --path-glob 'config/secret.yaml' --invert-paths
git filter-repo --replace-text <(echo 'OLD_PASSWORD==>***REMOVED***')

# 3. 强制推送
git push --force --all
git push --force --tags
```

### 场景二：合并了错误分支

```bash
# 如果还没 push
git reset --hard HEAD~1  # 撤销 merge commit

# 如果已 push（公共分支）
git revert -m 1 <merge-commit-hash>  # -m 1 表示保留主线，撤销合入的分支
```

### 场景三：提交到了错误的分支

```bash
# 情况：把本应在 feature 分支的提交提交到了 main

# 1. 在 feature 分支上 cherry-pick 这个提交
git switch feature/my-feature
git cherry-pick main  # cherry-pick main 的最新提交

# 2. 在 main 上回退
git switch main
git reset --hard HEAD~1
```

### 场景四：想分开提交同一文件的不同修改

```bash
# 使用交互式暂存
git add -p src/server.cpp
# 对每个 hunk 选择 y/n/s（s 可以再拆分）
```

### 场景五：rebase 冲突太多想跳过某个提交

```bash
git rebase -i main
# 将有冲突的提交改为 drop
# 或在 rebase 过程中：
git rebase --skip  # 跳过当前冲突的提交
```

### 场景六：大文件误提交导致仓库体积膨胀

```bash
# 查找大文件
git rev-list --objects --all | sort -k 2 > allfileshas.txt
git gc && git verify-pack -v .git/objects/pack/*.idx | sort -k 3 -n | tail -10

# 使用 BFG 清理大文件（比 filter-branch 快 10-720x）
java -jar bfg.jar --strip-blobs-bigger-than 10M
git reflog expire --expire=now --all
git gc --prune=now --aggressive
git push --force --all
```

---

## 17. Git 内部原理

理解底层原理能帮你更自信地使用 Git，出问题时不慌张。

### 17.1 .git 目录结构

```
.git/
├── HEAD              # 当前指向的分支或 commit
├── config            # 仓库级配置
├── index             # 暂存区
├── COMMIT_EDITMSG    # 最近一次 commit 信息
├── objects/          # 所有 Git 对象（blob/tree/commit/tag）
│   ├── 2f/
│   │   └── a3b4c...  # SHA-1 前两位为目录名
│   ├── pack/         # 打包的对象（gc 后）
│   └── info/
├── refs/
│   ├── heads/        # 本地分支
│   │   ├── main
│   │   └── feature/reconnect
│   ├── remotes/      # 远程追踪分支
│   │   └── origin/
│   │       └── main
│   └── tags/         # 标签
└── logs/
    ├── HEAD           # HEAD 移动日志（reflog 来源）
    └── refs/
```

### 17.2 对象存储机制

```bash
# 手动查看对象内容
git cat-file -t abc1234  # 查看对象类型（blob/tree/commit/tag）
git cat-file -p abc1234  # 查看对象内容

# 查看 commit 对象
git cat-file -p HEAD
# tree a3b4c5...
# parent d6e7f8...
# author Sicheng <email> 1709000000 +0800
# committer Sicheng <email> 1709000000 +0800
#
# feat: add connection pool

# 查看 tree 对象
git cat-file -p a3b4c5
# 100644 blob f1e2d3... .gitignore
# 100644 blob 4b5c6d... CMakeLists.txt
# 040000 tree 7c8d9e... src
```

### 17.3 分支本质

分支只是一个文件，里面存着一个 commit 的 SHA-1：

```bash
cat .git/refs/heads/main
# a1b2c3d4e5f6...（40个字符的 SHA-1）

# HEAD 文件
cat .git/HEAD
# ref: refs/heads/main（符号引用）
# 或
# a1b2c3d4e5f6...（detached HEAD 时直接是 SHA-1）
```

### 17.4 rebase 的本质

Rebase 不是"移动提交"，而是"重新创建提交"。每个新的 commit 都有新的 SHA-1，只是内容（patch）相同。这就是为什么 rebase 会改变历史。

---

## 18. 常用命令速查表

### 初始化与配置

```bash
git init                           # 初始化仓库
git clone <url>                    # 克隆仓库
git config --global user.name ""   # 设置用户名
git config --global user.email ""  # 设置邮箱
git config --list                  # 查看所有配置
```

### 日常操作

```bash
git status                   # 查看状态
git add .                    # 暂存所有
git add -p                   # 交互式暂存
git commit -m ""             # 提交
git commit --amend           # 修改最近提交
git log --oneline --graph    # 查看日志
git diff                     # 工作区差异
git diff --staged            # 暂存区差异
```

### 分支操作

```bash
git branch                   # 列出分支
git switch -c <branch>       # 创建并切换
git switch <branch>          # 切换分支
git merge <branch>           # 合并分支
git rebase <branch>          # 变基
git branch -d <branch>       # 删除分支
git cherry-pick <hash>       # 精选提交
```

### 远程操作

```bash
git remote -v                # 查看远程
git remote add origin <url>  # 添加远程
git fetch origin             # 拉取远程数据
git pull                     # 拉取并合并
git push origin <branch>     # 推送分支
git push -u origin <branch>  # 推送并设置追踪
git push --force-with-lease  # 安全强制推送
```

### 撤销操作

```bash
git restore <file>           # 丢弃工作区修改
git restore --staged <file>  # 取消暂存
git reset --soft HEAD~1      # 软回退（保留修改）
git reset --hard HEAD~1      # 硬回退（丢弃修改）
git revert <hash>            # 创建反向提交（安全）
git reflog                   # 查看操作历史
```

### 储藏与标签

```bash
git stash                    # 储藏
git stash push -m ""         # 带描述储藏
git stash list               # 查看储藏列表
git stash pop                # 应用并删除
git tag -a v1.0.0 -m ""      # 创建附注标签
git push origin --tags       # 推送所有标签
```

### 高级工具

```bash
git rebase -i HEAD~n         # 交互式变基
git bisect start/good/bad    # 二分查找 Bug
git blame <file>             # 查看行注释
git grep "keyword"           # 内容搜索
git shortlog -sn             # 贡献者统计
git gc                       # 垃圾回收
```

---

## 推荐学习路径

```
第一周：掌握基础
    init → add → commit → log → diff
    branch → switch → merge → 解决冲突

第二周：掌握远程协作
    remote → fetch/pull/push
    .gitignore → 规范 commit message

第三周：掌握高级操作
    rebase → cherry-pick → stash
    reset/revert → reflog

第四周：提升工程化
    Git Flow / GitHub Flow 工作流
    Hooks 自动化 → 理解内部原理
```

---

> 📌 **记住最重要的三条原则：**
> 1. **永远不要对公共分支执行 rebase**
> 2. **push 之前先 fetch**，了解远程的最新状态
> 3. **reflog 是你的最后防线**，大多数"删掉的"东西都能找回来