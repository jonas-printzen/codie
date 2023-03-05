#ifndef CODIE_TREE_HDR
#define CODIE_TREE_HDR

#include <codie/common.hpp>
#include <codie/slist.hpp>

namespace codie {

/** @addtogroup MODEL
 *
 * @{
 */

/** @brief Node in a tree
 *
 *  A Node has a name and either a value or children, not both!
 *  Priority is given to the order of insertion and to avoid moving
 *  data around. To assert this, children is stored as an slist of 
 *  owning pointers to Node's. 
 */
class Node {
public:
  using ptr_t = Node*;    ///< Raw pointer - used mainly to point to parent 
private:
  ptr_t _parent=nullptr;
};


/**
 * @}
 */

} // namespace codie

#endif  // CODIE_TREE_HDR