import SQ


class Stack(SQ):
    """
    This is the class that implements the Stack (LIFO) ADT

    """

    def __init__(self):
        self.l = []

    def push(self, x):
        self.l.append(x)

    def pop(self):
        return self.l.pop()

    def peek(self):
        return self.l[len(self.l)-1]

    def __str__(self):
        return str(self.l)
