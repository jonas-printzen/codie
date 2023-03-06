#ifndef CODIE_ISLIST_HDR
#define CODIE_ISLIST_HDR

#include <codie/common.hpp>

namespace codie {

template <typename T> class islink;
template <typename T> class islist;

/** @brief Intrusive part of islist.
 *
 * Put this as first base of your list-entry...
 *
 */
template <typename T>
class islink {
  islink(const islink&) = delete;
  friend class islist<T>;
public:
  using value_type = T;
  using pointer = value_type*;
  using const_pointer = const value_type*;

  islink(void) :_next(nullptr) {};
  virtual ~islink(void) {}

  inline islink* next(void) { return _next; }
  inline const islink* next(void) const { return _next; }

protected:
  islink* _next;
};

/** @brief Single linked intrusive list
 *
 * Put things in a list with minimal overhead...
 *
 */
template <typename T>
class islist : public islink<T> {
public:
  using link_t = islink<T>;
  using value_type = T;
  using size_type = size_t;

  using reference = value_type&;
  using pointer = value_type*;
  using const_reference = const value_type&;
  using const_pointer = const value_type*;

  ~islist(void) { clear(); }

  /** @brief  Clear out the linked... */
  inline void clear(void) {
    while( _next ) unlink_after( before_front() );
  }

  /** @brief Is the list empty? */
  inline bool empty(void) const { return nullptr == _last; }

  class const_iterator {
  public:
    using value_type = typename islist::value_type;
    using const_reference = const value_type&;
    using const_pointer = const value_type*;
    using link_t = islink<T>;

    constexpr const_iterator( const link_t *pi=nullptr ) : _pitem((const_pointer)pi) {}
    constexpr const_iterator( const const_iterator& ) = default;

    inline constexpr const_iterator& operator ++ () { 
      _pitem = (const_pointer)((const link_t*)_pitem)->_next; 
      return *this;
    }
    inline operator bool() const { return nullptr != _pitem; }

    inline const_reference operator * () const { return *(const_pointer)_pitem; }
    inline const_pointer operator -> () const { return (const_pointer)_pitem; }

    inline bool operator == ( const const_iterator &other ) const {
      return other._pitem == _pitem;
    }
  private:
    const_pointer _pitem;
  };

  class iterator {
  public:
    using value_type = typename islist::value_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using link_t = islink<T>;

    constexpr iterator( link_t *pi=nullptr ) : _pitem((pointer)pi) {}
    constexpr iterator( iterator& ) = default;

    inline constexpr iterator& operator ++ () { 
      _pitem = (pointer)(((link_t*)_pitem)->_next); 
      return *this;
    }
    inline operator bool() const { return nullptr != _pitem; }

    inline reference operator * () { return *(pointer)_pitem; }
    inline const_reference operator * () const { return *(const_pointer)_pitem; }
    inline pointer operator -> () { return (pointer)_pitem; }
    inline const_pointer operator -> () const { return (const_pointer)_pitem; }

    inline bool operator == ( const iterator &other ) const {
      return other._pitem == _pitem;
    }

  private:
    pointer _pitem;
  };

  inline iterator before_begin(void) { return iterator( (link_t*)&before_front()); }
  inline const_iterator before_begin(void) const { return const_iterator((const link_t*)&before_front()); }
  inline iterator begin(void) { return iterator(_next); }
  inline const_iterator begin(void) const { return const_iterator(_next); }

  inline iterator before_end(void) { return iterator( (link_t*)&back()); }
  inline const_iterator before_end(void) const { return const_iterator((const link_t*)&back()); }
  inline iterator end(void) { return iterator(nullptr); }
  inline const_iterator end(void) const { return const_iterator(nullptr); }

  inline reference front(void) { return *(pointer)_next; }
  inline const_reference front(void) const { return *(const_pointer)_next; }

  inline reference before_front(void) { return *(pointer)this; }
  inline const_reference before_front(void) const { return *(const_pointer)this; }

  inline reference back(void) { return *(pointer)_last; }
  inline const_reference back(void) const { return *(const_pointer)_last; }

  inline void link_after( reference at, reference item ) {
    ((link_t&)item)._next = ((link_t&)at)._next;
    ((link_t&)at)._next = &((link_t&)item);
    if( !((link_t&)item)._next ) _last = &((link_t&)item);
  }

  /** @brief Link element at the very end */
  inline void link_back( reference item ) {
    link_after( empty() ? before_front():back() , item );
  }

  /** @brief Link element at the very front */
  inline void link_front( reference item ) {
    link_after( before_front(), item );
  }

  /** @brief Fast-unlink when prev. is known */
  inline void unlink_after( reference at ) {
    link_t *item = ((link_t&)at)._next;
    // PRECOND( nullptr != item );
    ((link_t&)at)._next = item->_next;
    // If after is nullptr we have a new tail, nullptr if empty
    if( nullptr == ((link_t&)at)._next ) {
      _last = ((pointer)&at != (pointer)this) ? &((link_t&)at) : nullptr;
    } else {
      item->_next = nullptr;
    }
  }

  /** @brief Check if element is in this list ... */
  inline bool contains( const_reference what ) const {
    for( const link_t *where = this; nullptr!=where; where=where->_next ) {
      if( where == &what ) return true;
    }
    return false;
  }

  inline pointer find_before( reference at ) {
    link_t *p = this;
    while( p && (p->_next!=(link_t*)&at) ) p = p->_next;
    return (pointer)p;
  }

  /** @brief Find and unlink, expensive, but if prev. not known... */
  inline bool find_unlink( reference what ) {
    pointer where = find_before( what );
    if( where ) {
      unlink_after(*where);
      return true;
    }
    return false;
  }

  /** @brief Unlink the first element */
  inline void unlink_front() {
    unlink_after( before_front() );
  }

  /** @brief Walk through and count elements */
  inline size_type size( void ) const {
    size_type n=0;
    auto p = _next;

    while( p ) {
      ++n;
      p = p->_next;
    }
    return n;
  }

private:
  using link_t::_next;
  link_t *_last=nullptr;
};


} // namespace codie

#endif  // CODIE_ISLIST_HDR