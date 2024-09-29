#pragma once
#include "tri_mesh.hpp"
#include "ngon_mesh.hpp"


namespace mesh {


template <typename V, typename VertCloud>
inline TriMesh<V, VertCloud> toTriMesh(const Mesh<V, 4U, VertCloud>& quad_mesh) {

	// Smallest Diagonal Quad Triangulation ...



	return TriMesh<V, VertCloud>();

}


template <typename V, typename VertCloud>
inline TriMesh<V, VertCloud> toTriMesh(const NgonMesh<V, VertCloud>& mesh) {

	if (!mesh.is_triangulated())
		throw std::invalid_argument("")


	return TriMesh<V, VertCloud>();

}


}

