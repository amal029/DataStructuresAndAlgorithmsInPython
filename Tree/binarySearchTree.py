import binaryTree as BT


class BinarySearchTree(BT.manipulateBinaryTree):
    """Inherits from a Binary Tree
    """

    def __init__(self):
        """The constructor for the class, does nothing

        """
        super().__init__(None)

    @staticmethod
    def __find(k, v):
        ret = None
        if v is not None:
            (vk, vx) = v.root().getElement()
        else:
            return ret
        if vk == k:
            ret = v.root()
        elif k < vk:
            ret = BinarySearchTree.__find(k, v.root().getLeftSubTree())
        else:
            ret = BinarySearchTree.__find(k, v.root().getRightSubTree())
        return ret

    def find(self, k):
        """Find a single key in the binary search tree

        """
        ret = BinarySearchTree.__find(k, self._tree)
        if ret is None:
            raise RuntimeError('Key:', k, 'not found!')
        else:
            return ret

    def insert(self, k, x):
        """Insert (k, x) in the tree

        """
        def firstExternalNode(k, v):
            ret = None
            (vk, vx) = v.root().getElement()
            if vk == k:
                raise RuntimeError('Duplicate key:', k,
                                   'cannot be inserted')
            elif k < vk:
                lst = v.root().getLeftSubTree()
                if lst is None:
                    ret = (v.root(), True)
                else:
                    ret = firstExternalNode(k, lst)
            else:
                rst = v.root().getRightSubTree()
                if rst is None:
                    ret = (v.root(), rst)
                else:
                    ret = firstExternalNode(k, rst)
            return ret

        if self._tree is None:
            self._tree = BT.BinaryTree()
            self.addRoot((k, x))

        else:
            (ve, l) = firstExternalNode(k, self._tree)
            if l:
                self.insertLeft(ve, (k, x))
            else:
                self.insertRight(ve, (k, x))

    @classmethod
    def fInorderChild(cls, v):
        ret = None
        if v.getLeftSubTree() is None:
            ret = v
        else:
            ret = BinarySearchTree.fInorderChild(v.getLeftSubTree().root())
        return ret

    def remove(self, k):
        """Remove the node with key k. Overwritting the base

        """
        v = self.find(k)
        if v is None:
            raise RuntimeError('Key:', k, 'not in the Tree')
        elif v.getLeftSubTree() is None or v.getRightSubTree() is None:
            # print('Removing: ', v)
            return super().remove(v)
        else:
            # Now remove vertex if it has two children
            temp = v.getElement()
            f_in_order_child = self.fInorderChild(v.getRightSubTree().root())
            super().remove(f_in_order_child)
            v.setElement(f_in_order_child.getElement())
            # Now remove the f_in_order_child
        return temp

    def inOrderTraversal(self, v, func):
        if v.getLeftSubTree() is not None:
            self.inOrderTraversal(v.getLeftSubTree().root(), func)
        func(v)
        if v.getRightSubTree() is not None:
            self.inOrderTraversal(v.getRightSubTree().root(), func)


if __name__ == '__main__':
    bst = BinarySearchTree()
    bst.insert(2, 'a')
    bst.insert(5, 'b')
    bst.insert(1, 'c')
    bst.inOrderTraversal(bst._tree.root(), print)

    # Printing the tree
    # print(bst._tree)

    print('tutu:', bst.find(1))

    bst.remove(1)
    print('After removing key: 1')
    bst.inOrderTraversal(bst._tree.root(), print)

    bst.remove(2)
    print('After removing key: 2')
    bst.inOrderTraversal(bst._tree.root(), print)

    # Printing the tree
    # print(bst._tree)
