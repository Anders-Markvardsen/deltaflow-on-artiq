
import dill
#[[[cog
#    cog.outl(f"body = dill.loads({df_body})")
#    for port in top_p.inPorts:
#        cog.outl(f"{self.get_sysc_port_name(port)} = dill.loads({port.type})")
#    for port in top_p.outPorts:
#        cog.outl(f"{self.get_sysc_port_name(port)} = dill.loads({port.type})")
#]]]
body = dill.loads(b'\x80\x04\x95~\x01\x00\x00\x00\x00\x00\x00\x8c*deltaflow.wiring._node_classes.node_bodies\x94\x8c\nPyFuncBody\x94\x93\x94)\x81\x94}\x94(\x8c\x08callback\x94\x8c\ndill._dill\x94\x8c\x10_create_function\x94\x93\x94(h\x06\x8c\x0c_create_code\x94\x93\x94(K\x01K\x00K\x00K\x01K\x01KCC\x0c|\x00r\x08t\x00\x82\x01d\x00S\x00\x94N\x85\x94\x8c\x10DeltaRuntimeExit\x94\x85\x94\x8c\tcompleted\x94\x85\x94\x8c;/workdir/Deltaflow/examples/rabi_demo/deltaruntime_graph.py\x94\x8c\x12experiment_stopper\x94K\x14C\x04\x00\x02\x04\x01\x94))t\x94R\x94cDeltaflow.examples.rabi_demo.deltaruntime_graph\n__dict__\nh\x12NN}\x94Nt\x94R\x94\x8c\x0benvironment\x94Nub.')
sysc_completed = dill.loads(b'\x80\x04\x95k\x00\x00\x00\x00\x00\x00\x00\x8c!deltaflow.data_types._delta_types\x94\x8c\x05DBool\x94\x93\x94)\x81\x94}\x94\x8c\x04size\x94h\x00\x8c\x05DSize\x94\x93\x94)\x81\x94}\x94(\x8c\x0eis_placeholder\x94\x89\x8c\x03val\x94K\x08ubsb.')
#[[[end]]]
