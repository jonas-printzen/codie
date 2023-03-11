#include <codie/tree.hpp>

namespace codie {

Node::~Node() {
}

void Nodes::clear() {
  while( !empty() ) pop_front();
}

Nodes::~Nodes() {
  clear();
}

} // namespace codie