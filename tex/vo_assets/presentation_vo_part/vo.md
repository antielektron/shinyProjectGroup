#Volumetric Obscurance
-------------------------

###Motivation / Einleitung

* Physikalisch korrekte Schattenberechnung in Echtzeit sehr schwierig (frag mal Tom :) )
* Naheliegende Vermutung: wo viel Geometrie ist, ist auch viel Schatten
* Darauf basieren Konzepte wie Ambient Occlusion
* Im folgenden wird Volumetric Obscurance vorgestellt, eine Alternative zu AO

###Was bisher geschah: Ambient Occlusion
* Es gibt im Allgemeinen 2 AO ansätze:
    * "Global Ambient Occlusion": für jeden Punkt in der Szene wird geprüft, ob der Himmel von diesem
		Punkt aus sichtbar ist (bzw. bei geschlossenen Szenen: ob eine maximale Distanz ohne Kollision erreicht
		 werden kann). Dieses Verfahren ist relativ teuer, kann aber z.B. für statische Szenen vorberechnet
		 werden.
    * Screenspace Ambient Occlusion (SSAO): für jeden Pixel im Screenspace werden Point Samples
		innerhalb einer bestimmten Umgebung erstellt. Mit Hilfe der Tiefeninformationen des Depth Buffers
		wird die Anzahl der Samples, die hinter der Szenengeometrie liegen müssen bestimmt. Anhand dieser Anzahl
		kann der Pixel entsprechend abgedunkelt werden.
    * TODO: suche schöne Bilder

###Ansatz Volumetric Obscurance
* [Bild: AOvsSSAOvsVO]
* zwei Techniken:
    * Line Sampling
    * Area Sampling

###Line Sampling
[Bild: lineSampling.png]
* betrachte für jeden vorgerenderten Texel den korrespondierenden Punkt in der Welt
* sample Tiefenbuffer in der auf den Screenspace projezierten Umgebung
* berechne verdecktes Volumen
[Bild: AnnotatedSampleDisk]

###Algorithmus Line Sampling
[Bild: pseudoLineSampling]

###Screenshots:
[Bild: vo_lineSample_sample0001]\
[Bild: vo_lineSample_sample0010]\
[Bild: vo_lineSample_sample0100]\
[Bild: vo_lineSample_sample1000]

###Area Sampling:
* line Sampling ist teuer, weil unmengen an Samples pro Pixel durch die Gegend
    geschossen werden.
* wünschenswert: Eine statistische Beschreibung der Tiefenverteilung,
    die Zeitgleich eine genaue Rückrechnung ermöglicht.
* gibt es schon: VSM, selbes prinzip können wir auf VO anwenden.

###Area Sampling(2):
* Vorgehensweise:
    * schreibe z, z² in RG-Textur( also z -> R, z² -> G)
    * mipmappe jene Textur
    * bestimme im Compose Schritt zu nutzendes mipmap-Level in abh. d. Tiefe
    * voilà, Moment wurde so gefiltert. Nun wende Statistik zum Rückrechnen an:

###Area Sampling(3):
* Mathematisch ausgedrückt:
    * µ = R, sigma² = G - µ²
    * [Bild: areaTerm01]
    * wobei a und b definiert sind als: [Bild: areaTerm02]

###Schematisch:
[Bild: variance01]\
[Bild: variance02]\
[Bild: variance03]
