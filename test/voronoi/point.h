#ifndef __POINT_H_
#define __POINT_H_

class Point
{
public:
    Point(void)
        : mX(0)
        , mY(0)
    { /* ... */ }
    Point(qreal x, qreal y)
        : mX(x)
        , mY(y)
    { /* ... */ }
    Point(const Point& other)
        : mX(other.mX)
        , mY(other.mY)
    { /* ... */ }

    inline qreal x(void) const { return mX; }
    inline qreal y(void) const { return mY; }

    inline void setX(qreal x) { mX = x; }
    inline void setY(qreal y) { mY = y; }

    inline bool operator<(const Point& p) { return (mX == p.x())? mY > p.y() : mX > p.x(); }
    inline bool operator>(const Point& p) { return mX > p.x(); }

private:
    qreal mX;
    qreal mY;
};

#endif // __POINT_H_
