#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <sstream>

#include "BaseMesh.hxx"
#include "MeshView.hxx"
#include "MeshDelta.hxx"
#include "Log.hxx"

using namespace std;

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

Log LOG;

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
