#include "MeshView.hxx"
#include "BaseMesh.hxx"
#include "MeshDelta.hxx"
#include "Log.hxx"

#include <algorithm>

void
MeshView::iterate() const {
    LOGFUNCTION;
    // iterate over base mesh
    {
      LOG<<"Iterating through base mesh:\n";
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
        size_t counter = 1;
        LOG << "Iterating through deltas:\n";
        for(auto delta : _deltas) {
            LOG << "printing contributions of delta: " << counter << "\n";
            ++counter;
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
