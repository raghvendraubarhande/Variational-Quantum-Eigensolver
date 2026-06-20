#include <iostream>
#include <vector>
#include <complex>
#include <cmath> // Required for std::sqrt

using Complex = std::complex<double>;
using StateVector = std::vector<Complex>;
using Matrix = std::vector<std::vector<Complex>>;

// Function to dynamically generate an Ry gate based on an input angle
Matrix getRyGate(double theta) {
    // 1. Calculate the half-angle once to save CPU cycles
    double half_theta = theta / 2.0;

    // 2. Pre-compute the sine and cosine
    double cos_val = std::cos(half_theta);
    double sin_val = std::sin(half_theta);

    // 3. Construct and return the 2x2 unitary matrix
    return {
        {Complex(cos_val, 0.0),  Complex(-sin_val, 0.0)},
        {Complex(sin_val, 0.0),  Complex(cos_val, 0.0)}
    };
};
// Applies a 2x2 unitary matrix to a 1-qubit state vector
void applyGate(StateVector& state, const Matrix& gate) {
    
    // 1. Create a temporary vector initialized to zero
    // This is critical. We cannot overwrite the original state while we are still calculating with it.
    StateVector new_state(state.size(), Complex(0.0, 0.0));

    // 2. The Matrix-Vector Multiplication Loop
    // Loop through each row of the gate matrix
    for (int row = 0; row < 2; ++row) {
        // Loop through each column of the gate matrix
        for (int col = 0; col < 2; ++col) {
            // new_state[row] += U[row][col] * original_state[col]
            new_state[row] += gate[row][col] * state[col];
        }
    }

    // 3. Update the physical state
    // Replace the old vector with the newly calculated quantum state
    state = new_state;
};
// Calculates the inner product <bra | ket>
Complex innerProduct(const StateVector& bra, const StateVector& ket) {
    Complex result(0.0, 0.0);
    for (size_t i = 0; i < bra.size(); ++i) {
        // Multiply the complex conjugate of the Bra element by the Ket element
        result += std::conj(bra[i]) * ket[i];
    }
    return result;
};
// Calculates <psi | H | psi>
double getExpectationValue(const StateVector& psi, const Matrix& hamiltonian) {
    // 1. Create a copy of psi so we don't destroy the original state
    StateVector temp_ket = psi;
    
    // 2. Calculate H | psi> (This updates temp_ket)
    applyGate(temp_ket, hamiltonian);
    
    // 3. Calculate <psi | temp_ket>
    Complex energy = innerProduct(psi, temp_ket);
    
    // 4. Return just the real part, as physical energy must be real
    return energy.real();
};
int main(){
    int num_qubits = 1;
int state_size = 1 << num_qubits;

StateVector psi(state_size, Complex(0.0, 0.0));

psi[0] = Complex(1.0, 0.0);
psi[1] = Complex(0.0, 0.0);

// --- 1. The Pauli-X Gate (NOT Gate) ---
// Math: [[0, 1], [1, 0]]
Matrix pauli_X = {
    {Complex(0.0, 0.0), Complex(1.0, 0.0)},
    {Complex(1.0, 0.0), Complex(0.0, 0.0)}
};

// --- 2. The Pauli-Y Gate ---
// Math: [[0, -i], [i, 0]]
Matrix pauli_Y = {
    {Complex(0.0, 0.0), Complex(0.0, -1.0)}, // Notice the -1.0 in the imaginary slot
    {Complex(0.0, 1.0), Complex(0.0,  0.0)}
};

// --- 3. The Pauli-Z Gate (Phase Flip) ---
// Math: [[1, 0], [0, -1]]
Matrix pauli_Z = {
    {Complex(1.0, 0.0),  Complex(0.0, 0.0)},
    {Complex(0.0, 0.0), Complex(-1.0, 0.0)}
};

// --- 4. The Hadamard Gate (Superposition) ---
// Math: 1/sqrt(2) * [[1, 1], [1, -1]]
double inv_sqrt2 = 1.0 / std::sqrt(2.0);
Matrix hadamard = {
    {Complex(inv_sqrt2, 0.0),  Complex(inv_sqrt2, 0.0)},
    {Complex(inv_sqrt2, 0.0), Complex(-inv_sqrt2, 0.0)}
};

// --- THE PYTHON FFI BRIDGE ---

// Define your static Pauli-Z Hamiltonian globally or locally inside the wrapper
// Matrix pauli_Z = {
//     {Complex(1.0, 0.0),  Complex(0.0, 0.0)},
//     {Complex(0.0, 0.0), Complex(-1.0, 0.0)}
// };
}

// Wrap the export function in extern "C"
// --- THE PYTHON FFI BRIDGE ---

extern "C" {
    // Add __declspec(dllexport) right before the return type
    __declspec(dllexport) double simulate_vqe_step(double theta) {
        
        Matrix pauli_Z = {
            {Complex(1.0, 0.0),  Complex(0.0, 0.0)},
            {Complex(0.0, 0.0), Complex(-1.0, 0.0)}
        };

        StateVector psi(2, Complex(0.0, 0.0));
        psi[0] = Complex(1.0, 0.0);

        Matrix ry_gate = getRyGate(theta);
        applyGate(psi, ry_gate);

        return getExpectationValue(psi, pauli_Z);
    }
};


// 3. Apply the Gate and Print
    // Call the function on its own line, passing the full 'psi' vector
//     applyGate(psi, getRyGate(M_PI / 4.0)); 
    
//     // Print the elements of the resulting vector
//     std::cout << "State after Ry(pi/4):" << std::endl;
//     std::cout << "Amplitude |0>: " << psi[0] << std::endl;
//     std::cout << "Amplitude |1>: " << psi[1] << std::endl;
//     std::cout << "Expected Energy <psi|H|psi>: " << getExpectationValue(psi, pauli_Z) << std::endl;
// }