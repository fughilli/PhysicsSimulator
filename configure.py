#!/usr/bin/python

from ninja_syntax import Writer
import os, sys

source_dirs = [
        ".",
        "./VectorLib",
        "./Engines",
        "./Colliders",
        "./Renderers",
        "./Constraints",
        "./Util"
        ]

include_dirs = [
        "/usr/local/include/SDL2",
        "./VectorLib",
        "./Engines",
        "./Colliders",
        "./Renderers",
        "./Constraints",
        "./Util"
        ]

libraries = [
        "/usr/local/lib/libSDL2.a",
        "/usr/local/lib/libSDL2main.a",
        "/usr/lib/x86_64-linux-gnu/libSDL2_image.a"
        ]

defines = [
        ]

def subst_ext(fname, ext):
    return os.path.splitext(fname)[0] + ext

def get_sources():
    fnames = []
    for d in source_dirs:
        for f in os.listdir(d):
            fnames.append(os.path.join(d, f))
    return fnames

def get_includes():
    return " ".join(map(lambda x : "-I"+x, source_dirs + include_dirs))

def get_libs():
    return " ".join(libraries)
    #return "-Wl,-rpath," + (":".join(library_dirs))

def get_defines():
    return " ".join(map(lambda x : "-D"+x, defines))

with open("build.ninja", "w") as buildfile:
    n = Writer(buildfile)

    # Variable declarations
    n.variable("cxxflags", "-g -Wall -std=c++0x " + get_includes() + " " + get_defines())
    n.variable("cflags", "-g -Wall -std=c99 " + get_includes() + " " +  get_defines())
    n.variable("lflags", " -lm -lstdc++ -lc -lpthread -ldl -ltiff -lpng -ljpeg -lwebp")
    n.variable("libs", get_libs())

    # Rule declarations
    n.rule("cxx",
           command = "g++ $cxxflags -c $in -o $out")

    n.rule("cc",
           command = "gcc $cflags -c $in -o $out")

    n.rule("cl",
           command = "gcc -o $out $in $libs $lflags")

    n.rule("ocb",
           command = "objcopy -O binary $in $out")

    n.rule("cdb",
           command = "ninja -t compdb cc cxx > compile_commands.json")

    n.rule("cscf",
            command = "find " + " ".join(set(source_dirs + include_dirs)) + " -regex \".*\\(\\.c\\|\\.h\\|.cpp\\|.hpp\\)$$\" -and -not -type d > $out")

    n.rule("cscdb",
           command = "cscope -bq")

    # Build rules
    n.build("compile_commands.json", "cdb")
    n.build("cscope.files", "cscf")
    n.build(["cscope.in.out", "cscope.po.out", "cscope.out"], "cscdb", "cscope.files")

    objects = []

    def cc(name):
        ofile = subst_ext(name, ".o")
        n.build(ofile, "cc", name)
        objects.append(ofile)
    def cxx(name):
        ofile = subst_ext(name, ".o")
        n.build(ofile, "cxx", name)
        objects.append(ofile)
    def cl(oname, ofiles):
        n.build(oname, "cl", ofiles)

    sources = get_sources()
    map(cc, filter(lambda x : x.endswith(".c"), sources))
    map(cxx, filter(lambda x : x.endswith(".cpp"), sources))

    cl("main.elf", objects)

    n.build("main.bin", "ocb", "main.elf")

