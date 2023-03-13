#include "codie/common.hpp"
#include <codie/mfile.hpp>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/mman.h>

namespace codie {

MFile::~MFile() {
  close();
}

MFile::MFile() : _data((byte*)nullptr,0) {
}


MFile::MFile( const string &fname ) : _data((byte*)nullptr,0) {
  open(fname);
}

void MFile::open( const string &fname ) {

  if( data() ) throw RuntimeError("MFile::open(): Already opened!");
  int fd = ::open(fname.c_str(),O_RDONLY);
  if( 0 > fd ) throw RuntimeError(strerror(errno));
  struct stat fileInfo;
  fstat(fd,&fileInfo);
  auto p = mmap(NULL,fileInfo.st_size,PROT_READ,MAP_PRIVATE,fd,0);
  if( MAP_FAILED == p ) {
    throw RuntimeError(strerror(errno));
  }
  // That's it, we have it in memory ...
  _data={(const byte*)p,static_cast<size_t>(fileInfo.st_size)};
  ::close(fd);
}

void MFile::close() {
  if( _data.data() ) {
    ::munmap((void*)_data.data(),_data.size());
    _data={(const byte*)nullptr,0};
  }
}

} // pzen
