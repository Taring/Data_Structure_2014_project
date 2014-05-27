/** @file */
#ifndef __LINKEDLIST_H
#define __LINKEDLIST_H

#include "IndexOutOfBound.h"
#include "ElementNotExist.h"

/**
 * A linked list.
 *
 * The iterator iterates in the order of the elements being loaded into this list.
 */
template <class T>
class LinkedList {
private:
    struct Node {
        T data;
        Node *succ, *prec;
        Node() {
            succ = NULL, prec = NULL;
        }
        ~Node(){}
    };

    /*
     * &var start is the beginning of LinkedList
     * Note: the linked list is the cycle bidirectional list by insert node &var start
     * start->succ = first element of LinkedList
     * start->prec = last element of LinkedList
     */
    Node *start;
    int Size;

    /*
     * Clear the LinkedList
     */
    void makeEmpty() {
        Size = 0;
        Node *p, *q;
        p = start->succ;
        start->succ = start->prec = start;
        while (p !=  start) {
            q = p->succ; delete p; p = q;
        }
    }

    /*
     * TODO insert an element behind the element : e
     * All insert operation is based addAll
     */
    void addAll(Node *Pre, const T& e) {
        Node *Suc = Pre->succ;
        Node *p = new Node;
        p->data = e;
        p->succ = Suc; Suc->prec = p;
        p->prec = Pre; Pre->succ = p;
        ++Size;
    }

    /*
     * TODO delete the element : p
     * All delete operation is based removeAll
     */
    void removeAll(const Node *p) {
        Node *Suc = p->succ;
        Node *Pre = p->prec;
        Pre->succ = Suc; Suc->prec = Pre;
        delete p;
        --Size;
    }

public:

    class Iterator;

    /**
     * TODO Constructs an empty linked list
     */
    LinkedList() {
        start = new Node;
        start->succ = start->prec = start;
        Size = 0;
    }

    /**
     * TODO Copy constructor
     */
    LinkedList(const LinkedList<T> &c) {
        Size = 0;
        start = new Node;
        start->succ = start->prec = start;
        for (Node *k = c.start->succ; k != c.start; k = k->succ)
            addAll(start->prec, k->data);
    }

    /**
     * TODO Assignment operator
     */
    LinkedList<T>& operator=(const LinkedList<T> &c) {
        makeEmpty();
        Size = 0;
        start->succ = start->prec = start;
        for (Node *k = c.start->succ; k != c.start; k = k->succ)
            addAll(start->prec, k->data);    
    }

    /**
     * TODO Desturctor
     */
    ~LinkedList() {
        makeEmpty();
        delete start;
    }

    /**
     * TODO Appends the specified element to the end of this list.
     * Always returns true.
     */
    bool add(const T& e) {
        addAll(start->prec, e);
        return true;
    }

    /**
     * TODO Inserts the specified element to the beginning of this list.
     */
    void addFirst(const T& elem) {
        addAll(start, elem);
    }

    /**
     * TODO Insert the specified element to the end of this list.
     * Equivalent to add.
     */
    void addLast(const T &elem) {
        addAll(start->prec, elem);
    }

    /**
     * TODO Inserts the specified element to the specified position in this list.
     * The range of index parameter is [0, Size], where index=0 means inserting to the head,
     * and index=Size means appending to the end.
     * @throw IndexOutOfBound
     */
    void add(int index, const T& element) {
        if (index < 0 || index > Size) throw IndexOutOfBound();
        Node *p = start;
        while (index--) p = p->succ;
        addAll(p, element);
    }

    /**
     * TODO Removes all of the elements from this list.
     */
    void clear() {
        makeEmpty();
    }

    /**
     * TODO Returns true if this list contains the specified element.
     */
    bool contains(const T& e) const {
        for (Node *p = start->succ; p != start; p = p->succ)
            if (p->data == e) return true;
        return false;
    }

    /**
     * TODO Returns a const reference to the element at the specified position in this list.
     * The index is zero-based, with range [0, Size).
     * @throw IndexOutOfBound
     */
    const T& get(int index) const {
        if (index < 0 || index >= Size) throw IndexOutOfBound();
        Node *p = start->succ;
        while (index--) p = p->succ;
        return p->data;
    }

    /**
     * TODO Returns a const reference to the first element.
     * @throw ElementNotExist
     */
    const T& getFirst() const {
        if (start->succ == start) throw ElementNotExist();
        return start->succ->data;
    }

    /**
     * TODO Returns a const reference to the last element.
     * @throw ElementNotExist
     */
    const T& getLast() const {
        if (start->prec == start) throw ElementNotExist();
        return start->prec->data;
    }

    /**
     * TODO Returns true if this list contains no elements.
     */
    bool isEmpty() const {
        return Size == 0;
    }

    /**
     * TODO Removes the element at the specified position in this list.
     * The index is zero-based, with range [0, Size).
     * @throw IndexOutOfBound
     */
    void removeIndex(int index) {
        if (index < 0 || index >= Size) throw IndexOutOfBound();
        Node *p = start->succ;
        while (index--) p = p->succ;
        removeAll(p);   
    }

    /**
     * TODO Removes the first occurrence of the specified element from this list, if it is present.
     * Returns true if it was present in the list, otherwise false.
     */
    bool remove(const T &e) {
        for (Node *p = start->succ; p != start; p = p->succ)
            if (p->data == e) {
                removeAll(p);
                return true;
            }
        return false;        
    }

    /**
     * TODO Removes the first element from this list.
     * @throw ElementNotExist
     */
    void removeFirst() {
        if (start->succ == start) throw ElementNotExist();
        removeAll(start->succ);
    }

    /**
     * TODO Removes the last element from this list.
     * @throw ElementNotExist
     */
    void removeLast() {
        if (start->prec == start) throw ElementNotExist();
        removeAll(start->prec);        
    }

    /**
     * TODO Replaces the element at the specified position in this list with the specified element.
     * The index is zero-based, with range [0, Size).
     * @throw IndexOutOfBound
     */
    void set(int index, const T &element) {
        if (index < 0 || index >= Size)  throw IndexOutOfBound();
        Node *p = start->succ;
        while (index--) p = p->succ;
        p->data = element;
    }

    /**
     * TODO Returns the number of elements in this list.
     */
    int size() const {
        return Size;
    }

    /**
     * TODO Returns an iterator over the elements in this list.
     */
    Iterator iterator() {
        return Iterator(this);
    }
};

template <class T>
class LinkedList<T>::Iterator {
    private:
        Node *pos;
        LinkedList *container;
        bool dead;
    public:

        Iterator() {}
        Iterator(LinkedList *con) :pos(con->start), container(con), dead(false) {}

        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
            return pos->succ != container->start;
        }

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next() {
            if (!hasNext()) throw ElementNotExist();
            if (dead) dead = false;
            pos = pos->succ;
            return pos->data;
        }

        /**
         * TODO Removes from the underlying collection the last element
         * returned by the iterator
         * The behavior of an iterator is unspecified if the underlying
         * collection is modified while the iteration is in progress in
         * any way other than by calling this method.
         * @throw ElementNotExist
         */
        void remove() {
            if (pos == container->start || dead) throw ElementNotExist();
            dead = true;
            Node *p = pos->prec;
            container->removeAll(pos);
            pos = p;
        }
    };


#endif
