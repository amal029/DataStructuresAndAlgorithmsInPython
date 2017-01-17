class Dequeue:


    def __init__(self):
        self.l = []


    def push_front(self, x):
        self.l.insert(0,x)

    def push_end(self, x):
        self.l.append(x)

    def pop_front(self):
        return self.l.pop(0)

    def pop_end(self):
        return self.l.pop(len(self.l)-1)

    def __str__(self):
        return str(self.l)


