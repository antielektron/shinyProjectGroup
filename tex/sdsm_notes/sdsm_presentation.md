# SDSM

## Motivation
- Vermeide Oversampling / Undersampling / Geometrisches Aliasing
- Basierend auf Cascaded Shadow Maps
- Partitioniere Kamera Frustum entlang Z-Achse
- Vollautomatische Partitionierung


## Near und Far Plane
- Verschiebe Near- und Far-Plane dicht an die Geometrie
// gerade das kennen der tatsächlichen Near-Plane ist bei logarithmischer Partitionierung wichtig
- Min/Max Reduktion auf Depth-Buffer
// Diagramm

## Depth Partitioning
- Logarithmisch
// Minimiere perspektivisches Aliasing, z_near/z_far = const
- Adaptiv logarithmisch
  - Vermeidung von Lücken im Depth-Buffer Histogramm
- K-Means
  - Partitionen um Maxima des Depth-Buffer Histogramms
// Zusätzlicher Aufwand um Histogramm zu berechnen


## Tight Partition Frusta / Light Space
- Shadow Map Frusta eng um sichtbare Geometrie
// Andere Verfahren benutzen die Ecken des Kamera Frustums
- Rotiere Light Direction auf Z-Achse
- Rotiere Kamera Z-Achse um die Z-Achse auf X-Achse
// Rotiere so dass sie in der Projektion gleich sind
- Berechne AABB der sichtbaren Geometrie per Reduktion


## Reduktionen
- Betrachte Minimierungsprobleme
- Nutze 1D Textur
// Um Anzahl der Threads ohne Arbeit zu minimieren.
- Eine Ausgabe pro Threadgruppe
- Reduziere mehrere Werte simultan
// Reduziere alle Bounding Box Corners simultan
// TODO DIAGRAMM

# Vergleich

## Parallel Split Shadow Maps
- Near- und Far-Plane unbekannt
- Verschiebe Grenzen von der Kamera weg
- Konvexkombiniere Kaskadengrenzen linearer und logarithmischer Partitionierung
// kleine Visulaisierung, fiktiv

## Parallel Split Shadow Maps vs Sample Distribution Shadow Maps
- Screenshot

## Schöne Screenshots die Partitionen zeigen
- Screenshot

# Aussicht

## Sheared Sample Distribution Shadow Maps
- Screenshot
// Erzähle
