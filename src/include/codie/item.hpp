#ifndef CODIE_ITEM_HDR
#define CODIE_ITEM_HDR

#include <codie/common.hpp>

namespace codie {

// template <hash32_t H,typename T>
// class PItem;


/** @brief An object with a n uid
 *
 *  This is the base of a simple dispatchable object, typically used
 *  for singalling, events and meessages. When used as base of POD-like
 *  objects it is inherently cheap. It can also be used in switch-statemens,
 *  which is useful for dispatching events and messages. 
 *
 *  Although it is very similar to RTTI / typeid, I find it easier to rely 
 *  on since the `uid` is explicit and very fast.
 *
 *  @note Construction is deliberately hidden to force subclassing.
 */
class Item {
  template <hash32_t,typename>
  friend class PItem;
public:
  struct meta_t {
    uint32_t  pool : 8 ;
    uint32_t  user : 24;
  };

  const hash32_t uid;

  const meta_t& meta() const { return _meta; }

  static constexpr const uint32_t NONE=0;
  static constexpr const uint32_t HEAP=1;
  static constexpr const uint32_t NEW=2;

  /** @brief Deleter to use with aptr_t<FINAL> */
  template <typename T>
  struct delete_t { 
    inline void operator () ( T*p ) {
    switch( p->meta().pool ) {
    case Item::NEW: delete p; break;
    case Item::HEAP: ::free(p ); break;
    default:;
    }
    } 
  };

protected:
  constexpr Item( hash32_t hash, uint32_t pool=NONE ) 
    : uid(hash), _meta({0}) { _meta.pool = pool; }

protected:
  meta_t _meta;
};


/** @brief Item - wrapper
 *
 *  This class wrapps any value as an Item. When accessing the value itself
 *  it behaves like a '***pointer***', so you have to dereference it with 
 *  either `operator *` or `operator ->`.
 */
template <hash32_t H,typename T=void>
class AItem : public Item {
public:
  using type_t = T;

  inline constexpr type_t& operator *() { return _val; }
  inline constexpr const type_t& operator *() const { return _val; }
  inline constexpr type_t* operator ->() { return &_val; }
  inline constexpr const type_t* operator ->() const { return &_val; }

  /** @brief Forwarding constructor
   *
   *  This constructor forwards all (if any) arguments to the wrapped object
   */
  constexpr AItem( auto&&... ini ) : Item{H}, _val(std::forward<decltype(ini)>(ini)...) {}

  /** @brief Conditional access
   *
   *  This will allow single or no iterations, depending on matching `uid`.
   *
   * @see Test case: ItemIs
   */
  class Is {
  public:
    constexpr Is( Item &item ) : _item(item) {}

    inline constexpr type_t* begin() { return ID==_item.uid ? static_cast<AItem&>(_item).operator ->() : end(); }
    inline constexpr type_t* end() { return static_cast<AItem&>(_item).operator ->()+1; }
    inline constexpr const type_t* begin() const { return ID==_item.uid ? static_cast<AItem&>(_item).operator ->() : end(); }
    inline constexpr const type_t* end() const { return static_cast<AItem&>(_item).operator ->()+1; }
 
  private:
    Item& _item;
  };

  static const hash32_t ID=H;

private:
  type_t _val;
};

/** @brief The empty Item
 *
 *  An Item can be useful even without wrapping any data. The fact that an event
 *  occured, in it self is a singal/event.
 */
template <hash32_t H>
class AItem<H,void> : public Item {
public:

  using type_t = void;

  /** @brief Forwarding constructor
   *
   *  Default constructor for uid-only item.
   */
  constexpr AItem() : Item{H} {}

  static const hash32_t ID=H;
};

/** @brief A owning pointer wrapper for dynamically allocated Items
 *
 *  Use this to declare the type of an object that, by design, has
 *  dynamic lifetime. Avoid, if possible,  mixing ot AItem<T> and 
 *  PItem<T>, where T is the same.
 */
template <hash32_t H,typename T=void>
class PItem : public aptr_t<AItem<H,T>,Item::delete_t<AItem<H,T>>> {
public:
  using type_t = AItem<H,T>;
  using base_t = aptr_t<type_t,Item::delete_t<AItem<H,T>>>;

  PItem( type_t *p=nullptr ) : base_t( p ) {
    if( p ) p->_meta.pool = Item::NEW;
  }

  inline void reset( type_t *p ) {
    base_t::reset( p );
    if( p ) p->_meta.pool = Item::NEW;
  }

  /** @brief Create an instance... */
  static inline PItem create( auto&&...args ) { 
    return new type_t( std::forward<decltype(args)>(args)... ); 
  }

private:
};


} // namespace codie

#endif  // CODIE_ITE_HDR