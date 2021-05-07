#include "greedy_next.h"

ScanCover GreedyNext::solve() {
    // start time for computation time
    auto t_start = std::chrono::system_clock::now();    

    // init variables
    ScanCover solution = ScanCover();
    float curr_time = 0.0;
    satellite_orientation.clear();

    // Prepare edges for computation
    std::vector<const Edge*> remaining_edges;
    for (const Edge& e : instance.edges) {
        float t_communication = nextCommunication(e, 0.0f);
        if (t_communication < INFINITY) {
            remaining_edges.push_back(&e);
        }
    }
    size_t n_edges = remaining_edges.size();
    int finished_edges = 0;
    // choose best edge in each iteration.
    while (remaining_edges.size() > 0) {
        // allowed to continue?
        if(solver_abort == true)
            return ScanCover();        

        int best_edge_pos = 0;   // position in remaining edges
        float t_next = INFINITY; // absolute time

        // find best edge depending on the time passed
        for (int i = 0; i < remaining_edges.size(); i++) {
            const Edge& e = *remaining_edges.at(i);
            float next_communication = nextCommunication(e, curr_time);

            // edge is avaible earlier
            if (next_communication < t_next) {
                t_next = next_communication;
                best_edge_pos = i;
            }

            // it's not getting better
            if (t_next == 0)
                break;
        }

        if (callback != nullptr)
            callback((float)++finished_edges / n_edges);

        // refresh orientation of chosen satellites
        // todo only recalculate needed satellites ...
        const Edge* e = remaining_edges.at(best_edge_pos);
        EdgeOrientation new_orientations = e->getOrientation(t_next);
        satellite_orientation[&e->getV1()] = new_orientations.sat1;
        satellite_orientation[&e->getV2()] = new_orientations.sat2;

        // map position in remaining edges to position in all edges
        std::ptrdiff_t edge_index = remaining_edges[best_edge_pos] - &instance.edges[0];
        // add edge
        solution.addEdgeDialog(static_cast<int>(edge_index), t_next, new_orientations);
        remaining_edges.erase(remaining_edges.begin() + best_edge_pos);
        curr_time = t_next;
    }
    solution.setLowerBound(lowerBound());
    solution.sort();

    // end time for computation time
    auto t_end = std::chrono::system_clock::now();
    std::chrono::duration<float> diff = t_end - t_start;
    solution.setComputationTime(diff.count());

    return solution;
}