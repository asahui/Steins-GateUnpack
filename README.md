Steins;GateUnpack
================
Steins;Gate 線形拘束のフェのグラム(PS3) .CPK unpack tools from [kamemusi](http://www5.atpages.jp/kamemusi/tool/win/STEINSGATE.html)

png tools for output CG files from unpack files

<br />

build
-----
1. build cpk_unpack
> cd cpk_unpack
> make
>
> if make fails

2. build writePng
> cd writePng
> gcc writePng -lpng -o png

3. Unpack files and generate pictures
> ./cpk_unpack [Steins;Gate files path]/BG.CPK
files will be BG.CPK.outXXX

> ./png BG.CPK.270 270.png

