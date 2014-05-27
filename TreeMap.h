/** @file */
#ifndef __TREEMAP_H
#define __TREEMAP_H

#include "ElementNotExist.h"
#include <cstdlib>

/**
 * TreeMap is the balanced-tree implementation of map. The iterators must
 * iterate through the map in the natural order (operator<) of the key.
 */
template<class K, class V>
class TreeMap
{
public:
    class Entry;
    class Iterator;
private:
    int Size;
    Entry *root;
    Entry *begin;

    inline void removeAll(Entry *x) {
        if (x == NULL) return;
        removeAll(x->l);
        removeAll(x->r);
        delete x;
    }

    inline void copy(Entry* &x, const Entry* y) {
        if (y == NULL) {
            x = NULL; return;
        }
        x = new Entry(y->key, y->value, y->heap);
        copy(x->l, y->l);
        copy(x->r, y->r);
    }

    inline Entry* buildrank( Entry* x, Entry* y) {
        if (x == NULL) return NULL;
        Entry *tmp = buildrank(x->l, y);
        if (tmp == NULL) {
            y->succ = x; x->prev = y;
        } else {
            tmp->succ = x; x->prev = tmp;
        }
        tmp = buildrank(x->r, x);
        if (tmp == NULL) return x; else return tmp;
    }

    void rotate_l(Entry* &x) {
        Entry *y = x->r;
        x->r = y->l;
        y->l = x;
        x = y; 
    }

    void rotate_r(Entry* &x) {
        Entry *y = x->l;
        x->l = y->r;
        y->r = x;
        x = y;
    }

    inline int Travel(Entry *x) {
        int cnt = 1;
        if (x->l != NULL) cnt += Travel(x->l);
        if (x->r != NULL) cnt +=Travel(x->r);
    }

    inline Entry* Insert(Entry* &x, const K &key, const V &value) {
        if (x == NULL) {
            x = new Entry(key, value);
            return x;
        }
        Entry *ans;
        if (key < x->key) {
            ans = Insert(x->l, key, value);
            if (x->l->heap > x->heap) rotate_r(x);
            return ans;
        } else {
            ans = Insert(x->r, key, value);
            if (x->r->heap > x->heap) rotate_l(x);
            return ans;
        }
        return ans;
    }

    inline void Delete(Entry* &x, const K &key) {
        if (x == NULL) return;
        if (x->l == NULL && x->r == NULL) {
            Entry *Pre = x->prev;
            Entry *Suc = x->succ;
            Pre->succ = Suc;
            if (Suc != NULL) Suc->prev = Pre;
            delete x;
            x = NULL;
            return;
        }
        if (key < x->key) Delete(x->l, key);
            else if (x->key < key) Delete(x->r, key); else {
                if (x->l == NULL) rotate_l(x), Delete(x->l, key); else                
                if (x->r == NULL) rotate_r(x), Delete(x->r, key); else
                if (x->l->heap > x->r->heap) rotate_r(x), Delete(x->r, key); else
                   rotate_l(x), Delete(x->l, key); 
            }
    }

    inline Entry* Find_min(const K &key) {
        Entry *ans = NULL, *x = root;
        while (x)
            if (x->key < key) ans = x, x = x->r; 
                else x = x->l;
        return ans;
    }

public:

    /**
     * TODO Constructs an empty tree map.
     */
    TreeMap() { 
        root = NULL;
        Size = 0;
        begin = new Entry();
        begin->prev = begin->succ = NULL;
    }

    /**
     * TODO Destructor
     */
    ~TreeMap() {
        removeAll(root);
        delete begin;
    }

    /**
     * TODO Assignment operator
     */
    TreeMap &operator=(const TreeMap &x) { 
        if (this != &x) {
            removeAll(root);
            root = NULL;
            Size = x.Size;
            begin->succ = NULL;
            copy(root, x.root);
            buildrank(root, begin);
        }
        return *this;
    }

    /**
     * TODO Copy-constructor
     */
    TreeMap(const TreeMap &x) { 
        root = NULL;
        Size = x.Size;
        begin = new Entry();
        begin->prev = begin->succ = NULL;
        copy(root, x.root);
        buildrank(root, begin);
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
        removeAll(root);
        root = NULL;
        Size = 0;
        begin->succ = NULL;
    }

    /**
     * TODO Returns true if this map contains a mapping for the specified key.
     */
    bool containsKey(const K &key) const {
        Entry *x = root;
        while (x != NULL) {
            if (x->key == key) return true;
            if (key < x->key) x = x->l; else x = x->r;
        }
        return false;
    }

    /**
     * TODO Returns true if this map maps one or more keys to the specified value.
     */
    bool containsValue(const V &value) const {
        for(Entry *k = begin->succ; k != NULL; k = k->succ)
            if (k->value == value) return true;
        return false;
    }

    /**
     * TODO Returns a const reference to the value to which the specified key is mapped.
     * If the key is not present in this map, this function should throw ElementNotExist exception.
     * @throw ElementNotExist
     */
    const V &get(const K &key) const {
        Entry *x = root;
        while (x != NULL) {
            if (x->key == key) return x->value;
            if (key < x->key) x = x->l; else x = x->r;
        }
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
        Entry *x = root;
        while (x != NULL) {
            if (x->key == key) {
                x->value = value; 
                return;
            }
            if (key < x->key) x = x->l; else x = x->r;
        }
        ++Size;
        x = Insert(root, key, value);
        Entry *Pre = Find_min(key);
        if (Pre == NULL) Pre = begin;
        Entry *Suc = Pre->succ;
        
        x->prev = Pre; Pre->succ = x;
        if (Suc != NULL)
            x->succ = Suc, Suc->prev = x;
    }

    /**
     * TODO Removes the mapping for the specified key from this map if present.
     * If there is no mapping for the specified key, throws ElementNotExist exception.
     * @throw ElementNotExist
     */
    void remove(const K &key) {
        if (containsKey(key)) --Size, Delete(root, key);
            else throw ElementNotExist(); 
    }

    /**
     * TODO Returns the number of key-value mappings in this map.
     */
    int size() const {
        return Size;
    }
};

template<class K, class V>
class TreeMap<K, V>::Entry {
    public:    
        K key;
        V value;
        Entry *l, *r;
        Entry *prev, *succ;
        int heap;

        Entry() {}

        Entry(K k, V v) {
            key = k;
            value = v;
            heap = rand();
            l = r = NULL;
            prev = succ = NULL;
        }

        Entry(K k, V v, int h) {
            key = k;
            value = v;
            heap = h;
            l = r = NULL;
            prev = succ = NULL;            
        }

        K getKey() const {
            return key;
        }

        V getValue() const {
            return value;
        }
};

template<class K, class V>
class TreeMap<K, V>::Iterator{
    private:
        Entry *pos;
        const TreeMap *container;

    public:

        Iterator(){}
        
        Iterator(const TreeMap *c) : container(c), pos(c->begin) {}

        /**
         * TODO Returns true if the iteration has more elements.
         */
        bool hasNext() {
            return pos->succ != NULL;
        }

        /**
         * TODO Returns the next element in the iteration.
         * @throw ElementNotExist exception when hasNext() == false
         */
        const Entry &next() {
            if (hasNext()) {
                pos = pos->succ;
                return *pos;
            }
            throw ElementNotExist();
        }
};

#endif
