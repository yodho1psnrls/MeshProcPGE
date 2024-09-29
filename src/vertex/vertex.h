#pragma once
#include "../../olc/olcPixelGameEngine.h"

struct Vertex {
private:
	using vf2d = olc::vf2d;

public:
	vf2d pos;
	vf2d uv;

	//vf2d tan;
	//vf2d bitan;

	//Vertex() : pos(0.0f, 0.0f), uv(0.0f, 0.0f) {}
	//Vertex(const vf2d& Pos) : pos(Pos), uv(0.0f, 0.0f) {}

	Vertex(const vf2d& pos = vf2d(0.0f, 0.0f), const vf2d& uv = vf2d(0.0f, 0.0f))
		: pos(pos), uv(uv) {}


	//operator vf2d& () { return pos; }
	//operator const vf2d& () const { return pos; }
	//operator vi2d () const { return pos; }


	// Component/Attribute wise operations on vertices

	Vertex& operator+=(const Vertex& other) {
		pos += other.pos;
		uv += other.uv;

		return *this;
	}

	Vertex& operator-=(const Vertex& other) {
		pos -= other.pos;
		uv -= other.uv;

		return *this;
	}

	Vertex& operator*=(const float& scalar) {
		pos *= scalar;
		uv *= scalar;

		return *this;
	}

	Vertex& operator/=(const float& scalar) {
		pos /= scalar;
		uv /= scalar;

		return *this;
	}


	Vertex operator+(const Vertex& rhs) const {
		Vertex v(*this);
		return v += rhs;
	}

	Vertex operator-(const Vertex& rhs) const {
		Vertex v(*this);
		return v -= rhs;
	}

	Vertex operator*(const float& scalar) const {
		Vertex v(*this);
		return v *= scalar;
	}

	Vertex operator/(const float& scalar) const {
		Vertex v(*this);
		return v /= scalar;
	}

	/*friend Vertex operator*(const float& scalar, const Vertex& vert) {	// static	// friend // constexpr
		Vertex v(vert);
		return v *= scalar;
	}*/

};

//Vertex operator*(const float& scalar, Vertex vert) { return vert *= scalar; }
Vertex operator*(const float& scalar, const Vertex& vert) {
	Vertex v(vert);
	return v *= scalar;
}
