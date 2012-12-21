/********************************************************************************
	Copyright (C) 2004-2005 Sjaak Priester	

	This is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This file is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this application; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
********************************************************************************/

// Delaunay
// Class to perform Delaunay triangulation on a set of vertices
//
// Version 1.2 (C) 2005, Sjaak Priester, Amsterdam.
// - Removed stupid bug in SetY; function wasn't used, so no consequences. Thanks to squat.
//
// Version 1.1 (C) 2005, Sjaak Priester, Amsterdam.
// - Removed bug which gave incorrect results for co-circular vertices.
//
// Version 1.0 (C) 2004, Sjaak Priester, Amsterdam.
// mailto:sjaak@sjaakpriester.nl

#ifndef __DELAUNAY_H_
#define __DELAUNAY_H_

#include <QtCore>
#include <QPointF>
#include <set>
#include <algorithm>
#include <math.h>

///////////////////
// Vertex

class Vertex
{
public:
    Vertex()					: mPnt(0.0F, 0.0F)			{}
    Vertex(const Vertex& v)		: mPnt(v.mPnt)			{}
    Vertex(const QPointF& pnt)	: mPnt(pnt)				{}
    Vertex(qreal x, qreal y)		: mPnt(x, y)				{}
    Vertex(int x, int y)		: mPnt((qreal) x, (qreal) y)	{}

    bool operator<(const Vertex& v) const
	{
        if (mPnt.x() == v.mPnt.x()) return mPnt.y() < v.mPnt.y();
        return mPnt.x() < v.mPnt.x();
	}

    bool operator==(const Vertex& v) const
	{
        return mPnt.x() == v.mPnt.x() && mPnt.y() == v.mPnt.y();
	}
	
    qreal GetX()	const	{ return mPnt.x(); }
    qreal GetY() const	{ return mPnt.y(); }

    void SetX(qreal x)		{ mPnt.setX(x); }
    void SetY(qreal y)		{ mPnt.setY(y); }

    const QPointF& GetPoint() const		{ return mPnt; }
protected:
    QPointF	mPnt;
};

typedef std::set<Vertex> VertexSet;


///////////////////
// Triangle

class Triangle
{
public:
    Triangle(const Triangle& tri)
        : mCenter(tri.mCenter)
        , mR(tri.mR)
        , mR2(tri.mR2)
	{
        mVertices[0] = tri.mVertices[0];
        mVertices[1] = tri.mVertices[1];
        mVertices[2] = tri.mVertices[2];
	}
    Triangle(const Vertex* p0, const Vertex* p1, const Vertex* p2)
	{
        mVertices[0] = p0;
        mVertices[1] = p1;
        mVertices[2] = p2;
		SetCircumCircle();
	}
    Triangle(const Vertex* pV)
	{
        for (int i = 0; i < 3; i++)
            mVertices[i] = pV++;
		SetCircumCircle();
	}

    bool operator<(const Triangle& tri) const
	{
        if (mCenter.x() == tri.mCenter.x())
            return mCenter.y() < tri.mCenter.y();
        return mCenter.x() < tri.mCenter.x();
	}

    const Vertex* GetVertex(int i) const
	{
        return mVertices[i];
	}

    bool IsLeftOf(VertexSet::const_iterator itVertex) const
	{
		// returns true if * itVertex is to the right of the triangle's circumcircle
        return itVertex->GetPoint().x() > (mCenter.x() + mR);
	}

    bool CCEncompasses(VertexSet::const_iterator itVertex) const
	{
		// Returns true if * itVertex is in the triangle's circumcircle.
		// A vertex exactly on the circle is also considered to be in the circle.

		// These next few lines look like optimisation, however in practice they are not.
		// They even seem to slow down the algorithm somewhat.
		// Therefore, I've commented them out.

		// First check boundary box.
//		REAL x = itVertex->GetPoint().X;
//				
//		if (x > (mCenter.X + mR)) return false;
//		if (x < (mCenter.X - mR)) return false;
//
//		REAL y = itVertex->GetPoint().Y;
//				
//		if (y > (mCenter.Y + mR)) return false;
//		if (y < (mCenter.Y - mR)) return false;

        QPointF dist = itVertex->GetPoint() - mCenter;		// the distance between v and the circle center
        qreal dist2 = dist.x() * dist.x() + dist.y() * dist.y();		// squared
        return dist2 <= mR2;								// compare with squared radius
	}
protected:
    const Vertex* mVertices[3];	// the three triangle vertices
    QPointF mCenter;				// center of circumcircle
    qreal mR;			// radius of circumcircle
    qreal mR2;			// radius of circumcircle, squared

	void SetCircumCircle();
};

// Changed in verion 1.1: collect triangles in a multiset.
// In version 1.0, I used a set, preventing the creation of multiple
// triangles with identical center points. Therefore, more than three
// co-circular vertices yielded incorrect results. Thanks to Roger Labbe.
typedef std::multiset<Triangle> TriangleSet;


///////////////////
// Edge

class Edge
{
public:
    Edge(const Edge& e)	: mpV0(e.mpV0), mpV1(e.mpV1)	{}
    Edge(const Vertex* pV0, const Vertex* pV1)
        : mpV0(pV0), mpV1(pV1)
	{
	}

	bool operator<(const Edge& e) const
	{
        if (mpV0 == e.mpV0)
            return *mpV1 < *e.mpV1;
        return *mpV0 < *e.mpV0;
	}

    const Vertex* mpV0;
    const Vertex* mpV1;
};

typedef std::set<Edge> EdgeSet;



///////////////////
// Delaunay

class Delaunay
{
public:
	// Calculate the Delaunay triangulation for the given set of vertices.
    void triangulate(const VertexSet& vertices, TriangleSet& output);

	// Put the edges of the triangles in an edgeSet, eliminating double edges.
	// This comes in useful for drawing the triangulation.
    void TrianglesToEdges(const TriangleSet& triangles, EdgeSet& edges);
protected:
    void HandleEdge(const Vertex * p0, const Vertex * p1, EdgeSet& edges);
};

#endif // __DELAUNAY_H_
