#pragma once
#include <memory>
#include "Vector.h"

template <typename T>
class Matrix
{
  public:
    // Desc: Default destructor
    // Pre: None
    // Post: The matrix will be deleted
    virtual ~Matrix() {};

    // Desc: Sets the value used to evaluate zero
    // Pre: The tol parameter must be >= 0
    // Post: The zero tolerance member will be set
    // to the tol parameters
    virtual void SetTolerance(const T tol) = 0;
    // Desc: Returns the value used to evaluate zero
    // Pre: None
    // Post: The zero value will be returned
    virtual T GetTolerance() const = 0;
    // Desc: Returns the dimension of the matrix
    // Pre: None
    // Post: The dimension of the matrix is returned
     virtual T GetSize() const = 0;

    // Desc: Transposes the matrix
    // Pre: None
    // Post: Transforms the object into the transpose of itself
    virtual Matrix<T>& Transpose() = 0;
    // Desc: Checks for diagonal dominance of the matrix
    // Pre: None
    // Post: Returns a boolean for diagonal dominance
    virtual bool IsDiagDom() const = 0;

    // Desc: Returns the value in the requested matrix index
    // Pre: Parameters row and col must be between 0 and the size of the matrix
    // Post: The value of the matrix in that position will be returned
    virtual T operator()(const long row, const long col) const = 0;
    // Desc: Sets a value in the matrix
    // Pre: Parameters row and col must be between 0 and the size of the matrix
    // Post: The value of the matrix will be set to the supplied value
    virtual Matrix<T>& operator()(const long row, const long col, const T val) = 0;
    // Desc: Creates a new matrix with the values of the calling object
    // multiplied by the rhs parameter
    // Pre: The type T must have + and * defined for it
    // Post: A smart pointer to the newly created matrix will be returned
    // that contains the values of the calling object multiplied by rhs
    virtual unique_ptr<Matrix<T>> operator*(const T rhs) const = 0;
    // Desc: Creates a new vector with the values of the calling object
    // multiplied by the rhs vector
    // Pre: The rhs parameter must be of same size as object
    // and the type T must have + and * defined for it
    // Post: A new vector will be returned that contains the values of
    // the calling object multiplied by rhs
    virtual Vector<T> operator*(const Vector<T>& rhs) const = 0;
    // Desc: Creates a new matrix with the values of the calling object
    // multiplied by the rhs matrix
    // Pre: The rhs parameter must be of same size as object
    // and the type T must have + and * defined for it
    // Post: A smart pointer to the newly created matrix will be returned
    // that contains the values of the calling object multiplied by rhs
    virtual unique_ptr<Matrix<T>> operator*(const Matrix<T>& rhs) const = 0;
    // Desc: Creates a new matrix with the values of the calling object
    // plus the rhs parameter
    // Pre: The type T must have - defined for it
    // Post: A smart pointer to the newly created matrix will be returned
    // that contains the values of the calling object plus rhs
    virtual unique_ptr<Matrix<T>> operator+(const T rhs) const = 0;
    // Desc: Creates a new matrix with the values of the calling object
    // plus the rhs matrix
    // Pre: The rhs parameter must be of same size as object
    // and the type T must have + defined for it
    // Post: A smart pointer to the newly created matrix will be returned
    // that contains the values of the calling object plus rhs
    virtual unique_ptr<Matrix<T>> operator+(const Matrix<T>& rhs) const = 0;
    // Desc: Adds the parameter type to itself
    // Pre: The type T must have + defined for it
    // Post: Each value of the matrix will have the rhs
    // parameter added to it
    virtual Matrix<T>& operator+=(const T rhs) = 0;
    // Desc: Adds the parameter matrix to itself
    // Pre: The rhs parameter must be of same size as object
    // and the T type must have + defined for it
    // Post: The object will perform matrix addition and
    // take on new values
    virtual Matrix<T>& operator+=(const Matrix<T>& rhs) = 0;
    // Desc: Creates a new matrix with the values of the calling object
    // minus the rhs parameter
    // Pre: The type T must have - defined for it
    // Post: A smart pointer to the newly created matrix will be returned
    // that contains the values of the calling object minus rhs
    virtual unique_ptr<Matrix<T>> operator-(const T rhs) const = 0;
    // Desc: Creates a new matrix with the values of the calling object
    // minus the rhs matrix
    // Pre: The rhs parameter must be of same size as object
    // and the type T must have - defined for it
    // Post: A smart pointer to the newly created matrix will be returned
    // that contains the values of the calling object minus rhs
    virtual unique_ptr<Matrix<T>> operator-(const Matrix<T>& rhs) const = 0;
    // Desc: Subtracts the parameter type from itself
    // Pre: The type T must have - defined for it
    // Post: Each value of the matrix will have the rhs
    // parameter subtracted from it
    virtual Matrix<T>& operator-=(const T rhs) = 0;
    // Desc: Subtracts the parameter matrix from itself
    // Pre: The rhs parameter must be of same size as object
    // and the T type must have - defined for it
    // Post: The object will perform matrix subtraction and
    // take on new values
    virtual Matrix<T>& operator-=(const Matrix<T>& rhs) = 0;
    // Desc: Assigns the value of the parameter to itself
    // Pre: The rhs parameter must be of same size as object
    // Post: The matrix will take on the values of the rhs parameter
    virtual Matrix<T>& operator=(const Matrix<T>& rhs) = 0;


    // Desc: formats and outputs the calling Matrix to the passed ostream
    // Pre: None
    // Post: The passed in parameter m will be formatted and output to
    // the ostream. Any elements in m less than m's zero tolerance
    // will output the value of 0 instead
    friend ostream& operator<<(ostream& os, const Matrix<T>& m)
    {
      // for each element in v
      for (long i = 0; i < m.GetSize(); i++)
      {
        // write in equation form
        os << "[";
        for (long j = 0; j < m.GetSize(); j++)
        {
          // if the absolute value of the element is
          // less than the tolerance, output 0 instead
          // of the actual value
          if (fabs(m(i, j)) < m.GetTolerance())
            os << "0";
          else
            os << setprecision(10) << m(i, j);
          if (j < m.GetSize() - 1)
            os << ", ";
        }
        os << "]" << endl;
      }
      return os;
    }
   
    // Desc: Reads in Matrix elements from the ifstream
    // Pre: The is parameter must be opened and the parameter
    // m must have it's size defined
    // Post: The elements of m will have their values read in
    // from the ifstream is
    friend ifstream& operator>>(ifstream& is, Matrix<T>& m)
    {
      // for (long i = 0; i < m.m_size; i++)
      //   is >> m.m_data[i];
      for (long i = 0; i < m.GetSize(); i++)
      {
        for (long j = 0; j < m.GetSize(); j++)
        {
          T value;
          is >> value;
          m(i, j, value);
        }
      }
      return is;
    }
};

//#include "Matrix.hpp"