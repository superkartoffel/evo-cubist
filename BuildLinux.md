# Evo Cubist unter Linux kompilieren #

## Voraussetzungen ##

  * Qt SDK  — Einige Distributionen bieten Qt über den jeweiligen Paketmanager an. Unter Ubuntu sind zum Beispiel mindestens die Pakete `qt4-dev-tools`, `qt-qmake`, `libqt4-dev`, `libqt4-gui`, `libqt4-svg`, `libqt4-xml` und `libqt4-core` zu installieren. Alternativ kann man das SDK auch von der Qt-Homepage [herunterladen](http://qt-project.org/downloads).

  * C++-Compiler aus der GNU Compiler Collection (z.B. unter Ubuntu das Paket `g++` mit allen abhängigen Paketen)

  * Build-Werkzeuge (z.B. unter Ubuntu die Pakete `make` und `binutils` mit allen abhängigen Paketen)

  * Git

## Schritt für Schritt ##

Kopie des Repository anlegen:

```
git clone https://code.google.com/p/evo-cubist/
```

In das angelegte Verzeichnis wechseln:

```
cd evo-cubist
```

Folgende Eingaben generieren das Makefile und starten den Übersetzungsvorgang:

```
qmake evo-cubist-main.pro CONFIG+=release
make
```

Fertig. Das Binary lässt sich nun starten:

```
./evo-cubist &
```