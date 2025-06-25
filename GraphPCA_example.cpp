// GraphPCA.cpp
// C++ implementation of GraphPCA using Eigen
// Requires Eigen3 (install via your package manager or from https://eigen.tuxfamily.org/)

#include <Eigen/Core>
#include <Eigen/Sparse>
#include <Eigen/SparseLU>
#include <Eigen/Eigenvalues>
#include <iostream>

using namespace Eigen;
using Matrix = MatrixXd;
using SparseMatrixD = SparseMatrix<double>;

class GraphPCA {
public:
	// lambda: graph regularization strength, k: number of components
	GraphPCA(double lambda = 0.3, int k = 10)
	: lambda_(lambda), k_(k) {}
	
	// X: gene expression (n_spots x m_genes), A: sparse adjacency (n_spots x n_spots)
	void fit(const Matrix& X, const SparseMatrixD& A) {
		int n = A.rows();
		// Build Laplacian L = D - A
		VectorXd deg = A * VectorXd::Ones(n);
		SparseMatrixD D(n,n);
		for(int i=0;i<n;++i) D.insert(i,i) = deg(i);
		SparseMatrixD L = D - A;
		
		// K = I + lambda * L
		SparseMatrixD I(n,n);
		I.setIdentity();
		SparseMatrixD K = I + lambda_ * L;
		
		// Compute K^{-1} via SparseLU
		SparseLU<SparseMatrixD> solver;
		solver.analyzePattern(K);
		solver.factorize(K);
		
		// Smooth X: X_tilde = K^{-1} * X
		Matrix X_tilde(n, X.cols());
		for(int j=0;j<X.cols();++j) X_tilde.col(j) = solver.solve(X.col(j));
		
		// Covariance M = X_tilde^T * X_tilde
		Matrix M = X_tilde.transpose() * X_tilde;
		
		// Eigen-decomposition
		SelfAdjointEigenSolver<Matrix> eig(M);
		// Eigenvalues/vectors are in increasing order
		Matrix eigvec = eig.eigenvectors();
		// Select top k columns (last k)
		W_ = eigvec.rightCols(k_);
		Z_ = X_tilde * W_;
	}
	
	// Return embeddings Z and components W
	const Matrix& embedding() const { return Z_; }
	const Matrix& components() const { return W_; }
	
private:
	double lambda_;
	int k_;
	Matrix Z_, W_;
};

int main() {
	// Example usage
	// Compile with: g++ -std=c++17 GraphPCA.cpp -I /path/to/eigen -O3
	int n = 100, m = 500;
	Matrix X = Matrix::Random(n, m);
	SparseMatrixD A(n,n);
	// Construct simple chain adjacency (replace with actual kNN graph)
	for(int i=0;i<n-1;++i) {
		A.insert(i,i+1)=1;
		A.insert(i+1,i)=1;
	}
	
	GraphPCA gpca(0.3, 10);
	gpca.fit(X, A);
	
	Matrix Z = gpca.embedding();
	std::cout << "Embedding: " << Z.rows() << "x" << Z.cols() << std::endl;
	return 0;
}

