#BF2 CPython

This repository was created to replace DICE's py_dice.dll/.so with a newer version of CPython. The CPython lib that DICE provided uses Python 2.3.4 with minimal functionality(no threading, multiprocessing, os and sys are limited, etc.) which limits mod developers.

This type of upgrade was [previously done](https://blog.garethj.com/2008/05/07/replacing-python-on-a-battlefield-2-server) for 2.5.2 without any changes to the interpreter's implementation but it only worked on 32bit systems. Changes were required for the 32bit and 64bit versions of the python libraries to get BF2's embedded python modules to work with the newest 2.x python interpreter.

This version of the BF2 CPython adaptation was created with on a fork of the 2.7.12 stable tag and passes all the tests that 2.7.12 passes.


##Building and replacing libdice_py

###Installation
Linux: ./configure --enable-shared
       make
       cp libpython2.7.so {BF2 SERVER DIR}/bin/{ARCHITECTURE}/libdice_py.so

Windows: TODO


###Configuration:
Edit {BF2 SERVER/CLIENT DIR}/python/bf2/__init__.py:
    import host
    import sys

+   sys.path = ['{PATH TO NEW PYTHON SOURCE}/build/lib.linux-x86_64-2.7', '{PATH TO NEW PYTHON SOURCE}/Lib', 'python', 'admin', '{YOUR MOD PATH}' ]


##Notes
If you have any .pyc files in your mod directory they won't work with the new interpreter unless they were re-compiled with the version of python that matches this interpreter's version.

See README.orig for the original CPython README.


##Credits
[pavelhoral](github.com/Pavelhoral) for fixing the incompatability of py2.7's pyobject struct with py2.3 object struct.
