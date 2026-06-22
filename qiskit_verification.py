from qiskit import QuantumCircuit
from qiskit.quantum_info import Statevector, SparsePauliOp
import numpy as np

# 1. Define the exact same angle you used to test your C++ engine
theta = np.pi / 4  # 45 degrees

# 2. Build a 1-qubit quantum circuit and apply the Ry gate
qc = QuantumCircuit(1)
qc.ry(theta, 0)

# 3. Extract the perfect mathematical state vector (Your Day 1 C++ architecture)
state = Statevector(qc)

# 4. Define the Hamiltonian Observable (Your Day 2 C++ Pauli-Z Matrix)
observable = SparsePauliOp("Z")

# 5. Calculate <psi | Z | psi> (Your Day 5 C++ Engine)
expectation_value = state.expectation_value(observable)

print(f"--- Qiskit Baseline Validation ---")
print(f"Rotation Angle (theta): {theta} radians")
print(f"State Amplitudes: {state.data}")
print(f"Expectation Value <Z>: {expectation_value.real}")