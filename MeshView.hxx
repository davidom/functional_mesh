#ifndef __MESHVIEW_INCLUDE
#define __MESHVIEW_INCLUDE

#include "Log.hxx"

#include <deque>
#include <memory>

using std::deque;
using std::shared_ptr;

class MeshDelta;
class BaseMesh;

class MeshView
{
    BaseMesh const& _baseMesh;
    deque<shared_ptr<const MeshDelta> > _deltas;

   public:
    MeshView(BaseMesh const& baseMesh) : _baseMesh(baseMesh) {
      LOGFUNCTION;
    }
    void
    addDelta(shared_ptr<const MeshDelta> delta) {
      LOGFUNCTION;
        _deltas.emplace_front(std::move(delta));
    }
    void iterate() const;
};

#endif
