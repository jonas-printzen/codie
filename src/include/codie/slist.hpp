#ifndef CODIE_SLIST_HDR
#define CODIE_SLIST_HDR

#include <codie/common.hpp>
#include <codie/islist.hpp>

namespace codie {


/** @addtogroup MODEL
 *
 * @{
 */

/** @brief The missing slist
 *
 *
 */
template <typename T>
class slist {
public:
  using value_type = T;
  using size_type = size_t;

  using reference = value_type&;
  using pointer = value_type*;
  using const_reference = const value_type&;
  using const_pointer = const value_type*;

  /** @brief The mamanged item-type */
  struct item_t : islink<value_type> { 
    item_t( auto&&...init ) : value( std::forward<decltype(init)>(init)... ) {}
    T value; 
  };

  using iptr_t = aptr_t<item_t>;
  using items_t = islist<item_t>;

  using link_t = islink<value_type>;

  ~slist(void) { clear(); }

  class const_iterator : public items_t::const_iterator {
  public:
    using base_t = typename items_t::const_iterator;

    const_iterator( const base_t&it ) : base_t(it) {}
    using base_t::operator ==;

    inline const_iterator& operator ++ () {
      (void)base_t::operator++();
      return *this;
    } 

    inline const_reference operator * () const { return base_t::operator*().value; }
    inline const item_t& item() const { return base_t::operator*(); }
  };

  class iterator : public items_t::iterator {
  public:
    using base_t = typename items_t::iterator;

    iterator( const base_t&it ) : base_t((base_t&)it) {}
    using base_t::operator ==;

    inline iterator& operator ++ () {
      (void)base_t::operator++();
      return *this;
    } 

    inline reference operator * () { return base_t::operator*().value; }
    inline item_t& item() { return base_t::operator*(); }
    inline const_reference operator * () const { return base_t::operator*().value; }
    inline const item_t& item() const { return base_t::operator*(); }
  };

  /** @brief  Clear out the linked... */
  inline void clear(void) {
    while( !empty() ) pop_front();
  }

  /** @brief Is the list empty? */
  inline constexpr bool empty(void) const { return _items.empty(); }

  inline iterator begin(void) { return iterator(_items.begin()); }
  inline const_iterator begin(void) const { return const_iterator(_items.begin()); }
  inline iterator end(void) { return iterator(_items.end()); }
  inline const_iterator end(void) const { return const_iterator(_items.end()); }

  inline iterator before_begin(void) { return iterator(_items.before_begin()); }
  inline const_iterator before_begin(void) const { return const_iterator(_items.before_begin()); }

  inline iterator before_end(void) { return iterator(_items.before_end()); }
  inline const_iterator before_end(void) const { return const_iterator(_items.before_end()); }

  inline const_reference front(void) const { return _items.front().value; }
  inline reference& front(void) { return _items.front().value; }

  inline const_reference back(void) const { return _items.back().value; }
  inline reference back(void) { return _items.back().value; }

  inline reference push_front( auto&&...init ) {
    item_t *pi = new item_t( std::forward<decltype(init)>(init)... );
    _items.link_front( *pi );
    return pi->value;
  }

  inline reference emplace_after( iterator it, auto &&...init ) {
    item_t *pi = new item_t( std::forward<decltype(init)>(init)... );
    _items.link_after( it.item(), *pi );
    return pi->value;
  }

  inline reference insert_front( iptr_t &&p ) {
    item_t *pi = p.release();
    _items.link_front( *pi );
    return pi->value;
  }

  inline reference insert_after( iterator it, iptr_t &&p ) {
    item_t *pi = p.release();
    _items.link_after( it.item(), *pi );
    return pi->value;
  }

  inline void erase_after( iterator it ) {
    item_t *pi = (item_t*)it->next();
    _items.unlink_after( it.item() );
    delete pi;
  }

  inline iptr_t pull_after( iterator it ) {
    item_t *pi = (item_t*)it->next();
    _items.unlink_after( it.item() );
    return iptr_t(pi);
  }

  /** @brief Drop the first item */
  inline void pop_front() {
    item_t *pi = &_items.front();
    _items.unlink_front();
    delete pi;
  }

  /** @brief Get the first item out of the slist */
  inline iptr_t pull_front() {
    item_t *pi = &_items.front();
    _items.unlink_front();
    return iptr_t(pi);
  }

  inline reference push_back( auto&&...init ) {
    item_t *pi = new item_t( std::forward<decltype(init)>(init)... );
    _items.link_back( *pi );
    return pi->value;
  }

  /** @brief Drop the last item */
  inline void pop_back() {
    item_t *pi = &_items.back();
    _items.find_unlink( *pi );
    delete pi;
  }

  /** @brief Get the last item out of the slist */
  inline iptr_t pull_back() {
    item_t *pi = &_items.back();
    _items.unlink_after( *_items.find_before( _items.back() ) );
    return iptr_t(pi);
  }

  /** @brief Walk through and count elements */
  inline constexpr size_type size( void ) const {
    return _items.size();
  }

  /** @brief Get item-pointer from value-pointer
   *
   * @note Use with care ...
   */
  inline item_t* item( pointer p ) {
    return p ? (item_t*)(((int8_t*)p) - sizeof(link_t)) : nullptr;
  }

  /** @brief Unlink and forget
   * 
   * @note Use with care....
   */
  inline void drop( item_t* pi ) {
    // What is the `item_t` that has *p as value?
    _items.find_unlink( *pi );
  }


private:
  // NOTE! The pointer to first is `_next` in the base_class
  // link_t *_last=nullptr;
  items_t _items;
};

/**
 * @}
 */

} // namespace codie

#endif  // CODIE_SLIST_HDR