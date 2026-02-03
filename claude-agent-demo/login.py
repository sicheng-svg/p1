#!/usr/bin/env python3
"""简单的用户登录验证模块"""

def validate_credentials(username, password):
    """
    验证用户名和密码

    Args:
        username: 用户名
        password: 密码

    Returns:
        bool: 验证成功返回 True，否则返回 False
    """
    # 示例用户数据库（实际应用中应使用数据库和加密密码）
    users = {
        "admin": "admin123",
        "user": "password",
        "test": "test123"
    }

    return users.get(username) == password


def login():
    """交互式登录函数"""
    print("=== 用户登录系统 ===")

    max_attempts = 3
    attempts = 0

    while attempts < max_attempts:
        username = input("请输入用户名: ").strip()
        password = input("请输入密码: ").strip()

        if validate_credentials(username, password):
            print(f"\n登录成功！欢迎您，{username}！")
            return True
        else:
            attempts += 1
            remaining = max_attempts - attempts
            if remaining > 0:
                print(f"用户名或密码错误，您还有 {remaining} 次尝试机会。\n")
            else:
                print("登录失败，已达到最大尝试次数。")
                return False

    return False


if __name__ == "__main__":
    login()
