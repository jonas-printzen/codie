#ifndef CODIE_TREE_HDR
#define CODIE_TREE_HDR

#include <codie/common.hpp>
#include <codie/item.hpp>
#include <codie/slist.hpp>

#include <variant>
#include <map>
using std::map;
using std::variant;


namespace codie {

/** @addtogroup MODEL
 *
 * @{
 */

/** @brief Container of subnodes */
class Nodes;

/** @brief Node in a tree
 *
 *  A Node is a named element present in a tree-structure.  
 *
 *  There are two key-parts to a node: 
 *   * The Item - base provides RTTI with explicit behavior.
 *   * The parent-pointer, used for navigation and cleanup.
 *
 *  A Node can be `orphan` or `root` , which means it has no ***parent***.
 *  The ability to provide anything other than a `named` position in a tree,
 *  comes from subclassing. This is where the Item comes in handy.
 *
 *  A Node itself has no meaning, it is either a Nodes, with children or
 *  a ***leaf***-node carrying values or additional API. By making the Node's
 *  constructor `protected`, we prevent accidental creation of a mute node.
 *  
 */
class Node : public Item {
public:
  using ptr_t  = aptr_t<Node>;     ///< Smart, owning, pointer ...
  using hash_t = hash64_t;

  virtual ~Node();

  inline constexpr const string& name() const { return _name; }

  inline Nodes* parent() { return _parent; }
  inline const Nodes* parent() const { return _parent; }

protected:
  Node( const string&name, hash_t hash) 
    : Item(hash), _name(name) {}

private:
  friend class Nodes;
  Nodes *_parent = nullptr;     ///< Only Nodes can set this!
  string _name;
};

/** @brief Helper node creator
 *
 *  Use this template construct to create a value-node that simply
 *  carries a vlue, of any kind.
 *
 * @note This is a leaf-node, subclassing Node! 
 */
template <Item::hash_t H, typename T>
class ANode : public Node {
public:
  static constexpr const hash_t ID=H;
  using value_type = T;
  using reference = value_type&;

  /** @brief Forwarding constructor
   *
   *  This constructor forwards all (if any) arguments to the wrapped object
   */
  constexpr ANode( const string&name, auto&&... ini ) 
    : Node(name, ID), value(std::forward<decltype(ini)>(ini)...) {}

  /** @brief Check if node is of this (Item-)type */
  static inline constexpr bool Is( Node &node ) { return ID==node.uid; }

  /** @brief Conditional access
   *
   *  This will do a single, or no, iterations, depending on matching `uid`.
   *
   * @see Test case: ItemIs
   */
  class If {
  public:
    constexpr If( Node &node ) : _node( ID==node.uid ? &static_cast<ANode&>(node) : nullptr ) {}

    inline constexpr ANode* begin() { return _node; }
    inline constexpr ANode* end() { return _node?_node+1:_node; }
    inline constexpr const ANode* begin() const { return _node; }
    inline constexpr const ANode* end() const { return _node?_node+1:_node; }

  private:
    ANode* _node;
  };

  static ptr_t create( const string &name, auto&&...init ) {
    return ptr_t( new ANode(name, std::forward<decltype(init)>(init)...) );
  }

  value_type value;
};


/** @brief A Node with subnodes
 *
 *  This node is a container of uniquely named nodes. It is explicitly designed 
 *  to carry Node::ptr_t - values, which can be inserted and extracted from the
 *  Nodes container. To manage these subnodes, slist<Node::ptr_t> is used, while
 *  a map<Node::ptr_t*> is used to handle name uniquenes.
 * 
 */
class Nodes : public Node {
public:
  static constexpr const hash_t ID=UUID("Nodes");
  using nodes_t = slist<Node::ptr_t>;

  using value_type = Node;
  using pointer    = value_type*;
  using reference  = value_type&;
  using const_pointer    = const value_type*;
  using const_reference  = const value_type&;

  Nodes( const string&name ) : Node( name, ID ) {}

  ~Nodes();

  inline constexpr bool empty() const { return _nodes.empty(); }

  void clear();

  class const_iterator : public nodes_t::const_iterator {
  public:
    using base_t = typename nodes_t::const_iterator;

    const_iterator( const base_t&it ) : base_t(it) {}
    using base_t::operator ==;

    inline const_iterator& operator ++ () {
      (void)base_t::operator++();
      return *this;
    } 

    inline Nodes::const_reference operator * () const { return *base_t::operator*(); }
  };

  class iterator : public nodes_t::iterator {
  public:
    using base_t = typename nodes_t::iterator;

    iterator( const base_t&it ) : base_t((base_t&)it) {}
    using base_t::operator ==;

    inline iterator& operator ++ () {
      (void)base_t::operator++();
      return *this;
    } 

    inline Nodes::reference operator * () { return *base_t::operator*(); }
    inline Nodes::const_reference operator * () const { return *base_t::operator*(); }
  };

  inline iterator begin(void) { return iterator(_nodes.begin()); }
  inline const_iterator begin(void) const { return const_iterator(_nodes.begin()); }
  inline iterator end(void) { return iterator(_nodes.end()); }
  inline const_iterator end(void) const { return const_iterator(_nodes.end()); }

  template <typename T>
  inline T& push_back( const string&name, auto&&...init ) {
    if( _names.contains(name) ) throw KeyError(name);
    T *pn = new T(name, std::forward<decltype(init)>(init)... );
    pn->_parent = this;
    ptr_t& ref =_nodes.push_back(pn);
    _names[name] = &ref;
    return *pn;
  }

  inline void pop_front() {
    string name = _nodes.front()->name();
    _names.erase(name);
    _nodes.pop_front();
  }

  inline Node& operator[]( const string &name ) {
    return **_names.at(name);
  }

  template <typename T>
  inline T& as( const string &name ) {
    Node &node = operator[] ( name ); // May throw KeyError
    if( T::ID != node.uid ) throw RuntimeError("name");
    return (T&)node;
  }

protected:
  friend class Node;

private:
  map<string,nodes_t::pointer>  _names;    ///< Name index to child-nodes ptr_t
  nodes_t                       _nodes;    ///< Child-nodes
};

using ABool   = ANode<UUID("bool"), bool>;
using AString = ANode<UUID("string"), string>;
using AFloat  = ANode<UUID("float"), float>;
using ADouble = ANode<UUID("double"), double>;
using AInt32  = ANode<UUID("int32"), int32_t>;
using AInt64  = ANode<UUID("int64"), int64_t>;
using AUInt32 = ANode<UUID("uint32"), uint32_t>;
using AUInt64 = ANode<UUID("uint64"), uint64_t>;

using avar_t = variant<bool,long,string,double>;
using AVar   = ANode<UUID("var"), avar_t>;

/**
 * @}
 */

} // namespace codie

#endif  // CODIE_TREE_HDR