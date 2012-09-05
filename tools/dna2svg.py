#!/usr/bin/env python
# Copyright (c) 2012 Oliver Lau <oliver@von-und-fuer-lau.de>

import sys, json

filename = sys.argv[1]
f = open(filename, "r")
dna = json.loads(f.read())
f.close()
print "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"
print " <g transform=\"scale(%d,%d)\">" % (dna["size"]["width"], dna["size"]["height"])
for genome in dna["dna"]:
    c = genome["color"]
    vertices = genome["vertices"]
    print "  <path fill=\"rgb(%d,%d,%d)\" fill-opacity=\"%f\" d=\"M %f %f" % (c["r"], c["g"], c["b"], c["a"], vertices[0]["x"], vertices[0]["y"]),
    for v in vertices[1:]:
        print "L %f %f" % (v["x"], v["y"]),
    print "Z\"/>"
print " </g>"
print "</svg>\n"
