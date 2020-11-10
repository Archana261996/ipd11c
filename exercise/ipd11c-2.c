#include "graph.h"
#include <libipd.h>
#include <assert.h>
#include<math.h>
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

// Determines whether there is a path from node `src` to node `dst`
// whose length is `n` or fewer.
//
// Examples: See tests.
bool graph_has_path_n(const_graph_t g,
                      vertex_t src,
                      vertex_t dst,
                      size_t n)
{
    if((dst-src)>n) {
        return false;
    }
    vertex_t g_size = graph_size(g);

    assert( src < g_size );
    assert( dst < g_size );

    bool seen[g_size];

    for (vertex_t v = 0; v < g_size; ++v) {
        seen[v] = false;
    }

    return has_path_helper(g, src, dst, seen);
}


// Builds the graph 0 -> 1 -> ... -> (N-2) -> (N-1).
static graph_t build_linear_graph(size_t N)
{
    graph_t g = graph_create(N);
    assert( g );

    for (vertex_t i = 0; i + 1 < N; ++i) {
        graph_add_edge(g, i, i + 1);
    }

    return g;
}

static void test_ghpn_3(void)
{
    graph_t g3  = build_linear_graph(3);

    CHECK( graph_has_path_n(g3, 0, 0, 0) );
    CHECK( graph_has_path_n(g3, 0, 0, 1) );
    CHECK( graph_has_path_n(g3, 0, 0, 2) );

    CHECK( ! graph_has_path_n(g3, 0, 1, 0) );
    CHECK( graph_has_path_n(g3, 0, 1, 1) );
    CHECK( graph_has_path_n(g3, 0, 1, 2) );

    CHECK( ! graph_has_path_n(g3, 0, 2, 0) );
    CHECK( ! graph_has_path_n(g3, 0, 2, 1) );
    CHECK( graph_has_path_n(g3, 0, 2, 2) );
    CHECK( graph_has_path_n(g3, 0, 2, 3) );

    CHECK( ! graph_has_path_n(g3, 2, 0, 10) );

    graph_destroy(g3);
}

static void test_ghpn_16(void)
{
    graph_t g16 = graph_create(16);
    assert( g16 );

    for (vertex_t i = 0; i + 1 < 16; ++i) {
        for (size_t j = i + 1; j < 16; ++j) {
            graph_add_edge(g16, i, j);
        }
    }

    CHECK( !graph_has_path_n(g16, 0, 15, 2) );
    CHECK( graph_has_path_n(g16, 14, 15, 1) );
    CHECK( graph_has_path_n(g16, 13, 15, 2) );
    CHECK( ! graph_has_path_n(g16, 2, 15, 10) );
    CHECK( graph_has_path_n(g16, 3, 15, 14) );
    CHECK( graph_has_path_n(g16, 0, 1, 2) );
    CHECK( ! graph_has_path_n(g16, 1, 15, 4) );
    CHECK( ! graph_has_path_n(g16, 10, 14, 0) );
    CHECK( graph_has_path_n(g16, 4, 14, 10) );
    CHECK( graph_has_path_n(g16, 0, 2, 3) );
    CHECK( ! graph_has_path_n(g16, 15, 0, 15) );


    graph_destroy(g16);
}

static void test_cycle_graph(void)
{
    graph_t g4 = graph_create(4);
    assert( g4 );

    for (size_t i = 0; i < 4; ++i)
        graph_add_edge(g4, i, (i + 1) % 4);

    CHECK( ! graph_has_path_n(g4, 0, 1, 0) );
    CHECK(   graph_has_path_n(g4, 0, 1, 1) );
    CHECK( ! graph_has_path_n(g4, 0, 2, 1) );
    CHECK(   graph_has_path_n(g4, 0, 2, 2) );
    CHECK( ! graph_has_path_n(g4, 0, 3, 0) );
    CHECK(   graph_has_path_n(g4, 0, 3, 3) );

    CHECK( ! graph_has_path_n(g4, 1, 0, 5) );
    CHECK( ! graph_has_path_n(g4, 1, 2, 0) );
    CHECK(   graph_has_path_n(g4, 1, 2, 2) );
    CHECK( ! graph_has_path_n(g4, 1, 3, 1) );
    CHECK(   graph_has_path_n(g4, 1, 3, 5) );

    CHECK( ! graph_has_path_n(g4, 2, 0, 5) );
    CHECK( ! graph_has_path_n(g4, 2, 1, 9) );
    CHECK(   graph_has_path_n(g4, 2, 3, 1) );
    CHECK( ! graph_has_path_n(g4, 2, 3, 0) );

    CHECK( ! graph_has_path_n(g4, 3, 0, 5) );
    CHECK( ! graph_has_path_n(g4, 3, 1, 9) );
    CHECK( ! graph_has_path_n(g4, 3, 2, 1) );
    CHECK( ! graph_has_path_n(g4, 3, 0, 0) );
    CHECK(   graph_has_path_n(g4, 3, 0, 1) );






    graph_destroy(g4);
}

int main(void)
{
    test_ghpn_3();
    test_ghpn_16();
    test_cycle_graph();

    // TODO: At least one more test here -- something with a cycle!
}