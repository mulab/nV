add_subdirectory(doc)

include_directories(${PYTHON_INCLUDE_DIRS})

new_shared_library (nvinpython
    nVinPython.cpp
    nVutils.cpp
    pythonObject.cpp
)
target_link_libraries(nvinpython nV nVKernel nVParser
    nVPolynomial nVString nVSystem ${PYTHON_LIBRARIES})

new_shared_library (pythoninnv
    PythoninnV.cpp
    pythonObject.cpp
    nVutils.cpp
    nVinPython.cpp
)
target_link_libraries(pythoninnv nvinpython nvkernel ${PYTHON_LIBRARIES})

install(FILES pythoninnv.n DESTINATION Link/Python)
install(FILES setup.py DESTINATION Link/Python)
