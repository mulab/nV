#!/usr/bin/python
from distutils.core import setup, Extension
from distutils.sysconfig import get_python_lib, get_python_version
import os

if os.path.isfile("MANIFEST"):
    os.unlink("MANIFEST")

# You may have to change these
PYLIBS = ["python"+get_python_version(), "pthread", "util"]
PYLIBDIR = [get_python_lib(standard_lib=True)+"/config"]
nVROOT = "D:\project\jiangshu\\nV\\trunk\\"
#nVROOT = "E:\\SVN\\10.32.164.32\\nV\\trunk\\"
nVLIBS = ["nV", "nVGraphics", "nVKernel", "nVNumber", "nVParser", "nVPolynomial", "nVString", "nVSystem"]
nVLIBDIR = [nVROOT + "Release", nVROOT + "Lib"]
nVInclude = [nVROOT + "Include"]

PythoninnVLIBS = ["nV", "nVGraphics", "nVKernel", "nVNumber", "nVParser", "nVPolynomial", "nVString", "nVSystem", "python27"]
PythoninnVLIBDIR = ["D:\project\jiangshu\\nV\\trunk\Release", "D:\project\jiangshu\\nV\\trunk\Lib", "D:\Python\libs"]

setup(name="nV-python",
      version = "1.0",
      description = "Two-way bridge between Python and nV",
      author = "jiang",
      author_email = "",
      url = "http://labix.org/lunatic-python",
      license = "LGPL",
      long_description =
"""\
Lunatic Python is a two-way bridge between Python and Lua, allowing these
languages to intercommunicate. Being two-way means that it allows Lua inside
Python, Python inside Lua, Lua inside Python inside Lua, Python inside Lua
inside Python, and so on.
""",
      ext_modules = [
                     Extension("nV",
                               ["pythonObject.cpp", "nVutils.cpp", "nVinPython.cpp"],
				    include_dirs=nVInclude,
                               library_dirs=nVLIBDIR,
                               libraries=nVLIBS,
                               extra_compile_args=["-rdynamic"],
                               extra_link_args=["-rdynamic"]),
                    ],
      )
