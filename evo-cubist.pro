TEMPLATE = subdirs
SUBDIRS = evo-cubist-main \
    unit-tests \
    delaunay \
    voronoi

evo-cubist-main.file = evo-cubist-main.pro
unit-tests.file = test/unit/evo-cubist-test.pro
delaunay.file = test/delaunay/delaunay.pro
voronoi.file = test/voronoi/voronoi.pro
