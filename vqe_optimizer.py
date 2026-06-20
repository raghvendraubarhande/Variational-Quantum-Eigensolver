import ctypes
import os
import numpy as np
from scipy.optimize import minimize
import matplotlib.pyplot as plt

# --- 1. Load the C++ Engine ---
dll_name = 'vqe_engine.dll'
dll_path = os.path.join(os.path.abspath('.'), dll_name)
vqe_engine = ctypes.CDLL(dll_path)

vqe_engine.simulate_vqe_step.argtypes = [ctypes.c_double]
vqe_engine.simulate_vqe_step.restype = ctypes.c_double

# --- 2. Data Tracking ---
# This list will store the energy value at every single step
energy_history = []

# --- 3. Define the Cost Function ---
def cost_function(params):
    theta = params[0]
    
    # Calculate energy via C++
    energy = vqe_engine.simulate_vqe_step(theta)
    
    # Store the energy for our graph
    energy_history.append(energy)
    
    print(f"Angle: {theta:>8.4f} rad | Energy: {energy:>8.4f}")
    return energy

# --- 4. Execute the VQE Loop ---
if __name__ == "__main__":
    print("--- Starting VQE Ground State Search ---")
    
    # Let's start from a slightly worse guess to make the graph look more dramatic
    initial_guess = [0.5] 
    
    result = minimize(
        cost_function, 
        initial_guess, 
        method='COBYLA',
        options={'disp': False, 'tol': 1e-6}
    )
    
    print("\n--- Optimization Complete ---")
    print(f"Final Ground State Energy : {result.fun:.6f}")
    print(f"Optimal Rotation Angle    : {result.x[0]:.6f} radians")
    print(f"Total Iterations          : {result.nfev}")

    # --- 5. Generate the Convergence Graph ---
    plt.figure(figsize=(10, 6))
    
    # Plot the tracked energies
    plt.plot(energy_history, marker='o', linestyle='-', color='b', label='VQE Energy')
    
    # Draw a red dashed line at the theoretical minimum (-1.0)
    plt.axhline(y=-1.0, color='r', linestyle='--', label='Theoretical Minimum (-1.0)')
    
    # Graph Formatting
    plt.title("VQE Optimization Convergence (1-Qubit System)")
    plt.xlabel("Optimizer Iteration")
    plt.ylabel("Expectation Value <E>")
    plt.grid(True, linestyle=':', alpha=0.7)
    plt.legend()
    
    # Show the plot
    plt.tight_layout()
    plt.show()