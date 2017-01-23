class BinaryTree:
    """This is the binary tree ADT

    """

    def __init__(self, root=None):
        self._root = root

    def element(self):
        return self.element

    def root(self):
        return self._root

    def setRoot(self, r):
        self._root = r

    def parent(self, v):
        if v == self._root:
            raise RuntimeError('Root has no parent')
        else:
            # Search the children to get the node
            v = BinaryTree._getNode(self._root, v)
            return v.getParent()

    def children(self, v):
        if v == self._root:
            return [self._root.getLeftSubTree(), self._root.getRightSubTree()]
        else:
            r = BinaryTree._getNode(self._root, v)
            return [r.getLeftSubTree(), r.getRightSubTree()]

    def __str__(self):
        return str(BinaryTree.__getAllNodes(self._root))

    # The tree traversal methods
    @classmethod
    def _getNode(cls, root, v):
        ret = None
        # print('PIPI:', root, v)
        if root == v:
            ret = root
        if root.getLeftSubTree() is not None and ret is None:
            ret = BinaryTree._getNode(root.getLeftSubTree().root(), v)
        if root.getRightSubTree() is not None and ret is None:
            ret = BinaryTree._getNode(root.getRightSubTree().root(), v)
        return ret

    @classmethod
    def __getAllNodes(cls, root):
        """
        Does DFS traversal
        """
        ll = []
        BinaryTree.dfs(ll, root, func='__str__')
        return ll

    @staticmethod
    def dfs(ll, root, func=None):
        res = root
        if func is not None:
            res = getattr(root, func)()
        ll.append(res)
        if root.getLeftSubTree() is not None:
            BinaryTree.dfs(ll, root.getLeftSubTree().root(), func)
        if root.getRightSubTree() is not None:
            BinaryTree.dfs(ll, root.getRightSubTree().root(), func)


class Node:
    """
    This is the node of the Binary Tree
    """
    def __init__(self, element, parent=None, lc=None, rc=None):
        self._parent = parent
        self._element = element
        self._lc = lc
        self._rc = rc
        self._height = None

    def setParent(self, parent):
        self._parent = parent

    def setLeftSubTree(self, lc):
        self._lc = lc

    def setRightSubTree(self, rc):
        self._rc = rc

    def getLeftSubTree(self):
        return self._lc

    def getRightSubTree(self):
        return self._rc

    def getParent(self):
        return self._parent

    def __str__(self):
        return str(self._element)

    def getElement(self):
        return self._element

    def setElement(self, e):
        self._element = e


class manipulateBinaryTree(object):
    def __init__(self, tree):
        self._tree = tree

    def addRoot(self, e):
        if self._tree.root() is None:
            self._tree.setRoot(Node(e))
        else:
            raise RuntimeError('Tree is not empty')

    def insertLeft(self, v, e):
        vv = BinaryTree._getNode(self._tree.root(), v)
        if vv is not None:
            if vv.getLeftSubTree() is None:
                # Add
                w = Node(e, parent=vv)
                vv.setLeftSubTree(BinaryTree(w))
                return w
            else:
                raise RuntimeError('Node:', v, 'already has a left sub-tree')
        else:
            raise RuntimeError('Node:', v, 'not in the tree')

    def insertRight(self, v, e):
        vv = BinaryTree._getNode(self._tree.root(), v)
        if vv is not None:
            if vv.getRightSubTree() is None:
                # Add
                w = Node(e, parent=vv)
                vv.setRightSubTree(BinaryTree(w))
                return w
            else:
                raise RuntimeError('Node:', v.__str__(),
                                   'already has a right sub-tree')
        else:
            raise RuntimeError('Node:', v.__str__(), 'not in the tree')

    def remove(self, v):
        vv = BinaryTree._getNode(self._tree.root(), v)
        if vv is not None:
            element = vv.getElement()
            lst = vv.getLeftSubTree()
            rst = vv.getRightSubTree()
            if vv == self._tree.root():
                if lst is not None and rst is not None:
                    raise RuntimeError('Node', v, 'has two children')
                elif lst is not None:
                    lst.root().setParent(None)
                    self._tree.setRoot(lst.root())
                elif rst is not None:
                    rst.root().setParent(None)
                    self._tree.setRoot(rst.root())
            else:
                # Get the parent
                vp = vv.getParent()
                if lst is None and rst is None:
                    if vp.getLeftSubTree().root() == vv:
                        vp.setLeftSubTree(lst)
                    else:
                        vp.setRightSubTree(rst)
                elif lst is not None and rst is None:
                    lst.root().setParent(vp)
                    if vp.getLeftSubTree().root() == vv:
                        vp.setLeftSubTree(lst)
                    else:
                        vp.setRightSubTree(lst)
                elif lst is None and rst is not None:
                    rst.root().setParent(vp)
                    if vp.getLeftSubTree().root() == vv:
                        vp.setLeftSubTree(rst)
                    else:
                        vp.setRightSubTree(rst)
                else:
                    raise RuntimeError('Node', v, 'has two children')
            return element
        else:
            raise RuntimeError('Node', v, 'is not in the tree')

    def attach(self, v, t1, t2):
        vv = BinaryTree._getNode(self._tree.root(), v)
        if vv is not None:
            if vv.getLeftSubTree() is None and vv.getRightSubTree() is None:
                vv.setLeftSubTree(t1)
                vv.setRightSubTree(t2)
                # Now set the parent of t1's root and t2's root
                t1.root().setParent(vv)
                t2.root().setParent(vv)
            else:
                raise RuntimeError('Node:', v, 'is not an external node')
        else:
            raise RuntimeError('Node', v, 'is not in the tree')
