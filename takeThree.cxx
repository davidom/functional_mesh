#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iterator>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class MeshDelta {
    std::unordered_map<size_t, bool> _objects;

   public:
    void
    add(size_t i) {
        _objects.insert(std::make_pair(i, true));
    }
    bool
    describes(size_t i) const {
        return _objects.find(i) != _objects.end();
    }
    auto begin() const -> decltype(_objects.begin()) {
        return _objects.begin();
    }
    auto end() const -> decltype(_objects.end()) {
        return _objects.end();
    }
};

class MeshView {
    std::shared_ptr<MeshView const> _prevMV;
    mutable std::shared_ptr<MeshView const> _nextMV;
    std::unique_ptr<MeshDelta const> _delta;

   public:
    // constructors
    MeshView() : _delta(new MeshDelta), _prevMV(nullptr), _nextMV(nullptr) {
    }
    MeshView(std::unique_ptr<const MeshDelta> delta)
      : _delta(std::move(delta)), _prevMV(nullptr), _nextMV(nullptr) {
    }
    MeshView(std::unique_ptr<const MeshDelta> delta,
             std::shared_ptr<MeshView const> view)
      : _delta(std::move(delta)), _prevMV(view), _nextMV(nullptr) {
        view->_nextMV = std::shared_ptr<const MeshView>(this);
    }

    // interface
    bool
    has(size_t i) const {
        return _delta->describes(i);
    }
    bool
    describes(size_t i) const {
        return has(i) || (_prevMV && _prevMV->describes(i));
    }
    bool rdescribes(size_t i) const {
      return _nextMV && (_nextMV->has(i) || _nextMV->rdescribes(i));
    }
    auto begin() const -> decltype(_delta -> begin()) {
        return _delta->begin();
    }
    auto end() const -> decltype(_delta -> end()) {
        return _delta->end();
    }
    auto next() const -> decltype(_nextMV) { return _nextMV; }
    auto prev() const -> decltype(_prevMV) { return _prevMV; }
};

void
iterateThroughThings(const MeshView *mv) {
    std::vector<size_t> numbers;
    for_each(mv->begin(), mv->end(), [&](decltype(*mv->begin()) thing) {
        numbers.push_back(thing.first);
    });
    std::sort(numbers.begin(), numbers.end());
    unsigned i=0;
    for(auto n : numbers) {
      if(!mv->rdescribes(n)) { std::cout<<n<<" ";
        ++i;
      }
    }
    std::cout <<"("<<i<<" of "<<numbers.size()<<")"<<std::endl;
    if(mv->next())
      iterateThroughThings(mv->next().get());
}

size_t numNumbers = 20;

size_t
random10() {
    return std::rand() % numNumbers;
}

int
main() {
    MeshDelta *delta0 = new MeshDelta;
    delta0->add(0);
    delta0->add(1);
    MeshView *mv = new MeshView(std::unique_ptr<const MeshDelta>(delta0));
    std::cout << "for mv 0, added: 0 1" << std::endl;

    std::srand(size_t(std::time(0)));
    std::vector<size_t> additions(10);
    std::vector<const MeshView *> views;
    views.push_back(mv);
    for(unsigned i = 0; i < 3; ++i) {
        std::generate(additions.begin(), additions.end(), random10);
        MeshDelta *delta = new MeshDelta;
        for(auto n : additions) delta->add(n);
        views.push_back(
            new MeshView(std::unique_ptr<const MeshDelta>(delta),
                         std::shared_ptr<const MeshView>(views[i])));
        std::cout << "for mv " << i + 1 << ", added: ";
        for(size_t i = 0; i < numNumbers; ++i)
            if(delta->describes(i))
                std::cout << i << " ";
        std::cout << std::endl;
    }

    iterateThroughThings(views[0]);
    return 0;
}
