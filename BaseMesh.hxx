#ifndef __BASEMESH_INCLUDE
#define __BASEMESH_INCLUDE

#include "Log.hxx"
#include <vector>

using std::vector;

class BaseMesh
{
    vector<size_t> const& _vertices;

   public:
    BaseMesh(vector<size_t> const& vertices) : _vertices(vertices) {
      LOGFUNCTION;
    }
    auto begin() const -> decltype(_vertices.begin()) {
      LOGFUNCTION;
        return _vertices.begin();
    }
    auto end() const -> decltype(_vertices.end()) {
      LOGFUNCTION;
        return _vertices.end();
    }
};


#endif
