/** @file */
#ifndef __DEQUE_H
#define __DEQUE_H

#include "ElementNotExist.h"
#include "IndexOutOfBound.h"

/**
 * An deque is a linear collection that supports element insertion and removal at both ends.
 * The name deque is short for "double ended queue" and is usually pronounced "deck".
 * Remember: all functions but "contains" and "clear" should be finished in O(1) time.
 *
 * You need to implement both iterators in proper sequential order and ones in reverse sequential order. 
 */
template <class T>
class Deque
{

    int Size, head, tail;
    int save_size;
    T *data;

/*
 * double the space of data 
 * The mid between head and tail ( mid = (head + tail) / 2 ) would be save_size / 2.
 */

    void doubleSpace() {
        save_size *= 2;
        T *new_data = new T[save_size];
        int new_head = save_size / 2 - Size / 2;
        int new_tail = new_head + Size - 1;
        for (int i = head, j = new_head; i <= tail; ++i, ++j)
            new_data[j] = data[i];
        head = new_head;
        tail = new_tail;
        delete[] (data);
        data = new_data; 
    }

    void removeIndex(int direction, int Index) {
        --Size;
        if (direction == 0) {
            --tail;
            for (int i = Index; i <= tail; ++i)
                data[i] = data[i + 1];
        } else {
            ++head;
            for (int i = Index; i >= head; --i)
                data[i] = data[i - 1];
        }
    }

public:
    class Iterator
    {
        int index;
        int direction;
        Deque *container;
        bool dead;
    public:

        Iterator() {}
        Iterator(Deque *con, int x): dead(false), container(con), direction(x){
            if (x == 0) index = container->head - 1; else index = container->tail + 1;
        }

        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
            if (direction == 0) return index <= container->tail - 1 && index >= container->head - 1;
        	else return index >= container->head + 1 && index <= container->tail + 1;
        }

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next() {
            if (!hasNext()) throw ElementNotExist();
            if (dead) dead = false;
            if (direction == 0) return container->data[++index];
            return container->data[--index];
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
            if (index < container->head || container->tail < index || dead) throw ElementNotExist();
            dead = true;
            if (direction == 0) {
                container->removeIndex(0, index--);
            } else container->removeIndex(1, index++);

        }
    };

    /**
     * TODO Constructs an empty deque.
     */
    Deque() { 
        save_size = 4;
        Size = 0;
        data = new T[save_size];
        head = save_size / 2;
        tail = head - 1;
    }

    /**
     * TODO Destructor
     */
    ~Deque() { 
        delete[] (data);
    }

    /**
     * TODO Assignment operator
     */
    Deque& operator=(const Deque& x) { 
        if (this != &x) {
            if (save_size != x.save_size) {
                delete [] (data);
                save_size = x.save_size;
                data = new T[save_size];
            }

            Size = x.Size;
            head = x.head, tail = x.tail;
            for (int i = head; i <= tail; ++i)
                data[i] = x.data[i];
        }
        return (*this);
    }

    /**
     * TODO Copy-constructor
     */
    Deque(const Deque& x) {
        save_size = x.save_size;
        Size = x.Size;
        data = new T[save_size];
        head = x.head, tail = x.tail;
        for (int i = head; i <= tail; ++i)
            data[i] = x.data[i];
    }
	
	/**
	 * TODO Inserts the specified element at the front of this deque. 
	 */
	void addFirst(const T& e) { 
        if (head == 0) doubleSpace();
        ++Size;
        data[--head] = e;
    }

	/**
	 * TODO Inserts the specified element at the end of this deque.
	 */
	void addLast(const T& e) { 
        if (tail == save_size - 1) doubleSpace();
        ++Size;
        data[++tail] = e;
    }

	/**
	 * TODO Returns true if this deque contains the specified element.
	 */
	bool contains(const T& e) const { 
        for (int i = head; i <= tail; ++i)
            if (data[i] == e) return true;
        return false;
    }

	/**
	 * TODO Removes all of the elements from this deque.
	 */
	 void clear() { 
        Size = 0;
        head = save_size / 2;
        tail = head - 1;
     }

	 /**
	  * TODO Returns true if this deque contains no elements.
	  */
	bool isEmpty() const { 
        return Size == 0;
    }

	/**
	 * TODO Retrieves, but does not remove, the first element of this deque.
	 * @throw ElementNotExist
	 */
	 const T& getFirst() { 
        if (Size == 0) throw ElementNotExist();
        return data[head];
     }

	 /**
	  * TODO Retrieves, but does not remove, the last element of this deque.
	  * @throw ElementNotExist
	  */
	 const T& getLast() { 
        if (Size == 0) throw ElementNotExist();
        return data[tail];
     }

	 /**
	  * TODO Removes the first element of this deque.
	  * @throw ElementNotExist
	  */
	void removeFirst() { 
        if (Size == 0) throw ElementNotExist();
        ++head, --Size;
    }

	/**
	 * TODO Removes the last element of this deque.
	 * @throw ElementNotExist
	 */
	void removeLast() { 
        if (Size == 0) throw ElementNotExist();
        --tail, --Size;
    }

	/**
	 * TODO Returns a const reference to the element at the specified position in this deque.
	 * The index is zero-based, with range [0, size).
	 * @throw IndexOutOfBound
	 */
	const T& get(int index) const { 
        if (index < 0 || index >= Size) throw IndexOutOfBound();
        return data[head + index];
    }
	
	/**
	 * TODO Replaces the element at the specified position in this deque with the specified element.
	 * The index is zero-based, with range [0, size).
	 * @throw IndexOutOfBound
	 */
	void set(int index, const T& e) {
        if (index < 0 || index >= Size) throw IndexOutOfBound();
        data[head + index] = e;
    }

	/**
	 * TODO Returns the number of elements in this deque.
	 */
	 int size() const { 
        return Size;
     }

	 /**
	  * TODO Returns an iterator over the elements in this deque in proper sequence.
	  */
	 Iterator iterator() { 
        return Iterator(this, 0);
     }

	 /**
	  * TODO Returns an iterator over the elements in this deque in reverse sequential order.
	  */
	 Iterator descendingIterator() { 
        return Iterator(this, 1);
     }
};

#endif
