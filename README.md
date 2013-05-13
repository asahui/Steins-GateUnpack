Steins;GateUnpack
================
Steins;Gate 線形拘束のフェのグラム(PS3) .CPK unpack tools from [kamemusi](http://www5.atpages.jp/kamemusi/tool/win/STEINSGATE.html)

png tools for output CG files from unpack files

<br />

Build
-----
1. build cpk_unpack
> cd cpk_unpack
> make
>
> if make fails

2. build writePng
> cd writePng
> gcc writePng -lpng -o png

Usage
-----
1. Unpack files and generate pictures
Unpack CPK files with cpk_unpack, output files will be BG.CPK.outXXX
generate png files with png
> ./cpk_unpack [Steins;Gate files path]/BG.CPK
> ./png BG.CPK.270 270.png

2. Or you can use a bat or shell script to generate all CG
> ./writePng.sh
