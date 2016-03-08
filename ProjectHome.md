![http://evo-cubist.googlecode.com/files/Mona-Lisa-256x256-0000063135-000006630.png](http://evo-cubist.googlecode.com/files/Mona-Lisa-256x256-0000063135-000006630.png)
![http://evo-cubist.googlecode.com/files/Mona-Lisa-256x256.png](http://evo-cubist.googlecode.com/files/Mona-Lisa-256x256.png)
![http://evo-cubist.googlecode.com/files/Mona-Lisa-256x256-0002592562-000054322.png](http://evo-cubist.googlecode.com/files/Mona-Lisa-256x256-0002592562-000054322.png)

**Evolutionäre Entwicklung einer Vektorgrafik aus einem vorgegebenen Pixelbild**

Der für _Evo Cubist_ verwendete genetische Algorithmus ist einfach. Nach dem Generieren (zufälliger) Polygone mit 3 bis n Eckpunkten fährt er immer aufs Neue wie folgt fort:

  1. Mutiere einige Ecken und gelegentlich die Farben einiger Polygone nach dem Zufallsprinzip.
  1. Male die mutierten Polygone auf eine (weiße) Leinwand.
  1. Berechne die RGB-Differenzen jeder Pixel von generiertem und originalem Bild und summiere sie auf.
  1. Wenn die Summe kleiner ist als die zuletzt berechnete, passt das neue Bild besser und die Polygone werden für die nächste Generation verwendet. Ansonsten wird die Mutation verworfen.

Inspiration:

  * [Roger Alsing: Genetic Programming — Evolution of Mona Lisa](http://rogeralsing.com/2008/12/07/genetic-programming-evolution-of-mona-lisa/)
  * [Screaming Duck Stuff: Mona and Lena](http://www.screamingduck.com/Article.php?ArticleID=46&Show=ABCE)
  * [ImageLife by cr](https://github.com/cr/ImageLife)

Literatur:

  * Oliver Lau, Evo Cubist, Genetische Algorithmen erzeugen Vektorgrafiken aus Pixelbildern, [c't 22/12, S. 190](http://www.heise.de/ct/inhalt/2012/22/190/)
  * Thomas Blümecke, Wunder der Evolution, Optimierung mit Evolutionsstrategien und genetischen Algorithmen, c't 12/91, S. 228



## Nutzungshinweise ##

Die Software wurde zu Lehr- und Demonstrationszwecken geschaffen und ist nicht für den produktiven Einsatz vorgesehen. Der Autor und der Heise Zeitschriften Verlag haften daher nicht für Schäden, die aus der Nutzung der Software entstehen, und übernehmen keine Gewähr für ihre Vollständigkeit, Fehlerfreiheit und Eignung für einen bestimmten Zweck.

Falls Sie beabsichtigen, den Code in eigenen Produkten zu verwenden, so ist das im Rahmen der [GPLv3](http://www.gnu.de/documents/gpl.de.html) kostenfrei gestattet (siehe die Datei LICENSE.txt im [Repository](http://code.google.com/p/evo-cubist/source/browse/) für weitere Informationen).

Copyright (c) 2012 Oliver Lau <ola@ct.de>, [Heise Zeitschriften Verlag](http://www.heise.de/)