#include "utest.hpp"

#include <codie/tree.hpp>
using namespace codie;

using StringNode = ANode<UUID("string"), string>;
using IntNode = ANode<UUID("int"), int>;

TCASE( Tree, Node ) {
  StringNode s1("Killroy");
  TEXPECT_EQ( string("Killroy"), s1.name() );
  TEXPECT_EQ( true, s1.value.empty() );
  
  StringNode s2("was", "here");
  TEXPECT_EQ( false, s2.value.empty() );
  TEXPECT_EQ( string("was"), s2.name() );
  TEXPECT_EQ( string("here"), s2.value  );

  s1.value = "hepp!";
  TEXPECT_EQ( false, s1.value.empty() );

  Node::ptr_t pn;
  TEXPECT_EQ( false, bool(pn) );

  pn.reset( new StringNode("Hello", "world") );
  TEXPECT_EQ( true, bool(pn) );
  TEXPECT_EQ( true, StringNode::Is( *pn ) );
  TEXPECT_EQ( false, IntNode::Is( *pn ) );

  size_t count = 0;
  for( auto &s : StringNode::If(*pn) ) std::ignore = s, count++;
  TEXPECT_EQ( 1, count );

  count = 0;
  for( auto &s : IntNode::If(*pn) ) std::ignore = s,count++;
  TEXPECT_EQ( 0, count );

  auto p = IntNode::create( "age", 42 );
  TEXPECT_EQ( true, bool(p) );
  
  TEXPECT_EQ( true, IntNode::Is( *p ) );
  TEXPECT_EQ( false, StringNode::Is( *p ) );
}

struct A  {
  A() { constructs++; }
  ~A() { destructs++; }

  static size_t constructs;
  static size_t destructs;
  static void reset() {
    constructs = 0;
    destructs  = 0;
  }
};

size_t A::constructs=0;
size_t A::destructs=0;

using NodeA = ANode<UUID("A"),A>;

struct B  {
  B() { constructs++; }
  ~B() { destructs++; }
  static size_t constructs;
  static size_t destructs;
  static void reset() {
    constructs = 0;
    destructs  = 0;
  }
};

size_t B::constructs=0;
size_t B::destructs=0;

using NodeB = ANode<UUID("B"),B>;

TCASE(Tree,Nodes) {

  TEXPECT_EQ( 0, A::constructs );
  TEXPECT_EQ( 0, A::destructs );
  TEXPECT_EQ( 0, B::constructs );
  TEXPECT_EQ( 0, B::destructs );
  {
    Nodes root("root");

    TEXPECT_EQ( true, root.empty() );
    root.push_back<NodeA>("first");
    TEXPECT_EQ( 1, A::constructs );
    TEXPECT_EQ( 0, A::destructs );
    TEXPECT_EQ( 0, B::constructs );
    TEXPECT_EQ( 0, B::destructs );
    root.push_back<NodeB>("second" );
    TEXPECT_EQ( 1, A::constructs );
    TEXPECT_EQ( 0, A::destructs );
    TEXPECT_EQ( 1, B::constructs );
    TEXPECT_EQ( 0, B::destructs );

    TEXPECT_THROW( root.push_back<NodeA>("second"), KeyError );

    TEXPECT_NOTHROW( root["first"] );
    TEXPECT_NOTHROW( root["second"] );
    TEXPECT_THROW( root["dummy"], KeyError );

    size_t count = 0;
    for( Node &node : root ) std::ignore=node,++count;

    TEXPECT_EQ( 2, count );
    
  }

  TEXPECT_EQ( 1, A::constructs );
  TEXPECT_EQ( 1, A::destructs );
  TEXPECT_EQ( 1, B::constructs );
  TEXPECT_EQ( 1, B::destructs );
}

TCASE(Tree,Tree) {
  NodeA::value_type::reset();
  NodeB::value_type::reset();
 
  TEXPECT_EQ( 0, A::constructs );
  TEXPECT_EQ( 0, A::destructs );
  TEXPECT_EQ( 0, B::constructs );
  TEXPECT_EQ( 0, B::destructs );

  Nodes root("");

  auto &first  = root.push_back<Nodes>("first");
  TEXPECT_EQ( &root, first.parent() );
  
  auto &a = first.push_back<NodeA>("a");
  TEXPECT_EQ( &first, a.parent() );
  auto &b = first.push_back<NodeA>("b");
  TEXPECT_EQ( &first, b.parent() );
  auto &second = root.push_back<Nodes>("second");
  auto &c = second.push_back<NodeB>("c");
  auto &d = second.push_back<NodeB>("d");

  TEXPECT_EQ( 2, A::constructs );
  TEXPECT_EQ( 0, A::destructs );
  TEXPECT_EQ( 2, B::constructs );
  TEXPECT_EQ( 0, B::destructs );

  TEXPECT_THROW( root.as<NodeA>("dummy"), KeyError );
  TEXPECT_THROW( root.as<NodeA>("first"), RuntimeError );
  TEXPECT_NOTHROW( second.as<NodeB>("c") );
  TEXPECT_THROW( second.as<NodeA>("d"), RuntimeError );

  TEXPECT_EQ( (Node*)&c, &second["c"] );
  TEXPECT_EQ( (Node*)&d, &second["d"] );
  
  root.clear();
  TEXPECT_EQ( 2, A::constructs );
  TEXPECT_EQ( 2, A::destructs );
  TEXPECT_EQ( 2, B::constructs );
  TEXPECT_EQ( 2, B::destructs );
}