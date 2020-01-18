Steps to upgrade MicroPython:
- Clone the micropython project and checkout the current version
- Find the current patches and save them or make sure they were integrated to the next micropython version
        git diff Path/to/epsilon/py Path/to/micropython/py
- Checkout the new version in the micropython project
- Copy the micropython py files in epsilon py folder
- Update epsilon/python/Makefile and epsilon/python/port/genhdr/qstrdefs.in.h following the instructions in the files
- Update other epsilon/python/port/genhdr/ files :
        Get a clean copy of MicroPython
        Copy our mpconfigport.h over the "bare-arm" port of MicroPython
        "make" the bare-arm port of MicroPython (don't worry if it doesn't finish)
        Copy the wanted build/genhdr files to epsilon/python/port/genhdr/
- Put back the patches from the first step if needed

