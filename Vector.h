#pragma once

#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "Exceptions.h"
using namespace std;

// Class for a Vector in R^n
template <typename T>
class Vector
{
  public:
    // Desc: default constructor, with the passed in dimension (default 1)
    // Pre: None
    // Post: an instantiated instance of the Vector class in R^1, or R^n if
    // size is passed in
    Vector(const long size = 1);
    // Desc: copy constructor
    // Pre: copy is a valid Vector
    // Post: an instantiated instance of the Vector class with the same
    // dimension and data values as the copy Vector
    Vector(const Vector<T>& copy);
    // Desc: move constructor
    // Pre: copy is a valid Vector
    // Post: an instantiated instance of the Vector class with the same
    // dimension and data values as the copy Vector. The copy vector will
    // its values nullified
    Vector(Vector<T>&& copy);
    // Desc: default destructor
    // Pre: None
    // Post: the data array will be deleted and the size set to 0
    ~Vector();
    // Desc: sets the zero tolerance for the Vector
    // Pre: the parameter tol must be positive
    // Post: the zero tolerance for the Vector will be set 
    // to the passed in value
    void SetTolerance(const T tol);
    // Desc: Returns the dimension of the vector
    // Pre: None
    // Post: the dimension of the vector is returned
    long GetSize() const;
    // Desc: returns the magnitude of the calling Vector
    // Pre: None
    // Post: the magnitude of the Vector will be calculated
    // and returned as the same type as the Vector type
    T mag() const;
    // Desc: returns the inner (dot) product of the calling
    // and passed in Vector
    // Pre: the calling and passed in Vector must have the
    // same dimensions and type
    // Post: the inner (dot) product of the two Vectors will
    // be calculated and returned as the same type as the 
    // Vector type
    T dot(const Vector<T>& v) const;
    // Desc: operator to access the element at the ith dimension
    // Pre: i must be a positive value, no larger than the
    // dimension - 1 of the calling Vector
    // Post: the element in the ith dimension of the Vector
    // will be returned
    T& operator[](const long i) const;
    // Desc: returns a new Vector with the value of
    // the calling Vector multiplied by a value d
    // Pre: the parameter d must be a type that
    // can be static casted to type T
    // Post: a new Vector will be returned that has
    // the value of the calling Vector multiplied by
    // d, casted to T
    template <typename Y>
    Vector<T> operator*(const Y& d) const;
    // Desc: returns a new Vector with the value of
    // the calling Vector divided by a value d
    // Pre: the parameter d must be a type that
    // can be divided with type T
    // Post: a new Vector will be returned that has
    // the value of the calling Vector divided by d
    template <typename Y>
    Vector<T> operator/(const Y& d) const;
    // Desc: operator to add two Vectors
    // Pre: the parameter v must have the same dimension as
    // the calling Vector
    // Post: a new Vector will be returned that has the
    // value of the calling vector added element-wise with
    // the passed in Vector v
    Vector<T> operator+(const Vector<T>& v) const;
    // Desc: operator to subtract two Vectors
    // Pre: the parameter v must have the same dimension as
    // the calling Vector
    // Post: a new Vector will be returned that has the
    // value of the calling vector subtracted element-wise with
    // the passed in Vector v
    Vector<T> operator-(const Vector<T>& v) const;
    // Desc: operator to assign two Vectors
    // Pre: None
    // Post: the calling Vector will be modified to match
    // the parameter Vector v
    Vector<T>& operator=(const Vector<T>& v);
    // operator to divide Vector by a number
    template <typename Y>
    Vector<T>& operator/=(const Y& d);
    // operator to add Vector with a number
    template <typename Y>
    Vector<T>& operator+=(const Y& d);
    // operator to subtract Vector with a number
    template <typename Y>
    Vector<T>& operator-=(const Y& d);
    // operator to add Vector with a Vector
    Vector<T>& operator+=(const Vector<T>& v);
    // operator to subtract Vector with a Vector
    Vector<T>& operator-=(const Vector<T>& v);

    // Desc: formats and outputs the calling Vector to the passed ostream
    // Pre: None
    // Post: The passed in parameter v will be formatted and output to
    // the ostream. Any elements in v less than v's zero tolerance
    // will output the value of 0 instead
    friend ostream& operator<<(ostream& os, const Vector& v)
    {
      // write in equation form
      os << "<";
      // for each element in v
      for (long i = 0; i < v.m_size; i++)
      {
        // if the absolute value of the element is
        // less than the tolerance, output 0 instead
        // of the actual value
        if (fabs(v[i]) < v.zero_tol)
          os << "0";
        else
          os << setprecision(10) << v[i];
        if (i < v.m_size - 1)
          os << ", ";
      }
      os << ">";
      return os;
    }
   
    // Desc: Reads in Vector elements from the ifstream
    // Pre: The is parameter must be opened and the parameter
    // v must have it's size defined
    // Post: The elements of v will have their values read in
    // from the ifstream is
    friend ifstream& operator>>(ifstream& is, Vector& v)
    {
      for (long i = 0; i < v.m_size; i++)
        is >> v[i];
      return is;
    }

  private:
    // the dimension of the Vector
    long m_size;
    // the data values of the Vector
    T* m_data;
    // the zero tolerance of the Vector
    // i.e. when the value "is close enough" to 0
    T zero_tol;
};

#include "Vector.hpp"