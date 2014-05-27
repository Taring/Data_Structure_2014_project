/** @file */
#ifndef __PRIORITYQUEUE_H
#define __PRIORITYQUEUE_H

#include "ArrayList.h"
#include "ElementNotExist.h"

/**
 * This is a priority queue based on a priority priority queue. The
 * elements of the priority queue are ordered according to their 
 * natual ordering (operator<), or by a Comparator provided as the
 * second template parameter.
 * The head of this queue is the least element with respect to the
 * specified ordering (different from C++ STL).
 * The iterator does not return the elements in any particular order.
 * But it is required that the iterator will eventually return every
 * element in this queue (even if removals are performed).
 */

/*----------------------------------------------------------------------*/
/**
 * Default Comparator with respect to natural order (operator<).
 */
template <class V>
class Less
{
public:
    bool operator()(const V& a, const V& b) { return a < b; }
};

/**
 * To use this priority queue, users need to either use the
 * default Comparator or provide their own Comparator of this
 * kind.
 * The Comparator should be a class with a public function
 * public: bool operator()(const V&, const V&);
 * overriding the "()" operator.
 * The following code may help you understand how to use a
 * Comparator and, especially, why we override operator().
 */

// #include <iostream>
// using namespace std;
// 
// template <class T, class C = Less<T> >
// class Example
// {
// private:
//     C cmp;
// public:
//     bool compare(const T& a, const T& b)
//     {
//         return cmp(a, b);
//     }
// };
// 
// int main()
// {
//     Example<int, Less<int> > example; // Less<int> can be omitted.
//     cout << example.compare(1, 2) << endl;
//     cout << example.compare(2, 1) << endl;
// }

/*----------------------------------------------------------------------*/

template <class V, class C = Less<V> >
class PriorityQueue
{
private:
    
    /*
     * Node is the node of Linkedlist
     * Node->to means this node's position in Heap
     */
    struct Node{
        Node *prev, *succ;
        V v;
        int to;
        Node():prev(NULL), succ(NULL) {}
        Node(V _v, int t):v(_v), prev(NULL), succ(NULL), to(t) {}
    };
    
    C cmp;
    Node* *data;
    int Size, save_size;
    Node* begin;

    /*
     * double the size of *data
     */
    inline void doubleSpace() {
        save_size *= 2;
        Node* *new_data = new Node*[save_size];
        for (int i = 1; i <= Size; ++i)
            new_data[i] = data[i];
        delete[] (data);
        data = new_data;
    }

    /*
     * clear the LinkedList & Heap
     */
    inline void removeAll() {
        for (Node *x, *k = begin->succ; k != begin; k = x) {
            x = k->succ;
            delete k;
        }
        Size = 0;
    }

    /*
     * Create the LinkedList by using heap order
     */
    inline void SortedLink() {
        begin->succ = begin->prev = begin;
        if (Size > 0) {
            begin->succ = data[1]; data[1]->prev = begin;
            for (int i = 2; i <= Size; ++i) {
                data[i - 1]->succ = data[i];
                data[i]->prev = data[i - 1];
            }
            data[Size]->succ = begin; begin->prev = data[Size]; 
        }
    }

    /*
     * Swap two pointers x and y
     */
    inline void Swap(Node* &x, Node* &y) {
        Node *z = x;
        x = y;
        y = z;
    }

    /*
     * Swap two integers x and y
     */
    inline void Swap(int &x, int &y) {
        int z = x; x = y; y = z;
    }

    /*
     * Adjust the element x upward 
     */
    inline void heapup(int x) {
        for (;x > 1;) {
            int y = (x >> 1);
            if( cmp(data[x]->v, data[y]->v) ) {
                Swap(data[x]->to, data[y]->to);
                Swap(data[x], data[y]);
                x = y;
            } else break;
        }
    }

    /*
     * Adjust the element x downward 
     */
    inline void heapdown(int x) {
        for(;x + x <= Size; ) {
            int w = (x << 1);
            if (w + 1 <= Size && cmp(data[w + 1]->v, data[w]->v)) ++w;
            if (cmp(data[w]->v, data[x]->v)) {
                Swap(data[x]->to, data[w]->to);
                Swap(data[x], data[w]);
                x = w;
            } else break;
        }
    }

    /*
     * Delete the position &var Index in heap
     */
    inline void Delete(int Index) {
        Node *now = data[Index];
        Node *Pre = now->prev, *Suc = now->succ;
        Pre->succ = Suc; Suc->prev = Pre;

        if (Index == Size) {
            Size--;
            delete now; return;
        }

        data[Index] = data[Size];
        data[Index]->to = Index;
        data[Size--] = NULL;
        delete now;

        heapup(Index);
        heapdown(Index);
    }

public:
    class Iterator {

    PriorityQueue *container;
    Node *pos;
    bool dead;

    public:

        Iterator() {}
        Iterator(PriorityQueue *con):pos(con->begin), container(con), dead(false) {}

        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
            return pos->succ != container->begin;
        }

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const V &next() {
            if (!hasNext()) throw ElementNotExist();
            if (dead) dead = false;
            pos = pos->succ;
            return pos->v;
        }

		/**
		 * TODO Removes from the underlying collection the last element
		 * returned by the iterator.
		 * The behavior of an iterator is unspecified if the underlying
		 * collection is modified while the iteration is in progress in
		 * any way other than by calling this method.
		 * @throw ElementNotExist
		 */
		void remove() {
            if (pos == container->begin || dead) throw ElementNotExist();

            dead = true;
            Node *p = pos->prev;
            container->Delete(pos->to);
            pos = p;
        }
    };

    /**
     * TODO Constructs an empty priority queue.
     */
    PriorityQueue() { 
        data = new Node*[save_size = 4];
        Size = 0;
        begin = new Node;
        begin->succ = begin->prev = begin;
        cmp = C();
    }

    /**
     * TODO Destructor
     */
    ~PriorityQueue() { 
        removeAll();
        delete[] (data);
        delete begin;
    }

    /**
     * TODO Assignment operator
     */
    PriorityQueue &operator=(const PriorityQueue &x) { 
        if (this != &x) {
            if (save_size != x.save_size) {
                delete[] (data);
                data = new Node*[save_size = x.save_size];
            }
           
            Size = x.Size; 
            for (int i = 1; i <= Size; ++i)
                data[i] = new Node(x.data[i]->v, i);

            SortedLink();
            cmp = x.cmp;
        }
        return (*this);
    }

    /**
     * TODO Copy-constructor
     */
    PriorityQueue(const PriorityQueue &x) { 
        data = new Node*[save_size = x.save_size];
        Size = x.Size;
        for (int i = 1; i <= Size; ++i)
            data[i] = new Node(x.data[i]->v, i);
        begin = new Node;
        SortedLink();
        cmp = x.cmp;
    }

	/**
	 * TODO Initializer_list-constructor
	 * Constructs a priority queue over the elements in this Array List.
     * Requires to finish in O(n) time.
	 */
	PriorityQueue(const ArrayList<V> &x) {
        cmp = C();
        Size = x.size();
        save_size = 4;
        while (save_size < Size + 1)save_size *= 2; 
        data = new Node*[save_size];
        for (int i = 1; i <= Size; ++i)
            data[i] = new Node(x.get(i - 1), i);
        begin = new Node;
        SortedLink();

        for (int i = Size/2; i > 0; --i)
            heapdown(i);
    }

    /**
     * TODO Returns an iterator over the elements in this priority queue.
     */
    Iterator iterator() {
        return Iterator(this);
    }

    /**
     * TODO Removes all of the elements from this priority queue.
     */
    void clear() {
        removeAll();
        begin->succ = begin->prev = begin;
    }

    /**
     * TODO Returns a const reference to the front of the priority queue.
     * If there are no elements, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V &front() const {
        if (Size == 0) throw ElementNotExist();
        return data[1]->v;
    }

    /**
     * TODO Returns true if this PriorityQueue contains no elements.
     */
    bool empty() const {
        return Size == 0;
    }

    /**
     * TODO Add an element to the priority queue.
     */
    void push(const V &value) {
        if (save_size <= Size + 1) doubleSpace();
        ++Size;
        data[Size] = new Node(value, Size);
        /*
        data[++Size] = new Node(value, Size);
        */
        Node *Pre = begin, *Suc = begin->succ;
        data[Size]->prev = Pre; Pre->succ = data[Size];
        data[Size]->succ = Suc; Suc->prev = data[Size];
        heapup(Size);
    }

    /**
     * TODO Removes the top element of this priority queue if present.
     * If there is no element, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void pop() {
        if (Size == 0) throw ElementNotExist();
        Delete(1);
    }

    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const {
        return Size;
    }
};

#endif
