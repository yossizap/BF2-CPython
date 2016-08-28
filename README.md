#BF2 CPython

This repository was created to replace DICE's py_dice.dll/.so with a newer version of CPython. The CPython lib that DICE provided uses Python 2.3.4 with minimal functionality(no threading, multiprocessing, os and sys are limited, etc.) which limits mod developers.

This type of upgrade was [previously done](https://blog.garethj.com/2008/05/07/replacing-python-on-a-battlefield-2-server) for 2.5.2 without any changes to the interpreter's implementation but it only worked on 32bit systems. Changes were required for the 32bit and 64bit versions of the python libraries to get BF2's embedded python modules to work with the newest 2.x python interpreter.

This version of the BF2 CPython adaptation was created with a fork of the 2.7.12 stable tag and passed all the tests that 2.7.12 passes except for a couple of asserts that expect py2.7.12's sizes from python objects. Multiprocessing and threading works and you can add any external library that supports 2.7.12.


##Building and replacing libdice_py

###Building
Linux: 

    make distclean
    ./configure --enable-shared
    make
    cp libpython2.7.so {BF2 SERVER DIR}/bin/{ARCH}/libdice_py.so

Windows: TODO


###Configuration:
Edit {BF2 SERVER/CLIENT DIR}/python/bf2/`__init__.py`:

after the lines:

    import host
    import sys
add:

    sys.path = ['{PATH TO PYTHON SOURCE}/build/lib.linux-x86_{ARCH}-2.7', '{PATH TO PYTHON SOURCE}/Lib', 'python', 'admin', '{YOUR MOD PATH}' ]

Alternatively, if you prefer shipping your mod's server with verified pre-compiled binaries and selected libraries, you should get rid of any packages that you don't need and do the following steps:

    cd {PATH TO PYTHON SOURCE}/Lib/
    zip -r pylib-2.7.12.zip .
    cp pylib-2.7.12.zip {SERVER DIR}
    cp -r {PATH TO PYTHON SOURCE}/build/lib.linux-x86_{ARCH}-2.7/* {SERVER DIR}/bin/{ARCH}/pylib/
    
and then add the following line in the same way we added it in the previous solution:

    sys.path = ['pylib-2.7.12.zip', 'python', 'admin', '{YOUR MOD PATH}', 'bin/{ARCH}/pylib/]

To use the zip you will need to uncomment line 467 in Module/Setup.dist and go over the build process again.


##Notes
If you have any .pyc files in your mod directory they won't work with the new interpreter unless they were re-compiled with the version of python that matches this interpreter's version.

See README.orig for the original CPython README.


##Credits
[pavelhoral](github.com/Pavelhoral) for fixing the incompatability of py2.7's pyobject struct with py2.3 object struct.
