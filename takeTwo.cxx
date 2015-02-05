#include <iostream>
#include <memory>
#include <unordered_set>
#include <vector>

class Vertex {
    size_t _id;
    std::vector<double> _coordinates;

   public:
    Vertex(Vertex const& v) : _id(v._id), _coordinates(v._coordinates) {
    }
    Vertex(Vertex&& v)
      : _id(std::move(v._id)), _coordinates(std::move(v._coordinates)) {
    }
    Vertex(size_t id) : _id(id), _coordinates() {
    }
    Vertex(size_t id, double x, double y, double z)
      : _id(id), _coordinates({{x, y, z}}) {
    }
    class VertexHash {
       public:
        size_t operator()(Vertex const& v) const {
            return std::hash<size_t>()(v._id);
        }
    };
    friend class VertexHash;
    friend bool operator==(Vertex const& v1, Vertex const& v2) {
        return v1._id == v2._id;
    }
    bool
    isValid() const {
        return _id > 0;
    }
    auto getId() const -> decltype(_id) {
        return _id;
    }
    auto getCoordinates() const -> decltype(_coordinates) const & {
        return _coordinates;
    }
};

class MeshView {
    class MeshDelta {
        Vertex _badVertex;
        std::unordered_set<Vertex, Vertex::VertexHash> _things;

       public:
        MeshDelta() : _badVertex(0) {
        }
        bool
        has(size_t thing) const {
            return _things.find(thing) != _things.end();
        }
        Vertex const&
        get(size_t thing) const {
            auto vi = _things.find(thing);
            if(vi == _things.end())
                return _badVertex;
            else
                return *vi;
        }

        void
        add(size_t thing) {
            _things.insert(Vertex(thing));
        }
        template <typename... Args>
        void
        add(size_t thing, Args&&... args) {
            _things.insert(Vertex(thing, std::forward<Args>(args)...));
        }
    };

    std::unique_ptr<const MeshDelta> _meshDelta;
    std::shared_ptr<const MeshView> _prevView;

    MeshView() : _meshDelta(new MeshDelta), _prevView(nullptr) {
    }
    MeshView(std::unique_ptr<const MeshDelta> delta)
      : _meshDelta(std::move(delta)), _prevView(nullptr) {
    }
    MeshView(std::unique_ptr<const MeshDelta> delta,
             std::shared_ptr<const MeshView> view)
      : _meshDelta(std::move(delta)), _prevView(view) {
    }

   public:
    bool
    has(size_t thing) const {
        return _meshDelta->has(thing) || (_prevView && _prevView->has(thing));
    }
    static std::shared_ptr<const MeshView>
    makeView() {
        return std::shared_ptr<const MeshView>(new MeshView());
    }
    static std::shared_ptr<const MeshView>
    addThing(std::shared_ptr<const MeshView> view, size_t thing) {
        std::unique_ptr<MeshDelta> delta(new MeshDelta);
        delta->add(thing);
        return std::shared_ptr<MeshView>(new MeshView(std::move(delta), view));
    }
    template <typename... Args>
    static std::shared_ptr<const MeshView>
    addThing(std::shared_ptr<const MeshView> view,
             size_t thing,
             Args&&... args) {
        std::unique_ptr<MeshDelta> delta(new MeshDelta);
        delta->add(thing, std::forward<Args>(args)...);
        return std::shared_ptr<MeshView>(new MeshView(std::move(delta), view));
    }
    auto get(size_t thing) const -> decltype(_meshDelta -> get(thing)) {
        return _meshDelta->get(thing);
    }
};

int
main() {
    auto mv0 = MeshView::makeView();
    size_t thing = 0;
    auto mv1 = MeshView::addThing(mv0, thing);
    auto mv2 = MeshView::addThing(mv1, thing + 3, 1., 2., 3.);
    auto mv3 = MeshView::addThing(mv1, thing + 2);
    auto mv4 = MeshView::addThing(mv2, thing + 3, 8., 9., 10.);

    for(size_t i = 0; i < 4; ++i) {
        std::cout << "has? mv0(" << i << "," << mv0->has(i) << ")        mv1("
                  << i << "," << mv1->has(i) << ")"
                  << ")        mv2(" << i << "," << mv2->has(i) << ")"
                  << ")        mv3(" << i << "," << mv3->has(i) << ")"
                  << ")        mv4(" << i << "," << mv4->has(i) << ")"
                  << std::endl;
    }
    auto c4 = mv4->get(thing+3).getCoordinates();
    for(auto i : c4) std::cout << i << " "; std::cout << std::endl;

    auto c2 = mv2->get(thing+3).getCoordinates();
    for(auto i : c2) std::cout << i << " "; std::cout << std::endl;

    return 0;
}
