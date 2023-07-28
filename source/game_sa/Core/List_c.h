/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "ListItem_c.h"

/**
 * Double linked list base implementation
 *
 * NOTE: You should not use this class directly, use TList_c template instead.
 */
class List_c {
public:
    List_c() : m_pHead(nullptr), m_pTail(nullptr), m_nCount(0) {}
    ~List_c() = default;

public:
    ListItem_c* m_pHead;
    ListItem_c* m_pTail;
    uint32      m_nCount;

public:
    static void InjectHooks();

    // Add new item to the head
    void AddItem(ListItem_c* item);

    // Remove given item from the list and decrease counter
    void RemoveItem(ListItem_c* item);

    // Remove heading item and return it's pointer
    ListItem_c* RemoveHead();

    // Remove tail item and return it's pointer
    ListItem_c* RemoveTail();

    // Remove all items
    void RemoveAll();

    // Get number of items in the list
    uint32 GetNumItems() const;

    // Append item to the list
    void AppendItem(ListItem_c* item);

    // Append item to the list
    void InsertAfterItem(ListItem_c* addedItem, ListItem_c* pExistingItem);

    // Append item to the list
    void InsertBeforeItem(ListItem_c* addedItem, ListItem_c* pExistingItem);

    // Get list head
    ListItem_c* GetHead();

    // Get list head
    ListItem_c* GetTail();

    // Get next item in a list
    static ListItem_c* GetNext(ListItem_c* item);

    // Get previous item
    static ListItem_c* GetPrev(ListItem_c* item);

    // Get N-th item from list head/tail
    ListItem_c* GetItemOffset(bool bFromHead, int32 iOffset);
};

/**
 * Double linked list template wrapper
 * (not an original game class name)
 */
template <typename T>
class TList_c : public List_c {
public:
    class Iterator {
        using ListT             = TList_c<T>;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = T;
        using pointer           = T*;
        using reference         = T&;

        Iterator(pointer ptr) : m_ptr{ptr} {}

        reference operator*() const { return *m_ptr; }
        pointer operator->() { return m_ptr; }
        Iterator& operator++() { m_ptr = m_ptr->m_pNext; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
        friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
        friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

    private:
        pointer m_ptr;
    };
public:
    T* GetHead() {
        return static_cast<T*>(List_c::GetHead());
    }

    T* GetTail() {
        return static_cast<T*>(List_c::GetTail());
    }

    T* RemoveHead() {
        return static_cast<T*>(List_c::RemoveHead());
    }

    T* RemoveTail() {
        return static_cast<T*>(List_c::RemoveTail());
    }

    T* GetNext(T* item) {
        return static_cast<T*>(List_c::GetNext(item));
    }

    T* GetPrev(T* item) {
        return static_cast<T*>(List_c::GetPrev(item));
    }

    T* GetItemOffset(bool bFromHead, int32 iOffset) {
        return static_cast<T*>(List_c::GetItemOffset(bFromHead, iOffset));
    }

    // Iterate over the list [NOTSA]
    template<typename Fn>
    void ForEach(Fn fn) {
        for (auto it = GetHead(); it; it = GetNext(it)) {
            std::invoke(fn, *it);
        }
    }
};
VALIDATE_SIZE(List_c, 0xC);
