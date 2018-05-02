#pragma once

#include "Matrix.h"

template <typename T>
class SymMatrix : virtual public Matrix<T>
{
  public:
    // Desc: Instantiates a new matrix of the given size
    // Pre: Size must be > 0
    // Post: A new matrix will be created of the given dimension
    SymMatrix(const long size = 1);
    // Desc: Copies the given matrix into a new instance of the class
    // Pre: None
    // Post: A new matrix will be created will the same values of
    // the given parameter
    SymMatrix(const SymMatrix<T>& copy);
    // Desc: Copies the given matrix into a new instance of the class
    // Pre: None
    // Post: A new matrix will be created will the same values of
    // the given parameter
    SymMatrix(const Matrix<T> * copy);
    // Desc: Default destructor
    // Pre: None
    // Post: The matrix will be deleted
    ~SymMatrix();

    // Desc: Sets the value used to evaluate zero
    // Pre: The tol parameter must be >= 0
    // Post: The zero tolerance member will be set
    // to the tol parameters
    virtual void SetTolerance(const T tol);
    // Desc: Returns the value used to evaluate zero
    // Pre: None
    // Post: The zero value will be returned
    virtual T GetTolerance() const;
    // Desc: Returns the dimension of the matrix
    // Pre: None
    // Post: The dimension of the matrix is returned
     virtual T GetSize() const;

    // Desc: Transposes the matrix
    // Pre: None
    // Post: Transforms the object into the transpose of itself
    virtual Matrix<T>& Transpose();
    // Desc: Checks for diagonal dominance of the matrix
    // Pre: None
    // Post: Returns a boolean for diagonal dominance
    virtual bool IsDiagDom() const;

    // Desc: Returns the value in the requested matrix index
    // Pre: Parameters row and col must be between 0 and the size of the matrix
    // Post: The value of the matrix in that position will be returned
    virtual T operator()(const long row, const long col) const;
    // Desc: Sets a value in the matrix
    // Pre: Parameters row and col must be between 0 and the size of the matrix
    // Post: The value of the matrix will be set to the supplied value
    virtual Matrix<T>& operator()(const long row, const long col, const T val);
    // Desc: Creates a new matrix with the values of the calling object
    // multiplied by the rhs parameter
    // Pre: The type T must have + and * defined for it
    // Post: A smart pointer to the newly created matrix will be returned
    // that contains the values of the calling object multiplied by rhs
    virtual unique_ptr<Matrix<T>> operator*(const T rhs) const;
    // Desc: Creates a new vector with the values of the calling object
    // multiplied by the rhs vector
    // Pre: The rhs parameter must be of same size as object
    // and the type T must have + and * defined for it
    // Post: A new vector will be returned that contains the values of
    // the calling object multiplied by rhs
    virtual Vector<T> operator*(const Vector<T>& rhs) const;
    // Desc: Creates a new matrix with the values of the calling object
    // multiplied by the rhs matrix
    // Pre: The rhs parameter must be of same size as object
    // and the type T must have + and * defined for it
    // Post: A smart pointer to the newly created matrix will be returned
    // that contains the values of the calling object multiplied by rhs
    virtual unique_ptr<Matrix<T>> operator*(const Matrix<T>& rhs) const;
    // Desc: Creates a new matrix with the values of the calling object
    // plus the rhs parameter
    // Pre: The type T must have - defined for it
    // Post: A smart pointer to the newly created matrix will be returned
    // that contains the values of the calling object plus rhs
    virtual unique_ptr<Matrix<T>> operator+(const T rhs) const;
    // Desc: Creates a new matrix with the values of the calling object
    // plus the rhs matrix
    // Pre: The rhs parameter must be of same size as object
    // and the type T must have + defined for it
    // Post: A smart pointer to the newly created matrix will be returned
    // that contains the values of the calling object plus rhs
    virtual unique_ptr<Matrix<T>> operator+(const Matrix<T>& rhs) const;
    // Desc: Adds the parameter type to itself
    // Pre: The type T must have + defined for it
    // Post: Each value of the matrix will have the rhs
    // parameter added to it
    virtual Matrix<T>& operator+=(const T rhs);
    // Desc: Adds the parameter matrix to itself
    // Pre: The rhs parameter must be of same size as object
    // and the T type must have + defined for it
    // Post: The object will perform matrix addition and
    // take on new values
    virtual Matrix<T>& operator+=(const Matrix<T>& rhs);
    // Desc: Creates a new matrix with the values of the calling object
    // minus the rhs parameter
    // Pre: The type T must have - defined for it
    // Post: A smart pointer to the newly created matrix will be returned
    // that contains the values of the calling object minus rhs
    virtual unique_ptr<Matrix<T>> operator-(const T rhs) const;
    // Desc: Creates a new matrix with the values of the calling object
    // minus the rhs matrix
    // Pre: The rhs parameter must be of same size as object
    // and the type T must have - defined for it
    // Post: A smart pointer to the newly created matrix will be returned
    // that contains the values of the calling object minus rhs
    virtual unique_ptr<Matrix<T>> operator-(const Matrix<T>& rhs) const;
    // Desc: Subtracts the parameter type from itself
    // Pre: The type T must have - defined for it
    // Post: Each value of the matrix will have the rhs
    // parameter subtracted from it
    virtual Matrix<T>& operator-=(const T rhs);
    // Desc: Subtracts the parameter matrix from itself
    // Pre: The rhs parameter must be of same size as object
    // and the T type must have - defined for it
    // Post: The object will perform matrix subtraction and
    // take on new values
    virtual Matrix<T>& operator-=(const Matrix<T>& rhs);
    // Desc: Assigns the value of the parameter to itself
    // Pre: The rhs parameter must be of same size as object
    // Post: The matrix will take on the values of the rhs parameter
    virtual Matrix<T>& operator=(const Matrix<T>& rhs);
    // Desc: Assigns the value of the parameter to itself
    // Pre: The rhs parameter must be of same size as object
    // Post: The matrix will take on the values of the rhs parameter
    virtual SymMatrix<T>& operator=(const SymMatrix<T>& rhs);

  private:
    // the dimension of the matrix
    long m_size;
    // the data values of the Matrix
    Vector<T> * m_data;
    // the zero tolerance of the Matrix
    // i.e. when a value "is close enough" to 0
    T m_zero;
};

#include "SymMatrix.hpp"