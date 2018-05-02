#pragma once

#include <stdexcept>
#include <iostream>
#include <string>
using namespace std;

class MatrixErr
{
  public:
    virtual string what() const = 0;
};

template <typename T>
class RangeErr : public MatrixErr
{
  public:
    RangeErr();
    RangeErr(const T i);
    T& BadRange() const;
    virtual string what() const;
  private:
    T index = NULL;
};

class DiagDomErr : public MatrixErr
{
  public:
    DiagDomErr();
    virtual string what() const;
};

class PosSemiDefErr : public MatrixErr
{
  public:
    PosSemiDefErr();
    virtual string what() const;
};

class SubscriptErr : public MatrixErr
{
  public:
    SubscriptErr();
    SubscriptErr(const long i = -1);
    long& BadSubscript();
    virtual string what() const;
  private:
    long index = -1;
};

class DivByZeroErr : public MatrixErr
{
  public:
    DivByZeroErr();
    virtual string what() const;
  private:
};

class SizeErr : public MatrixErr
{
  public:
    SizeErr();
    SizeErr(const long a, const long b);
    virtual string what() const;
  private:
    long m_a = 0;
    long m_b = 0;
};

class NodeNameErr : public MatrixErr
{
  public:
    NodeNameErr();
    NodeNameErr(const std::string& node_name);
    virtual string what() const;
  private:
    std::string name;
};

#include "Exceptions.hpp"