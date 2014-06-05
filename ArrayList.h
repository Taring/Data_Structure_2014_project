/** @file */
#ifndef __ARRAYLIST_H
#define __ARRAYLIST_H

#include "IndexOutOfBound.h"
#include "ElementNotExist.h"

/**
 * The ArrayList is just like vector in C++.
 * You should know that "capacity" here doesn't mean how many elements are now in this list, where it means
 * the length of the array of your internal implemention
 *
 * The iterator iterates in the order of the elements being loaded into this list
 */
template <class T>
class ArrayList
{
private:
    int Size, save_size;
    T *data;

    /*
     * double the space of data
     */
    void doubleSpace() {
        save_size *= 2;
        T *new_data = new T[save_size];
        for (int i = 0; i < Size; ++i)
            new_data[i] = data[i];
        delete[] (data);
        data = new_data;
    }

public:
    class Iterator
    {
    private:
        int index;
        ArrayList *container;
        /*
         * if the Iterator has been removed, dead will be true
         */
        bool dead;
    public:

        Iterator(){}
        Iterator(ArrayList *con): index(-1), dead(false), container(con){}

        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
            if (-1 <= index && index <= container->Size - 2) return true;
            return false;
        }

        /**
         * TODO Returns the next element in the iteration.
         * and the iteration move to the next element
         * @throw ElementNotExist exception when hasNext() == false
         */
        const T &next() {
            if (!hasNext()) throw ElementNotExist();
            if (dead) dead = false;
            return container->data[++index];
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
            if (index < 0 || dead) throw ElementNotExist();
            dead = true;
            container->removeIndex(index--);
        }
    };

    /**
     * TODO Constructs an empty array list.
     */
    ArrayList() { 
        save_size = 4;
        data = new T[save_size];
        Size = 0;
    }

    /**
     * TODO Destructor
     */
    ~ArrayList() {
        delete[] (data);
    }

    /**
     * TODO Assignment operator
     */
    ArrayList& operator=(const ArrayList& x) { 
        if (this != &x) {
            if (save_size != x.save_size) {
                delete [] (data);
                save_size = x.save_size;
                data = new T[save_size];
            }

            Size = x.Size;
            for (int i = 0; i < Size; ++i)
                data[i] = x.data[i];
        }
        return (*this);
    }

    /**
     * TODO Copy-constructor
     */
    ArrayList(const ArrayList& x) { 
        Size = x.Size;
        save_size = x.save_size;
        data = new T[save_size];
        for (int i = 0; i < Size; ++i)
            data[i] = x.data[i];
    }

    /**
     * TODO Appends the specified element to the end of this list.
     * Always returns true.
     */
    bool add(const T& e) {
        if (Size == save_size) doubleSpace();
        data[Size++] = e;
        return true;
    }

    /**
     * TODO Inserts the specified element to the specified position in this list.
     * The range of index parameter is [0, size], where index=0 means inserting to the head,
     * and index=size means appending to the end.
     * @throw IndexOutOfBound
     */
    void add(int index, const T& element) {
        if (index <0 || index > size) throw IndexOutOfBound();
        if (Size == save_size) doubleSpace();
        for (int i = Size - 1; i >= index; --i)
            data[i + 1] = data[i];
        data[index] = element;
        ++Size;
    }

    /**
     * TODO Removes all of the elements from this list.
     */
    void clear() {
        Size = 0;
    }

    /**
     * TODO Returns true if this list contains the specified element.
     */
    bool contains(const T& e) const {
        for (int i = 0; i < Size; ++i)
            if (data[i] == e) return true;
        return false;
    }

    /**
     * TODO Returns a const reference to the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    const T& get(int index) const {
        if (index < 0 || index >= Size) throw  IndexOutOfBound();
        return data[index];    
    }

    /**
     * TODO Returns true if this list contains no elements.
     */
    bool isEmpty() const {
        return Size == 0;
    }

    /**
     * TODO Removes the element at the specified position in this list.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void removeIndex(int index) {
        if (index < 0 || index >= Size) throw IndexOutOfBound();
        --Size;
        for (int i = index; i < Size; ++i)
            data[i] = data[i + 1];
    }

    /**
     * TODO Removes the first occurrence of the specified element from this list, if it is present.
     * Returns true if it was present in the list, otherwise false.
     */
    bool remove(const T &e) {
        for (int i = 0; i < Size; ++i)
            if (data[i] == e) {
                removeIndex(i);
                return true;
            }
        return false;
    }

    /**
     * TODO Replaces the element at the specified position in this list with the specified element.
     * The index is zero-based, with range [0, size).
     * @throw IndexOutOfBound
     */
    void set(int index, const T &element) {
        if (index < 0 || index >= Size) throw IndexOutOfBound();
        data[index] = element;
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

#endif
