set RINGQT_WEBASSEMBLY=
set RINGQT_ANDROID=

set RINGQT_NOCORE=
set RINGQT_NOWIDGETS=1
set RINGQT_NOGUI=1
set RINGQT_NOTESTLIB=1
set RINGQT_NOOPENGL=1
set RINGQT_NOWEBKIT=1
set RINGQT_NOBLUETOOTH=1
set RINGQT_NOMULTIMEDIA=1
set RINGQT_NOMULTIMEDIAWIDGETS=1
set RINGQT_NOPRINTSUPPORT=1
set RINGQT_NONETWORK=1
set RINGQT_NOSQL=1
set RINGQT_NOPOSITIONING=1
set RINGQT_NOWEBKITWIDGETS=1
set RINGQT_NOWEBENGINEWIDGETS=1
set RINGQT_NOSERIALPORT=1
set RINGQT_NOAXCONTAINER=1
set RINGQT_NOQUICKWIDGETS=1
set RINGQT_NOQML=1
set RINGQT_NOSPEECH=1
set RINGQT_NOQT3D=1
set RINGQT_NOCHARTS=1
set RINGQT_NOGRAPHICSSVG=1

set RINGQT_CORE_ALONE=1

..\..\bin\ring ..\codegen\parsec.ring classes\qt.cf cpp\src\ring_qtcore.cpp ..\..\libraries\guilib\classes\ring_qtcore.ring
cd events
..\..\..\bin\ring qtevents.ring
cd ..
set RINGQT_CORE_ALONE= 
set RINGQT_NOWIDGETS=
set RINGQT_NOGUI=
set RINGQT_NOTESTLIB=
set RINGQT_NOOPENGL=
set RINGQT_NOWEBKIT=
set RINGQT_NOBLUETOOTH=
set RINGQT_NOMULTIMEDIA=
set RINGQT_NOMULTIMEDIAWIDGETS=
set RINGQT_NOPRINTSUPPORT=
set RINGQT_NONETWORK=
set RINGQT_NOSQL=
set RINGQT_NOPOSITIONING=
set RINGQT_NOWEBKITWIDGETS=
set RINGQT_NOWEBENGINEWIDGETS=
set RINGQT_NOSERIALPORT=
set RINGQT_NOAXCONTAINER=
set RINGQT_NOQUICKWIDGETS=
set RINGQT_NOQML=
set RINGQT_NOSPEECH=
set RINGQT_NOQT3D=
set RINGQT_NOCHARTS=
