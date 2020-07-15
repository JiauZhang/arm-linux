# code visualization aka cviz
class Node():
    def __init__(self, name='', child='', slibing=''):
        self.name_ = name
        self.child_ = child
        self.slibing_ = slibing

    def name(self):
        return self.name_

class Stack():

    def __init__(self):
     # 创建空列表实现栈
        self.__list = []

    def is_empty(self):
    # 判断是否为空
        return self.__list == []
    def push(self,item):
    # 压栈，添加元素
        self.__list.append(item)

    def pop(self):
    # 弹栈，弹出最后压入栈的元素
        if self.is_empty():
            return 
        else:
            return self.__list.pop()

    def top(self): 
    # 取最后压入栈的元素
        if self.is_empty():
            return
        else:
            return self.__list[-1]


if __name__ == "__main__":
    root = Node(name='start')
    print("root: {}".format(root.name()))
