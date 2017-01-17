import SQ


class Queue(SQ):

    """

    This is the class that implements the Queue (FIFO) ADT

    """

    def __init__(self):
        self.l = []

    def push(self, x):
        self.l.append(x)

    def pop(self):
        return self.l.pop(0)

    def peek(self):
        return self.l[0]

    def __str__(self):
        return str(self.l)
