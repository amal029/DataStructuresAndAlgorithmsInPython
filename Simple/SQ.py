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
    def peek(self):
        raise NotImplementedError('Should be implemented by the ADTs')
