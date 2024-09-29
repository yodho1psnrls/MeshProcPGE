#pragma once
// #include "half_edge.hpp"
#include "mesh/generate_plane.hpp"
#include "mesh/tri_mesh.hpp"
#include "utilities.hpp"
#include "point_cloud/point_vec.hpp"

// #include "vertex/vertex.h"
#include "../olc/olcPixelGameEngine.h"
#include <iostream>

// for debugging
#include <set>

const float PINDIST = 8.0f;   // the range that the mouse will select a vertex
const int INIT_HEDGE_ID = 15; // 15

template <typename V, typename PointCloud = pc::PointVec<V>>
class Example : public olc::PixelGameEngine {

  using Mesh = mesh::TriMesh<V, PointCloud>;
  using vert_handle = typename Mesh::vert_handle;
  using face_handle = typename Mesh::face_handle;

  Mesh mesh;
  //	HEMesh<V> he_mesh;

  vert_handle selected_vert_id;
  // int hedge_id;
  //	hedge_iter hedge;

  //	std::vector<std::pair<HEMesh<V>, int>> prev_history;
  //	std::vector<std::pair<HEMesh<V>, int>> next_history;

public:
  Example() { sAppName = "Example"; }

  void drawMesh(const Mesh &mesh, const olc::Pixel &color = olc::WHITE) {
    for (auto face_id : mesh.faces()) {
      auto indices = mesh.verts(face_id);

      const V &a = mesh.vert(indices[0]);
      const V &b = mesh.vert(indices[1]);
      const V &c = mesh.vert(indices[2]);

      // olc::vf2d v;
      // Example<float> kekw = v.dot(v);

      // mesh.add_vert();

      // To better visualize the different triangles, we will color them based
      // on their uv.x coordinate
      float diffuse = (a.uv.x + b.uv.x + c.uv.x) / 3.0f;
      // float diffuse = (b.pos - a.pos).norm().perp().dot((c.pos -
      // a.pos).norm());
      // float diffuse = 0.5f;

      FillTriangle(a.pos, b.pos, c.pos, color * diffuse);
    }

    for (auto vert_id : mesh.verts()) {
      FillCircle(mesh.vert(vert_id).pos, 5.0f, olc::RED);
    }
  }

  void drawWireFrame(const Mesh &mesh, const olc::Pixel &color = olc::WHITE) {
    for (auto fh : mesh.faces()) {
      auto indices = mesh.verts(fh);

      DrawTriangle(mesh.vert(indices[0]).pos, mesh.vert(indices[1]).pos,
                   mesh.vert(indices[2]).pos, color);
    }
  }

  void drawHalfArrow(const olc::vf2d &from, const olc::vf2d &to,
                     const olc::Pixel &color = olc::WHITE) {
    float sep = 6.0f;
    olc::vf2d n = (to - from).norm();

    for (float i = 0.0f; i < 3.0f; i += 1.0f) {
      olc::vf2d dif = n.perp() * (0.25f * sep + i);
      DrawLine(from + dif, to + dif, color);
      DrawLine(to + dif, to - n * sep * 1.25f + n.perp() * sep * 0.75f + dif,
               color);
    }

    // DrawLine(to, to - n * sep + n.perp() * sep, color);
  }

  void print_info(const Mesh &m) {
    std::cout << "verts: " << m.verts_size() << "\n";
    std::cout << "triangles: " << m.faces_size() << "\n";

    /*int n = 0;
    for(auto vh : m.verts()) {
      std::cout << m.vert(vh).pos << "\n";
      ++n;
    }*/
  }

  void moveVertexWithCursor() {
    olc::vf2d mouse_pos = GetMousePos();

    if (GetMouse(0).bPressed)
      for (auto vh : mesh.verts())
        if ((mesh.vert(vh).pos - mouse_pos).mag() < PINDIST) {
          selected_vert_id = vh;
          break;
        }

    if (GetMouse(0).bReleased)
      selected_vert_id = -1;

    if (selected_vert_id != -1)
      // mesh.vertices[selected_vert_id].pos = mouse_pos;
      mesh.vert(selected_vert_id).pos = mouse_pos;
  }

  // void drawHalfEdge(const HalfEdgeMesh<vf2d>::HalfEdge* edge, const
  // olc::Pixel& color = olc::WHITE) {
  /*void drawHalfEdge(const hedge_iter& e, const olc::Pixel& color = olc::WHITE)
  { if (e.index() == -1) return;

          drawHalfArrow(e.tail()->pos, e.head()->pos, color);

          // Draw the head vertex index
          DrawStringDecal(e.head()->pos, std::to_string(e.head().index()),
  olc::GREEN);

          // Draw the half edge index
          DrawStringDecal(0.5f * (e.tail()->pos + e.head()->pos) +
  olc::vf2d(0.0f, 4.0f) , std::to_string(e.index()), olc::MAGENTA);

  }

  void drawBorders(const HEMesh<V>& hm, const olc::Pixel& color = olc::RED) {

          for (const face_iter& face : hm.borders()) {
                  hedge_iter begin = face;
                  hedge_iter e = begin;

                  do {
                          //drawHalfEdge(e, color);

                          const olc::vf2d& from = e.tail()->pos;
                          const olc::vf2d& to = e.head()->pos;
                          DrawLine(from, to, color);

                          olc::vf2d n = (to - from).norm().perp();
                          DrawLine(from + n, to + n, color);


                          e = e.next();
                  } while (e != begin);
          }
  }*/

  /*
          void moveVertexWithCursor() {
                  olc::vf2d mouse_pos = GetMousePos();

                  if (GetMouse(0).bPressed)
                          for (auto vit = he_mesh.begin_verts(); vit !=
     he_mesh.end_verts(); ++vit) if ((vit->pos - mouse_pos).mag() < PINDIST) {
                                          selected_vert_id = vit.index();
                                          break;
                                  }


                  if (GetMouse(0).bReleased)
                          selected_vert_id = -1;

                  if (selected_vert_id != -1)
                          //mesh.vertices[selected_vert_id].pos = mouse_pos;
                          he_mesh.vert(selected_vert_id)->pos = mouse_pos;

          }

          void keyInput() {

                  if (GetKey(olc::I).bReleased) print_info(he_mesh);
                  if (GetKey(olc::G).bReleased) mesh = he_mesh;
                  if (GetKey(olc::N).bReleased) hedge = hedge.next();
                  if (GetKey(olc::P).bReleased) hedge = hedge.prev();
                  if (GetKey(olc::T).bReleased) hedge = hedge.twin();
                  if (GetKey(olc::V).bReleased) he_mesh.check_validity();

                  if (GetKey(olc::Z).bReleased && !prev_history.empty()) {
                          next_history.push_back({ he_mesh, hedge.index() });

                          auto pairr = prev_history.back();
                          he_mesh = pairr.first;
                          hedge = he_mesh.hedge(pairr.second);

                          prev_history.pop_back();
                  }

                  if (GetKey(olc::X).bReleased && !next_history.empty()) {
                          prev_history.push_back({ he_mesh, hedge.index() });

                          auto pairr = next_history.back();
                          he_mesh = pairr.first;
                          hedge = he_mesh.hedge(pairr.second);

                          next_history.pop_back();
                  }

                  // Resets to initial state
                  if (GetKey(olc::C).bReleased) {
                          prev_history.push_back({ he_mesh, hedge.index() });

                          auto pairr = prev_history.front();
                          he_mesh = pairr.first;
                          hedge = he_mesh.hedge(pairr.second);

                          next_history.insert(next_history.end(),
     prev_history.rbegin(), --prev_history.rend()); prev_history.clear();
                  }


                  if (GetKey(olc::S).bReleased) {
                          he_mesh.remove_isolated_verts();
                          he_mesh.shrink();

                          if (hedge.index() >= he_mesh.hedges_max_size())
                                  hedge = he_mesh.begin_hedges();
                  }

                  if (GetKey(olc::E).bReleased) {
                          int given_hedge_id;
                          std::cin >> given_hedge_id;
                          hedge = he_mesh.hedge(given_hedge_id);
                  }

                  // Draws the begin half-edge of the loop and indicates is it a
     face(GREEN) or border(RED) if (GetKey(olc::SPACE).bHeld) {
                          drawHalfEdge(hedge.face(), olc::WHITE);

                          if (hedge.is_border())
                                  FillRect({ 0, 0 }, { 15, 15 }, olc::RED);
                          else
                                  FillRect({ 0, 0 }, { 15, 15 }, olc::GREEN);
                  }


                  if (GetKey(olc::R).bReleased) {
                          next_history.clear();
                          prev_history.push_back({ he_mesh, hedge.index() });

                          auto temp = hedge;

                          if (hedge.next().twin() == hedge)
                                  temp = hedge.next().next();
                          else
                                  temp = hedge.next();

                          he_mesh.remove_edge(hedge.index());

                          hedge = temp;

                          he_mesh.check_validity();
                  }

                  if (GetKey(olc::F).bReleased) {
                          next_history.clear();
                          prev_history.push_back({ he_mesh, hedge.index() });

                          //he_mesh.triangulate();
                          //auto temp = hedge.prev();
                          //he_mesh.remove_vert(hedge.head());

                          //he_mesh.shift_begin(hedge.face(), 13);
                          //he_mesh.flip_edge(hedge.index());
                          //he_mesh.refine_edge(hedge);

                          //V v = *hedge.head();
                          //v.pos += olc::vf2d(-10.0f, -17.0f);
                          //he_mesh.split_vert_to_edge(hedge,
     hedge.next().twin(), v);
                          //he_mesh.split_vert_to_faces(hedge,
     hedge.next().twin(), v);

                          //he_mesh.refine_edge(hedge);
                          //he_mesh.split_edge(hedge, 0.5f);
                          //he_mesh.clip_corner(hedge);

                          //auto temp = hedge.prev().twin();
                          //he_mesh.collapse_edge(hedge);

                          //auto temp = hedge.prev().twin().next();
                          //he_mesh.collapse_face(hedge);
                          //he_mesh.begin_hedges();

                          //hedge = he_mesh.bevel_face(hedge);
                          //he_mesh.bevel_vert(hedge.head());
                          //he_mesh.bevel_edge(hedge);

                          //auto temp = hedge.next().twin();
                          //V v = hedge.head()->pos + olc::vf2d(10.0f, 17.0f);
                          //he_mesh.split_vert_to_edge(hedge, hedge, v);
                          //he_mesh.split_vert_to_edge(hedge,
     hedge.next().twin(), v);

                          //he_mesh.cut_edge(hedge);
                          he_mesh.cut_edge(hedge);


                          //	hedge = temp;
                          he_mesh.check_validity();
                  }

          }

          void print_info(const Mesh<V>& m) const {
                  std::cout << "================\n";
                  std::cout << "vertices: " << m.vertices.size() << "\n";
                  std::cout << "triangles: " << m.indices.size() / 3ULL << "\n";
                  std::cout << "\n";
          }*/

  /*void print_info(const HEMesh<V>& hm) const {
          std::cout << "================\n";
          std::cout << "vertices: " << hm.verts_size() << "\n";
  //	std::cout << "max_verts: " << hm.verts_max_size() << "\n";
  //	std::cout << "\n";
          std::cout << "edges: " << hm.edges_size() << "\n";
  //	std::cout << "max_edges: " << hm.edges_max_size() << "\n";
  //	std::cout << "\n";
          std::cout << "faces: " << hm.faces_size() << "\n";
          std::cout << "borders: " << hm.borders_size() << "\n";
          std::cout << "\n";
  }*/

  void test_mesh(const Mesh &m) {
    std::set<int> unique_verts;

    //  for(auto fh : m.faces()) {
    //    for (auto vh : m.verts(fh)) {
    //    //  unique_verts.insert(vh);
    //      std::cout << vh << ", ";
    //    }
    //    std::cout << "\n";
    //  }

    // for(auto vh : unique_verts)
    //   std::cout << vh << "\n";
  }

public:
  bool OnUserCreate() override {
    // Try removing some faces of the grid and see if most of the times
    //  the begin index of the loop is preserved, if the begin index is not
    //  removed

    mesh = mesh::generate_plane<V, PointCloud>(3, 0.9f);
    // mesh = mesh::generate_plane<V>(3, 30.0f);
    // mesh = Mesh::plane(3, 30.0f);

    print_info(mesh);
    //	he_mesh = mesh;

    test_mesh(mesh);

    // he_mesh = test_mesh4();
    // mesh = he_mesh;

    // test_mesh3();

    //		he_mesh.check_validity();
    //		print_info(he_mesh);
    //		selected_vert_id = -1;
    //		hedge = he_mesh.hedge(INIT_HEDGE_ID);

    return true;
  }

  bool OnUserUpdate(float fElapsedTime) override {
    Clear(olc::BLACK);

    // mesh = he_mesh;

    // FillCircle(vf2d());

    drawMesh(mesh);
    // drawWireFrame(he_mesh);
    // drawBorders(he_mesh);

    // drawHalfEdge(hedge, olc::DARK_CYAN);
    moveVertexWithCursor();
    // moveVertexWithCursor();
    // keyInput();

    return true;
  }
};
