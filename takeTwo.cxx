#include <iostream>
#include <memory>
#include <unordered_set>

using std::cout;
using std::endl;
using std::make_shared;
using std::move;
using std::shared_ptr;
using std::unique_ptr;
using std::unordered_set;

class MeshView
{
    class MeshDelta {
        unordered_set<size_t> _things;

       public:
        bool
        has(size_t thing) const {
          _things.find(thing) != _things.end();
        }
        void
        add(size_t thing) {
            _things.insert(thing);
        }
    };

    unique_ptr<const MeshDelta> _meshDelta;
    shared_ptr<const MeshView> _prevView;

    MeshView() : _meshDelta(new MeshDelta), _prevView(nullptr) {
    }
    MeshView(unique_ptr<const MeshDelta> delta)
      : _meshDelta(std::move(delta)), _prevView(nullptr) {
    }
    MeshView(unique_ptr<const MeshDelta> delta, shared_ptr<const MeshView> view)
      : _meshDelta(std::move(delta)), _prevView(view) {
    }

   public:
    bool
    has(size_t thing) const {
        return _meshDelta->has(thing) || (_prevView && _prevView->has(thing));
    }
    static shared_ptr<const MeshView>
    makeView() {
        return shared_ptr<const MeshView>(new MeshView());
    }
    static shared_ptr<const MeshView>
    addThing(shared_ptr<const MeshView> view, size_t thing) {
        unique_ptr<MeshDelta> delta(new MeshDelta);
        delta->add(thing);
        return shared_ptr<MeshView>(new MeshView(move(delta), view));
    }
};

int main()
{
  auto mv0 = MeshView::makeView();
  size_t thing = 0;
  auto mv1 = MeshView::addThing(mv0, thing);
  auto mv2 = MeshView::addThing(mv1, thing+4);
  auto mv3 = MeshView::addThing(mv1, thing+2);

  for(size_t i=0; i<10; ++i) {
    cout<<"has? mv0("<<i<<","<<mv0->has(i)
        <<")        mv1("<<i<<","<<mv1->has(i)<<")"
        <<")        mv2("<<i<<","<<mv2->has(i)<<")"
        <<")        mv3("<<i<<","<<mv3->has(i)<<")"
        <<endl;
  }

  return 0;
}
