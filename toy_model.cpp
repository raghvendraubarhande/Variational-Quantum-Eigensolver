#include <iostream>
#include <vector>
#include <complex>

// --- Core Data Structure Aliases ---
using Complex = std::complex<double>;
using StateVector = std::vector<Complex>;

int main() {
    // 1. Define the physical system size
    int num_qubits = 1;
    
    // 2. Calculate vector size using bitwise shift (2^n)
    int state_size = 1 << num_qubits; 

    // 3. Allocate the memory and initialize everything to exactly 0.0 + 0.0i
    StateVector psi(state_size, Complex(0.0, 0.0));

    // 4. Initialize the ground state |0>
    // mathematically: [1, 0]^T
    psi[0] = Complex(1.0, 0.0); // alpha amplitude
    psi[1] = Complex(0.0, 0.0); // beta amplitude

    // -- Verification --
    std::cout << "Quantum Simulator Initialized." << std::endl;
    std::cout << "Qubits: " << num_qubits << std::endl;
    std::cout << "State Vector Size: " << psi.size() << " elements" << std::endl;
    std::cout << "State |psi> = [" << psi[0] << ", " << psi[1] << "]" << std::endl;

    return 0;
}