#ifndef CODIE_MFILE_HDR
#define CODIE_MFILE_HDR

#include <codie/common.hpp>
#include <istream>

namespace codie {

/** @addtogroup IO
 *
 * @{
 */

/** @brief Memory mapped file
 *
 *  A file mapped to memory can be processed as a simple block of memory.
 *  This is intended for sppedy read-only access to input data...
 *  
 */
class MFile {
public:

  /** @brief Default constructor, to delay opening file.. */
  MFile();
  MFile( const string &fname );
  ~MFile();

  /** @brief Constructor to immediately map the file */
  struct isbuf : std::streambuf {
    isbuf( const cchars& data ) {
      setg((char*)data.data(), (char*)data.data(), (char*)data.data()+data.size());
    }
  };

  /** @brief Open and map a file
   *
   * @param fname - name of the file to map
   * @throw RuntimeError of file can't be mapped
   */
  void open( const string &fname );

  /** @brief Unmapp the memory for the file */
  void close();

  /** @brief Get a span to the data in file, as const char array */
  inline cchars as_text() const { 
    return  {(const char*)_data.data(), _data.size() }; 
  }

  /** @brief Get a span to the data in file, as const byte array */
  inline cbytes as_bytes() const {
    return  {(const byte*)_data.data(), _data.size()}; 
  }

  /** @brief Create an istream-compatible buffer
   *
   *  Use this to create an istream-compatible buffer
   *
   * @code
   * MFile afile("afile.txt");
   * auto buf = afile.as_buf();
   * istream in(&buf);
   *
   * in >> my_data >> other_data; 
   * @endcode
   */
  inline isbuf as_buf() const {
    return  isbuf(as_text());
  }

  inline constexpr const void*data() const { return  _data.data(); }

  inline constexpr size_t size() const { return  _data.size();}

private:
  cbytes _data;
};



/**
 * @}
 */

} // namespace codie

#endif  // CODIE_MFILE_HDR