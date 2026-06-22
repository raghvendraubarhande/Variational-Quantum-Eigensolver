import ctypes
import os
import numpy as np

# -- 1. Load the C++ Shared Library --
dll_name = 'vqe_engine.dll' 
dll_path = os.path.join(os.path.abspath('.'), dll_name)

print(f"Loading engine from: {dll_path}")
vqe_engine = ctypes.CDLL(dll_path, winmode=0)

# -- 2. Configure the Function Interface --
# the C++ function: double simulate_vqe_step(double theta)

vqe_engine.simulate_vqe_step.argtypes = [ctypes.c_double] # Input is a double
vqe_engine.simulate_vqe_step.restype = ctypes.c_double    # Output is a double

# -- 3. Test the Bridge --
if __name__ == "__main__":
    theta = np.pi / 4  

    energy = vqe_engine.simulate_vqe_step(theta)

    print("\n--- FFI Bridge Successful ---")
    print(f"Input Angle (theta) : {theta}")
    print(f"C++ Engine Energy   : {energy}")