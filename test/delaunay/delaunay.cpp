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
	along with Tinter; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
********************************************************************************/

// Delaunay
// Class to perform Delaunay triangulation on a set of vertices
//
// Version 1.1 (C) 2005, Sjaak Priester, Amsterdam.
// - Removed bug which gave incorrect results for co-circular vertices.
//
// Version 1.0 (C) 2004, Sjaak Priester, Amsterdam.
// mailto:sjaak@sjaakpriester.nl


#include "delaunay.h"

const qreal REAL_EPSILON = 1.192092896e-07F;
const qreal sqrt3 = 1.732050808F;

void Triangle::SetCircumCircle()
{
    qreal x0 = mVertices[0]->GetX();
    qreal y0 = mVertices[0]->GetY();

    qreal x1 = mVertices[1]->GetX();
    qreal y1 = mVertices[1]->GetY();

    qreal x2 = mVertices[2]->GetX();
    qreal y2 = mVertices[2]->GetY();

    qreal y10 = y1 - y0;
    qreal y21 = y2 - y1;

	bool b21zero = y21 > -REAL_EPSILON && y21 < REAL_EPSILON;

	if (y10 > -REAL_EPSILON && y10 < REAL_EPSILON)
	{
		if (b21zero)	// All three vertices are on one horizontal line.
		{
			if (x1 > x0)
			{
				if (x2 > x1) x1 = x2;
			}
			else
			{
				if (x2 < x0) x0 = x2;
			}
            mCenter.setX((x0 + x1) * .5F);
            mCenter.setY(y0);
		}
        else	// mVertices[0] and mVertices[1] are on one horizontal line.
		{
            qreal m1 = - (x2 - x1) / y21;

            qreal mx1 = (x1 + x2) * .5F;
            qreal my1 = (y1 + y2) * .5F;

            mCenter.setX((x0 + x1) * .5F);
            mCenter.setY(m1 * (mCenter.x() - mx1) + my1);
		}
	}
    else if (b21zero)	// mVertices[1] and mVertices[2] are on one horizontal line.
	{
        qreal m0 = - (x1 - x0) / y10;

        qreal mx0 = (x0 + x1) * .5F;
        qreal my0 = (y0 + y1) * .5F;

        mCenter.setX((x1 + x2) * .5F);
        mCenter.setY(m0 * (mCenter.x() - mx0) + my0);
	}
	else	// 'Common' cases, no multiple vertices are on one horizontal line.
	{
        qreal m0 = - (x1 - x0) / y10;
        qreal m1 = - (x2 - x1) / y21;

        qreal mx0 = (x0 + x1) * .5F;
        qreal my0 = (y0 + y1) * .5F;

        qreal mx1 = (x1 + x2) * .5F;
        qreal my1 = (y1 + y2) * .5F;

        mCenter.setX((m0 * mx0 - m1 * mx1 + my1 - my0) / (m0 - m1));
        mCenter.setY(m0 * (mCenter.x() - mx0) + my0);
	}

    qreal dx = x0 - mCenter.x();
    qreal dy = y0 - mCenter.y();

    mR2 = dx * dx + dy * dy;	// the radius of the circumcircle, squared
    mR = (qreal) sqrt(mR2);	// the proper radius

    // Version 1.1: make mR2 slightly higher to ensure that all edges
	// of co-circular vertices will be caught.
	// Note that this is a compromise. In fact, the algorithm isn't really
	// suited for very many co-circular vertices.
    mR2 *= 1.000001f;
}

// Function object to check whether a triangle has one of the vertices in SuperTriangle.
// operator() returns true if it does.
class triangleHasVertex
{
public:
    triangleHasVertex(const Vertex SuperTriangle[3]) : mpSuperTriangle(SuperTriangle)	{}
    bool operator()(const Triangle& tri) const
	{
		for (int i = 0; i < 3; i++)
		{
            const Vertex * p = tri.GetVertex(i);
            if (p >= mpSuperTriangle && p < (mpSuperTriangle + 3)) return true;
		}
		return false;
	}
protected:
    const Vertex * mpSuperTriangle;
};

// Function object to check whether a triangle is 'completed', i.e. doesn't need to be checked
// again in the algorithm, i.e. it won't be changed anymore.
// Therefore it can be removed from the workset.
// A triangle is completed if the circumcircle is completely to the left of the current vertex.
// If a triangle is completed, it will be inserted in the output set, unless one or more of it's vertices
// belong to the 'super triangle'.
class triangleIsCompleted
{
public:
    triangleIsCompleted(VertexSet::const_iterator itVertex, TriangleSet& output, const Vertex SuperTriangle[3])
        : mitVertex(itVertex)
        , mOutput(output)
        , mpSuperTriangle(SuperTriangle)
	{}
    bool operator()(const Triangle& tri) const
	{
        bool b = tri.IsLeftOf(mitVertex);

		if (b)
		{
            triangleHasVertex thv(mpSuperTriangle);
            if (! thv(tri)) mOutput.insert(tri);
		}
		return b;
	}

protected:
    VertexSet::const_iterator mitVertex;
    TriangleSet& mOutput;
    const Vertex * mpSuperTriangle;
};

// Function object to check whether vertex is in circumcircle of triangle.
// operator() returns true if it does.
// The edges of a 'hot' triangle are stored in the edgeSet edges.
class vertexIsInCircumCircle
{
public:
    vertexIsInCircumCircle(VertexSet::const_iterator itVertex, EdgeSet& edges) : mitVertex(itVertex), mEdges(edges)	{}
    bool operator()(const Triangle& tri) const
	{
        bool b = tri.CCEncompasses(mitVertex);

		if (b)
		{
			HandleEdge(tri.GetVertex(0), tri.GetVertex(1));
			HandleEdge(tri.GetVertex(1), tri.GetVertex(2));
			HandleEdge(tri.GetVertex(2), tri.GetVertex(0));
		}
		return b;
	}
protected:
    void HandleEdge(const Vertex * p0, const Vertex * p1) const
	{
        const Vertex * pVertex0(NULL);
        const Vertex * pVertex1(NULL);

		// Create a normalized edge, in which the smallest vertex comes first.
		if (* p0 < * p1)
		{
			pVertex0 = p0;
			pVertex1 = p1;
		}
		else
		{
			pVertex0 = p1;
			pVertex1 = p0;
		}

        Edge e(pVertex0, pVertex1);

		// Check if this edge is already in the buffer
        EdgeSet::iterator found = mEdges.find(e);

        if (found == mEdges.end()) mEdges.insert(e);		// no, it isn't, so insert
        else mEdges.erase(found);							// yes, it is, so erase it to eliminate double edges
	}

    VertexSet::const_iterator mitVertex;
    EdgeSet& mEdges;
};

void Delaunay::triangulate(const VertexSet& vertices, TriangleSet& output)
{
	if (vertices.size() < 3) return;	// nothing to handle

	// Determine the bounding box.
    VertexSet::const_iterator itVertex = vertices.begin();

    qreal xMin = itVertex->GetX();
    qreal yMin = itVertex->GetY();
    qreal xMax = xMin;
    qreal yMax = yMin;

	++itVertex;		// If we're here, we know that vertices is not empty.
	for (; itVertex != vertices.end(); itVertex++)
	{
		xMax = itVertex->GetX();	// Vertices are sorted along the x-axis, so the last one stored will be the biggest.
        qreal y = itVertex->GetY();
		if (y < yMin) yMin = y;
		if (y > yMax) yMax = y;
	}

    qreal dx = xMax - xMin;
    qreal dy = yMax - yMin;

	// Make the bounding box slightly bigger, just to feel safe.
    qreal ddx = dx * 0.01F;
    qreal ddy = dy * 0.01F;

	xMin -= ddx;
	xMax += ddx;
	dx += 2 * ddx;

	yMin -= ddy;
	yMax += ddy;
	dy += 2 * ddy;

	// Create a 'super triangle', encompassing all the vertices. We choose an equilateral triangle with horizontal base.
	// We could have made the 'super triangle' simply very big. However, the algorithm is quite sensitive to
	// rounding errors, so it's better to make the 'super triangle' just big enough, like we do here.
    Vertex vSuper[3];

    vSuper[0] = Vertex(xMin - dy * sqrt3 / 3.0F, yMin);	// Simple highschool geometry, believe me.
    vSuper[1] = Vertex(xMax + dy * sqrt3 / 3.0F, yMin);
    vSuper[2] = Vertex((xMin + xMax) * 0.5F, yMax + dx * sqrt3 * 0.5F);

    TriangleSet workset;
    workset.insert(Triangle(vSuper));

	for (itVertex = vertices.begin(); itVertex != vertices.end(); itVertex++)
	{
		// First, remove all 'completed' triangles from the workset.
		// A triangle is 'completed' if its circumcircle is entirely to the left of the current vertex.
		// Vertices are sorted in x-direction (the set container does this automagically).
		// Unless they are part of the 'super triangle', copy the 'completed' triangles to the output.
		// The algorithm also works without this step, but it is an important optimalization for bigger numbers of vertices.
		// It makes the algorithm about five times faster for 2000 vertices, and for 10000 vertices,
		// it's thirty times faster. For smaller numbers, the difference is negligible.
        TriangleSet::iterator itEnd = remove_if(workset.begin(), workset.end(), triangleIsCompleted(itVertex, output, vSuper));

        EdgeSet edges;

		// A triangle is 'hot' if the current vertex v is inside the circumcircle.
		// Remove all hot triangles, but keep their edges.
		itEnd = remove_if(workset.begin(), itEnd, vertexIsInCircumCircle(itVertex, edges));
		workset.erase(itEnd, workset.end());	// remove_if doesn't actually remove; we have to do this explicitly.

		// Create new triangles from the edges and the current vertex.
        for (EdgeSet::iterator it = edges.begin(); it != edges.end(); it++)
            workset.insert(Triangle(it->mpV0, it->mpV1, & (* itVertex)));
	}

	// Finally, remove all the triangles belonging to the 'super triangle' and move the remaining
	// triangles tot the output; remove_copy_if lets us do that in one go.
    TriangleSet::iterator where = output.begin();
	remove_copy_if(workset.begin(), workset.end(), inserter(output, where), triangleHasVertex(vSuper));
}

void Delaunay::TrianglesToEdges(const TriangleSet& triangles, EdgeSet& edges)
{
    for (TriangleSet::const_iterator it = triangles.begin(); it != triangles.end(); ++it)
	{
		HandleEdge(it->GetVertex(0), it->GetVertex(1), edges);
		HandleEdge(it->GetVertex(1), it->GetVertex(2), edges);
		HandleEdge(it->GetVertex(2), it->GetVertex(0), edges);
	}
}

void Delaunay::HandleEdge(const Vertex* p0, const Vertex* p1, EdgeSet& edges)
{
    const Vertex* pV0(NULL);
    const Vertex* pV1(NULL);

	if (* p0 < * p1)
	{
		pV0 = p0;
		pV1 = p1;
	}
	else
	{
		pV0 = p1;
		pV1 = p0;
	}

	// Insert a normalized edge. If it's already in edges, insertion will fail,
	// thus leaving only unique edges.
    edges.insert(Edge(pV0, pV1));
}
