// Ported to C++ by Oliver Lau <oliver@von-und-fuer-lau.de>
// See http://www.cs.hmc.edu/~mbrubeck/voronoi.html

#ifndef __VORONOI_H_
#define __VORONOI_H_


#include <QVector>
#include <QRectF>
#include <QScopedPointer>
#include <qmath.h>
#include <queue>
#include "point.h"
#include "../../helper.h"

struct Arc;
struct Seg;

struct Event
{
    qreal x;
    Point p;
    Arc* a;
    bool valid;

    Event(qreal x, const Point& p, Arc* a)
        : x(x)
        , p(p)
        , a(a)
        , valid(true)
    { /* ... */ }

};


struct Arc
{
    Point p;
    Arc* prev;
    Arc* next;
    Event* e;
    Seg* s0;
    Seg* s1;

    Arc(const Point& p, Arc* a = NULL, Arc* b = NULL)
        : p(p)
        , prev(a)
        , next(b)
        , e(NULL)
        , s0(NULL)
        , s1(NULL)
    { /* ... */ }
};


struct Seg
{
    Seg(const Point& p)
        : start(p)
        , end(0, 0)
        , done(false)
    { /* ... */ }

    void finish(const Point& p)
    {
        if (done)
            return;
        end = p;
        done = true;
    }

    Point start;
    Point end;
    bool done;
};



class Voronoi
{
public:
    Voronoi(void)
        : mRoot(NULL)
    { /* ... */ }
    ~Voronoi() { /* ... */ }

    void add(const Point& p)
    {
        mPoints.push(p);
        if (p.x() < mBoundingBox.left())
            mBoundingBox.setLeft(p.x());
        if (p.x() > mBoundingBox.right())
            mBoundingBox.setRight(p.x());
        if (p.y() < mBoundingBox.top())
            mBoundingBox.setTop(p.y());
        if (p.y() > mBoundingBox.bottom())
            mBoundingBox.setBottom(p.y());

        while (!mPoints.empty()) {
            if (!mEvents.empty() && mEvents.top()->x <= mPoints.top().x())
                processEvent();
            else
                processPoint();
        }

        while (!mEvents.empty())
            processEvent();

        finishEdges();
    }


    void clear(void)
    {
        mResult.clear();
        while (!mPoints.empty())
            mPoints.pop();
    }


    QVector<Seg*> segments(void) const
    {
        return mResult;
    }


    const QRectF& rect(void) const
    {
        return mBoundingBox;
    }


private:
    bool circle(const Point& a, const Point& b, const Point& c, qreal& x, Point& o)
    {
        // Check that bc is a "right turn" from ab.
        if ((b.x()-a.x())*(c.y()-a.y()) - (c.x()-a.x())*(b.y()-a.y()) > 0)
            return false;

        // Algorithm from O'Rourke 2ed p. 189.
        const qreal A = b.x() - a.x();
        const qreal B = b.y() - a.y();
        const qreal C = c.x() - a.x();
        const qreal D = c.y() - a.y();
        const qreal E = A*(a.x()+b.x()) + B*(a.y()+b.y());
        const qreal F = C*(a.x()+c.x()) + D*(a.y()+c.y());
        const qreal G = 2*(A*(c.y()-b.y()) - B*(c.x()-b.x()));
        if (qFuzzyIsNull(G))
            return false; // Points are co-linear.

        // Point o is the center of the circle.
        o.setX((D*E-B*F)/G);
        o.setY((A*F-C*E)/G);

        // o.x plus radius equals max x coordinate.
        x = o.x() + sqrt(square(a.x() - o.x()) + square(a.y() - o.y()));
        return true;
    }


    void processPoint(void)
    {
       Point p = mPoints.top();
       mPoints.pop();
       frontInsert(p);
    }


    Seg* makeSeg(const Point& p)
    {
        Seg* s = new Seg(p);
        mResult.append(s);
        return s;
    }


    void processEvent(void)
    {
        Event* e = mEvents.top();
        mEvents.pop();

        if (e->valid) {
            Seg* s = makeSeg(e->p);
            Arc* a = e->a;
            if (a->prev) {
                a->prev->next = a->next;
                a->prev->s1 = s;
            }
            if (a->next) {
                a->next->prev = a->prev;
                a->next->s0 = s;
            }
            // Finish the edges before and after a.
            if (a->s0)
                a->s0->finish(e->p);
            if (a->s1)
                a->s1->finish(e->p);

            // Recheck circle events on either side of p:
            if (a->prev)
                checkCircleEvent(a->prev, e->x);
            if (a->next)
                checkCircleEvent(a->next, e->x);
        }

        delete e;
    }


    void frontInsert(const Point& p)
    {
       if (!mRoot) {
          mRoot = new Arc(p);
          return;
       }

       // Find the current arc(s) at height p.y (if there are any).
       for (Arc* i = mRoot; i; i = i->next) {
          Point z, zz;
          if (intersect(p, i, &z)) {
             // New parabola intersects arc i.  If necessary, duplicate i.
             if (i->next && !intersect(p, i->next, &zz)) {
                i->next->prev = new Arc(i->p, i, i->next);
                i->next = i->next->prev;
             }
             else {
                 i->next = new Arc(i->p, i);
             }
             i->next->s1 = i->s1;

             // Add p between i and i->next.
             i->next->prev = new Arc(p, i, i->next);
             i->next = i->next->prev;

             i = i->next; // Now i points to the new arc.

             // Add new half-edges connected to i's endpoints.
             i->prev->s1 = i->s0 = makeSeg(z);
             i->next->s0 = i->s1 = makeSeg(z);

             // Check for new circle events around the new arc:
             checkCircleEvent(i, p.x());
             checkCircleEvent(i->prev, p.x());
             checkCircleEvent(i->next, p.x());
             return;
          }
       }

       // Special case: If p never intersects an arc, append it to the list.
       Arc *i;
       for (i = mRoot; i->next; i = i->next)
           ;

       i->next = new Arc(p, i);
       // Insert segment between p and i
       Point start(mBoundingBox.left(), (i->next->p.y() + i->p.y()) / 2);
       i->s1 = i->next->s0 = makeSeg(start);
    }


    // Look for a new circle event for arc i.
    void checkCircleEvent(Arc* i, qreal x0)
    {
       // Invalidate any old event.
       if (i->e && i->e->x != x0)
          i->e->valid = false;
       i->e = NULL;
       if (!i->prev || !i->next)
          return;
       qreal x;
       Point o;
       if (circle(i->prev->p, i->p, i->next->p, x, o) && x > x0) {
          // Create new event.
          i->e = new Event(x, o, i);
          mEvents.push(i->e);
       }
    }


    // Will a new parabola at point p intersect with arc i?
    bool intersect(const Point& p, Arc* i, Point *res)
    {
       if (i->p.x() == p.x())
           return false;
       qreal a, b;
       if (i->prev) // Get the intersection of i->prev, i.
          a = intersection(i->prev->p, i->p, p.x()).y();
       if (i->next) // Get the intersection of i->next, i.
          b = intersection(i->p, i->next->p, p.x()).y();
       if ((!i->prev || a <= p.y()) && (!i->next || p.y() <= b)) {
          res->setY(p.y());
          // Plug it back into the parabola equation.
          res->setX((i->p.x()*i->p.x() + (i->p.y()-res->y())*(i->p.y()-res->y()) - p.x()*p.x()) / (2*i->p.x() - 2*p.x()));
          return true;
       }
       return false;
    }

    // Where do two parabolas intersect?
    Point intersection(const Point& p0, const Point& p1, qreal l)
    {
       Point res, p = p0;
       if (p0.x() == p1.x())
          res.setY((p0.y() + p1.y()) / 2);
       else if (p1.x() == l)
          res.setY(p1.y());
       else if (p0.x() == l) {
          res.setY(p0.y());
          p = p1;
       }
       else {
          // Use the quadratic formula.
          const qreal z0 = 2*(p0.x() - l);
          const qreal z1 = 2*(p1.x() - l);
          const qreal a = 1/z0 - 1/z1;
          const qreal b = -2*(p0.y()/z0 - p1.y()/z1);
          const qreal c = (square(p0.y()) + square(p0.x()) - square(l))/z0 - (square(p1.y()) + square(p1.x()) - square(l))/z1;
          res.setY((-b-sqrt(b*b-4*a*c))/(2*a));
       }
       // Plug back into one of the parabola equations.
       res.setX((square(p.x()) + square(p.y()-res.y()) - square(l))/(2*p.x()-2*l));
       return res;
    }


    void finishEdges(void)
    {
       // Advance the sweep line so no parabolas can cross the bounding box.
       const qreal l = mBoundingBox.right() + mBoundingBox.width() + mBoundingBox.height();

       // Extend each remaining segment to the new parabola intersections.
       for (Arc* i = mRoot; i->next != NULL; i = i->next)
          if (i->s1)
             i->s1->finish(intersection(i->p, i->next->p, l*2));
    }

private:
    struct gt {
       bool operator()(Point a, Point b) { return a.x()==b.x() ? a.y()>b.y() : a.x()>b.x(); }
       bool operator()(Event *a, Event *b) { return a->x>b->x; }
    };

    std::priority_queue<Point, QVector<Point>, gt> mPoints;
    std::priority_queue<Event*, QVector<Event*>, gt> mEvents;
    QRectF mBoundingBox;
    Arc* mRoot;
    QVector<Seg*> mResult;
};


Voronoi& operator<<(Voronoi& v, const Point& p);


#endif // __VORONOI_H_
