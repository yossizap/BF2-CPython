#BF2 CPython

This repository was created to replace DICE's py_dice.dll/.so with a newer version of CPython. The CPython lib that DICE provided uses Python 2.3.4 with minimal functionality(no threading, multiprocessing, os and sys are limited, etc.) which limits mod developers.

This type of upgrade was [previously done](https://blog.garethj.com/2008/05/07/replacing-python-on-a-battlefield-2-server) for 2.5.2 without any changes to the interpreter's implementation but it only worked on 32bit systems. Changes were required for the 32bit and 64bit versions of the python libraries to get BF2's embedded python modules to work with the newest 2.x python interpreter.

This version of the BF2 CPython adaptation was created with a fork of the 2.7.12 stable tag and passed all the tests that 2.7.12 passes except for a couple of asserts that expect py2.7.12's sizes from python objects. Multiprocessing and threading works and you can add any external library that supports 2.7.12.


##Building and replacing libdice_py

###Adding external modules as builtins
Some of the modules in `Modules/` are compiled as external libs - .pyd files. For some reason, which requires further investigation, BF2 crashes when trying to import .pyd files in windows so any external modules that you would like to include should be added as builtin modules in the python .dll.

######Windows
To include modules in windows you'll need to add their init function to `PC\config.c` and add their path to the `pythoncore.vcxproj`+`pythoncore.vcxproj.filter` files. I have already done this in the `bf2-2.7` branch for the `_socket` module since it's required by `socket.py` that bf2 uses, take a look at [c364cee](https://github.com/yossizap/BF2-CPython/commit/c364cee33a30164fc8a5f436a496279f6881734d) and [ddcf99e](https://github.com/yossizap/BF2-CPython/commit/ddcf99e3b8ea419330c8a2b690d13744abdcf633) as examples. You can also do the opposite to remove builtin modules that you don't need. 

Once you're done building, run python.exe from the build folder to see if the modules that you added are accessible by executing the command `sys.builtin_module_names`.

######Linux
It's not necessary to compile all the libraries into the linux .so since they can be dynamically loaded but it's possible. 
To add existing modules simply find them in Modules/Setup.dist and uncomment their line.
To add your own module all you have to do it is place it in Modules/, add it to the Setup.dist file in the `[module name] [sources] [compilation flags]` format. Alternatively, if you don't want to use additional compilation flags, you can just place your module in Modules/ and have the init function's name match init[module_name] e.g. initthread for threadmodule.c, initbf2core for bf2core.c. One of the building scripts will automatically detect this add the module into your final python .so.


###Building
######Linux: 

    make distclean
    ./configure --enable-shared
    make
    cp libpython2.7.so {BF2 SERVER DIR}/bin/{ARCH}/libdice_py.so

######Windows: 
Follow the instructions in `PCbuild/readme.txt` for the pre-requisites. I have only managed to get the dll to work with the VS8+VS10 combination. Make sure that you have perl and svn and that you are using the VS8 toolchain with .NET4.
    
    cd PCbuild
    get_externals.bat
    build.bat --no-ssl --no-tkinter --no-bsddb -c Release -p {Win32/x64}
    cp python27.dll {BF2_FOLDER}/dice_py.dll
    
You don't have to disable ssl, tkinter and bsddb but I thought that they aren't necessary for a minimal build

###Configuration:

######Linux:

Edit {BF2 SERVER DIR}/python/bf2/`__init__.py`:

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

    sys.path = ['pylib-2.7.12.zip', 'python', 'admin', '%s' % bf2.gameLogic.getModDir(), 'bin/{ARCH}/pylib/]

*Note*: to use the zip you will need to uncomment line 467 in Module/Setup.dist to enable zlib compilation and go over the build process again.

######Windows:

Edit {BF2 DIR}/python/bf2/`__init__.py`:

after the lines:

    import host
    import sys

add: 

    sys.path = ['pylib-2.7.12.zip', 'python', '%s' % bf2.gameLogic.getModDir(), 'admin'}
    
Select the libs that you want to use and zip them in pylib-2.7.12.zip using your preferred win archive utility. You don't need to re-compile the .dll with zlib to use the .zip since the windows version comes with zlib as a builtin.


##Notes
If you have any .pyc files in your mod directory they won't work with the new interpreter unless they were re-compiled with the version of python that matches this interpreter's version.

See README.orig for the original CPython README.

You can take a look at the new features since 2.3.4 at the [python manual](https://docs.python.org/2.7/whatsnew/index.html).


##Credits
[pavelhoral](github.com/Pavelhoral) for fixing the incompatibility of py2.7's pyobject struct with py2.3 object struct.
