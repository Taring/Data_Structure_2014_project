/** @file */

#ifndef __HASHMAP_H
#define __HASHMAP_H

#include "ElementNotExist.h"

/**
 * HashMap is a map implemented by hashing. Also, the 'capacity' here means the
 * number of buckets in your internal implemention, not the current number of the
 * elements.
 *
 * Template argument H are used to specify the hash function.
 * H should be a class with a static function named ``hashCode'',
 * which takes a parameter of type K and returns a value of type int.
 * For example, the following class
 * @code
 *      class Hashint {
 *      public:
 *          static int hashCode(int obj) {
 *              return obj;
 *          }
 *      };
 * @endcode
 * specifies an hash function for integers. Then we can define:
 * @code
 *      HashMap<int, int, Hashint> hash;
 * @endcode
 *
 * Hash function passed to this class should observe the following rule: if two keys
 * are equal (which means key1 == key2), then the hash code of them should be the
 * same. However, it is not generally required that the hash function should work in
 * the other direction: if the hash code of two keys are equal, the two keys could be
 * different.
 *
 * Note that the correctness of HashMap should not rely on the choice of hash function.
 * This is to say that, even the given hash function always returns the same hash code
 * for all keys (thus causing a serious collision), methods of HashMap should still
 * function correctly, though the performance will be poor in this case.
 *
 * The order of iteration could be arbitary in HashMap. But it should be guaranteed
 * that each (key, value) pair be iterated exactly once.
 */
template <class K, class V, class H>
class HashMap {
public:
    /*
     * Entry if the node int Hash Table.
     */
    class Entry{
        public:
        K key;
        V value;
        Entry *next;
        Entry(K k, V v) {
            key = k;
            value = v;
            next = NULL;
        }

        Entry(K k, V v, Entry *n): key(k), value(v), next(n){}

        K getKey() const {
            return key;
        }

        V getValue() const {
            return value;
        }

        void changeValue(const V &v){
            value = v;
        }
    };
    class Iterator;
//private:
/*
 * @var Hash_max The size of hash -> the prime  min(abs(x - 888888))
 */
    static const int Hash_max = 888887;
    Entry **head;
    int Size;

    H func;
    /*
     * use Hash_max to Hash it again.
     */
    inline unsigned int get_hash(const K &key) const {
        return (unsigned int) func.hashCode(key) % Hash_max;
    }

    /*
     * Clear the hash table
     */
    void removeAll() {
        for (int i = 0; i < Hash_max; ++i)
            for (Entry *x, *k = head[i]; k != NULL; k = x) {
                x = k->next;
                delete k;
            }
    }

    void clear_head() {
        for (int i = 0; i < Hash_max; ++i)
            head[i] = NULL;
    }

public:

    /**
     * TODO Constructs an empty hash map.
     */
    HashMap() { 
        Size = 0;
        head = new Entry*[Hash_max];
        clear_head();
        func = H();
    }

    /**
     * TODO Destructor
     */
    ~HashMap() { 
        removeAll();
        delete[] head;
    }

    /**
     * TODO Assignment operator
     */
    HashMap &operator=(const HashMap &x) { 
        if (this != &x) {
            removeAll();
            clear_head();
            Size = x.Size;
            func = x.func;
            for (int i = 0; i < Hash_max; ++i)
                for (Entry *k = x.head[i]; k != NULL; k = k->next) {
                    Entry *tmp = new Entry(k->getKey(), k->getValue(), head[i]);
                    head[i] = tmp;
                }
        }
        return *this;
    }

    /**
     * TODO Copy-constructor
     */
    HashMap(const HashMap &x) { 
        head = new Entry*[Hash_max];
        clear_head();
        Size = x.Size;
        func = x.func;
        for (int i = 0; i < Hash_max; ++i)
                for (Entry *k = x.head[i]; k != NULL; k = k->next) {
                    Entry *tmp = new Entry(k->getKey(), k->getValue(), head[i]);
                    head[i] = tmp;
                }
    }

    /**
     * TODO Returns an iterator over the elements in this map.
     */
    Iterator iterator() const {
        return Iterator(this);
    }

    /**
     * TODO Removes all of the mappings from this map.
     */
    void clear() {
        removeAll();
        clear_head();
        Size = 0;
    }

    /**
     * TODO Returns true if this map contains a mapping for the specified key.
     */
    bool containsKey(const K &key) const {
        int t = get_hash(key);
        for (Entry *k = head[t]; k != NULL; k = k->next)
            if (k->key == key) return true;
        return false;
    }

    /**
     * TODO Returns true if this map maps one or more keys to the specified value.
     */
    bool containsValue(const V &value) const {
        for (int i = 0; i < Hash_max; ++i)
            for (Entry *k = head[i]; k != NULL; k = k->next)
                if (k->value == value) return true;
        return false;
    }

    /**
     * TODO Returns a const reference to the value to which the specified key is mapped.
     * If the key is not present in this map, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V &get(const K &key) const {
        int t = get_hash(key);
        for (Entry *k = head[t]; k != NULL; k = k->next)
            if (k->key == key) return k->value;
        throw ElementNotExist();
    }

    /**
     * TODO Returns true if this map contains no key-value mappings.
     */
    bool isEmpty() const {
        return Size == 0;
    }

    /**
     * TODO Associates the specified value with the specified key in this map.
     */
    void put(const K &key, const V &value) {
        int t = get_hash(key);
        for (Entry *k = head[t]; k != NULL; k = k->next)
            if (k ->getKey() == key) {
                k->changeValue(value);
                return;
            }
        Entry *tmp = new Entry(key, value, head[t]);
        head[t] = tmp;
        ++Size;
    }

    /**
     * TODO Removes the mapping for the specified key from this map if present.
     * If there is no mapping for the specified key, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void remove(const K &key) {
        int t = get_hash(key);
        if (head[t] != NULL && head[t]->key == key) {
            --Size;
            Entry *q = head[t]->next;
            delete head[t];
            head[t] = q;
            return;   
        }
        for (Entry *k = head[t]; k->next != NULL; k = k->next)
            if (k->next->key == key) {
                --Size;
                Entry *q = k->next->next;
                delete k->next;
                k->next = q;
                return;
            }
        throw ElementNotExist();
    }

    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const {
        return Size;
    }
};

template<class K, class V, class H>
class HashMap<K, V, H>::Iterator{
    private:
        int now_i;
        Entry *nowEntry;
        const HashMap *container;

        /**
         * return true if the iteration has more elements
         * and the next Iterator is transfer by next_i & nextEntry
         */
        bool try_next(int &next_i, Entry* &nextEntry) {
            next_i = now_i; nextEntry = nowEntry;
            if (next_i < 0) {
                next_i = 0;
                nextEntry = container->head[0];
            } else nextEntry = nextEntry->next;
            if (nextEntry == NULL) {
                for (++next_i; container->head[next_i] == NULL && next_i < HashMap::Hash_max; ++next_i);
                if (next_i < HashMap::Hash_max) nextEntry = container->head[next_i];
            }
            return nextEntry != NULL;
        }

    public:
        Iterator() {}
        Iterator(const HashMap *c):container(c), now_i(-1), nowEntry(NULL) {}
        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
            int next_i;
            Entry *nextEntry;
            return try_next(next_i, nextEntry);
        }

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const Entry &next() {
            int next_i;
            Entry *nextEntry;
            if(!try_next(next_i, nextEntry)) throw ElementNotExist();
            now_i = next_i;
            nowEntry = nextEntry;
            //Entry *a = new Entry(nowEntry->key, nowEntry->next);
            return *nowEntry;
        }
    };


#endif
