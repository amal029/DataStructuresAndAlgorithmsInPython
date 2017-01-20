import abc


class SQ(abc.ABC):

    """This is the abstract class that should be implemented by the Stack
    and Queue ADTs.

    """
    @abc.abstractmethod
    def push(self, x):
        raise NotImplementedError('Should be implemented by the ADTs')

    @abc.abstractmethod
    def pop(self):
        raise NotImplementedError('Should be implemented by the ADTs')

    @abc.abstractmethod
    def __str__(self):
        raise NotImplementedError('Should be implemented by the ADTs')

    @abc.abstractmethod
    def __iter__(self):
        raise NotImplementedError('Should be implemented by the ADTs')

    @abc.abstractmethod
    def peek(self):
        raise NotImplementedError('Should be implemented by the ADTs')

    @abc.abstractmethod
    def isEmpty(self):
        raise NotImplementedError('Should be implemented by the ADTs')


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

    def isEmpty(self):
        return self.l == []

    def __iter__(self):
        return iter(self.l)


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

    def __iter__(self):
        return iter(self.l)

    def isEmpty(self):
        return self.l == []

    def sort(self, key=None):
        self.l.sort(key=key)
