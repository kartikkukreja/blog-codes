class BST:
    def __init__(self, data, left, right):
        self.data = data
        self.left = left
        self.right = right

def inorderFromStart(bst):
    stack = [bst]
    while bst.left is not None:
        bst = bst.left
        stack.append(bst)
    while stack:
        top = stack.pop()
        if top.right is not None:
            bst = top.right
            stack.append(bst)
            while bst.left is not None:
                bst = bst.left
                stack.append(bst)
        yield top

def inorderFromEnd(bst):
    stack = [bst]
    while bst.right is not None:
        bst = bst.right
        stack.append(bst)
    while stack:
        top = stack.pop()
        if top.left is not None:
            bst = top.left
            stack.append(bst)
            while bst.right is not None:
                bst = bst.right
                stack.append(bst)
        yield top

def twoSum(bst, k):
    if bst is None: return None
    bstStart = inorderFromStart(bst)
    bstEnd = inorderFromEnd(bst)
    start, end = bstStart.next(), bstEnd.next()
    while start != end:
        if start.data + end.data == k: return (start.data, end.data)
        elif start.data + end.data > k: end = bstEnd.next()
        else: start = bstStart.next()
    return None

if __name__ == '__main__':
    bst = BST(5, BST(3, BST(1, None, BST(2, None, None)), BST(4, None, None)), BST(7, BST(6, None, None), BST(8, None, BST(10, BST(9, None, None), None))))
    print twoSum(bst, 10)
