import abc


class TreeInterface(abc.ABC):

    @abc.abstractmethod
    def element(self):
        raise NotImplementedError('ADTs should implement this method')

    @abc.abstractmethod
    def root(self):
        raise NotImplementedError('ADTs should implement this method')

    @abc.abstractmethod
    def parent(self, v):
        raise NotImplementedError('ADTs should implement this method')

    @abc.abstractmethod
    def children(self, v):
        raise NotImplementedError('ADTs should implement this method')

    @abc.abstractmethod
    def __str__(self):
        raise NotImplementedError('ADTs should implement this method')

    @abc.abstractmethod
    def __iter__(self):
        raise NotImplementedError('ADTs should implement this method')
