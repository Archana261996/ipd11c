#include "graph_search.h"

#include <assert.h>

static bool
has_path_helper(const_graph_t g,
                vertex_t curr,
                vertex_t goal,
                bool seen[graph_size(g)])
{
    if (curr == goal) {
        return true;
    }

    if (seen[curr]) {
        return false;
    }

    seen[curr] = true;

    vertex_t g_size = graph_size(g);

    for (vertex_t succ = 0; succ < g_size; ++succ) {
        if (graph_has_edge(g, curr, succ) &&
            has_path_helper(g, succ, goal, seen)) {
            return true;
        }
    }

    return false;
}

bool graph_has_path(const_graph_t g, vertex_t start, vertex_t goal)
{
    vertex_t g_size = graph_size(g);

    assert( start < g_size );
    assert( goal < g_size );

    bool seen[g_size];

    for (vertex_t v = 0; v < g_size; ++v) {
        seen[v] = false;
    }

    return has_path_helper(g, start, goal, seen);
}

