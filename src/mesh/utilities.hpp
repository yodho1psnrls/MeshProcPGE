#pragma once

#include "base_mesh.hpp"
#include "tri_mesh.hpp"
#include "ngon_mesh.hpp"
#include <type_traits>


namespace mesh {


//template <typename MeshType, typename = typename std::enable_if<std::is_base_of<BaseMesh, MeshType>::value>::type>
//template <typename V, typename PointCloud>
//template <typename MeshType, typename V, typename PointCloud, typename = typename std::enable_if_t<std::is_base_of_v<BaseMesh<V, PointCloud>, MeshType>>>



}

