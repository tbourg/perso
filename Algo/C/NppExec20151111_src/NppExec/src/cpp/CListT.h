/***********************************************
 *  
 *  CListT ver. 1.1.0
 *  --------------------------------  
 *  (C) DV, Nov 2006 - Sep 2013
 *  --------------------------------
 *
 *  Template:
 *  >>  CListT<type T>
 *
 *  Pre-defined types: none
 *  >>  Example:  typedef CListT<CStr>  CStrList;
 *  >>  Example:  typedef CListT<CWStr> CWStrList;
 *
 ***********************************************/

#ifndef _list_t_h_
#define _list_t_h_
//----------------------------------------------------------------------------

template <class T> class CListT;

template <class T> class CListItemT 
{
private:
  friend class CListT<T>;
  CListItemT<T>* _pPrevItem;
  CListItemT<T>* _pNextItem;
  CListT<T>*     _pListOwner;
  T              _item;

public:
  const T&       GetItem() const  { return _item; }
  T&             GetItem()  { return _item; }
  CListItemT<T>* GetNext() const  { return _pNextItem; }
  CListItemT<T>* GetPrev() const  { return _pPrevItem; }
  CListT<T>*     GetOwner() const  { return _pListOwner; }
  void           SetItem(const T& item)  { _item = item; }

#if __cplusplus > 199711L
  void           SetItem(T&& item)  { _item = item; }
#endif

};

template <class T> class CListT 
{
public:
  typedef T value_type;
  typedef T CValueType;
  typedef CListItemT<T>* CListItemPtr;

private:
  CListItemPtr   m_pFirstItem;
  CListItemPtr   m_pLastItem;
  int            m_nCount;

  CListItemPtr   itemCreate(const T& item);
  void           itemDestroy(CListItemT<T>* pItemPtr);
  void           itemQuickSort(
                   CListItemPtr* pListItems, 
                   int           lower, 
                   int           upper);
  void           itemSwap(CListItemT<T>* p1, CListItemT<T>* p2);

public:
  CListT();
  ~CListT();
  CListItemT<T>* Add(const T& item);
  int            AddItems(const CListT<T>& List);  // returns items count
  void           Clear()  { DeleteAll(); }
  int            Copy(const CListT<T>& List);      // returns items count
  bool           Delete(CListItemT<T>* pItemPtr);
  void           DeleteAll();
  bool           DeleteFirst();
  bool           DeleteLast();
  CListItemT<T>* FindExact(
                   const T&             item, 
                   const CListItemT<T>* pStartItemPtr = NULL) const;
  int            GetCount() const  { return m_nCount; }
  CListItemT<T>* GetFirst() const  { return m_pFirstItem; }
  CListItemT<T>* GetLast() const  { return m_pLastItem; }
  CListItemT<T>* Insert(
                   CListItemT<T>* pListItemPtr, 
                   bool           bAfterListItem, 
                   const T&       item);
  CListItemT<T>* InsertFirst(const T& item);
  bool           IsEmpty() const  { return (m_nCount == 0); }
  bool           Sort();
  bool           Swap(CListItemT<T>* pItemPtr1, CListItemT<T>* pItemPtr2);
  void           Swap(CListT<T>& List);

#if __cplusplus > 199711L
  int            Copy(CListT<T>&& List);      // returns items count
#endif

};

//----------------------------------------------------------------------------

template <class T> CListT<T>::CListT()
{
    m_pFirstItem = NULL;
    m_pLastItem = NULL;
    m_nCount = 0;
}

template <class T> CListT<T>::~CListT()
{
    DeleteAll();
}

template <class T> CListItemT<T>* CListT<T>::itemCreate(const T& item)
{
    CListItemT<T>* pNewItem = new CListItemT<T>;
    if ( pNewItem )
    {
        pNewItem->_item = item;
        pNewItem->_pListOwner = this;
        pNewItem->_pNextItem = NULL;
        pNewItem->_pPrevItem = NULL;
    }
    return pNewItem;
}

template <class T> void CListT<T>::itemDestroy(CListItemT<T>* pItemPtr)
{
    delete pItemPtr;
}

template <class T> void CListT<T>::itemQuickSort(
  CListItemPtr* pListItems, 
  int           lower, 
  int           upper)
{
    if ( lower < upper ) 
    {
        itemSwap( pListItems[lower], pListItems[(upper + lower)/2] );
    
        T   pivot = pListItems[lower]->_item;
        int m = lower;
        int i = lower + 1;
        while ( i <= upper ) {
            if ( pListItems[i]->_item < pivot ) {
                ++m;
                itemSwap( pListItems[m], pListItems[i] );
            }
            ++i;
        }
    
        itemSwap( pListItems[lower], pListItems[m] );
        itemQuickSort( pListItems, lower, m - 1 );
        itemQuickSort( pListItems, m + 1, upper );
    }
}

template <class T> void CListT<T>::itemSwap(CListItemT<T>* p1, CListItemT<T>* p2)
{
    if ( p1 != p2 )
    {
        T item = p2->_item;
        p2->_item = p1->_item;
        p1->_item = item;
    }
}

template <class T> CListItemT<T>* CListT<T>::Add(const T& item)
{
    CListItemT<T>* pNewItem = itemCreate(item);
    if ( pNewItem )
    {
        if ( m_nCount == 0 )
        {
            m_pFirstItem = pNewItem;
        }
        else
        {
            pNewItem->_pPrevItem = m_pLastItem;
            m_pLastItem->_pNextItem = pNewItem;
        }
        m_pLastItem = pNewItem;
        ++m_nCount;
    }
    return pNewItem;
}

template <class T> int CListT<T>::AddItems(const CListT<T>& List)
{
    CListItemT<T>* pItem = (CListItemT<T> *) List.GetFirst();
    while ( pItem )
    {
        Add( pItem->GetItem() );
        pItem = pItem->GetNext();
    }
    return m_nCount;
}

template <class T> int CListT<T>::Copy(const CListT<T>& List)
{
    DeleteAll();
    return AddItems(List);
}

template <class T> bool CListT<T>::Delete(CListItemT<T>* pItemPtr)
{
    if ( !pItemPtr )
        return false;

    CListT<T>* pListOwner = pItemPtr->_pListOwner;
    if ( pListOwner != this )
        return false;
  
    CListItemT<T>* pPrev = pItemPtr->_pPrevItem;
    CListItemT<T>* pNext = pItemPtr->_pNextItem;
    
    --(pListOwner->m_nCount);
    if ( pItemPtr == pListOwner->m_pFirstItem )
        pListOwner->m_pFirstItem = pNext;
    if ( pItemPtr == pListOwner->m_pLastItem )
        pListOwner->m_pLastItem = pPrev;
  
    if ( pPrev )
        pPrev->_pNextItem = pNext;
    if ( pNext )
        pNext->_pPrevItem = pPrev;
    itemDestroy(pItemPtr);

    return true;  
}

template <class T> void CListT<T>::DeleteAll()
{
    CListItemT<T>* pNext;
    CListItemT<T>* pItem = m_pFirstItem;
    while ( pItem )
    {
        pNext = pItem->_pNextItem;
        itemDestroy(pItem);
        pItem = pNext;
    }
    m_pFirstItem = NULL;
    m_pLastItem = NULL;
    m_nCount = 0;
}

template <class T> bool CListT<T>::DeleteFirst()
{
    return Delete(m_pFirstItem);
}

template <class T> bool CListT<T>::DeleteLast()
{
    return Delete(m_pLastItem);
}

template <class T> CListItemT<T>* CListT<T>::FindExact(
  const T&             item, 
  const CListItemT<T>* pStartItemPtr ) const
{
    if ( m_pFirstItem )
    {
        if ( !pStartItemPtr )  pStartItemPtr = m_pFirstItem;
        if ( pStartItemPtr->_pListOwner == this )
        {
            while ( pStartItemPtr )
            {
                if ( pStartItemPtr->_item == item )
                    break;
                else
                    pStartItemPtr = pStartItemPtr->_pNextItem;
            }
            return ( (CListItemT<T>*) pStartItemPtr );
        }
    }
    return NULL;
}

template <class T> CListItemT<T>* CListT<T>::Insert(
  CListItemT<T>* pListItemPtr, 
  bool           bAfterListItem, 
  const T&       item)
{
    if ( (!pListItemPtr) || (pListItemPtr->_pListOwner != this) )
        return NULL;

    CListItemT<T>* pNewItem = itemCreate(item);
    if ( pNewItem )
    {
        CListItemT<T>* pItem;
        if ( bAfterListItem )
        {
            pItem = pListItemPtr->_pNextItem;
            pListItemPtr->_pNextItem = pNewItem;
            pNewItem->_pPrevItem = pListItemPtr;
            pNewItem->_pNextItem = pItem;
            if ( pItem )
                pItem->_pPrevItem = pNewItem;
            if ( m_pLastItem == pListItemPtr )
                m_pLastItem = pNewItem;
        }
        else
        {
            pItem = pListItemPtr->_pPrevItem;
            pListItemPtr->_pPrevItem = pNewItem;
            pNewItem->_pNextItem = pListItemPtr;
            pNewItem->_pPrevItem = pItem;
            if ( pItem )
                pItem->_pNextItem = pNewItem;
            if ( m_pFirstItem == pListItemPtr )
                m_pFirstItem = pNewItem;
        }
        ++m_nCount;
    }
    return pNewItem;
}

template <class T> CListItemT<T>* CListT<T>::InsertFirst(const T& item)
{
    if ( m_nCount == 0 )
        return Add(item);
    else
        return Insert( m_pFirstItem, false, item );
}

template <class T> bool CListT<T>::Sort()
{
    if ( m_nCount > 0 )
    {
        CListItemPtr* pListItems = new CListItemPtr[m_nCount];
        if ( pListItems )
        {
            int i = 0;
            CListItemT<T>* pItem = m_pFirstItem;
      
            while ( pItem )
            {
                pListItems[i++] = pItem;
                pItem = pItem->_pNextItem;
            }
            itemQuickSort( pListItems, 0, m_nCount - 1 );

            delete [] pListItems;

            return true;
        } // else out of memory: sorting is impossible
    } // else nothing to sort
    return false;
}

template <class T> bool CListT<T>::Swap(
  CListItemT<T>* pItemPtr1, 
  CListItemT<T>* pItemPtr2)
{
    if ( (!pItemPtr1) || (!pItemPtr2) )
        return false;

    if ( (pItemPtr1->_pListOwner != this) ||
         (pItemPtr2->_pListOwner != this) )
    {
        return false;
    }

    itemSwap( pItemPtr1, pItemPtr2 );
    return true;
}

template <class T> void CListT<T>::Swap(CListT<T>& List)
{
    CListItemPtr listFirstItem = List.m_pFirstItem;
    CListItemPtr listLastItem  = List.m_pLastItem;
    int          listCount     = List.m_nCount;

    List.m_pFirstItem = m_pFirstItem;
    List.m_pLastItem  = m_pLastItem;
    List.m_nCount     = m_nCount;

    m_pFirstItem = listFirstItem;
    m_pLastItem  = listLastItem;
    m_nCount     = listCount;
}

#if __cplusplus > 199711L
template <class T> int CListT<T>::Copy(CListT<T>&& List)
{
    DeleteAll();
    Swap(List);
    return m_nCount;
}
#endif

//----------------------------------------------------------------------------
#endif

