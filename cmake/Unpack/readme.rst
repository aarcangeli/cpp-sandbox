Unpack
------------------

Generic archive unpacker.

Syntax:
    unpack(<target> <from> <to>)

Arguments:
    <target>        A unique name to identify the archive

    <from>          Location if archive

    <to>            Location of destionation directory

This command extract the content of an archive into the destionation directory

Example:

.. code-block:: cmake

    include(unpack)

    set(SDL2_PACKAGE "${MY_LIBS_DIRECTORY}/SDL2/SDL2-devel-2.0.8-mingw.tar.gz")
    set(SDL2_UNPACK_DIR ${CMAKE_BINARY_DIR})
    set(SDL2_BINARY_HOME ${SDL2_UNPACK_DIR}/SDL2-2.0.8)
    unpack(SDL2 ${SDL2_PACKAGE} ${SDL2_UNPACK_DIR})
