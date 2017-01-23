from binarySearchTree import BinarySearchTree
from binaryTree import BinaryTree
import sys
sys.path.append('../Simple')
from SQ import Queue


class AVLTree(BinarySearchTree):
    def __init__(self):
        super().__init__()

    def updateTreeHeights(self):
        # First get all the external nodes in the tree
        ll = []
        BinaryTree.dfs(ll, self._tree.root())
        for x in ll:
            x._height = None

        # These are all the external nodes
        q = Queue()
        [q.push(x) for x in ll
         if x.getLeftSubTree() is None and x.getRightSubTree() is None]
        for x in q:
            x._height = 0

        # Now process the tree upwards
        while not q.isEmpty():
            v = q.pop()         # The vertex to process
            vp = v.getParent()
            if vp is not None:
                lsth, rsth = None, None
                push = False
                if vp.getLeftSubTree() is None:
                    push = True
                    lsth = 0
                elif vp.getLeftSubTree().root()._height is not None:
                    lsth = vp.getLeftSubTree().root()._height
                    push = True
                else:
                    continue
                if vp.getRightSubTree() is None:
                    push = True
                    rsth = 0
                elif vp.getRightSubTree().root()._height is not None:
                    push = True
                    rsth = vp.getRightSubTree().root()._height
                else:
                    continue
                if push:
                    vp.root()._height = max(lsth, rsth) + 1
                    q.push(vp)

    def restructure(self, z, y, x):
        a, b, c = None, None, None,
        t0, t1, t2, t3 = None, None, None, None

        def assign(v):
            nonlocal a, b, c, t0, t1, t2, t3
            lst = v.getLeftSubTree()
            if lst is not None:
                assign(lst.root())
            # Do some computation on z
            if a is None:
                if v == x or v == y or v == z:
                    a = v
                elif t0 is None:
                    t0 = v
                elif t1 is None:
                    t1 = v
                elif t2 is None:
                    t2 = v
                elif t3 is None:
                    t3 = v
            elif b is None:
                if v == x or v == y or v == z:
                    b = v
                elif t0 is None:
                    t0 = v
                elif t1 is None:
                    t1 = v
                elif t2 is None:
                    t2 = v
                elif t3 is None:
                    t3 = v
            elif c is None:
                if v == x or v == y or v == z:
                    c = v
                elif t0 is None:
                    t0 = v
                elif t1 is None:
                    t1 = v
                elif t2 is None:
                    t2 = v
                elif t3 is None:
                    t3 = v
            rst = v.getRightSubTree()
            if rst is not None:
                assign(rst.root())

        assign(z)

        # Now the actual restructuring
        # step-0
        zp = z.getParent()
        if zp is not None:
            isLeft = zp.getLeftSubTree().root() == z

        # step-1
        b.setParent(zp)
        if zp is not None and isLeft:
            zp.getLeftSubTree().setRoot(b)
        elif zp is not None:
            zp.getRightSubTree().setRoot(b)

        b.setLeftSubTree(AVLTree())
        b.getLeftSubTree().setRoot(a)
        b.setLeftSubTree(AVLTree())
        b.getLeftSubTree().setRoot(c)

        a.setParent(b)
        a.setLeftSubTree(t0)
        if t0 is not None:
            t0.setParent(a)
        a.setRightSubTree(t1)
        if t1 is not None:
            t1.setParent(a)

        c.setParent(b)
        c.setLeftSubTree(t2)
        if t2 is not None:
            t2.setParent(c)
        c.setRightSubTree(t3)
        if t3 is not None:
            t3.setParent(c)

        return b

    def insert(self, k, x):
        # First insert in the binarySearchTree
        super().insert(k, x)    # O(log n)

        # update the heights of the tree
        self.updateTreeHeights()

        # Now check if it is balanced
        w = self.find(k)        # O(log n)

        def firstImbalancedParent(v):
            ret = None
            vp = v.getParent()
            if vp is not None:
                lsth, rsth = None, None
                if vp.getLeftSubTree() is None:
                    lsth = 0
                else:
                    lsth = vp.getLeftSubTree().root()._height
                if vp.getRightSubTree() is None:
                    rsth = 0
                else:
                    rsth = vp.getRightSubTree().root()._height
                if abs(lsth - rsth) > 1:
                    ret = [vp]
                else:
                    ret = firstImbalancedParent(vp)
                    if ret is not None:
                        ret.append(vp)  # Return things in order

            return ret

        parents = firstImbalancedParent(w)
        if parents is not None:
            # If there is an imbalance then restructure, else done!
            if len(parents) != 3:
                raise RuntimeError('Something went wrong restructuring: ',
                                   print(w), 'unbalanced parents:',
                                   print(parents))
            else:
                self.restructure(parents[0], parents[1], parents[2])

    def remove(self, k):
        v = self.find(k)

        # Step-1: get w
        w = None
        if v.getLeftSubTree() is not None and v.getRightSubTree is not None:
            w = self.fInorderChild(v.getRightSubTree().root()).getParent()
        else:
            w = v.getParent()

        # Delete v from the tree
        super().remove(k)
        # Update the heights
        self.updateTreeHeights()

        def firstImbalancedParent(v):
            ret = None
            vp = v.getParent()
            if vp is not None:
                if vp.getLeftSubTree() is None:
                    lsth = 0
                else:
                    lsth = vp.getLeftSubTree().root()._height
                if vp.getRightSubTree() is None:
                    rsth = 0
                else:
                    rsth = vp.getRightSubTree().root()._height
                if abs(lsth - rsth) > 1:
                    ret = [vp]
                    vpcl, vpcr = 0, 0
                    if lsth > rsth:
                        ret.append(vp.getLeftSubTree().root())
                        if (vp.getLeftSubTree().
                            root().getLeftSubTree() is not None):
                            vpcl = (vp.getLeftSubTree().
                                    root().getLeftSubTree().root()._height)
                        if (vp.getLeftSubTree().
                            root().getRightSubTree() is not None):
                            vpcr = (vp.getLeftSubTree().
                                    root().getRightSubTree().root()._height)
                        if vpcl > vpcr:
                            ret.append(vp.getLeftSubTree().
                                       root().getLeftSubTree().root())
                        elif vpcr > vpcl:
                            ret.append(vp.getLeftSubTree().
                                       root().getRightSubTree().root())
                        else:
                            ret.append(vp.getLeftSubTree().
                                       root().getLeftSubTree().root())

                    else:
                        ret.append(vp.getRightSubTree().root())
                        if (vp.getRightSubTree().
                            root().getLeftSubTree() is not None):
                            vpcl = (vp.getRightSubTree().
                                    root().getLeftSubTree().root()._height)
                        if (vp.getRightSubTree().
                            root().getRightSubTree() is not None):
                            vpcr = (vp.getRightSubTree().
                                    root().getRightSubTree().root()._height)
                        if vpcl > vpcr:
                            ret.append(vp.getRightSubTree().
                                       root().getLeftSubTree().root())
                        elif vpcr > vpcl:
                            ret.append(vp.getRightSubTree().
                                       root().getRightSubTree().root())
                        else:
                            ret.append(vp.getRightSubTree().
                                       root().getRightSubTree().root())
                else:
                    ret = firstImbalancedParent(vp)
            return ret

        # Now restructure parents
        if w is not None:
            # Get the unbalanced parents
            parents = firstImbalancedParent(w)
            while parents is not None:
                if len(parents) != 3:
                    raise RuntimeError('Something went wrong restructuring: ',
                                       print(w), 'unbalanced parents:',
                                       print(parents))
                else:
                    w = self.restructure(parents[0], parents[1], parents[2])
                    # Now we might have b's ancestor as unbalanced.
                    parents = firstImbalancedParent(w)


#  LocalWords:  binarySearchTree LocalWords
