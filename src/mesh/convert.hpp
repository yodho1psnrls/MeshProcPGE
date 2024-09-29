#pragma once

#include "tri_mesh.hpp"
//#include "base_mesh.hpp"
#include "ngon_mesh.hpp"

#include <type_traits>


// Convertions will be allowed only between meshes which obey
// the same topology constraints
// for example, a NgonMesh can be converted to a triangular mesh only
// if all its faces are triangles

namespace mesh {

/*
template <
  typename V,
  typename PointCloud,

  typename MeshType,
  typename std::enable_if<std::is_base_of_v<BaseMesh<V, PointCloud>, MeshType>>
>
TriMesh<V, PointCloud>& operator=(
  TriMesh<V, PointCloud>& tri_mesh, const MeshType& other_mesh) {
*/

/*
template <typename MeshType>
void triangulate(MeshType m) {

  using vert_handle = typename MeshType::vert_handle;
  using face_handle = typename MeshType::face_handle;

  for(face_handle fh : other_mesh.faces()) {
    auto face_verts = m.vert_handle

    face_handle begin_vh = 

    for(vert_handle vh : other_mesh.verts(fh)) {
      tri_mesh.add_face
    }
  }
}
*/


/*template <typename V, typename PointCloud>
TriMesh<V, PointCloud> to_tri_mesh(const NgonMesh<V, PointCloud>& other_mesh) {
  TriMesh<V, PointCloud> mesh;
  mesh.add_face()

}
*/

/*
template <typename V, typename PointCloud>
TriMesh<V, PointCloud> to_tri_mesh(const Mesh<V, 4U, PointCloud>& quad_mesh) {

  using face_handle = typename TriMesh<V, PointCloud>::face_handle;
  using vert_handle = typename TriMesh<V, PointCloud>::vert_handle;
  
  TriMesh<V, PointCloud> tri_mesh;
  

  for (face_handle fh : quad_mesh) {
    auto indices = quad_mesh.verts(fh);

    for(int i = 0; i < 2; ++i) {
      const V& a = quad_mesh.vert(indices[i]);
      const V& b = quad_mesh.vert(indices[i + 1]);
      const V& c = quad_mesh.vert(indices[i + 2]);
      const V& d = quad_mesh.vert(indices[i + 3]);
    
      if ((a.pos - c.pos).mag2() < (b - d).mag2())
    }
    
    
  }

  
  return tri_mesh;
}

*/

}
