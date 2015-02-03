#ifndef __MESHDELTA_INCLUDE
#define __MESHDELTA_INCLUDE

#include "Log.hxx"
#include <cassert>
#include <unordered_map>

using std::make_pair;
using std::unordered_map;

class MeshDelta
{
    unordered_map<size_t, bool> _entities;

   public:
    bool
    has(size_t thing) const {
      LOGFUNCTION;
        return _entities.find(thing) != _entities.end();
    }
    bool
    isValid(size_t thing) const {
      LOGFUNCTION;
        assert(has(thing));
        return _entities.find(thing)->second;
    }
    void
    add(size_t thing, bool value = true) {
      LOGFUNCTION;
        assert(!has(thing));
        _entities.insert(make_pair(thing, value));
    }
    auto begin() const -> decltype(_entities.begin()) {
      LOGFUNCTION;
        return _entities.begin();
    }
    auto end() const -> decltype(_entities.end()) {
      LOGFUNCTION;
        return _entities.end();
    }
};

#endif
