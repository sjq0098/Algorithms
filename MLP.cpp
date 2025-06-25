// Simple Multilayer Perceptron implementation in C++
// Supports one hidden layer, sigmoid activation, mean squared error loss

#include <iostream>
#include <vector>
#include <cmath>
#include <random>

using Vector = std::vector<double>;
using Matrix = std::vector<Vector>;

// Sigmoid activation and its derivative
inline double sigmoid(double x) {
	return 1.0 / (1.0 + std::exp(-x));
}
inline double sigmoid_derivative(double y) {
	// y = sigmoid(x)
	return y * (1.0 - y);
}

// Initialize matrix with random values in [-1,1]
void init_matrix(Matrix &m, int rows, int cols) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(-1.0, 1.0);
	m.assign(rows, Vector(cols));
	for (int i = 0; i < rows; ++i)
		for (int j = 0; j < cols; ++j)
			m[i][j] = dis(gen);
}

// Dot product: vector x matrix
Vector dot(const Vector &v, const Matrix &m) {
	int rows = m.size();
	int cols = m[0].size();
	Vector result(cols, 0.0);
	for (int j = 0; j < cols; ++j)
		for (int i = 0; i < rows; ++i)
			result[j] += v[i] * m[i][j];
	return result;
}

// Add bias: append 1.0 to vector
Vector add_bias(const Vector &v) {
	Vector res = v;
	res.push_back(1.0);
	return res;
}

int main() {
	// XOR dataset
	std::vector<Vector> inputs = {{0,0}, {0,1}, {1,0}, {1,1}};
	std::vector<Vector> targets = {{0}, {1}, {1}, {0}};
	
	int input_dim = 2;
	int hidden_dim = 2;
	int output_dim = 1;
	double lr = 0.5;
	int epochs = 10000;
	
	// Weights: input->hidden and hidden->output (with bias)
	Matrix W1, W2;
	init_matrix(W1, input_dim+1, hidden_dim); // +1 for bias
	init_matrix(W2, hidden_dim+1, output_dim);
	
	// Training loop
	for (int e = 0; e < epochs; ++e) {
		double loss = 0.0;
		for (size_t idx = 0; idx < inputs.size(); ++idx) {
			// Forward pass
			Vector x_b = add_bias(inputs[idx]); // add bias
			Vector h_raw = dot(x_b, W1);
			Vector h(h_raw.size());
			for (size_t i = 0; i < h_raw.size(); ++i)
				h[i] = sigmoid(h_raw[i]);
			Vector h_b = add_bias(h);
			
			Vector o_raw = dot(h_b, W2);
			Vector o(o_raw.size());
			for (size_t i = 0; i < o_raw.size(); ++i)
				o[i] = sigmoid(o_raw[i]);
			
			// Compute error and loss
			Vector err(output_dim);
			for (int k = 0; k < output_dim; ++k) {
				err[k] = targets[idx][k] - o[k];
				loss += err[k] * err[k] * 0.5;
			}
			
			// Backward pass
			// Output layer delta
			Vector delta_o(output_dim);
			for (int k = 0; k < output_dim; ++k)
				delta_o[k] = err[k] * sigmoid_derivative(o[k]);
			
			// Hidden layer delta (excluding bias)
			Vector delta_h(hidden_dim, 0.0);
			for (int i = 0; i < hidden_dim; ++i) {
				double sum = 0.0;
				for (int k = 0; k < output_dim; ++k)
					sum += delta_o[k] * W2[i][k];
				delta_h[i] = sum * sigmoid_derivative(h[i]);
			}
			
			// Update W2
			for (int i = 0; i < hidden_dim+1; ++i)
				for (int k = 0; k < output_dim; ++k)
					W2[i][k] += lr * delta_o[k] * (i == hidden_dim ? 1.0 : h[i]);
			
			// Update W1
			for (int j = 0; j < input_dim+1; ++j)
				for (int i = 0; i < hidden_dim; ++i)
					W1[j][i] += lr * delta_h[i] * (j == input_dim ? 1.0 : inputs[idx][j]);
		}
		
		if (e % 1000 == 0)
			std::cout << "Epoch " << e << ", Loss: " << loss << std::endl;
	}
	
	// Validation on XOR
	std::cout << "\nValidation:" << std::endl;
	for (size_t idx = 0; idx < inputs.size(); ++idx) {
		Vector x_b = add_bias(inputs[idx]);
		Vector h_raw = dot(x_b, W1);
		Vector h(h_raw.size());
		for (size_t i = 0; i < h_raw.size(); ++i)
			h[i] = sigmoid(h_raw[i]);
		Vector h_b = add_bias(h);
		
		Vector o_raw = dot(h_b, W2);
		Vector o(o_raw.size());
		for (size_t i = 0; i < o_raw.size(); ++i)
			o[i] = sigmoid(o_raw[i]);
		
		std::cout << inputs[idx][0] << "," << inputs[idx][1]
		<< " -> " << o[0] << std::endl;
	}
	
	return 0;
}

