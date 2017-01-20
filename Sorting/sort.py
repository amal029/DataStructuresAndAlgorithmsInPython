import random
import math


def insertionSort(ll=[]):
    """The in-place insertionSort algorithm

    """
    for i in range(1, len(ll)):
        for j in range(0, i):
            if ll[i] < ll[j]:
                t = ll[i]
                for k in range(i, j, -1):
                    ll[k] = ll[k-1]
                ll[j] = t
                break
    return ll


def mergeSort(ll=[]):
    """The merge sort algorithm
    """
    if len(ll) <= 1:
        return ll
    else:
        lh = mergeSort(ll[0: math.ceil(len(ll)/2)])
        rh = mergeSort(ll[math.ceil(len(ll)/2): len(ll)])
        # Now merge the returned lists
        ll = []
        while lh != [] and rh != []:
            if lh[0] <= rh[0]:
                ll.append(lh.pop(0))
            else:
                ll.append(rh.pop(0))
        ll += lh
        ll += rh
        return ll


def quickSort(ll=[]):
    """The out of place quick sort algorithm
    """
    if len(ll) <= 1:
        return ll
    else:
        pp = ll[len(ll)-1]
        lp = [x for x in ll if x < pp]
        le = [x for x in ll if x == pp]
        lg = [x for x in ll if x > pp]
        return (quickSort(lp) + le + quickSort(lg))


if __name__ == '__main__':
    ll = random.sample(range(100), 5)
    print('Before sorting:', ll)
    print('After Insertion sort:', insertionSort(ll))
    print('After Merge sort:', mergeSort(ll))
    print('After Quick sort:', quickSort(ll))
