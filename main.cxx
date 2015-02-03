#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <sstream>

using namespace std;

//#define DEBUG 1

#ifdef DEBUG
  #define LOGFUNCTION LOG<<__PRETTY_FUNCTION__<<"\n"
#else
  #define LOGFUNCTION
#endif
#define PRINTLOG std::cout<<LOG<<std::endl

class Log
{
    stringstream _ss;

   public:
    template <typename T>
    friend Log& operator<<(Log& log, T& t) {
        log._ss << t;
        return log;
    }
    friend ostream& operator<<(ostream& stream, Log const& log) {
        stream << log._ss.str();
        return stream;
    }
};

static Log LOG;

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

class VertexAdder
{
   public:
    VertexAdder() {
        LOGFUNCTION;
    }

    shared_ptr<const MeshDelta>
    execute(size_t thing) const {
        LOGFUNCTION;
        shared_ptr<MeshDelta> delta(new MeshDelta());
        delta->add(thing);
        return delta;
    }
};

class VertexRemover
{
   public:
    VertexRemover() {
        LOGFUNCTION;
    }

    shared_ptr<const MeshDelta>
    execute(size_t thing) const {
        LOGFUNCTION;
        shared_ptr<MeshDelta> delta(new MeshDelta());
        delta->add(thing, false);
        return delta;
    }
};

int main()
{
    LOGFUNCTION;
    vector<size_t> vertexIds(10);
    size_t start = 0;
    generate(vertexIds.begin(), vertexIds.end(), [&] { return start++; });

    BaseMesh baseMesh(vertexIds);
    MeshView meshView(baseMesh);
    VertexAdder adder;
    VertexRemover remover;
    meshView.addDelta(adder.execute(13));
    meshView.addDelta(remover.execute(6));
    meshView.addDelta(adder.execute(6));
    meshView.addDelta(remover.execute(13));
    meshView.addDelta(adder.execute(20));
    meshView.addDelta(adder.execute(2));
    meshView.iterate();

    PRINTLOG;
    return 0;
}

void MeshView::iterate() const {
    // iterate over base mesh
    {
        auto findInDeltas = [&](decltype(*_baseMesh.begin()) thing) {
            return find_if(_deltas.begin(),
                           _deltas.end(),
                           [&](decltype(*_deltas.begin()) delta) {
                       return delta->has(thing);
                   }) != _deltas.end();
        };
        for_each(_baseMesh.begin(),
                 _baseMesh.end(),
                 [&](decltype(*_baseMesh.begin()) thing) {
            if(!findInDeltas(thing)) {
                LOG << thing << " ";
            }
        });
        LOG << "\n";
    }
    // iterate over deltas
    {
        for(auto delta : _deltas) {
            for(auto thing : *delta) {
                bool inPreviousDelta = false;
                for(auto otherDelta : _deltas) {
                    if(otherDelta == delta)
                        break;
                    if(otherDelta->has(thing.first)) {
                        inPreviousDelta = true;
                        break;
                    }
                }
                if(inPreviousDelta) {
                    LOG << "skipping " << thing.first;
                    continue;
                }
                if(!thing.second)
                    LOG << "(";
                LOG << thing.first;
                if(!thing.second)
                    LOG << ")";
                LOG << " ";
            }
            LOG << "\n";
        }
    }
}
