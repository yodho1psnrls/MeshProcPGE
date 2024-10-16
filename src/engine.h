#pragma once
// #include "half_edge.hpp"
#include "mesh/generate_plane.hpp"
#include "mesh/tri_mesh.hpp"
#include "mesh/ngon_mesh.hpp"
#include "point_cloud/point_vec.hpp"
#include "utilities.hpp"

#include "mesh/he/he_mesh.hpp"

#include "../olc/olcPixelGameEngine.h"
#include "vertex/vertex.h"
#include <iostream>

#include <triangulate.hpp>


const float PINDIST = 8.0f;   // the range that the mouse will select a vertex
const int INIT_HEDGE_ID = 0; // 15


class Example : public olc::PixelGameEngine {

  using V = Vertex;
  using Mesh = typename mesh::TriMesh<V, pc::PointVec<V>>;
  using HEMesh = typename mesh::he::HEMesh<V>;
  
  //using face_handle = typename HEMesh::face_handle;
  using vert_handle = typename HEMesh::vert_handle;
  using hedge_handle = typename HEMesh::hedge_handle;
  //using edge_handle = typename HEMesh::edge_handle;

  Mesh mesh;
  HEMesh he_mesh;

  vert_handle selected_vert_id;
  hedge_handle hedge_id;

  std::vector<std::pair<HEMesh, hedge_handle>> prev_history;
  std::vector<std::pair<HEMesh, hedge_handle>> next_history;

public:
  Example() { sAppName = "Example"; }

  template <typename MeshType>
  void drawPoints(const MeshType& m, const uint radius = 5, const olc::Pixel& color = olc::YELLOW) {
    for (auto vert_id : m.verts())
      FillCircle(m.vert(vert_id).pos, radius, color);
  }


  template <typename MeshType>
  void drawMesh(const MeshType& m, const olc::Pixel &color = olc::WHITE) {
    for (typename MeshType::face_handle face_id : mesh.faces()) {
//      auto indices = mesh.verts(face_id);

      std::array<typename MeshType::vert_handle, 3ULL> indices;
      auto iter = mesh.verts(face_id).begin();
      for(typename MeshType::vert_handle& vh : indices)
          vh = *(iter++);


      const V &a = mesh.vert(indices[0]);
      const V &b = mesh.vert(indices[1]);
      const V &c = mesh.vert(indices[2]);  


      // To better visualize the different triangles, we will color them based
      // on their uv.x coordinate
      float diffuse = (a.uv.x + b.uv.x + c.uv.x) / 3.0f;  // READ ACCESS VIOLATION , THREAD STOPPED
      // float diffuse = (b.pos - a.pos).norm().perp().dot((c.pos -
      // a.pos).norm());
      //float diffuse = 0.5f;

      FillTriangle(a.pos, b.pos, c.pos, color * diffuse);
    }  
  }


  void drawWireFrame(const HEMesh &hm, const olc::Pixel &color = olc::WHITE) {
    for (HEMesh::face_handle fh : hm.faces()) {
      for(auto e : hm.edges()) {
        auto he = hm.hedge(e);

        if (hm.is_removed_hedge(he))
          continue;

        const V& a = hm.vert(hm.head(he));
        const V& b = hm.vert(hm.tail(he));
      
        DrawLine(a.pos, b.pos, color);
      }
    }
  }


  void moveVertexWithCursor() {
    olc::vf2d mouse_pos = GetMousePos();

    if (GetMouse(0).bPressed)
      for (vert_handle vh : he_mesh.verts()) {
        
        if (he_mesh.is_removed_vert(vh))
          continue;

        if ((he_mesh.vert(vh).pos - mouse_pos).mag() < PINDIST) {
          selected_vert_id = vh;
          break;
        }

      }
      
    if (GetMouse(0).bReleased)
      //selected_vert_id = -1;
      selected_vert_id = vert_handle();

    if (selected_vert_id != -1)
      // mesh.vertices[selected_vert_id].pos = mouse_pos;
      he_mesh.vert(selected_vert_id).pos = mouse_pos;
  }


  void drawHalfArrow(const olc::vf2d &from, const olc::vf2d &to,
                     const olc::Pixel &color = olc::WHITE) {
    
    const bool CCW = true;

    float sep = 6.0f;
    olc::vf2d n = (to - from).norm();
    olc::vf2d side = CCW ? -n.perp() : n.perp();

    for (float i = 0.0f; i < 3.0f; i += 1.0f) {
      olc::vf2d dif = side * (0.25f * sep + i);
      
      DrawLine(from + dif, to + dif, color);
      DrawLine(to + dif, to - n * sep * 1.25f + side * sep * 0.75f + dif,
               color);
    }

    // DrawLine(to, to - n * sep + n.perp() * sep, color);
  }


  // void drawHalfEdge(const HalfEdgeMesh<vf2d>::HalfEdge* edge, const
  // olc::Pixel& color = olc::WHITE) {
  void drawHalfEdge(const HEMesh& hm, const hedge_handle e, const olc::Pixel &color = olc::WHITE) {
    //if (hm.is_removed_hedge(e))
    //if (e == hedge_handle::INVALID_INDEX || e == mesh::he::ISOLATED_INDEX)
    if (e < 0 || e >= hm.hedges_size() || hm.is_removed_hedge(e)) {
//      std::cout << "Invalid half-edge: " << e << "\n";
      return;
    }
      

    drawHalfArrow(hm.vert(hm.tail(e)).pos, hm.vert(hm.head(e)).pos, color);

    // Draw the head vertex index
    DrawStringDecal(hm.vert(hm.head(e)).pos, std::to_string(uint(hm.head(e))),
                    olc::GREEN);

    // Draw the half-edge index
    DrawStringDecal(0.5f * (hm.vert(hm.tail(e)).pos + hm.vert(hm.head(e)).pos) +
                        olc::vf2d(0.0f, 4.0f),
                    std::to_string(uint(e)), olc::MAGENTA);
  }


  void drawBorders(const HEMesh &hm, const olc::Pixel &color = olc::RED) {

    for (auto b : hm.borders()) {
      hedge_handle begin_he = hm.hedge(b);
      hedge_handle he = begin_he;

      do {
        // drawHalfEdge(e, color);

        const olc::vf2d &from = hm.vert(hm.tail(he)).pos;
        const olc::vf2d &to = hm.vert(hm.head(he)).pos;
        DrawLine(from, to, color);

        olc::vf2d n = -(to - from).norm().perp();
        DrawLine(from + n, to + n, color);

        he = hm.next(he);
      } while (he != begin_he);
    }
  }


  void update_hedge() {
    HEMesh& hm = he_mesh;
    
    if (hedge_id >= hm.hedges_size()) {
      hedge_id = hedge_handle(0U);
      
      while(hm.is_removed_hedge(hedge_id)) {
        hedge_id += 2U;
      }
      
    }
  }


  void keyInput() {
    
    Mesh& m = mesh;
    HEMesh& hm = he_mesh;


    if (GetKey(olc::I).bReleased) print_info(he_mesh);
//    if (GetKey(olc::G).bReleased) mesh = he_mesh;
    if (GetKey(olc::N).bReleased) hedge_id = hm.next(hedge_id);
    if (GetKey(olc::P).bReleased) hedge_id = hm.prev(hedge_id);
    if (GetKey(olc::T).bReleased) hedge_id = hm.twin(hedge_id);
    if (GetKey(olc::V).bReleased) he_mesh.check_validity();

    if (GetKey(olc::W).bReleased) std::cout << hedge_id << "\n";

    if (GetKey(olc::Z).bReleased && !prev_history.empty()) {
      next_history.push_back({he_mesh, hedge_id});

      auto pairr = prev_history.back();
      he_mesh = pairr.first;
      hedge_id = pairr.second;

      prev_history.pop_back();
    }

    if (GetKey(olc::X).bReleased && !next_history.empty()) {
      prev_history.push_back({he_mesh, hedge_id});

      auto pairr = next_history.back();
      he_mesh = pairr.first;
      hedge_id = pairr.second;

      next_history.pop_back();
    }

    // Resets to initial state
    if (GetKey(olc::C).bReleased) {
      prev_history.push_back({he_mesh, hedge_id});

      auto pairr = prev_history.front();
      he_mesh = pairr.first;
      hedge_id = pairr.second;

      next_history.insert(next_history.end(), prev_history.rbegin(), --prev_history.rend());
      prev_history.clear();
    }

    if (GetKey(olc::S).bReleased) {
      he_mesh.remove_isolated_verts();
      he_mesh.shrink();

      if (hedge_id >= he_mesh.hedges_size())
        hedge_id = he_mesh.hedges().begin();
    }

    if (GetKey(olc::E).bReleased) {
      uint given_hedge_id;
      std::cin >> given_hedge_id;
      hedge_id = (hedge_handle)given_hedge_id;
    }

    // Draws the begin half-edge of the loop and indicates is it a face(GREEN) or border(RED)
    if (GetKey(olc::SPACE).bHeld) {
      auto f = he_mesh.face(hedge_id);
      drawHalfEdge(he_mesh, he_mesh.hedge(f), olc::WHITE);

      if (he_mesh.is_border_face(f))
        FillRect({0, 0}, {15, 15}, olc::RED);
      else
        FillRect({0, 0}, {15, 15}, olc::GREEN);
    }

    //if(GetKey(olc::SPACE).bReleased)
    //  std::cout << "face: " << hm.face(hedge_id) << "\n";


    if (GetKey(olc::R).bReleased) { 
      
      next_history.clear();
      prev_history.push_back({he_mesh, hedge_id});

      auto temp = hedge_id;
      if (he_mesh.twin(he_mesh.next(hedge_id)) == hedge_id)
        temp = he_mesh.next(he_mesh.next(hedge_id));
      else
        temp = he_mesh.next(hedge_id);

      he_mesh.remove_edge(hedge_id);

      hedge_id = temp;
      he_mesh.check_validity();

    }


    if (GetKey(olc::F).bReleased) {
      next_history.clear();
      prev_history.push_back({he_mesh, hedge_id});

      HEMesh &hm = he_mesh;

/*      auto fh = he_mesh.face(hedge_id);
      if (he_mesh.is_border_face(fh))
        he_mesh.make_face(fh);
      else
        he_mesh.make_border(fh);*/

      //hm.flip_edge(hedge_id);
      //hm.refine_edge(hedge_id);

      //V v = hm.vert(hm.head(hedge_id));
      //v.pos += olc::vf2d(10.0f, -15.0f);
      //hm.split_vert_to_faces(hedge_id, hm.twin(hm.next(hedge_id)), v);
     
      //hm.split_edge(hedge_id);
      //hm.clip_corner(hedge_id);
      
      //hm.collapse_edge(hedge_id);
      //hm.collapse_face(hedge_id);
      
      //hm.bevel_vert(hm.head(hedge_id));
      
      //hm.bevel_edge(hedge_id);

      //hm.bevel_face(hedge_id);
      //hm.cut_edge(hedge_id);

      //hm.join_faces(hedge_id, hm.twin(hedge_id));
      
      hedge_id = hm.remove_internal_edges(hedge_id);



      he_mesh.check_validity();
    }
  

  }


  void print_info(const Mesh &m) const {
    std::cout << "================\n";
    std::cout << "vertices: " << m.verts_size() << "\n";
    std::cout << "triangles: " << m.faces_size() << "\n";
    std::cout << "\n";
  }


  void print_info(const HEMesh &hm) const {
    std::cout << "================\n";
    std::cout << "vertices: " << hm.verts_size() << "\n";
    std::cout << "edges: " << hm.edges_size() << "\n";
    std::cout << "faces: " << hm.faces_size() << "\n";
    std::cout << "borders: " << hm.borders_size() << "\n";
    std::cout << "\n";
  }

  
  void test_mesh0() {

    // Copy vertices
    for(auto vh : mesh.verts()) {
      he_mesh.add_vert(mesh.vert(vh));
    }
    
    // Copy faces
    for(auto fh : mesh.faces()) {
    
      auto face_verts = mesh.verts(fh);

      for (auto vit = face_verts.begin(); vit != face_verts.end(); ++vit) {
        auto nit = vit;
        ++nit;
        if (nit == face_verts.end())
          nit = face_verts.begin();

        he_mesh.add_edge(*vit, *nit);  
      }
    }

  }

  void test_mesh1() {

    he_mesh.add_vert(olc::vf2d(200.0f, 100.0f));  
    he_mesh.add_vert(olc::vf2d(100.0f, 100.0f));
    he_mesh.add_vert(olc::vf2d(200.0f, 200.0f));
    he_mesh.add_vert(olc::vf2d(100.0f, 200.0f));

    
    //he_mesh.add_edge(vert_handle(0U), vert_handle(1U));
    //he_mesh.add_edge(vert_handle(1U), vert_handle(2U));
//    he_mesh.add_edge(vert_handle(2U), vert_handle(0U));

    //he_mesh.add_edge(he_mesh.ingoing(vert_handle(2U)), he_mesh.ingoing(vert_handle(0U)));
    //he_mesh.add_edge(vert_handle(2U), he_mesh.vert(vert_handle(0U)));


//    he_mesh.split_face_at(hedge_handle(2), hedge_handle(1));

    he_mesh.add_edge(vert_handle(3U), vert_handle(2U));
//    he_mesh.add_edge(vert_handle(2U), vert_handle(1U));
    he_mesh.add_edge(vert_handle(1U), vert_handle(3U));

    he_mesh.add_edge(vert_handle(0), vert_handle(1));

    //he_mesh.add_edge(he_mesh.head(hedge_handle(0)), vert_handle(0));
    he_mesh.add_edge(hedge_handle(0), vert_handle(0));


    /*std::vector<vert_handle> face_verts = {
      vert_handle(0U),
      vert_handle(1U),
      vert_handle(3U),
      vert_handle(2U)
    };

    he_mesh.add_face(face_verts);*/


    for(auto vh : he_mesh.verts()) {
      olc::vf2d pos = he_mesh.vert(vh).pos;
      he_mesh.vert(vh).uv = pos / olc::vf2d(scrW, scrH);
    }

    
//    he_mesh.remove_edge(hedge_id);
//    hedge_id = hedge_handle(2U);


  }


public:
  bool OnUserCreate() override {

    mesh = mesh::generate_plane<V, pc::PointVec<V>>(3, 0.9f);
    he_mesh = mesh;
    //selected_vert_id = vert_handle::INVALID_INDEX;
    selected_vert_id = vert_handle();
    //hedge_id = he_mesh.hedge(INIT_HEDGE_ID);
    hedge_id = (hedge_handle)INIT_HEDGE_ID;

    //he_mesh.remove_edge(HEMesh::edge_handle(0U));
    //hedge_id = (hedge_handle)5U;

    //test_mesh1();

    he_mesh.check_validity();
    print_info(he_mesh);
    //print_info(mesh);

    //std::cout << he_mesh.face_size(he_mesh.hedge(he_mesh.borders().begin())) << "\n";
    //std::cout << he_mesh.borders().begin() << "\n";

    return true;
  }


  bool OnUserUpdate(float fElapsedTime) override {
    Clear(olc::BLACK);

    // mesh = he_mesh;

//    mesh::NgonMesh<V, pc::PointVec<V>> temp = he_mesh;
//    mesh = temp;

    he_mesh.remove_isolated_verts();
    he_mesh.shrink();
    mesh = triangulated(he_mesh);
 
 //   auto temp = he_mesh;
 //   temp.shrink();
 //   mesh = temp;

    // FillCircle(vf2d());

    drawMesh(mesh);
//    drawPoints(mesh);

     drawWireFrame(he_mesh);
     drawBorders(he_mesh);

    drawHalfEdge(he_mesh, hedge_id, olc::DARK_CYAN);
    
    moveVertexWithCursor();
    keyInput();

    update_hedge();

    return true;
  }

};


