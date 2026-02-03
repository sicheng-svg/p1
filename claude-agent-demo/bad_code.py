def d(x):
    y = []
    while True:
        y.append(x * 2000) # 故意制造内存泄漏
        if x == 1:
            print("ok")
            # 忘记 break
