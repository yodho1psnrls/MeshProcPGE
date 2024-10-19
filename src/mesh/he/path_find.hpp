#pragma once

#include "he_mesh.hpp"
#include "../../utilities.hpp"  // uint

#include <vector>
#include <limits>
#include <algorithm>            // std::reverse


namespace mesh {


namespace he {


// Notes:
// The cuts need to not intersect (either it will iterate forever because of
// null twins
//  or even if you make the border iteration correct, it will need to move
//  around the preceeding cut

// Returns the half-edge IDs of the shortest path between the two given vertex
// ids

// @todo: It needs to give the shortestPath between a set of vertices, with the
// same pos
template <typename V>
inline std::pair<std::vector<typename HEMesh<V>::hedge_handle>, float>
shortest_path(const HEMesh<V> &hm, typename HEMesh<V>::vert_handle source_id,
             typename HEMesh<V>::vert_handle target_id) {
 
  using hedge_handle = typename HEMesh<V>::hedge_handle;
  using edge_handle = typename HEMesh<V>::edge_handle;
  using vert_handle = typename HEMesh<V>::vert_handle;
  using face_handle = typename HEMesh<V>::face_handle;

  static constexpr float inff = std::numeric_limits<float>::max();

  // Doesnt work with a bounded mesh or with holes

  // https://www.geeksforgeeks.org/shortest-path-in-a-directed-graph-by-dijkstras-algorithm/

  // Precalculate edge distance's so you dont have to calculate them multiple
  // times
  // vector<float> hedge_dists;

  // unordered_set<hedge_handle> visited_hedges;
  // unordered_set<vert_handle> visited_verts;
  std::vector<bool> visited;
  visited.assign(hm.verts_size(), false);

  // distance of all vertices from the source and the previous vertex
  // , so u can tract the shortest path

  //			              dist   prev_hedge
  std::vector<std::pair<float, hedge_handle>> dist_from;
  dist_from.assign(hm.verts_size(), {inff, hedge_handle()});

  vert_handle curr_id = source_id; // current vertex id
  dist_from[source_id].first = 0.0f;

  // Set of vertices that has
  // a parent (one or more)
  // marked as visited
  std::unordered_set<vert_handle> sett; // mainly used for termination of the while loop

  while (true) {

    visited[curr_id] = true;

    // for every child of the node // every vertex in its 1-ring neighbourhood
    hedge_handle id_begin = hm.outgoing(curr_id);
    hedge_handle e = id_begin; // child id of the curr_id

    do {
      vert_handle next_id = hm.head(e);

      if (visited[next_id]) {
        if (hm.is_border_hedge(e))
          break;
        e = hm.next(hm.twin(e));
        continue;
      }

      float alt = dist_from[curr_id].first + (hm.vert(curr_id).pos - hm.vert(next_id).pos).mag();

      if (alt > dist_from[target_id].first) {
        if (hm.is_border_hedge(e))
          break;
        e = hm.next(hm.twin(e));
        continue;
      }

      sett.insert(next_id);

      if (alt < dist_from[next_id].first)
        // dist_from[nextID] = { alt, c };
        dist_from[next_id] = {alt, e};
      // dist_from[nextID] = { alt, curr_id };

      if (hm.is_border_hedge(e))
        break;
      e = hm.next(hm.twin(e));

    } while (e != id_begin);

    sett.erase(curr_id);

    if (sett.empty())
      break;

    // The new current
    float min_dist = inff;
    vert_handle index;

    // Loop to update the distance of the vertices of the graph
    for (vert_handle a : sett) {
      if (dist_from[a].first < min_dist) {
        min_dist = dist_from[a].first;
        index = a;
      }
    }

    curr_id = index;
  }

  // if (dist_from[target_id].second == -1)
  if (dist_from[target_id].first == inff)
    return {std::vector<hedge_handle>(), inff};

  // Backtracking the dist and from pairs to find the shortest path
  std::vector<hedge_handle> result;
  float total_dist = 0.0f;
  curr_id = target_id;

  while (curr_id != source_id) {
    // uint hedge_id = twins[dist_from[curr_id].second];

    hedge_handle hedge_id = dist_from[curr_id].second;
    total_dist += dist_from[curr_id].first;

    result.push_back(hedge_id);
    curr_id = hm.head(hm.twin(hedge_id));
  }

  std::reverse(result.begin(), result.end());

  // returns the sequence of half-edge ids
  return {result, total_dist};
}



} // namespace he


} // namespace mesh
