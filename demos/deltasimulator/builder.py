from os import mkdir, path
import shutil
import traceback

from examples.rabi_demo import exp_graph as rabi_demo
from deltalanguage.runtime import serialize_graph

from deltasimulator.build_tools import BuildArtifact, write
from deltasimulator.build_tools.environments import (CPPEnv,
                                                   HostEnv,
                                                   PythonatorEnv,
                                                   VerilatorEnv,
                                                   WiringEnv)
from deltasimulator.lib import full_build

if __name__ == "__main__":

    build_repo = "/workdir/build"
    program_name = "rabi_demo"
    sc_hal_hpp_file = "sc_hal_block.hpp"
    projectq_hal_hpp_file = "projectq_hal.hpp"
    main_cpp_file = "main.cpp"
    hal_py_file = "projectq_hal.py"

    try:
        shutil.copy(
            "/workdir/demos/deltasimulator/" + main_cpp_file,
            build_repo + "/" + main_cpp_file
        )

        shutil.copy(
            "/workdir/demos/common/" + sc_hal_hpp_file,
            build_repo + "/" + sc_hal_hpp_file
        )

        shutil.copy(
            "/workdir/demos/common/"  + projectq_hal_hpp_file,
            build_repo + "/" + projectq_hal_hpp_file
        )

        shutil.copy(
            "/workdir/demos/common/" + hal_py_file,
            build_repo + "/" + hal_py_file
        )

    except FileExistsError:
        pass

    graph, _ = rabi_demo.get_graph()
    dotdf_bytes, program = serialize_graph(graph, name=program_name)
    full_build(program, main_cpp=path.join(path.dirname(__file__), "main.cpp"), build_dir='/workdir/build')

