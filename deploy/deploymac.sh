#!/bin/bash
# Copyright (c) 2011 Oliver Lau <oliver@von-und-fuer-lau.de>
# All rights reserved.

# ----------------------------------------------------
# CHANGE NAMES AND PATHS ACCORDING TO YOUR ENVIRONMENT
# ----------------------------------------------------
APP=Evo\ Cubist
VERSION=1.1-MacOSX-x64

# ----------------------------------------
# DO NOT CHANGE ANYTHING BENEATH THIS LINE
# ----------------------------------------
APPBUNDLE=$APP.app

mv evo-cubist.app "$APPBUNDLE"

rm Evo-Cubist-$VERSION.dmg

echo Generating disk image ..

hdiutil create -srcfolder "$APPBUNDLE" -volname "$APP $VERSION" -fs HFS+ -format UDZO -size 100M "$APP-$VERSION.dmg"
