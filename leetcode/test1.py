a, b, c, d = 20, 5.5, True, 4+3j

# print(isinstance(a, int))
# print(isinstance(b, float))
# print(isinstance(c, int))
# print(isinstance(d, complex))

# print("\n")
# print(type(a))
# print(type(b))
# print(type(c))
# print(type(d))

# 在 python中，如果想要比较值是否相等，则用 ==
# 如果想比较对象，是不是同一个对象，则用 is

def add(x, y):
    """两数之和"""
    return x + y

# print(add.__doc__)

from hmac import new
import time

from regex import W

# for i in range(101): # 添加进度条图形和百分比
#     bar = '[' + '=' * (i // 2) + ' ' * (50 - i // 2) + ']'
#     print(f"\r{bar} {i:3}%", end='', flush=True)
#     time.sleep(0.05)
# print()


# 列表推导式
names = ['Bob','Tom','alice','Jerry','Wendy','Smith']
new_names = [name.upper()for name in names if len(name)<=3]
# print(new_names)

# 生成一个列表，包含1-9之间的偶数
multilist = [x for x in range(1, 10) if x % 2 == 0]
# print(multilist)


# 字典推导式
listdemo = ['Google','Runoob', 'Taobao']
newdict = {key: len(key) for key in listdemo}
# print(newdict)

# 字典推导式，生成一个字典，包含0~9之间的偶数何其平方
dic = {x: x**2 for x in range(10) if x % 2 == 0}
# print(dic)

# 集合推导式
setdemo = {x for x in range(10) if x % 3 == 0}
# print(setdemo)

# 元组推导式
tupledemo = (x for x in range(10) if x % 3 == 0)
# print(tuple(tupledemo))


"""迭代器和生成器"""
# 迭代器
# iter() 函数用于从一个可迭代对象中返回一个迭代器对象。
# next() 函数用于从迭代器中获取下一个元素。
# 当next走到结束位置后，会抛出StopIteration异常。python中的for循环会捕获这个异常从而知道该停止了
list1 = [1, 2, 3, 4, 5]
iter1 = iter(list1) # 创建一个迭代器对象
print(next(iter1))

list2 = [1, 2, 3, 4, 5]
iter2 = iter(list2) # 创建一个迭代器对象
for x in iter2:
    print(x, end=" ")
print("\n")

# 生成器
# 在 Python 中，使用了 yield 的函数被称为生成器（generator）。
# yield 是一个关键字，用于定义生成器函数，生成器函数是一种特殊的函数，可以在迭代过程中逐步产生值，而不是一次性返回所有结果。
def countdown(n):
    while n > 0:
        yield n
        n -= 1
 
# 创建生成器对象
generator = countdown(5)
 
# 通过迭代生成器获取值
print(next(generator))  # 输出: 5
print(next(generator))  # 输出: 4
print(next(generator))  # 输出: 3
 
# 使用 for 循环迭代生成器
for value in generator:
    print(value)  # 输出: 2 1


"""with关键字"""
# with 关键字依靠的是python的上下文管理协议，该协议要求对象必须实现__enter__和__exit__方法
# __enter__方法进入上下文时调用，返回值赋值给as后面的变量
# __exit__方法退出上下文时调用，无论是否发生异常都会调用
with open('test.py', 'r') as file:
    content = file.read()
    print(content)
# 结束后文件自动关闭


# 如果想要自定义一个上下文管理器，要么实现一个类，定义__enter__和__exit__方法，要么使用contextlib模块中的contextmanager装饰器来简化上下文管理器的创建
# python中的上下文管理器采用的就是RAII（资源获取即初始化）的设计模式，确保资源在使用完毕后能够正确释放，避免资源泄漏和其他潜在问题。
# __enter__ 就类似于C++中的构造函数，申请资源
# __exit__ 就类似于C++中的析构函数，释放资源
import time
from contextlib import contextmanager

@contextmanager
def timer(name):
    start = time.time()
    print(f"[{name}] 开始执行...")
    
    # 这里的 yield 返回了一个字典，可以在 as 后面接收
    state = {"status": "running"}
    yield state  
    
    end = time.time()
    print(f"[{name}] 执行完毕，耗时: {end - start:.4f} 秒。状态: {state['status']}")

# 使用示例
with timer("数据库查询") as t:
    time.sleep(0.5) # 模拟耗时操作
    print("正在查询...")
    t["status"] = "success" # 在 with 内部修改从 yield 拿到的状态对象


"""python 函数传参"""
"""python 传参是根据可变类型还是不可变类型而言的"""
# 对于可变类型来说，list、dict、set等，他们作为参数传递给函数类似于C++的传引用
# 对于不可变类型来说，他们创建后就由指定变量来管理，如果变量指向了其他值，则原来的内容就被回收了
def change(a):
    print(id(a))   # 指向的是同一个对象
    a=10
    print(id(a))   # 一个新对象
 
a=1
print(id(a))
change(a)


"""不定长参数，在定义不定参函数时，如果不定参使用的是*args，则在函数内部收到后就是一个tuple；如果不定参使用的是**kwargs，则在函数内部收到后就是一个dict"""
def func(args, **kwargs):
    print("arg:", args)
    print("kwargs:", kwargs)
def func1(args, *kwargs):
    print("arg:", args)
    print("kwargs:", kwargs)
func(1, a=2, b=3) # 因为参数会以字典的形式传递，所以要指定key和value
func1(1, 3, 5, 7, 9)

"""如果在函数列表中*单独出现，则*后面的参数必须以关键字参数的形式传递"""
def func2(a, b, *, c=None):
    print(a, b, c)
func2(1, 2) # 如果c没有默认参数直接调用就会报错
func2(1, 2, c=3)


"""装饰器"""
from functools import wraps
# python中的装饰器本质上就是接受一个函数，返回一个增强版的函数。
# 一个标准的自定义包装器如下，有外层函数加内层wraper组成。wrapper的作用主要是为了避免直接调用原函数，起到了延迟调用的作用。
# wrapper的参数使用完美转发的模式，可以接受任意类型的参数，这样的装饰器就可以做到通用性
def decorator(func):
    @wraps(func) # 使用wraps装饰器可以让增强版函数保持原函数的元信息，如函数名、文档字符串等
    def wrapper(*args, **kwargs):
        print("函数执行前的操作")
        result = func(*args, **kwargs)  # 调用原函数
        print("函数执行后的操作")
        return result
    return wrapper

@decorator
def say_hello(name):
    print(f"Hello, {name}!")

say_hello("Alice")
print(say_hello.__name__)


"""类装饰器 TODO"""


"""闭包 就是在内层函数中捕获外层函数的变量。当函数销毁后，被内层函数捕获的变量不会被释放，可以修改。"""
# 闭包需要借助nonlocal关键字来修改外层函数的变量
# 对于闭包来说，需要满足一下几个要求才能算作闭包：
# 1. 必须有一个内嵌函数（函数里套函数）。
# 2. 内嵌函数必须引用了外部函数的变量（不能是全局变量，必须是包着它的那个外层函数的局部变量）。
# 3. 外部函数的返回值必须是这个内嵌函数。
def outer(name):
    # 条件 1：有内嵌函数
    def inner():
        # 条件 2：用到了外层函数的变量 name
        print(f"Hello, {name}") 
        
    # 条件 3：把带着 name 的 inner 送了出去
    return inner 

say_hello = outer("Alice") 
say_hello()  # 输出 Hello, Alice
print(say_hello.__closure__)  # 输出 (<cell at 0x7f8c8c8c8c8c: str object at 0x7f8c8c8c8c8c>)