description: Can you crack them all? (Flag is not in RITSEC{} format. Once solved, flag must be wrapped in RITSEC{})

solution: RITSEC{automated_analysis_saves_the_day_phantom_approves}
500+ points
I'm sure there are many solutions. Here is one that uses symbolic execution:

import angr
import claripy

project = angr.Project("biodiversity/0", auto_load_libs=True)

project.hook(0x400670, angr.SIM_PROCEDURES['libc']['strlen']())
project.hook(0x400690, angr.SIM_PROCEDURES['libc']['memcmp']())

def length(state):
    state.regs.rax = 32

project.hook(0x40080c, length, length=5)

arg1 = claripy.BVS('arg1', 32*8)
initial_state = project.factory.entry_state(args=["biodiversity/0", arg1], add_options={"BYPASS_UNSUPPORTED_SYSCALL"})

for byte in arg1.chop(8):
        initial_state.add_constraints(byte != '\x00') # null
        initial_state.add_constraints(byte >= ' ') # '\x20'
        initial_state.add_constraints(byte <= '~') # '\x7e'

sm = project.factory.simulation_manager(initial_state)

sm.explore(find=0x400903)
print(sm.found[0].solver.eval(arg1, cast_to=bytes))
