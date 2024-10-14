#pragma once

#include "../../utilities.hpp" 
#include "../handlers.h"
//#include "../ngon_mesh.hpp"
//#include "../../point_cloud/point_vec.hpp"
#include "../base_mesh.hpp"

#include <iterator>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <set>

#include <stdexcept>
#include <type_traits>
#include <cassert>

//#define HE_INVALID_INDEX -1
//#define ISOLATED_INDEX -2


namespace mesh {

namespace he {

// Min Heap Priority Queue   // Access to the smallest element
//template<typename T>
//using rpriority_queue = typename std::priority_queue<T, std::vector<T>, std::greater<T>>;

static const index_hedge_handle ISOLATED_INDEX = index_hedge_handle(uint(-2));
//constexpr index_hedge_handle ISOLATED_INDEX = index_hedge_handle(uint(-2));


struct HalfEdge {

  pc::index_vert_handle head_id;
  index_hedge_handle next_id;
  index_face_handle face_id;  // face or border id
  
/*  HalfEdge(
    pc::index_vert_handle head_id = pc::index_vert_handle::INVALID_INDEX,
    index_hedge_handle next_id = index_hedge_handle::INVALID_INDEX,
    index_face_handle face_id = index_face_handle::INVALID_INDEX
  ) : head_id(head_id), next_id(next_id), face_id(face_id) {}
*/

  HalfEdge() {}

  HalfEdge(
    pc::index_vert_handle head_id,
    index_hedge_handle next_id,
    index_face_handle face_id
  ) : head_id(head_id), next_id(next_id), face_id(face_id) {}

};


// Face or Border face, that only stores one of its half-edges
/*struct Face {

  index_hedge_handle begin_id;

  Face() {}
//  Face() : begin_id(index_hedge_handle::INVALID_INDEX) {}
  Face(index_hedge_handle begin_id) : begin_id(begin_id) {}

};*/


// =============================================================================================== //


template <typename V>
class HEMesh {
public:

  
  using vert_handle = typename pc::index_vert_handle;
  using hedge_handle = typename mesh::he::index_hedge_handle;
  using edge_handle = typename mesh::he::index_edge_handle;
  using face_handle = typename mesh::index_face_handle;


  // --------------------------------------------------------------------------------------------- //


  //using face_handle = typename face_handle;
  //using border_handle = typename face_handle;

  void check_validity() const;

  HEMesh() = default;
 
  template <typename MeshType, typename = typename std::enable_if_t<
    std::is_base_of_v<BaseMesh<V, pc::PointVec<V>>, MeshType>
  //  && !std::is_same_v<MeshType, HEMesh<V>>
  >>
  HEMesh(const MeshType& m);

  template <typename MeshType, typename = typename std::enable_if_t<
    std::is_base_of_v<BaseMesh<V, pc::PointVec<V>>, MeshType>
  //  && !std::is_same_v<MeshType, HEMesh<V>>
  >>
  operator MeshType() const;


  void clear();
  void shrink_verts();
  void shrink_edges();
  void shrink_faces();
  void shrink();


  bool empty() const;
  bool are_verts_shrinked() const;
  bool are_edges_shrinked() const;
  bool are_faces_shrinked() const;
  bool is_shrinked() const;         // is the whole DS shrinked


// --------------------------------------------------------------------------- //


  const V& vert(const vert_handle) const;
  V& vert(const vert_handle);

  // half-edge to vertex
  vert_handle head(const hedge_handle) const;
  vert_handle tail(const hedge_handle) const;

  // vertex to half-edge
  hedge_handle outgoing(const vert_handle) const;
  hedge_handle ingoing(const vert_handle) const;

  // half-edge to incident half-edge
  hedge_handle next(const hedge_handle) const;
  hedge_handle prev(const hedge_handle) const;
  hedge_handle twin(const hedge_handle) const; // other-half/twin/opposite half-edge

  hedge_handle hedge(const edge_handle) const;
  hedge_handle hedge(const face_handle) const;
  edge_handle edge(const hedge_handle) const;  
  face_handle face(const hedge_handle) const; // get the face or border face of the half-edge

  hedge_handle hedge(const vert_handle, const vert_handle) const; // returns ISOLATED_INDEX if not found
  edge_handle edge(const vert_handle, const vert_handle) const;   // returns ISOLATED_INDEX if not found
  face_handle face(const vert_handle, const vert_handle) const;   // returns HE_INVALID_INDEX if not found
//  hedge_handle begin(const hedge_handle) const;
  
   
  // Those do NOT give the actual count of each 
  //  element type(verts, edges, ...) if it is not shrinked ! 
  uint verts_size() const;
  uint edges_size() const;
  uint hedges_size() const;
//  uint indices_size() const;  // Same as hedges_size, but added for generalization with the other mesh types
  uint faces_size() const;
  uint borders_size() const;

  void reserve_verts(const uint capacity);
  void reserve_hedges(const uint capacity);
  void reserve_edges(const uint capacity);
  void reserve_faces(const uint capacity); // reserve faces + borders count

  Iterable<vert_handle> verts() const;
  Iterable<edge_handle> edges() const;
  Iterable<hedge_handle> hedges() const;
  Iterable<face_handle> faces() const;
  Iterable<face_handle> borders() const;


  bool is_removed_face(const face_handle) const;
  bool is_removed_hedge(const hedge_handle) const;
  bool is_removed_edge(const edge_handle) const;
  //bool is_removed_edge(const hedge_handle) const;
  bool is_removed_vert(const vert_handle) const;

  bool is_border_face(const face_handle) const;
  bool is_border_hedge(const hedge_handle) const;
  bool is_border_edge(const hedge_handle) const;
  bool is_border_vert(const vert_handle) const;

  bool is_isolated_face(const hedge_handle) const; // face_handle or hedge_handle ??
  bool is_isolated_edge(const hedge_handle) const;
  bool is_isolated_vert(const vert_handle) const;


  //bool is_triangulated() const; // maybe do this as a generic function that works both on HEMesh, NgonMesh and other non-statically triangular Mesh classes 
  //bool is_face_size(const hedge_handle hedge_id, const uint n) const; // true if the face_size(hedge_id) == n, use this if you want to check for a size of the face, because its cheaper if it is a big face and you just want to check if it is a triangle for example
  //uint face_size(const hedge_handle hedge_id) const; // the number of adjacent edges/vertices to the face or border face of the half-edge
  
  
  bool is_face_size(const hedge_handle hedge_id, const uint n) const;
  bool is_face_size(const face_handle face_id, const uint n) const; // true if the face_size(hedge_id) == n, use this if you want to check for a size of the face, because its cheaper if it is a big face and you just want to check if it is a triangle for example 
  uint face_size(const hedge_handle hedge_id) const;
  uint face_size(const face_handle face_id) const; // the number of adjacent edges/vertices to the face or border face of the half-edge
 
  uint valence(const vert_handle vert_id) const;      // the number of adjacent edges to the vertex
  bool is_extremal(const vert_handle vert_id) const;  // a vertex is extremal if it is adjacent to only one edge

// --------------------------------------------------------------------------- //

  face_handle make_face(const face_handle);
  face_handle make_border(const face_handle);

  // Removes edges around the vertex untill all outgoing half-edges
	//  point to a unique head vertex
  void remove_dub_edges_around_vert(const vert_handle vert_id);

  // Removes edges around the face or border loop of the half-edge, that
	//  are not on the face, but the vertices at both ends of the edge are on the face.
	// So if we have a face with vertices 0, 1, 2, 3 , then the face edges
	//  are with vertices (0, 1) (1, 2), (2, 3), (3, 0) and they wont be removed
	//  but if there is an adjacent edge like an edge with vertices (0, 2) or (1, 3),
	//  then it will be removed. It is useful for conditioning large faces 
	//  (which are more likely to have such edges) before collapse 
  void remove_dub_edges_around_face(const hedge_handle hedge_id);


	// ============================ BASIC OPERATIONS =================================== //


	// Fills all holes of the surface of the mesh (all border loops are made face loops)
	void fill_holes();

	// Removes all internal edges (if it has any) in the face or border loop
	//  of the half-edge. Returns a half-edge that is in the same loop, but
	//  not removed, and if the whole face was a long chain of edges that do
	//  not for a loop, then the whole face is destroyed and returns HE_INVALID_INDEX
	hedge_handle remove_internal_edges(hedge_handle hedge_id);
  void remove_internal_edges(const face_handle face_id);

	// Removes internal edges of all faces
	void remove_internal_edges();

	// Removes all faces with size 2
	// (faces that only have 2 adjcacent edges and 2 adjacent vertices)
	void remove_degrade_faces();

	// Clears the mesh of all vertices that doesnt have any adjacent/incident edges
	void remove_isolated_verts();


  // Swaps the next half-edge index of the two half edges
  void swap_next(const hedge_handle hedge1, const hedge_handle hedge2);
 
  hedge_handle split_face_at(const hedge_handle back_hedge_id, const hedge_handle front_hedge_id);

  void make_isolated(const vert_handle vert_id);

  vert_handle add_vert(const V& v);
  
  hedge_handle add_edge(const hedge_handle back_hedge_id, const V& new_head_vert);
  hedge_handle add_edge(const vert_handle tail_id, const V& new_head_vert);
  
  hedge_handle add_edge(const vert_handle tail_id, const vert_handle head_id);
  //edge_handle add_edge(vert_handle a, hedge_handle b);
  hedge_handle add_edge(const hedge_handle back_hedge_id, const vert_handle head_id);
  hedge_handle add_edge(const hedge_handle back_hedge_id, const hedge_handle front_hedge_id);

  face_handle add_face(const std::vector<hedge_handle>& hedge_indices);
  face_handle add_face(const std::vector<vert_handle>& new_indices);

  void remove_vert(const vert_handle vert_id);
  void remove_edge(const hedge_handle hedge_id);
  bool remove_edge(const vert_handle vertA, const vert_handle vertB);
  void remove_face(const hedge_handle hedge_id);
  void remove_face(const face_handle face_id);

  void move_edge(const hedge_handle hedge_id, const hedge_handle front_hedge_id);
  void move_edge(const hedge_handle hedge_id, const hedge_handle back_hedge_id, const hedge_handle front_hedge_id);


  // --------------------------------------------------

  void flip_edge(const edge_handle hedge_id);

  vert_handle refine_edge(const hedge_handle hedge_id, const V& v); 
  vert_handle refine_edge(const hedge_handle hedge_id, const float h = 0.5f); 

  hedge_handle split_vert_to_edge(const hedge_handle hedgeA, const hedge_handle hedgeB, const V& v);
  hedge_handle split_vert_to_faces(const hedge_handle hedgeA, const hedge_handle hedgeB, const V& v);
  vert_handle split_edge(const hedge_handle hedge_id, const V& v);
  vert_handle split_edge(const hedge_handle hedge_id, const float h = 0.5f); 

  hedge_handle clip_corner(const hedge_handle hedge_id);
  
  vert_handle collapse_edge(const hedge_handle hedge_id, const V& v);
  vert_handle collapse_edge(const hedge_handle hedge_id, const float h = 0.5f);
  
  vert_handle collapse_face(hedge_handle hedge_id, const V& v);
  vert_handle collapse_face(const face_handle face_id, const V& v);
  vert_handle collapse_face(const hedge_handle hedge_id);
  vert_handle collapse_face(const face_handle face_id);

  face_handle bevel_vert(const vert_handle, const float h = 0.5f);
  face_handle bevel_edge(const hedge_handle hedge_id, const float h = 0.5f);
  face_handle bevel_face(const hedge_handle hedge_id, const float h = 0.5f);
  face_handle bevel_face(const face_handle face_id, const float h = 0.5f);

  face_handle cut_edge(const hedge_handle hedge_id);
//  face_handle join_faces(const hedge_handle hedge1, const hedge_handle hedge2); // removes all adjacent edges between the two faces



// --------------------------------------------------------------------------- //

private:

  V face_center(const hedge_handle hedge_id);

  void shift_borders_forward(); // Moves the first border at the place right after the last border
  void shift_borders_backward(); // Moves the last border at the place right before the first border
  //void shift_borders(const int how_much);

  hedge_handle new_edge();
  hedge_handle new_edge(const vert_handle tail_id, const vert_handle head_id); 

  //face_handle new_face(); // This one creates a new non-border face with an invalid half-edge
  //face_handle new_face(const hedge_handle begin_id);  // This one creates a new non-border face with a given valid half-edge and sets the whole half-edge loop with the new face
  face_handle new_face(const hedge_handle begin_id = hedge_handle()); // This one doesnt set the face indexes of the given half-edge
  
  //face_handle new_border(); // This one creates a new border face with an invalid half-edge 
  //face_handle new_border(const hedge_handle begin_id); // This one creates a new border face with a given valid half-edge and sets the whole half-edge loop with the new border
  face_handle new_border(const hedge_handle begin_id = hedge_handle()); // This one doesnt set the border indexes of the given half-edge


  void invalidate_vert(const vert_handle vert_id);
  void invalidate_edge(const edge_handle edge_id);
  void invalidate_face(const face_handle face_id);
  //void invalidate_border(const face_handle border_id);


  void free_vert(const vert_handle vert_id);
  void free_edge(const edge_handle edge_id);
  void free_face(const face_handle face_id);
  void free_border(const face_handle border_id);

  bool is_in_range_vert(const vert_handle vert_id) const;
  bool is_in_range_hedge(const hedge_handle hedge_id) const;
  //bool is_in_range_edge(const edge_handle edge_id) const;
  bool is_in_range_face(const face_handle face_id) const;
  //bool is_in_range_face(const face_handle face_id) const;
  //bool is_in_range_border(const face_handle face_id) const;


  void check_vert_id(const vert_handle vert_id) const;
  void check_hedge_id(const hedge_handle hedge_id) const;
  //void check_edge_id(const edge_handle& edge_id) const;
  void check_face_id(const face_handle face_id) const; // check if it is a valid face or border handler


  // Checks if two half-edges share the same face , iterating through the next half-edges, it does not just compare the loop_ids
  bool are_same_face(const hedge_handle hedge_id1, const hedge_handle hedge_id2) const;

  void shift_begin(const face_handle face_id, const uint how_much = 1);
  //void set_face_id(const hedge_handle hedge_id);
  //void set_begin_id(const hedge_handle hedge_id, const hedge_handle begin_id);
  void set_face_id(const hedge_handle hedge_id, const face_handle face_id);
  void set_face_id(const face_handle face_id);
  void set_vert_id(const hedge_handle hedge_id, const vert_handle new_head_vert_id); 
  

  face_handle find_common_face_around_head(hedge_handle& he1, hedge_handle& he2);
  face_handle find_border_around_head(hedge_handle& hedge_id);

  hedge_handle ltwin(const hedge_handle hedge_id) const;
  hedge_handle rtwin(const hedge_handle hedge_id) const;


// ------------------------------------------------------------------------ //


  std::vector<V> _vertices;
  std::vector<hedge_handle> _vert_to_hedge;

  std::vector<HalfEdge> _hedges;

  // All loops that form a face or border
  // Faces span from 0 to _faces_cap - 1 // Borders span from _faces_cap to _faces_and_borders.size() 
  //std::vector<Face> _faces_and_borders;  
  std::vector<hedge_handle> _face_to_hedge;

  uint _faces_cap{};

  // Garbage collection
  std::set<vert_handle> _removed_verts;
  std::set<edge_handle> _removed_edges;

  // This stores only removed faces
  // The borders will be always shrinked
  std::set<face_handle> _removed_faces;

  // BORDERS WILL ALWAYS BE SHRINKED, TO AVOID THE NEED TO
  // MAINTAIN BOTH _removed_faces and _removed_borders sets,
  // BECAUSE IT WOULD BE TRICKY IF THE FACES RESIZE AND IT
  // HAPPENS SUCH THAT THERE IS A FACE INDEX IN _removed_faces,
  // OR THERE IS A BORDER INDEX IN _removed_borders

};


// ========================= IMPLEMENTATION ==================================== //


template<typename V>
inline void HEMesh<V>::check_validity() const {
	//std::cout << "======================================================\n";

	for (face_handle f : faces()) {
    if(is_removed_face(f))
      continue;  

		hedge_handle e = hedge(f);

    do {
			if (is_removed_hedge(e)) {
				std::cout << "Invalid hedge " << e << " with begin of face " << f << "\n";
				break;
			}
			else if (face(e) != f) {
        std::cout << "Hedge " << e << " points to another face " << face(e) << " rather than its face " << f << "\n";

        for(face_handle other_face : faces())
          if (hedge(other_face) == e)
            std::cout << "Dublicated face hedge " << e << " in face " << f << "\n";

        for(face_handle other_face : borders())
          if (hedge(other_face) == e)
            std::cout << "Dublicated border hedge " << e << " in face " << f << "\n";
			 
      }

			e = next(e);
		} while (e != hedge(f));
	}


	for (face_handle b : borders()) {
    if(is_removed_face(b))
      continue;

		hedge_handle e = hedge(b);
	
    do {
			if (is_removed_hedge(e)) {
				std::cout << "Invalid hedge " << e << " at border " << b << "\n";
				break;
			}
			else if (face(e) != b) {
        std::cout << "Hedge " << e << " points to another face " << face(e) << " rather than its border " << b << "\n";

        for(face_handle other_face : faces())
          if (hedge(other_face) == e)
            std::cout << "Dublicated face hedge " << e << " in border " << b << "\n";

        for(face_handle other_face : borders())
          if (hedge(other_face) == e)
            std::cout << "Dublicated border hedge " << e << " in border " << b << "\n";
			 
      }
			e = next(e);
		} while (e != hedge(b));
	}


	for (vert_handle vi : verts()) {
		hedge_handle e = _vert_to_hedge[vi];

		if (e == hedge_handle() || e == ISOLATED_INDEX)
			continue;

		if (is_removed_hedge(e))
			std::cout << "Tail vert " << vi << " points to removed hedge " << e << "\n";
		else if (tail(e) != vi)
			std::cout << "Tail vert " << vi << " points to outgoing hedge " << e << ", but its tail is " << tail(e) << "\n";

		e = twin(e);
		hedge_handle beg = e;

		do {
			if (head(e) != vi)
				//std::cout << "Vert " << vi << " has an incident ingoing hedge " << e << ", but the hedge points to a head vertex " << head(e) << "\n";
				std::cout << "Head vert " << vi << " points to ingoing hedge " << e << ", but its head is " << head(e) << "\n";

			e = twin(next(e));
		} while (e != beg);
	}

	// Check if it points to a valid face or border index
	for (hedge_handle e : hedges()) {
		if (is_removed_hedge(e)) {
		//if (_hedges[e] == HalfEdge())
		
			// Edges are managed as pairs of half-edges
			// so if a half-edge is removed, its twin should also be marked as removed
			if (!is_removed_hedge(twin(e)))
				std::cout << "Hedge " << e << " is invalidated, but its twin is not\n";

			if (_removed_edges.find(ltwin(e)) == _removed_edges.end())
				std::cout << "Hedge " << e << " is invalidated, but is not in the garbage list of removed hedges\n";
			
			continue;
		}

		if (head(twin(e)) != head(prev(e)))
			//std::cout << "Hedge " << e << "'s tail vertex is inconsistent\n";
			std::cout << "The twin and prev hedges of hedge " << e << " , should have the same head vertex\n";

    

		if (e == next(e))
			std::cout << "Infinite loop: hedge " << e << "'s next hedge is itself\n";

		if (head(e) == tail(e))
			std::cout << "Degrade edge " << e << "(its head and tail vertices are the same)\n";

		if (next(next(e)) == e)
			std::cout << "Degrade face " << face(e) << "\n";

		if (outgoing(head(e)) == ISOLATED_INDEX)
			std::cout << "Hedge " << e << " points to a head vertex " << head(e) << " , which is marked as isolated\n";
		else if (outgoing(head(e)) == hedge_handle(hedge_handle::INVALID_INDEX))
			std::cout << "Hedge " << e << " points to a head vertex " << head(e) << " , which is marked as removed\n";
		else if (head(e) != tail(e)) { // if not degrade (because a degrade edge will always fail this test)

			hedge_handle head_hedge = twin(outgoing(head(e)));
			hedge_handle ee = head_hedge;
			bool isValid = false;

			do {
				if (ee == e)
					isValid = true;

				ee = twin(next(ee));
			} while (ee != head_hedge);

			if (!isValid)
				std::cout << "Hedge " << e << " points to an adjacent vertex from which you cannot find adjacency with the hedge\n";
		}
	}

	for (edge_handle e : _removed_edges) {
		hedge_handle he(e);

    if (he % 2 != 0)
			std::cout << "Only even half-edge indexes should be in the garbage set of half-edges !\n";
		
		//if (_hedges[he] != HalfEdge() || _hedges[he + 1] != HalfEdge())
		if (!is_removed_hedge(he) || !is_removed_hedge(twin(he)))
			std::cout << "There is an pair of hedges " << he << " and " << he + 1 << " which are garbaged, but not invalidated\n";
	
	}


  // The borders should be always dense/shrinked
  for (face_handle b : borders()) {
    if (is_removed_face(b))
      std::cout << "Border " << b << " is invalid\n";
  }

  for(face_handle f : _removed_faces) {
    if (!is_in_range_face(f))
      std::cout << "Face " << f << " is in in the removed_faces list, but is out of range\n";
    else {
      if (!is_removed_face(f))
        std::cout << "Face " << f << " is in the removed_faces list, but is NOT invalidated\n";
      if (f >= _faces_cap)
        std::cout << "All removed faces should be non-border, but " << f << " is\n";
    }
  }


}


// ------------------------------------------------------------------------ //


template <typename V>
//inline HEMesh<V>::HEMesh(const NgonMesh<V, pc::PointVec<V>>& m) {
template <typename MeshType, typename>
inline HEMesh<V>::HEMesh(const MeshType& m) {
//template <typename V>
//template <typename MeshType, typename>
//inline HEMesh<V>::HEMesh(const MeshType& m) {

  _vertices = m; // NgonMesh<V, PointVec> -> PointVec<V> -> std::vector<V>
  //_vertices.insert(other_verts.begin(), other_verts.end());

  _vert_to_hedge.assign(_vertices.size(), ISOLATED_INDEX);
	_face_to_hedge.reserve(m.faces_size());
	_hedges.reserve(m.indices_size());
	//_hedges.assign(m.indices.size(), HalfEdge());
	//_hedges.resize(indices.size());

	std::unordered_map<std::pair<uint, uint>, hedge_handle> vert_pair_to_hedge;

	//uint found_edges = 0U;

	hedge_handle curr_hedge_id;
	hedge_handle prev_hedge_id;
	hedge_handle curr_begin_id;
  face_handle curr_face_id;

  for (face_handle fh : m.faces()) {
		//faces().push_back(i);
  
    auto face_verts = m.verts(fh);
    curr_face_id = (face_handle)_face_to_hedge.size();
    //curr_face_id = _faces_cap++;

    auto it = face_verts.begin();

    do { 
      auto nit = it;
      ++nit;

      if (nit == face_verts.end())
        nit = face_verts.begin();

      uint id0 = *it;
      uint id1 = *nit;     
     
     	if (vert_pair_to_hedge.find({ id0, id1 }) != vert_pair_to_hedge.end())
				throw std::invalid_argument("Trying to convert a Non-Consistently-Orientated or Non-2-Manifold mesh to a half edge data structure !");

			auto mp = vert_pair_to_hedge.find({ id1, id0 });
			if (mp == vert_pair_to_hedge.end()) {
				//curr_hedge_id = 2ULL * found_edges;
				//++found_edges;

				curr_hedge_id = hedge_handle(_hedges.size());
				_hedges.push_back(HalfEdge());
				_hedges.push_back(HalfEdge());

				vert_pair_to_hedge[{id0, id1}] = curr_hedge_id;
			}
			else {
				curr_hedge_id = hedge_handle(mp->second + 1ULL);
				vert_pair_to_hedge.erase(mp); // erase, so later we are left only with the border half edges
			}

			_vert_to_hedge[id0] = curr_hedge_id;

//	    if (j != 0ULL)
      if (it != face_verts.begin())
        _hedges[prev_hedge_id].next_id = curr_hedge_id;
      else
				curr_begin_id = curr_hedge_id;

			prev_hedge_id = curr_hedge_id;

			_hedges[curr_hedge_id].face_id = curr_face_id;

			//_hedges[curr_hedge_id] = HalfEdge(id1);
			_hedges[curr_hedge_id].head_id = vert_handle(id1);

      it = nit;
		} while (it != face_verts.begin());

		_face_to_hedge.push_back(curr_begin_id);
		_hedges[prev_hedge_id].next_id = curr_begin_id;

	}

  _faces_cap = _face_to_hedge.size();

	// Assign Borders ========================================================== //

	std::unordered_set<hedge_handle> inner_border_hedges;
	for (const auto& it : vert_pair_to_hedge)
		inner_border_hedges.insert(it.second);

	// Traverses the boundary inner HalfEdges, to create the outer HalfEdges
	while (!inner_border_hedges.empty()) {
		hedge_handle begin_he = *inner_border_hedges.begin();   // inner edge border begin
		hedge_handle e = begin_he;								// inner border edge
		hedge_handle prev = hedge_handle(hedge_handle::INVALID_INDEX);                // previous inner border edge

		do {

			inner_border_hedges.erase(inner_border_hedges.find(e));

			// If not first iteration
			//if (e != begin_he)
			if (prev != hedge_handle::INVALID_INDEX) {
				//_hedges[twin(e)].next_id = prev;
				_hedges[twin(e)].next_id = twin(prev);
				_hedges[twin(e)].head_id = _hedges[prev].head_id;
				//_hedges[twin(e)].begin_id = curr_begin_id;  
			  _hedges[twin(e)].face_id = curr_face_id;
      }
			else {
        curr_begin_id = twin(e);
        curr_face_id = (face_handle)_face_to_hedge.size(); // Here in the border assignment we will use the curr_face_id as curr_border_id
      }				

			prev = e;

			// Find Next Inner Edge
			e = _hedges[e].next_id;
			while (e != begin_he) {
				//if (next(twin(e)) == HE_INVALID_INDEX)					// !!!!
				if (_hedges[twin(e)].next_id == hedge_handle::INVALID_INDEX)
					break;

				e = _hedges[twin(e)].next_id;
			}

		} while (e != begin_he);

		_hedges[twin(e)].next_id = twin(prev);
		_hedges[twin(e)].head_id = _hedges[prev].head_id;
		//_hedges[twin(e)].begin_id = curr_begin_id;
    _hedges[twin(e)].face_id = curr_face_id;

		_face_to_hedge.push_back(curr_begin_id);
	}

}


template <typename V>
//inline HEMesh<V>::operator NgonMesh<V, pc::PointVec<V>>() const {
template <typename MeshType, typename> 
inline HEMesh<V>::operator MeshType() const {
  //if (!is_shrinked())
  if (!are_verts_shrinked())
    throw std::invalid_argument("Shrink the HEMesh's vertices before converting to another Mesh Type");

	//NgonMesh<V, pc::PointVec<V>> m(_vertices);
	MeshType m(_vertices);
  //m.indices.reserve(_hedges.size());
  m.reserve_indices(_hedges.size());

  hedge_handle e;
  hedge_handle begin_id;
  
  std::vector<vert_handle> curr_face_indices;

	for (face_handle fh : faces()) {
    if (is_removed_face(fh))
      continue;

    //he = hedge(f);
    begin_id = _face_to_hedge[fh];

//    if (is_face_size(begin_id, 2))
//			continue;
    
    curr_face_indices.clear();

    e = begin_id;
    do {
      curr_face_indices.push_back(head(e));

      e = next(e);
    } while (e != begin_id);

    m.add_face(curr_face_indices);
	}

	return m;
}


// ------------------------------------------------------------------------ //


template<typename V>
inline void HEMesh<V>::clear() {
	_vertices.clear();
	_vert_to_hedge.clear();
	_hedges.clear();
	_face_to_hedge.clear();

	_removed_verts.clear();
	_removed_edges.clear();
  _removed_faces.clear();

  _faces_cap = 0;
}


template<typename V>
inline void HEMesh<V>::shrink_verts() {
	for (auto it = _removed_verts.rbegin(); it != _removed_verts.rend(); ++it)
		free_vert(*it);

	_removed_verts.clear();
}

template<typename V>
inline void HEMesh<V>::shrink_edges() {
	for (auto it = _removed_edges.rbegin(); it != _removed_edges.rend(); ++it)
		free_edge(*it);

	_removed_edges.clear();
}


template<typename V>
inline void HEMesh<V>::shrink_faces() {
	for (auto it = _removed_faces.rbegin(); it != _removed_faces.rend(); ++it)
		free_face(*it);

	_removed_faces.clear();
}


template<typename V>
inline void HEMesh<V>::shrink() {
	//std::cout << "removed verts: " << _removed_verts.size() << "\n";
	//std::cout << "removed edges: " << _removed_edges.size() << "\n";
  //std::cout << "removed faces: " << _removed_faces.size() << "\n";

	shrink_verts();
	shrink_edges();
  shrink_faces();

}


template<typename V>
inline bool HEMesh<V>::empty() const {
	//return faces().empty() && _borders.empty()
	return _vertices.size() == _removed_verts.size()
		&& _hedges.size() == (_removed_edges.size() << 1ULL);
  //  && _faces_and_borders.size() == _removed_faces.size();
	//	&& _hedges.size() == 2ULL * _removed_edges.size();
}


template<typename V>
inline bool HEMesh<V>::are_verts_shrinked() const {
	return _removed_verts.empty();
}

template<typename V>
inline bool HEMesh<V>::are_edges_shrinked() const {
	return _removed_edges.empty();
}

template<typename V>
inline bool HEMesh<V>::are_faces_shrinked() const {
	return _removed_faces.empty();
}


template<typename V>
inline bool HEMesh<V>::is_shrinked() const {
	return _removed_verts.empty() && _removed_edges.empty() && _removed_faces.empty();
}


// ----------------------------------------------------------- //


template <typename V>
inline V& HEMesh<V>::vert(const vert_handle vert_id) {
	check_vert_id(vert_id);
  return _vertices[vert_id];
}

template <typename V>
inline const V& HEMesh<V>::vert(const vert_handle vert_id) const {
  check_vert_id(vert_id);
  return _vertices[vert_id];
}

template<typename V>
inline HEMesh<V>::vert_handle HEMesh<V>::head(const hedge_handle hedge_id) const {
	return _hedges[hedge_id].head_id;
}

template<typename V>
inline HEMesh<V>::vert_handle HEMesh<V>::tail(const hedge_handle hedge_id) const {
	return _hedges[twin(hedge_id)].head_id;
}

template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::outgoing(const vert_handle vert_id) const {
	return _vert_to_hedge[vert_id];
}

template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::ingoing(const vert_handle vert_id) const {
	hedge_handle he = _vert_to_hedge[vert_id];

	//if (!is_in_range(he))
	if (he != hedge_handle::INVALID_INDEX && he != ISOLATED_INDEX)
    he = twin(he);

	return he;
}

template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::next(const hedge_handle hedge_id) const {
	return _hedges[hedge_id].next_id;
}

template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::prev(const hedge_handle hedge_id) const {
	hedge_handle e = hedge_id;

  // The most of the faces may be triangles, which are faster to iterate by the face loop
  // But most of the borders are way longer than triangles, so borders we will iterate
  //  through the tail vertex
  // The average vertex valence is 6, so for borders longer than 6 edges, it should be worth it

  if (!is_border_hedge(hedge_id)) {
	  while (next(e) != hedge_id)
		  e = next(e);
  }
  else {
    e = twin(e);
    while(next(e) != hedge_id)
      e = twin(next(e));
  }
  
	return e;
}

template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::twin(const hedge_handle hedge_id) const {
	//uint j = hedge_id % 2;
	//return hedge_id - j + (j + 1) % 2;
	return hedge_handle(hedge_id ^ 1U);
}


template <typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::hedge(const edge_handle edge_id) const {
	//return hedge_handle(edge_id << 1);
	return hedge_handle(edge_id);
}

template <typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::hedge(const face_handle face_id) const {
	return _face_to_hedge[face_id];
}

template <typename V>
inline HEMesh<V>::edge_handle HEMesh<V>::edge(const hedge_handle hedge_id) const {
	//return edge_handle(hedge_id >> 1);
	return edge_handle(hedge_id);
}

template <typename V>
inline HEMesh<V>::face_handle HEMesh<V>::face(const hedge_handle hedge_id) const {
	//return edge_handle(hedge_id >> 1);
	return _hedges[hedge_id].face_id;
}


template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::hedge(const vert_handle tail_vert_id, const vert_handle head_vert_id) const {
	hedge_handle et = _vert_to_hedge[tail_vert_id];
	hedge_handle eh = _vert_to_hedge[head_vert_id];

	if (et != ISOLATED_INDEX && eh != ISOLATED_INDEX) {
		hedge_handle e = et;

		do {
			if (head(e) == head_vert_id)
				return e;

			e = next(twin(e));
		} while (e != et);
	}

	return ISOLATED_INDEX;
}

template<typename V>
inline HEMesh<V>::edge_handle HEMesh<V>::edge(const vert_handle tail_vert_id, const vert_handle head_vert_id) const {
  //return edge(hedge(tail_vert_id, head_vert_id));
  hedge_handle he = hedge(tail_vert_id, head_vert_id);
  
  if (he == ISOLATED_INDEX)
    return ISOLATED_INDEX;

  return edge_handle(he);
}

template<typename V>
inline HEMesh<V>::face_handle HEMesh<V>::face(const vert_handle vert_id1, const vert_handle vert_id2) const {
	const hedge_handle& beg1 = _vert_to_hedge[vert_id1];
	const hedge_handle& beg2 = _vert_to_hedge[vert_id2];

  // If one of the vertices is isolated, then there is
  //  no way they share the same face or border face
  if (beg1 == ISOLATED_INDEX || beg2 == ISOLATED_INDEX)
//    return face_handle::INVALID_INDEX;
    return face_handle();

	hedge_handle he1 = beg1;
	hedge_handle he2 = beg2;

	do {
		do {

			if (face(he1) == face(he2))
				return face(he1);

			he2 = next(twin(he2));
		} while (he2 != beg2);

		he1 = next(twin(he1));
	} while (he1 != beg1);

//	return face_handle::INVALID_INDEX;
  return face_handle();
}


//template<typename V>
//inline const uint& HEMesh<V>::hedge(const uint& tail_vert_id) const {
//	return _vert_to_hedge[tail_vert_id];
//}


//template <typename V>
//inline typename HEMesh<V>::hedge_handle HEMesh<V>::begin(const hedge_handle hedge_id) const {
//  return hedge(face(hedge_id));
//}


// -----------------------------------------------------


template<typename V>
inline uint HEMesh<V>::verts_size() const {
//	return _vertices.size() - _removed_vertices.size();
	return _vertices.size();
}

template<typename V>
inline uint HEMesh<V>::hedges_size() const {
//	return _hedges.size() - _removed_edges.size() * 2ULL;
	return _hedges.size();
}

/*template<typename V>
inline uint HEMesh<V>::indices_size() const {
	return _hedges.size();

//  uint n = 0;
//  for(face_handle b : borders())
//    n += face_size(b);
//  return _hedges.size() - n; 
}*/

template<typename V>
inline uint HEMesh<V>::edges_size() const {
	//return _hedges.size() / 2ULL - _removed_hedges.size();
	return uint(_hedges.size()) >> 1;
}

template<typename V>
inline uint HEMesh<V>::faces_size() const {
	return _faces_cap;
}

template<typename V>
inline uint HEMesh<V>::borders_size() const {
	return _face_to_hedge.size() - _faces_cap;
}

template<typename V>
inline void HEMesh<V>::reserve_verts(const uint capacity) {
	_vertices.reserve(capacity);
}

template<typename V>
inline void HEMesh<V>::reserve_hedges(const uint capacity) {
	_hedges.reserve(capacity);
}

/*template<typename V>
inline void HEMesh<V>::reserve_indices(const uint capacity) {
	_hedges.reserve(capacity);
}*/

template<typename V>
inline void HEMesh<V>::reserve_edges(const uint capacity) {
	_hedges.reserve(capacity << 1);
}

template<typename V>
inline void HEMesh<V>::reserve_faces(const uint capacity) {
	_face_to_hedge.reserve(capacity);
}

/*template<typename V>
inline void HEMesh<V>::reserve_faces(const uint capacity) {
	uint cap = capacity + borders_size();
  _faces_and_borders.reserve(cap);
}

template<typename V>
inline void HEMesh<V>::reserve_borders(const uint capacity) {
	uint cap = capacity + faces_size();
  _faces_and_borders.reserve(cap);
}*/


template <typename V>
inline Iterable<typename HEMesh<V>::vert_handle> HEMesh<V>::verts() const {
  return Iterable(vert_handle(0), vert_handle(_vertices.size()));
}


template <typename V>
inline Iterable<typename HEMesh<V>::edge_handle> HEMesh<V>::edges() const {
  return Iterable(edge_handle(0), edge_handle(edges_size()));
}


template <typename V>
inline Iterable<typename HEMesh<V>::hedge_handle> HEMesh<V>::hedges() const {
  return Iterable(hedge_handle(0), hedge_handle(_hedges.size()));
}


template <typename V>
inline Iterable<typename HEMesh<V>::face_handle> HEMesh<V>::faces() const {
  return Iterable(face_handle(0), face_handle(_faces_cap));
}


template <typename V>
inline Iterable<typename HEMesh<V>::face_handle> HEMesh<V>::borders() const {
  return Iterable(face_handle(_faces_cap), face_handle(_face_to_hedge.size()));
}


// -----------------------------------------------------------------------


template<typename V>
inline bool HEMesh<V>::is_removed_vert(const vert_handle vert_id) const {
	return _vert_to_hedge[vert_id] == hedge_handle::INVALID_INDEX;
}

template<typename V>
inline bool HEMesh<V>::is_removed_hedge(const hedge_handle hedge_id) const {
	return _hedges[hedge_id].next_id == hedge_handle::INVALID_INDEX;
}

template<typename V>
inline bool HEMesh<V>::is_removed_edge(const edge_handle edge_id) const {
	return _hedges[uint(edge_id) << 1].next_id == hedge_handle::INVALID_INDEX;
	//return _hedges[hedge(edge_id)].next_id == HE_INVALID_INDEX;
	//return _hedges[hedge_id] == HalfEdge();
}

template <typename V>
inline bool HEMesh<V>::is_removed_face(const face_handle face_id) const { 
  return _face_to_hedge[face_id] == hedge_handle::INVALID_INDEX;
}


template <typename V>
inline bool HEMesh<V>::is_border_face(const face_handle face_id) const {
  if (!is_in_range_face(face_id))
    throw std::out_of_range("Out of range face or border index");

  return face_id >= _faces_cap;
}

template<typename V>
inline bool HEMesh<V>::is_border_hedge(const hedge_handle hedge_id) const {
	check_hedge_id(hedge_id);
  return _hedges[hedge_id].face_id >= _faces_cap;
}

template<typename V>
inline bool HEMesh<V>::is_border_edge(const hedge_handle hedge_id) const {
	//check_hedge_id(hedge_id);
	return is_border_hedge(hedge_id) || is_border_hedge(twin(hedge_id));
}

template<typename V>
inline bool HEMesh<V>::is_border_vert(const vert_handle vert_id) const {
	check_vert_id(vert_id);
	hedge_handle begin_id = _vert_to_hedge[vert_id];
	hedge_handle e = begin_id;

	do {

		//if (is_border_edge(e))
		if (is_border_hedge(e))
			return true;

		e = next(twin(e));
	} while (e != begin_id);

	return false;
}


template<typename V>
inline bool HEMesh<V>::is_isolated_vert(const vert_handle vert_id) const {
	check_vert_id(vert_id);
	return _vert_to_hedge[vert_id] == ISOLATED_INDEX;
}

template<typename V>
inline bool HEMesh<V>::is_isolated_edge(const hedge_handle hedge_id) const {
	check_hedge_id(hedge_id);
	hedge_handle t = twin(hedge_id);
	return next(hedge_id) == t && next(t) == hedge_id;
}

template<typename V>
inline bool HEMesh<V>::is_isolated_face(const hedge_handle hedge_id) const {
	check_hedge_id(hedge_id);

	face_handle f = face(hedge_id);
	
  hedge_handle t = twin(hedge_id);
	hedge_handle e = t;

	for (hedge_handle e = next(t); e != t; e = next(e))
		if (face(twin(e)) != f)
			return false;

	return true;
}


// -------------------------------------------------------------


template<typename V>
inline bool HEMesh<V>::is_face_size(const hedge_handle hedge_id, const uint n) const {
	check_hedge_id(hedge_id); 
  hedge_handle e = hedge_id;

	for (uint i = 0; i < n; ++i)
		e = next(e);

	return e == hedge_id;
}


template <typename V>
inline bool HEMesh<V>::is_face_size(const face_handle face_id, const uint n) const {
  return is_face_size(_face_to_hedge[face_id], n);
}


template<typename V>
inline uint HEMesh<V>::face_size(const hedge_handle hedge_id) const {
	check_hedge_id(hedge_id);
  hedge_handle e = hedge_id;
	uint n = 0;

	do {
		++n;
		e = next(e);
	} while (e != hedge_id);

	return n;
}


template <typename V>
inline uint HEMesh<V>::face_size(const face_handle face_id) const {
  return face_size(_face_to_hedge[face_id]);
}


template<typename V>
inline uint HEMesh<V>::valence(const vert_handle vert_id) const {
	check_vert_id(vert_id);

	const hedge_handle& beg = _vert_to_hedge[vert_id];
	if (beg == ISOLATED_INDEX)
		return 0;

	hedge_handle e = beg;
	uint n = 0;

	do {
		++n;
		e = next(twin(e));
	} while (e != beg);

	return n;
}

template<typename V>
inline bool HEMesh<V>::is_extremal(const vert_handle vert_id) const {
	check_vert_id(vert_id);
	const hedge_handle& he = _vert_to_hedge[vert_id];

	if (he == ISOLATED_INDEX)
		return false;

	return next(twin(he)) == he;
}

// ----------------------------------------------------------------------------


template<typename V>
inline typename HEMesh<V>::face_handle
  HEMesh<V>::make_face(const face_handle border_id) {
	
  check_face_id(border_id);
  if (!is_border_face(border_id))
    return border_id;
 
  face_handle fh;		// new half-edge index
  auto front_iter = _removed_faces.begin();
    
  //if (_removed_faces.empty()) {
  if (front_iter == _removed_faces.end()) {
    fh = (face_handle)_faces_cap;
    hedge_handle hedge_id = _face_to_hedge[border_id];

    // Move the first border at the place of the removed border
    _face_to_hedge[border_id] = std::move(_face_to_hedge[_faces_cap]);
    set_face_id(hedge(border_id), border_id);

    // Then at the place of the previously first border, 
    //  place the new face (at the back of the faces)
    _face_to_hedge[_faces_cap] = hedge_id;
    
    ++_faces_cap;
  }
  else {
    fh = *front_iter;
    _face_to_hedge[fh] = std::move(_face_to_hedge[border_id]);
    _removed_faces.erase(front_iter);
    
    free_border(border_id);
  }
 
  set_face_id(hedge(fh), fh); 
  return fh;
}


template<typename V>
inline typename HEMesh<V>::face_handle
  HEMesh<V>::make_border(const face_handle face_id) {
	
  check_face_id(face_id);
  if (is_border_face(face_id))
    return face_id;
 
  hedge_handle hedge_id(hedge(face_id));
  face_handle bh(new_border(hedge_id));

  set_face_id(hedge_id, bh);
  invalidate_face(face_id);
  
  return bh;
}



template<typename V>
inline void HEMesh<V>::remove_dub_edges_around_vert(const vert_handle vert_id) {
	hedge_handle beg = _vert_to_hedge[vert_id];
	if (beg == ISOLATED_INDEX)
		return;

	hedge_handle e = beg;
  hedge_handle ne;

	// if the beg hedge is degrade and is removed, then an infinite face happens
	do {		
		ne = next(twin(e));

		// Very important !!!, it prevents infinite face, if it happens that at
		//  the start of the iteration, the first edge (beg) is removed
		if (is_removed_hedge(beg))
			beg = e;

		//if (is_face_size(e, 2) || head(e) == vert_id) {
		// if it is a degrade face and has 2 edges or if it is a degrade edge
		if (next(next(e)) == e && twin(next(e)) != e || head(e) == vert_id) {
			make_face(face(e));
			remove_edge(edge_handle(e));
		}

		e = ne;

	} while (e != beg);

	
}


// Should we detect the dublicate edges by hedge connectivity or by head vertices ?
template<typename V>
inline void HEMesh<V>::remove_dub_edges_around_face(const hedge_handle hedge_id) {

	hedge_handle e = hedge_id;
	face_handle f = face(e); 
	
  do {
		// Outside the face adjacent edge
		hedge_handle tn = next(twin(e)); // twin next of the half-edge

		// Dont do anything if the edge of tn was part of the face
		if (next(twin(tn)) != e) { // is the tail vertex of e with valence 2

			// Here we detect for an edge that is not part of the face,
			// but is adjacent to the face from both ends
			if (face(twin(next(tn))) == f)
				remove_edge(tn);
		
		}

		e = next(e);
	} while (e != hedge_id);

}

// =============================================================================== //



template<typename V>
inline void HEMesh<V>::swap_next(const hedge_handle hedge_id1, const hedge_handle hedge_id2) {

	if (head(hedge_id1) != head(hedge_id2))
		throw std::invalid_argument("The two half-edges need to point to the same head vertex to swap their next half-edges");

	// Also if this error occurs (because of the above error is passed)
	//  it means that the edge of the twin half-edges is degrade
	//  (it is an edge that point to the same vertex on both ends)
	if (hedge_id1 == twin(hedge_id2))
		throw std::invalid_argument("Cannot use swap_next on twin half-edges, it will result in half-edges that point to themselves, and this will make infinite loops");

	if (hedge_id1 == hedge_id2)
		return;

	bool bSameLoop = face(hedge_id1) == face(hedge_id2);

	if (!bSameLoop) {	// join the two loops
		// Erase the old face of hedge2 and replace it with the face of hedge1
		face_handle fa = face(hedge_id1);
		face_handle fb = face(hedge_id2);

		// makes such that if fa or fb is a border, it joins the two faces into a border
		if (!is_border_face(fa))
			std::swap(fa, fb);

    // if fa and fb were swapped, then heA and heB should have also been swapped !
    set_face_id(hedge(fb), fa);
//    _face_to_hedge[face(fa)] = fb;

    invalidate_face(fb);
	}

	std::swap(_hedges[hedge_id1].next_id, _hedges[hedge_id2].next_id);

	if (bSameLoop) {	// split it into two loops
		face_handle f = face(hedge_id1);	// current loop begin
/*		face_handle nf = are_same_face(hedge_id1, hedge(f)) ? face(hedge_id2) : face(hedge_id1);	// new loop begin

		if (!is_border_face(f)) make_face(nf);
		else make_border(nf);

 		set_face_id(nf);*/

    hedge_handle new_begin_id = are_same_face(hedge_id1, hedge(f)) ? hedge_id2 : hedge_id1;
    face_handle nf;

    if (!is_border_face(f))
      nf = new_face(new_begin_id);
    else
      nf = new_border(new_begin_id);

    set_face_id(new_begin_id, nf);
  }

}


template<typename V>
//inline typename HEMesh<V>::face_handle HEMesh<V>::split_face_at(const hedge_handle back_hedge_id, const hedge_handle front_hedge_id) {
inline typename HEMesh<V>::hedge_handle HEMesh<V>::split_face_at(const hedge_handle back_hedge_id, const hedge_handle front_hedge_id) {
  check_hedge_id(back_hedge_id);
	check_hedge_id(front_hedge_id);

  if (back_hedge_id == front_hedge_id)
    throw std::invalid_argument("Cannot use split_face_at with two equal half-edges");

  // We first create the new face, and after that work with the previous face of the hedges, so the indexes are updated, if it happens that it is a border
 	hedge_handle neA = new_edge(); // new half-edge
	hedge_handle neB = twin(neA);  // new twin half-edge 
  
  // this does not set the new face index in back_hedge_id
  face_handle nf = new_face(back_hedge_id);
	face_handle f = face(back_hedge_id);

  if (f != face(front_hedge_id))
    throw std::invalid_argument("The two half-edges should be in the same face or border loop");


  _hedges[neA] = _hedges[front_hedge_id];
	_hedges[neB] = _hedges[back_hedge_id];

	_hedges[back_hedge_id].next_id = neA;
	_hedges[front_hedge_id].next_id = neB;


  set_face_id(back_hedge_id, nf);

  if (are_same_face(back_hedge_id, hedge(f)))
    _face_to_hedge[f] = front_hedge_id;   

	//return nf;
  return neA;
}


template<typename V>
inline void HEMesh<V>::make_isolated(const vert_handle vert_id) {
//inline void HEMesh<V>::remove_edges(const vert_handle& vert_id) {
	check_vert_id(vert_id);

	// Remove all of its adjacent edges

	//while (!is_isolated(vert_id))
	//	remove(edge_handle(outgoing(vert_id)));

	for (hedge_handle he = outgoing(vert_id); he != ISOLATED_INDEX; he = outgoing(vert_id))
		remove_edge(he);
}


// --------------------------------------------------------------------------------------- //


template<typename V>
inline void HEMesh<V>::fill_holes() {
  _faces_cap = _face_to_hedge.size();
}


template<typename V>
inline typename HEMesh<V>::hedge_handle
    HEMesh<V>::remove_internal_edges(hedge_handle hedge_id) {
	check_hedge_id(hedge_id);

	// first we search for a half-edge that is not gonna be removed
	face_handle f = face(hedge_id);
	hedge_handle e = hedge_id;
	do {
		if (face(twin(e)) != f)
			break;

		e = next(e);
	} while (e != hedge_id);

	hedge_id = e;

	// Then we remove the internal edges. Also if the whole face is just a chain of
	// edges it will removed them all
	hedge_handle ne;
	hedge_handle t;
	
  do {
		// It may still happen that hedge_id is gonna be removed, so that why we need
		//  this termination
		if (is_removed_hedge(e)) // then the whole face is destroyed
			return hedge_handle();

		ne = next(e);
		t = twin(e);

		if (next(t) == e)
			remove_edge(e);

		e = ne;
	} while (e != hedge_id);

  return hedge_id;
}

template<typename V>
inline void HEMesh<V>::remove_internal_edges(const face_handle face_id) {
  remove_internal_edges(hedge(face_id));
}

template<typename V>
inline void HEMesh<V>::remove_internal_edges() {
  //for (const face_handle f : faces())
  //  if (!is_removed_face(f))
  //    remove_internal_edges(f);

  for (uint i = 0U; i < _faces_cap; ++i) {
    hedge_handle he = _face_to_hedge[i];
    if (he != hedge_handle::INVALID_INDEX)
      remove_internal_edges(he);
  }
}


template<typename V>
inline void HEMesh<V>::remove_degrade_faces() {
	//for (const face_handle f : faces())
	//	if (!is_removed_face(f) && is_face_size(f, 2U))
	//		remove_edge(hedge(f));

  for (const hedge_handle he : _face_to_hedge)
    if (he != hedge_handle::INVALID_INDEX && next(next(he)) == he)
      remove_edge(he);
}


template<typename V>
inline void HEMesh<V>::remove_isolated_verts() {
	for (const vert_handle vi : verts())
		if (_vert_to_hedge[vi] == ISOLATED_INDEX) {
			invalidate_vert(vi);
		}
}


// ------------------------------------------------------------------- //


template <typename V>
inline typename HEMesh<V>::hedge_handle
    HEMesh<V>::add_edge(const hedge_handle back_hedge_id, const V& new_head_vert) {
	
  check_hedge_id(back_hedge_id);
	vert_handle vi = add_vert(new_head_vert);

	hedge_handle ne = new_edge(head(back_hedge_id), vi);	// new edge half-edge that points to head_id
	hedge_handle net = twin(ne);	// new twin half-edge
	set_face_id(ne, new_face(ne));

	_vert_to_hedge[vi] = net;

	swap_next(back_hedge_id, net);
	return ne;
}


template <typename V>
inline typename HEMesh<V>::hedge_handle
  HEMesh<V>::add_edge(const vert_handle tail_id, const V& new_head_vert) {
	
  check_vert_id(tail_id);

	vert_handle head_id = add_vert(new_head_vert);

	//int bFace = false;	// should it make the new edge a face or border
	hedge_handle heA = _vert_to_hedge[tail_id];

	hedge_handle ne = new_edge(tail_id, head_id);		// new half edge
	hedge_handle net = twin(ne);	// new half edge twin
  set_face_id(ne, new_border(ne));

	if (heA != ISOLATED_INDEX) {
		heA = twin(heA);	// make it point to the tail vertex

		// if its tail is an extremal vertex (adjacent only to one edge)
		//  , it doesn't need to search for a border edge
    if (twin(next(heA)) != heA) {
			face_handle b = find_border_around_head(heA);

			if (b == face_handle::INVALID_INDEX)
				throw std::invalid_argument("Edges can only be added around isolated, extremal or border vertices");
		}

    //if (!is_border_hedge(heA))
      make_face(face(ne));
		
    swap_next(heA, net);
	}

	_vert_to_hedge[tail_id] = ne;
	_vert_to_hedge[head_id] = net;

	return ne;
}


// @todo: Try to optimize it, or at least organize the implementation code better
template <typename V>
inline typename HEMesh<V>::hedge_handle HEMesh<V>::add_edge(const vert_handle tail_id, const vert_handle head_id) {
	check_vert_id(tail_id);
	check_vert_id(head_id);

	// Already added edge
	hedge_handle current_hedge = hedge(tail_id, head_id);
	if (current_hedge != hedge_handle(ISOLATED_INDEX))
		return edge_handle(current_hedge);

	hedge_handle heA = _vert_to_hedge[tail_id];
	hedge_handle heB = _vert_to_hedge[head_id];

	// First check if they share a face, and if so, just split the face with the new edge
	if (heA != ISOLATED_INDEX && heB != ISOLATED_INDEX) {
		hedge_handle eA = twin(heA);
		hedge_handle eB = twin(heB);

		face_handle f = find_common_face_around_head(eA, eB);
		if (f != face_handle::INVALID_INDEX) {

      //face_handle nf = split_face_at(eA, eB);
      //make_face(nf);
      
//      split_face_at(eA, eB);
      // make_face(face(eA));           
//			return next(eA);

      return split_face_at(eA, eB);
		}
	}

	hedge_handle ne = new_edge(tail_id, head_id);  // new half-edge
	hedge_handle net = twin(ne);			// new half edge twin

  set_face_id(ne, new_border(ne));
 
  bool isValidOper = true;

	if (heA != ISOLATED_INDEX) {
		heA = twin(heA);	// now it points towards the vertex

		if (heA == twin(next(heA)))	// its extremal vertex
			swap_next(heA, net);
		else {
			face_handle b = find_border_around_head(heA);		// its border vertex
			if (b != face_handle::INVALID_INDEX)
				swap_next(heA, net);
			else
				isValidOper = false;
		}

	}

	if (heB != ISOLATED_INDEX && isValidOper) {
		heB = twin(heB);	// now it points towards the vertex

		// try if not this and the other, and use joinHedges not within the conditionals for better handling

		if (heB == twin(next(heB))) {	// its extremal vertex
			swap_next(heB, ne);

		}
		else {
			face_handle b = find_border_around_head(heB);		// its border vertex
			if (b != face_handle::INVALID_INDEX) {
				swap_next(heB, ne);

			}
			else
				isValidOper = false;
		}
	}

  if (!isValidOper)
    throw std::invalid_argument("Invalid edge placement");

  _vert_to_hedge[tail_id] = ne;
	_vert_to_hedge[head_id] = net;
	
  return ne;	
}


template <typename V>
inline typename HEMesh<V>::hedge_handle
    HEMesh<V>::add_edge(const hedge_handle back_hedge_id, const hedge_handle front_hedge_id) {
	check_hedge_id(back_hedge_id);
	check_hedge_id(front_hedge_id);

	vert_handle vb = head(front_hedge_id);
	vert_handle va = head(back_hedge_id);

	hedge_handle ne = new_edge(va, vb);  // new half-edge
	hedge_handle net = twin(ne);			// new half edge twin

	bool bSameLoop = face(back_hedge_id) == face(front_hedge_id); 
  set_face_id(ne, new_border(ne));

	swap_next(back_hedge_id, net);
	swap_next(front_hedge_id, ne);

//	face_handle f = face(ne);
	if (bSameLoop) { // && is_border_face(f)
		make_face(face(ne));
	}

	_vert_to_hedge[va] = ne;
	_vert_to_hedge[vb] = net;

	return ne;
}



template <typename V>
inline typename HEMesh<V>::hedge_handle HEMesh<V>::add_edge(const hedge_handle back_hedge_id, const vert_handle head_id) {

	check_hedge_id(back_hedge_id);
	check_vert_id(head_id);

	vert_handle va = head(back_hedge_id);
	vert_handle vb = head_id;

	hedge_handle ne = new_edge(va, vb);  // new half-edge
	hedge_handle net = twin(ne);			// new half edge twin

  set_face_id(ne, new_border(ne));

  swap_next(back_hedge_id, net);
  
 
  hedge_handle front_hedge_id = outgoing(head_id);

  if (front_hedge_id != ISOLATED_INDEX) {
		front_hedge_id = twin(front_hedge_id);	// now it points towards the vertex

		// try if not this and the other, and use joinHedges not within the conditionals for better handling

		if (front_hedge_id == twin(next(front_hedge_id))) {	// its extremal vertex
			swap_next(front_hedge_id, ne);

		}
		else {
			face_handle b = find_border_around_head(front_hedge_id);		// its border vertex
			if (b != face_handle::INVALID_INDEX) {
				swap_next(front_hedge_id, ne);
			}
			else
				return hedge_handle();
		}
	}
  else
    _vert_to_hedge[vb] = net;    

	return ne;
}


// @todo: Try not to use add_edge and constrain it to not allow (inner-edges after the operation) or (edges/faces that cross the new face)
template <typename V>
inline typename HEMesh<V>::face_handle HEMesh<V>::add_face(const std::vector<vert_handle>& vert_indices) {
	if (vert_indices.size() < 2ULL)
		throw std::invalid_argument("The number of new face indices cannot be smaller than 2");

	bool isEdgeAdded = false;

	for (int i = 0, n = vert_indices.size() - 1ULL; i < n; ++i) {
		check_vert_id(vert_indices[i]);

		if (hedge(vert_indices[i], vert_indices[i + 1]) == ISOLATED_INDEX) {
			add_edge(vert_indices[i], vert_indices[i + 1]);
			isEdgeAdded = true;
		}
	}

	hedge_handle e = hedge(vert_indices.back(), vert_indices.front());
	if (e == ISOLATED_INDEX) {
		e = add_edge(vert_indices.back(), vert_indices.front());
		isEdgeAdded = true;
	}

	face_handle f = face(e);
	if (!isEdgeAdded) {
	  make_face(f);	
  }

	return f;
}


// @todo: This method here will brake if there is already added edge in the
//		  where a new edge will be added, so try to handle that
template <typename V>
inline typename HEMesh<V>::face_handle HEMesh<V>::add_face(const std::vector<hedge_handle>& hedge_indices) {

	split_face_at(hedge_indices.back(), hedge_indices.front());
	hedge_handle prev_hedge_id = next(hedge_indices.back());
	//	int last_hedge = twin(prev_hedge_id);

	//	for (int i = 1, n = hedge_indices.size(); i < n - 1; ++i) {
	for (int i = 1, n = hedge_indices.size(); i < n; ++i) {
		const hedge_handle& he = hedge_indices[i];
		split_face_at(prev_hedge_id, he);
		prev_hedge_id = next(prev_hedge_id);
	}

	//	split_face_at(prev_hedge_id, last_hedge);

	face_handle f = face(prev_hedge_id);
  if (is_border_face(f)) {
    make_face(f);
  }

	return f;
}



// ------------------------------------------------------------------- //



template<typename V>
inline void HEMesh<V>::remove_vert(const vert_handle vert_id) {
	make_isolated(vert_id);
	invalidate_vert(vert_id);
}


template<typename V>
inline void HEMesh<V>::remove_edge(const hedge_handle hedge_id) {

	//if (is_removed_edge(edge_id))
	//	return;

	check_hedge_id(hedge_id);

  hedge_handle c = hedge_id;
	hedge_handle t = twin(c);
	hedge_handle nc = next(c);
	hedge_handle nt = next(t);
	hedge_handle pc = prev(c);
	hedge_handle pt = prev(t);

	face_handle f = face(c);
	face_handle tf = face(t);  

  if (nc == t && nt == c) {  // Then it is an isolated edge
    _vert_to_hedge[head(c)] = ISOLATED_INDEX;
    _vert_to_hedge[head(t)] = ISOLATED_INDEX;

    invalidate_face(f);
    invalidate_edge(c);
   
    return;
  }

  // Shift the begin half-edges of the faces
  //  to not be a half-edge that is gonna be removed
  if (hedge(f) == c)
    _face_to_hedge[f] = nc;
  if (hedge(tf) == t) {
    if (nt != c)
      _face_to_hedge[tf] = nt;
    else
      _face_to_hedge[tf] = pt;
  }

  bool bSameFace = f == tf; 

  if (!bSameFace) {
    if (is_border_face(f)) {
      set_face_id(t, f);
      invalidate_face(tf);
    }
    else {
      set_face_id(c, tf);
      invalidate_face(f);
    }
  }
  
  if (nt != c) {
		_hedges[pc].next_id = nt;
		_vert_to_hedge[head(t)] = nt;
		//_vert_to_hedge[tail(nt)] == nt;
	}
  else {
		_vert_to_hedge[head(t)] = ISOLATED_INDEX;
	}

	if (nc != t) {
		_hedges[pt].next_id = nc;
		_vert_to_hedge[head(c)] = nc;
	}
	else {
		_vert_to_hedge[head(c)] = ISOLATED_INDEX;
	}


  if (bSameFace && (nc != t && nt != c)) {
    hedge_handle begin_id = are_same_face(nc, hedge(f)) ? nt : nc;
    face_handle nf;
  
    if (is_border_face(f))
      nf = new_border(begin_id);
    else
      nf = new_face(begin_id);

    set_face_id(begin_id, nf);
  }

	invalidate_edge(c);
}


// @todo: Try to make this method to conserve begin half-edge indexes as much as possible
/*template<typename V>
inline void HEMesh<V>::remove_edge(const hedge_handle hedge_id) {

	//if (is_removed_edge(edge_id))
	//	return;

  const hedge_handle& c = hedge_id;
	check_hedge_id(c);
	
//	int c = hedge_id;
	hedge_handle t = twin(c);
	hedge_handle nc = next(c);
	hedge_handle nt = next(t);
	hedge_handle pc = prev(c);
	hedge_handle pt = prev(t);

	face_handle f = face(c);
	face_handle tf = face(t);  

  
  if (nt != c) {
		_hedges[pc].next_id = nt;
		_vert_to_hedge[head(t)] = nt;
		//_vert_to_hedge[tail(nt)] == nt;
	}
  else {
		_vert_to_hedge[head(t)] = ISOLATED_INDEX;
	}

	if (nc != t) {
		_hedges[pt].next_id = nc;
		_vert_to_hedge[head(c)] = nc;
	}
	else {
		_vert_to_hedge[head(c)] = ISOLATED_INDEX;
	}

	// Faces and borders arrangement

	bool bFace = !is_border_face(f) && !is_border_face(tf);

  invalidate_face(f);
 
  tf = face(t);
  
  if (tf != f)
    invalidate_face(tf); 


  face_handle nf;

	if (nt != c) {
    if (bFace)
      nf = new_face(nt);
    else
      nf = new_border(nt);
    
    set_face_id(nt, nf);	
  }

	if (nc != t) {
		if (nt == c || (nt != c && f == tf)) {
      if (bFace)
        nf = new_face(nc);
      else
        nf = new_border(nc);

      set_face_id(nc, nf);
    }
	}


  invalidate_edge(c);
}*/


template<typename V>
inline bool HEMesh<V>::remove_edge(const vert_handle vertA, const vert_handle vertB) {

	hedge_handle hedge_id = hedge(vertA, vertB);
	if (hedge_id == ISOLATED_INDEX)
		return false;

	remove_edge(hedge_id);
	return true;
}


template<typename V>
inline void HEMesh<V>::remove_face(const hedge_handle hedge_id) {
	check_hedge_id(hedge_id);

	hedge_handle he = hedge_id;

	std::vector<hedge_handle> edges_to_remove;

	do {
		if (is_border_hedge(twin(he)))
			edges_to_remove.push_back(he);

		he = next(he);
	} while (he != hedge_id);

	if (edges_to_remove.empty()) {
    make_border(face(hedge_id));
  }
	else
		for (const hedge_handle& e : edges_to_remove)
			remove_edge(e);

}


template<typename V>
inline void HEMesh<V>::remove_face(const face_handle face_id) {
  remove_face(_face_to_hedge[face_id]);  
}


template<typename V>
inline void HEMesh<V>::move_edge(const hedge_handle hedge_id, const hedge_handle front_hedge_id) {
	check_hedge_id(hedge_id);
	check_hedge_id(front_hedge_id);

	if (hedge_id == twin(next(front_hedge_id)))
		return; // then they are already connected

	hedge_handle t = twin(hedge_id);
	vert_handle vi = head(hedge_id);

	// Disconnect previous connections, if the front of the half-edge was connected with another edge
	if (next(hedge_id) != t) {
		_vert_to_hedge[vi] = next(hedge_id);
		swap_next(hedge_id, prev(t));
	}
	else
		_vert_to_hedge[vi] = ISOLATED_INDEX;

	_hedges[hedge_id].head_id = head(front_hedge_id); // Update the vertices
	swap_next(hedge_id, front_hedge_id); // Connect the hedge to the new front hedge
}


template<typename V>
inline void HEMesh<V>::move_edge(const hedge_handle hedge_id, const hedge_handle back_hedge_id, const hedge_handle front_hedge_id) {
	move_edge(hedge_id, front_hedge_id);
	move_edge(twin(hedge_id), back_hedge_id);
}


// ======================================================================================= //


template<typename V>
inline void HEMesh<V>::flip_edge(const edge_handle edge_id) {

	hedge_handle c = hedge(edge_id);		// current half-edge
	check_hedge_id(c);

	hedge_handle t = twin(c);  // twin half-edge

	hedge_handle nc = next(c); // next half-edge
	hedge_handle nt = next(t);		// next of twin half-edge

	hedge_handle pc = prev(c); // previous half-edge
	hedge_handle pt = prev(t);		// previous of twin half-edge

  if (nc == twin(pt) || nt == twin(pc))
    throw std::invalid_argument("Cant flip an edge, that is going to result in degrade edge (an edge which head and tail vertices are gonna be the same)");

  face_handle f = face(c);
  face_handle tf = face(t);

	// Update face or border loops
  if (nc == hedge(f))
    _face_to_hedge[f] = pc;     // next(nc), c, pc
//		shift_begin(nc);
	
  if (nt == hedge(tf))
    _face_to_hedge[tf] = pt;    // next(nt), t, pt
//		shift_begin(nt);

	// Update the vert to hedges
  _vert_to_hedge[head(c)] = nc;
  _vert_to_hedge[head(t)] = nt;

	_hedges[pc].next_id = nt;
	_hedges[pt].next_id = nc;

	_hedges[c] = _hedges[nc];
	_hedges[t] = _hedges[nt];

	_hedges[nc].next_id = t;
	_hedges[nt].next_id = c;

  std::swap(_hedges[nc].face_id, _hedges[nt].face_id);
 
}


template<typename V>
inline typename HEMesh<V>::vert_handle HEMesh<V>::refine_edge(const hedge_handle hedge_id, const V& v) {
	check_hedge_id(hedge_id);
	
	hedge_handle t = twin(hedge_id);

	hedge_handle ne = new_edge();
	hedge_handle net = twin(ne);

	vert_handle vi = add_vert(v);
	_vert_to_hedge[vi] = ne;
	_vert_to_hedge[head(hedge_id)] = net;

	_hedges[prev(t)].next_id = net;
	_hedges[ne] = _hedges[hedge_id];

	_hedges[hedge_id] = HalfEdge(vi, ne, face(ne));
	_hedges[net] = HalfEdge(vi, t, face(t));

	return vi;
}

template<typename V>
inline typename HEMesh<V>::vert_handle HEMesh<V>::refine_edge(const hedge_handle hedge_id, const float h) {
	const V& va = _vertices[head(hedge_id)];	// head vertex
	const V& vb = _vertices[tail(hedge_id)]; // tail vertex

	return refine_edge(hedge_id, h * va + (1.0f - h) * vb);
}


template<typename V>
inline typename HEMesh<V>::hedge_handle HEMesh<V>::split_vert_to_edge(const hedge_handle hedgeA, const hedge_handle hedgeB, const V& v) {
	check_hedge_id(hedgeA);
	check_hedge_id(hedgeB);

	if (head(hedgeA) != head(hedgeB))
		throw std::invalid_argument("The two half-edges should point to the same head vertex, in order to split the vertex to an edge");

	hedge_handle ne = new_edge();	// new edge half-edge index
	hedge_handle net = twin(ne);	// new edge twin half-edge index

	vert_handle vi = add_vert(v);	// new vertex index
	_vert_to_hedge[vi] = net;
	_vert_to_hedge[head(hedgeA)] = ne;

	_hedges[ne] = _hedges[hedgeA];
//	_hedges[ne].head_id = vi;
	_hedges[hedgeA].next_id = ne;

	_hedges[net] = _hedges[hedgeB];
	_hedges[hedgeB].head_id = vi;
	_hedges[hedgeB].next_id = net;

	set_vert_id(ne, vi);	// !!!

	return ne;
}

template<typename V>
inline HEMesh<V>::HEMesh<V>::hedge_handle HEMesh<V>::split_vert_to_faces(const hedge_handle hedgeA, const hedge_handle hedgeB, const V& v) {	
	hedge_handle ne = split_vert_to_edge(hedgeA, hedgeB, v);
	hedge_handle net = twin(ne);

	split_face_at(ne, prev(hedgeA));
	split_face_at(net, prev(hedgeB));
  //split_face_at(prev(hedgeB), net);

//	make_face(ne);
//	make_face(net);

	return ne;
}

template<typename V>
inline typename HEMesh<V>::vert_handle HEMesh<V>::split_edge(const hedge_handle hedge_id, const V& v) {
//	hedge_handle hedge_id = hedge(edge_id);
	vert_handle vi = refine_edge(hedge_id, v);

	hedge_handle t = twin(hedge_id);
	hedge_handle ne = next(hedge_id); // new half-edge after the refinement

	if (!is_border_hedge(hedge_id))
		split_face_at(hedge_id, next(ne));

	if (!is_border_hedge(t))
		split_face_at(twin(ne), next(t));

	return vi;
}


template<typename V>
inline typename HEMesh<V>::vert_handle HEMesh<V>::split_edge(const hedge_handle hedge_id, const float h) {
	const V& va = _vertices[head(hedge_id)];	// head vertex
	const V& vb = _vertices[tail(hedge_id)]; // tail vertex

	return split_edge(hedge_id, h * va + (1.0f - h) * vb);
}



template<typename V>
inline typename HEMesh<V>::hedge_handle HEMesh<V>::clip_corner(const hedge_handle hedge_id) {
	check_hedge_id(hedge_id);

  hedge_handle nc = next(hedge_id);
  hedge_handle pc = prev(hedge_id);

	//if (face_size(hedge_id) <= 3)
//	if (is_face_size(hedge_id, 2U) || is_face_size(hedge_id, 3U))
  if (nc == pc || next(nc) == pc)
    //throw std::invalid_argument("Cannot clip the corner of a degrade or triangle face");
    return hedge_handle();

  //return split_face_at(next(hedge_id), prev(hedge_id));
  return split_face_at(nc, pc);
}


// @todo: It works most of the time on border edges, but at some point
//		  it makes the border a face
template <typename V>
inline typename HEMesh<V>::vert_handle HEMesh<V>::collapse_edge(const hedge_handle hedge_id, const V& v) {
	check_hedge_id(hedge_id);

  const hedge_handle& c = hedge_id;
	hedge_handle t = twin(c);

	const hedge_handle& nc = next(c);
	const hedge_handle& nt = next(t);

	vert_handle vt = head(t);	// the vertex that we collapse the edge into
	vert_handle vc = head(c);	// the deleted/removed vertex after the collapse
	_vertices[vt] = v;

	// Checking if it has adjacent faces (that are not the edge itself) and if so, are those faces degrade (2-sided), then it is a degrade face
	// if (!is_isolated_edge(c) && is_face_size(c, 2))
	if (nc != t && next(nc) == c)
		remove_edge(nc);
	if (nt != c && next(nt) == t)
		remove_edge(nt);

	// Remove the faces, if isolated or shift the face beging hedges, if it happens that it has adjacent edges
	// and one of the adjacent faces begin half edge is in the collapsed edge
	//if (is_isolated(edge(c))) {
	if (is_isolated_edge(hedge_id)) {
    invalidate_face(face(c));
  }
	else {
		if (hedge(face(t)) == t) {
			uint by = nt != c ? 1U : 2U;
			shift_begin(face(t), by);
		}

		if (hedge(face(c)) == c) {
			uint by = nc != t ? 1U : 2U;
			shift_begin(face(c), by);
		}
	}

	hedge_handle pc = prev(c);
	hedge_handle pt = prev(t);

	// Rearrange the adjacent of the collapsed edge half-edges and update their head vertex 
	if (nc != t) {
		set_vert_id(twin(nc), vt);
		_hedges[pt].next_id = nt != c ? nt : nc;
	}
	if (nt != c) {
		_hedges[pc].next_id = nc != t ? nc : nt;
		_vert_to_hedge[vt] = next(t);
	}
	else
		_vert_to_hedge[vt] = nc != t ? nc : ISOLATED_INDEX; // !!!

	// Remove dublicate edges
	remove_dub_edges_around_vert(vt);

	invalidate_edge(c);
	invalidate_vert(vc);

	return vt;
}


template<typename V>
inline typename HEMesh<V>::vert_handle HEMesh<V>::collapse_edge(const hedge_handle hedge_id, const float h) {
	const V& va = _vertices[head(hedge_id)];	// head vertex
	const V& vb = _vertices[tail(hedge_id)]; // tail vertex

	return collapse_edge(hedge_id, h * va + (1.0f - h) * vb);
}


template<typename V>
inline typename HEMesh<V>::vert_handle HEMesh<V>::collapse_face(hedge_handle hedge_id, const V& v) {
	check_hedge_id(hedge_id);

	// Preparing/Conditioning the face loop in a valid state for the collapsing
	
	hedge_id = remove_internal_edges(hedge_id);

	if (hedge_id == hedge_handle::INVALID_INDEX)
		throw std::invalid_argument("The face was a chain of edges that does not form a loop, so it cannot be collapsed");

	remove_dub_edges_around_face(hedge_id);

	vert_handle vi = head(hedge_id);	// we need to set its vert_to_hedge afterwards !
	_vertices[vi] = v;

	hedge_handle begin_id = hedge_id;
	hedge_handle e = next(begin_id);
	int n = 1;

	// During the iterative removal of the face edges, we cannot invalidate the vertices
	//  because it will mess up the edge removal algorithm, but after the edges are removed
	//  we cannot know their head vertex ids (if it happens that no adjacent edges are
	//  around the face to store those vert ids), so we need to collect them first and
	//  invalidate them after the complete removal of all edges that lie on the face
	std::vector<vert_handle> verts_to_remove;
	while (e != begin_id) {
		++n;
		verts_to_remove.push_back(head(e));
		e = next(e);
	}

	// Collect the adjacent hedges that are not on the face, but around it
	std::vector<hedge_handle> adj_hedges;
	adj_hedges.reserve(n);
	do {
		hedge_handle ne = next(e);
		hedge_handle t = twin(e);

		if (next(twin(ne)) != t)
			adj_hedges.push_back(prev(t));

		e = ne;
	} while (e != begin_id);


	// fill adj_hedges with valid adjacent half-edges (that are NOT on the face)
	// and remove any degrade adjacent faces
	for (int i = 0; i < n; ++i) {
		hedge_handle ne = next(e);		// next half-edge
		hedge_handle t = twin(e);		// twin half-edge
		hedge_handle tn = next(twin(e)); // twin next half-edge

		//if (is_face_size(t, 2) && !is_isolated(t))
		//if (is_face_size(t, 2) && twin(next(t)) != t)
		if (next(tn) == t && twin(tn) != t)
			remove_edge(tn);

		remove_edge(e);

		e = ne;
	}

	for (const vert_handle vi : verts_to_remove)
		invalidate_vert(vi);

	// Now make the remaining adjacent half-edges to point to the vertex
	for (const hedge_handle he : adj_hedges)
		set_vert_id(he, vi);

	if (!adj_hedges.empty()) {
		hedge_handle temp_hedge = adj_hedges.front();

		// Update the vertex to half-edge connectivity
		_vert_to_hedge[vi] = twin(temp_hedge);

		// Update the half-edges connectivity around the vertex
		for (auto it = ++adj_hedges.begin(); it != adj_hedges.end(); ++it)
			
			// if temp_hedge and *it are twins, swap_next will result in half-edges
			//  which next hedge is their own id, resulting in an infinite loop
	//		if (t != *it)
				swap_next(temp_hedge, *it);

	}

	remove_dub_edges_around_vert(vi);

	return vi;
}


template <typename V>
inline typename HEMesh<V>::vert_handle HEMesh<V>::collapse_face(const face_handle face_id, const V& v) {
  return collapse_face(hedge(face_id), v);
}


template<typename V>
inline typename HEMesh<V>::vert_handle HEMesh<V>::collapse_face(const hedge_handle hedge_id) {
	return collapse_face(hedge_id, face_center(hedge_id));
}


template <typename V>
inline typename HEMesh<V>::vert_handle HEMesh<V>::collapse_face(const face_handle face_id) {
  return collapse_face(hedge(face_id));
}


// @todo: Think about how vertices with only 1 or 2 adjacent edges, should be handled
// This version preserves the faces and borders
// The older version would have made all adjacent faces/borders into
// either a face or a border, but this one makes such that if an
// adjacent face was face/border it stays as a face/border after the beveling
template<typename V>
inline typename HEMesh<V>::face_handle HEMesh<V>::bevel_vert(const vert_handle vert_id, const float h) {
	check_vert_id(vert_id);

	if (is_isolated_vert(vert_id))
		throw std::invalid_argument("Cannot bevel an isolated vertex");

	// Collect the adjacent outgoing half-edges
	//  (the half-edges, which tail vertex is vert_id)
	hedge_handle beg = outgoing(vert_id);
	hedge_handle e = beg;
	std::vector<hedge_handle> adj_hedges;
	do {
		adj_hedges.push_back(e);
		e = next(twin(e));
	} while (e != beg);
	int n = adj_hedges.size();

	// Collect the new vertices and the one that will be reused
	std::vector<vert_handle> adj_verts;
	for (int i = 0; i < n - 1; ++i)
		adj_verts.push_back(add_vert(V()));
	adj_verts.push_back(vert_id);

	// Create the new edges, update their connectivity with adj_hedges
	//  , update vertices blended position and update the adj_hedges
	//  to store the new hedges that are for the new face
	for (int i = 0; i < n; ++i) {
		hedge_handle e = adj_hedges[i]; // points out of vert_id
		hedge_handle t = twin(e); // points towards vert_id

		// Update the new blended vertices
		const V& ov = _vertices[head(e)];
		const vert_handle& nvi = adj_verts[(i + 1) % n];
		const vert_handle& vi = adj_verts[i];
		_vertices[vi] = h * ov + (1.0f - h) * _vertices[vert_id];
		_vert_to_hedge[vi] = e;

		// Create a new edge that will be on the new face
		hedge_handle ne = new_edge();	// new half-edge
		hedge_handle net(ne + 1U);		// new twin half-edge
		_hedges[ne] = HalfEdge(nvi, next(t), face(t)); // the one that is out of the new face
		_hedges[net] = HalfEdge(vi, hedge_handle(), face_handle()); // the one that is in the new face

		// Update the adjacent hedge to now point to the new vertex and new half-edge
		_hedges[t].head_id = vi;
		_hedges[t].next_id = ne;

		adj_hedges[i] = net;	// reuse the adj_edges vector to store the new hedges

	}

	const hedge_handle& face_begin_hedge = adj_hedges.front();
  face_handle fh = new_face(face_begin_hedge);

	// Now that adj_edges stores the new hedges, we can update them, so they all connect
	//  and form the loop of the new face
	for (int i = 0; i < n; ++i) {

		// reverse the order that we set the next_ids, because the order at which
		//  adjacent hedges are iterated around the vertex
		//  is opposite to the orientation of the new face
		const hedge_handle& e = adj_hedges[(i + 1) % n];
		const hedge_handle& ne = adj_hedges[i];

		_hedges[e].face_id = fh;
		_hedges[e].next_id = ne;
	}

	return fh;
}

template<typename V>
inline typename HEMesh<V>::face_handle HEMesh<V>::bevel_edge(const hedge_handle hedge_id, const float h) {
	check_hedge_id(hedge_id);
  const hedge_handle& c = hedge_id;

	if (is_isolated_edge(hedge_id))
		throw std::invalid_argument("Cannot bevel an isolated edge");
	
	hedge_handle t = twin(c);

	std::vector<hedge_handle> front_hedges;
	std::vector<hedge_handle> back_hedges;

	for (hedge_handle e = next(c); e != t; e = next(twin(e)))
		front_hedges.push_back(e);
	for (hedge_handle e = next(t); e != c; e = next(twin(e)))
		back_hedges.push_back(e);

	split_face_at(c, prev(c));
	_vertices.reserve(_vertices.size() + front_hedges.size() + back_hedges.size());

	hedge_handle ne = c;
	t = next(c);
	
	for (auto it = front_hedges.begin(); it != front_hedges.end(); ++it) {
		const hedge_handle& he = *it;
		V cv = _vertices[head(ne)];
		const V& ov = _vertices[head(he)];

		_vertices[head(ne)] = h * ov + (1.0f - h) * cv;

		if (std::next(it) != front_hedges.end())
			split_vert_to_edge(twin(he), ne, cv);
	}
	
	ne = t;

	for (auto it = back_hedges.begin(); it != back_hedges.end(); ++it) {
		hedge_handle he = *it;
		const V& cv = _vertices[head(ne)];
		const V& ov = _vertices[head(he)];

		_vertices[head(ne)] = h * ov + (1.0f - h) * cv;

		if (std::next(it) != back_hedges.end())
			split_vert_to_edge(twin(he), ne, cv);
	}

	return face(c);
}


template<typename V>
inline typename HEMesh<V>::face_handle HEMesh<V>::bevel_face(const hedge_handle hedge_id, const float h) {
	check_hedge_id(hedge_id);

	// Calculate mid-point/center
	V center = face_center(hedge_id);
	hedge_handle he = hedge_id;

	// Create the new edges that connect the old face and the new inner face
	std::vector<hedge_handle> new_hedges;
	do {
		hedge_handle ne = next(he);

		const V& v = _vertices[head(he)];
		new_hedges.push_back(add_edge(he, h * center + (1.0f - h) * v));

		he = ne;
	} while (he != hedge_id);
	
	return add_face(new_hedges);
}


template<typename V>
inline HEMesh<V>::face_handle HEMesh<V>::cut_edge(const hedge_handle hedge_id) {
	check_hedge_id(hedge_id);

	if (is_border_edge(hedge_id))
		//throw std::invalid_argument("Cannot cut around a border (where it has already been cut)");
		return face_handle();

	split_face_at(hedge_id, prev(hedge_id));
	face_handle f = face(hedge_id);
  make_border(f);
	hedge_handle hedge_ids[2] = { hedge_id, next(hedge_id) };

	for (int i = 0; i < 2; ++i) {
		hedge_handle he = hedge_ids[i];

		hedge_handle bh = twin(next(he)); // border half-edge index  // one step ahead, so find_border_around_head doesnt always stop at he
		face_handle b = find_border_around_head(bh); // border begin half-edge index

		// it will always be a valid index because he is a border half-edge
		if (bh != he) { // && b != HE_INVALID_INDEX 
			vert_handle vi = head(he);
			_vert_to_hedge[vi] = twin(he);

			// split the vertex when two borders are joined
			//vert_handle nvi = _vertices.size();
			//_vertices.push_back(_vertices[vi]);
			//_vert_to_hedge.push_back(twin(bh));
			
			vert_handle nvi = add_vert(_vertices[vi]);
			_vert_to_hedge[nvi] = twin(bh);


			swap_next(bh, he); // join the borders
			set_vert_id(bh, nvi);
		}
	}

	//return face(hedge_id);
	return face(hedge_ids[1]);	// better return the other border, if it happens that the other hedge is now separated from hedge_id
}



// ------------------------------------------------------------------- //


template <typename V>
inline V HEMesh<V>::face_center(const hedge_handle hedge_id) {
  check_hedge_id(hedge_id);

  V v{};
  int n = 0;

  hedge_handle e = hedge_id;

  do {
    v += _vertices[head(e)];

    ++n;
    e = next(e);
  } while(e != hedge_id);

  return v /= n; 
}


//template <typename V>
//inline void HEMesh<V>::shift_borders(const int how_much) {}   

template <typename V>
inline void HEMesh<V>::shift_borders_forward() {
  hedge_handle hedge_id = _face_to_hedge[_faces_cap];
  set_face_id(hedge_id, (face_handle)_face_to_hedge.size());
  _face_to_hedge.push_back(hedge_id);
  ++_faces_cap;
}

template <typename V>
inline void HEMesh<V>::shift_borders_backward() {
  --_faces_cap; 
  _face_to_hedge[_faces_cap] = std::move(_face_to_hedge.back());
  face_handle first_border(_faces_cap);
  set_face_id(hedge(first_border), first_border);
  _face_to_hedge.pop_back(); 
}


template<typename V>
inline typename HEMesh<V>::vert_handle HEMesh<V>::add_vert(const V& v) {
  vert_handle vh;
  auto front_iter = _removed_verts.begin();

  //if (_removed_verts.empty())
  if (front_iter == _removed_verts.end()) {
    vh = (vert_handle)_vertices.size();
    _vertices.push_back(v);
    _vert_to_hedge.push_back(ISOLATED_INDEX);
  }
  else {
    vh = *front_iter;
    _vertices[vh] = v;
    _vert_to_hedge[vh] = ISOLATED_INDEX;
    _removed_verts.erase(front_iter);
  }
  
  return vh;
}

template<typename V>
inline typename HEMesh<V>::hedge_handle HEMesh<V>::new_edge() {
	hedge_handle ne;		// new half-edge index
  auto front_iter = _removed_edges.begin();

	//if (_removed_edges.empty()) {
	if (front_iter == _removed_edges.end()) {
    ne = (hedge_handle)hedges_size();
		_hedges.push_back(HalfEdge());
		_hedges.push_back(HalfEdge());
	}
	else {
		ne = *front_iter;

		//if (ne % 2 != 0 || _hedges[ne] != HalfEdge() || _hedges[ne + 1] != HalfEdge())
		//	throw std::exception("There is an edge that is garbaged, but not invalidated");

		_removed_edges.erase(front_iter);
	}

	return ne;
}

template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::new_edge(const vert_handle tail_id, const vert_handle head_id) {
	hedge_handle ne(new_edge());		// new half-edge index
	hedge_handle net(ne + 1U);	// new twin half-edge index

  // Should this method create a new face ??

  _hedges[ne] = HalfEdge(head_id, net, face_handle());
  _hedges[net] = HalfEdge(tail_id, ne, face_handle());	

	return ne;
}


template <typename V>
inline typename HEMesh<V>::face_handle HEMesh<V>::new_face(
  const hedge_handle begin_id) {
 
  face_handle fh;		// new half-edge index
  auto front_iter = _removed_faces.begin();
    
  //if (_removed_faces.empty()) {
  if (front_iter == _removed_faces.end()) {
    fh = face_handle(_faces_cap);
    shift_borders_forward();

    // The new face takes the place of the previously first border

    //fh = face_handle(_faces_cap - 1U);
    _face_to_hedge[fh] = begin_id;
  }
  else {
    fh = *front_iter;
    _face_to_hedge[fh] = begin_id;   
    _removed_faces.erase(front_iter);
  }
 
  return fh; 
}


template <typename V>
inline typename HEMesh<V>::face_handle HEMesh<V>::new_border(
    const hedge_handle begin_id) {
 
  face_handle fh(_face_to_hedge.size());
  _face_to_hedge.push_back(begin_id);  

  return fh; 
}


template<typename V>
inline void HEMesh<V>::invalidate_vert(const vert_handle vert_id) {
	_vert_to_hedge[vert_id] = hedge_handle(hedge_handle::INVALID_INDEX);
	_removed_verts.insert(vert_id);
}

template<typename V>
inline void HEMesh<V>::invalidate_edge(const edge_handle edge_id) {
	hedge_handle he(edge_id);

  _hedges[he] = HalfEdge();
	_hedges[he + 1] = HalfEdge();
  _removed_edges.insert(edge_id);
}

template<typename V>
inline void HEMesh<V>::invalidate_face(const face_handle face_id) {
  assert(is_in_range_face(face_id) && "Face index out of range, to be invalidated");
 
  if (!is_border_face(face_id)) {
    _face_to_hedge[face_id] = hedge_handle();
    _removed_faces.insert(face_id);
  }
  else {
    free_border(face_id);
  }
}


// The invalidation of a border, directly shrinks the borders part of the _face_to_hedge vector
/*template <typename V>
inline void HEMesh<V>::invalidate_border(const face_handle border_id) {
  assert(is_border_face(border_id) && "You try to invalidate a border, given a face index");


  free_border(border_id); 
}*/


template<typename V>
inline void HEMesh<V>::free_vert(const vert_handle vert_id) {
	assert(is_in_range_vert(vert_id) && "Cannot free space of a vertex which index is out of range !");
	assert(is_removed_vert(vert_id) && "The vertex should be removed, to free its memory space");

	vert_handle ov = vert_handle((uint)_vertices.size() - 1U);
	if (vert_id != ov) {
		_vertices[vert_id] = _vertices[ov];

		hedge_handle& out_hedge = _vert_to_hedge[vert_id];
		out_hedge = _vert_to_hedge[ov];

		if (out_hedge != ISOLATED_INDEX)
			set_vert_id(twin(out_hedge), vert_id); // should i make set_vert_id to set _vert_to_hedge too ???

	  //_vert_to_hedge[vert_id] = _vert_to_hedge[ov];
	}

	_vertices.pop_back();
	_vert_to_hedge.pop_back();
}


template<typename V>
inline void HEMesh<V>::free_edge(const edge_handle edge_id) {
	hedge_handle hedge_id(edge_id);
  assert(is_in_range_hedge(hedge_id) && "Cannot free space of a half-edge that is out of range !");
  assert(is_removed_hedge(hedge_id) && "The edge should be removed, to free its memory space");

	//if (hedge_id >= (int)_hedges.size() - 2) {
	if (hedge_id + 2U >= hedges_size()) {
		_hedges.pop_back();
		_hedges.pop_back();
		return;
	}

	hedge_handle t = ltwin(hedge_id);
	hedge_handle c = twin(t);
	hedge_handle cm = hedge_handle(uint(_hedges.size()) - 1U);
	hedge_handle tm = hedge_handle(uint(_hedges.size()) - 2U);

	// Update the previous of the last hedges to point to the new location 
	hedge_handle pcm = prev(cm);
	hedge_handle ptm = prev(tm);
	_hedges[pcm].next_id = c;
	_hedges[ptm].next_id = t;

	// Replase the erased edge with the last edge	
  _hedges[c] = _hedges.back();
	_hedges.pop_back();
	_hedges[t] = _hedges.back();
	_hedges.pop_back();

  _vert_to_hedge[head(c)] = t;
	_vert_to_hedge[head(t)] = c;

	// Update the faces begin hedges, if it happens that
  //  the moved hedges are the beginning of a face loop
  face_handle fc = face(c);
	if (hedge(fc) == cm)
    _face_to_hedge[fc] = c;

  face_handle ft = face(t);
	if (hedge(ft) == tm)
    _face_to_hedge[ft] = t;
	
}


template <typename V>
inline void HEMesh<V>::free_face(const face_handle face_id) {
//	assert(is_in_range_face(face_id) && "Cannot free space of a border which index is out of range !");
//	assert(is_removed_face(face_id) && "The border should be removed, to free its memory space");
  assert(!is_border_face(face_id) && "Trying to free the space for a face, but a border index is given");

  face_handle of(_faces_cap - 1U);

  // Move the last face to the place of the free-d face
  if (face_id != of) {
    hedge_handle hedge_id(_face_to_hedge[of]);
    _face_to_hedge[face_id] = hedge_id;
    set_face_id(hedge_id, face_id);
  }

  shift_borders_backward();
}


template <typename V>
inline void HEMesh<V>::free_border(const face_handle border_id) {
  assert(is_border_face(border_id) && "Trying to free the space for a border, but a face index is given");
	assert(is_in_range_face(border_id) && "Cannot free space of a border which index is out of range !");
//	assert(is_removed_face(border_id) && "The border should be removed, to free its memory space");

  // other border, in this case it is the last border
  face_handle ob((uint)_face_to_hedge.size() - 1U);

  // Move the last border at the place of the free-d border
  if (border_id != ob) {
    hedge_handle hedge_id = _face_to_hedge[ob];
    _face_to_hedge[border_id] = hedge_id; 
    set_face_id(hedge_id, border_id);
  }

  _face_to_hedge.pop_back();
}



template <typename V>
inline bool HEMesh<V>::is_in_range_vert(const vert_handle vert_id) const {
  return vert_id >= 0 && vert_id < _vertices.size();
}

template <typename V>
inline bool HEMesh<V>::is_in_range_hedge(const hedge_handle hedge_id) const {
  return hedge_id >= 0 && hedge_id < _hedges.size();
}

//template<typename V>
//inline bool HEMesh<V>::is_in_range_edge(const edge_handle edge_id) const {
//	return 0 <= edge_id && edge_id < edges_size();
//}

template <typename V>
inline bool HEMesh<V>::is_in_range_face(const face_handle face_id) const {
  return face_id >= 0 && face_id < _face_to_hedge.size();
}

/*template <typename V>
inline bool HEMesh<V>::is_in_range_face(const face_handle face_id) const {
  return face_id >= 0 && face_id < _faces_cap;
}

template <typename V>
inline bool HEMesh<V>::is_in_range_border(const face_handle face_id) const {
  return face_id >= _faces_cap && face_id < _faces_and_borders.size();
}*/


template<typename V>
inline void HEMesh<V>::check_vert_id(const vert_handle vert_id) const {
	if (!is_in_range_vert(vert_id))
		throw std::out_of_range("Vertex index out of range");
	if (is_removed_vert(vert_id))
		throw std::invalid_argument("Removed vertex");
}

template<typename V>
inline void HEMesh<V>::check_hedge_id(const hedge_handle hedge_id) const {
	if (!is_in_range_hedge(hedge_id))
		throw std::out_of_range("Half edge index out of range");
	if (is_removed_hedge(hedge_id))
		throw std::invalid_argument("Removed half edge");
}

template<typename V>
inline void HEMesh<V>::check_face_id(const face_handle face_id) const {
	if (!is_in_range_face(face_id))
		throw std::out_of_range("Half edge index out of range");
	if (is_removed_face(face_id))
		throw std::invalid_argument("The face is removed");
}

/*template<typename V>
inline void HEMesh<V>::check_face_id(const face_handle face_id) const {
	if (!is_in_range_face(face_id))
		throw std::out_of_range("Half edge index out of range");
	if (is_removed_face(face_id))
		throw std::invalid_argument("Removed half edge");
}

template<typename V>
inline void HEMesh<V>::check_border_id(const face_handle border_id) const {
	if (!is_in_range_border(border_id))
		throw std::out_of_range("Half edge index out of range");
	if (is_removed_face(border_id))
		throw std::invalid_argument("Removed half edge");
}*/


template<typename V>
inline bool HEMesh<V>::are_same_face(const hedge_handle hedge_id1, const hedge_handle hedge_id2) const {
	check_hedge_id(hedge_id1);
	check_hedge_id(hedge_id2);
	hedge_handle e = hedge_id1;

	do {
		if (e == hedge_id2)
			return true;

		e = next(e);
	} while (e != hedge_id1);

	return false;
}


template<typename V>
inline void HEMesh<V>::shift_begin(const face_handle face_id, const uint how_much) {
	check_face_id(face_id);

  hedge_handle nc = _face_to_hedge[face_id];
  for (uint i = 0; i < how_much; ++i)
    nc = next(nc);

  _face_to_hedge[face_id] = nc;
}

template<typename V>
inline void HEMesh<V>::set_face_id(const face_handle face_id) {
  hedge_handle begin_id = hedge(face_id);
  hedge_handle e = begin_id;
  
  //while (_hedges[e].face_id != face_id) {
  do {
		_hedges[e].face_id = face_id;

		e = next(e);
	} while (e != begin_id);

  //_faces_and_borders[face_id].begin_id = hedge_id;
}

template<typename V>
inline void HEMesh<V>::set_face_id(const hedge_handle hedge_id, const face_handle face_id) {
  hedge_handle e = hedge_id;
  
  //while (_hedges[e].face_id != face_id) {
  do {
		_hedges[e].face_id = face_id;

		e = next(e);
	} while (e != hedge_id);

  //_faces_and_borders[face_id].begin_id = hedge_id;
}


template<typename V>
inline void HEMesh<V>::set_vert_id(const hedge_handle hedge_id, const vert_handle new_head_vert_id) {
	hedge_handle beg = hedge_id;	// beggining
	hedge_handle e = beg;

	do {
		_hedges[e].head_id = new_head_vert_id;
		e = twin(next(e));
	} while (e != beg);

	//for (uint e = hedge_id; head(e) != new_head_vert_id; e = twin(next(e)))
	//	_hedges[e].head_id = new_head_vert_id;

//	_vert_to_hedge[new_head_vert_id] = next(hedge_id);	// should this method also update the _vert_to_hedge vector ??
}

template<typename V>
inline HEMesh<V>::face_handle HEMesh<V>::find_common_face_around_head(HEMesh<V>::hedge_handle& he1, HEMesh<V>::hedge_handle& he2) {

  face_handle f;
	hedge_handle hedge1 = he1;
	hedge_handle hedge2 = he2;

//	face_handle b = face_handle::INVALID_INDEX;
  face_handle b;
  hedge_handle be1 = he1;
	hedge_handle be2 = he2;

	// brute force
	do {
		do {
			f = face(he1);

			if (f == face(he2)) {
				if (!is_border_face(f))
					return f;

				b = f;
				be1 = he1;
				be2 = he2;
			}

			he1 = twin(next(he1));	// rotates around the head vertex
		} while (he1 != hedge1);

		he2 = twin(next(he2));	// rotates around the head vertex
	} while (he2 != hedge2);

	he1 = be1;
	he2 = be2;

	return b;
}

template<typename V>
inline HEMesh<V>::face_handle HEMesh<V>::find_border_around_head(hedge_handle& hedge_id) {
	hedge_handle begin_id = hedge_id;

	do {
		face_handle b = face(hedge_id);
		if (is_border_face(b))
			return b;

		hedge_id = twin(next(hedge_id));
	} while (hedge_id != begin_id);

//	return face_handle::INVALID_INDEX;
  return face_handle();
}


template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::ltwin(const hedge_handle hedge_id) const {
	//return (hedge_id / 2) * 2;
	//return hedge_handle((hedge_id >> 1) << 1); // make the number even
	return hedge_handle(uint(hedge_id) & ~1U);
}

template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::rtwin(const hedge_handle hedge_id) const {
	//return hedge_handle(((hedge_id >> 1) << 1) + 1); // make the number odd
	return hedge_handle(uint(hedge_id) | 1U); // make the least significant bit equal 1
}



} // namespace he


} // namespace mesh

