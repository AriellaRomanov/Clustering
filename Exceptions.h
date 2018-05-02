#pragma once

#include <stdexcept>
#include <iostream>
#include <string>
using namespace std;

template <typename T>
class RangeErr
{
  public:
    RangeErr();
    RangeErr(const T i);
    T& BadRange() const;
    string what() const;
  private:
    T index = NULL;
};

class DiagDomErr
{
  public:
    DiagDomErr();
    string what() const;
};

class PosSemiDefErr
{
  public:
    PosSemiDefErr();
    string what() const;
};

class SubscriptErr
{
  public:
    SubscriptErr();
    SubscriptErr(const long i = -1);
    long& BadSubscript();
    string what() const;
  private:
    long index = -1;
};

class DivByZeroErr
{
  public:
    DivByZeroErr();
    string what() const;
  private:
};

class SizeErr
{
  public:
    SizeErr();
    SizeErr(const long a, const long b);
    string what() const;
  private:
    long m_a = 0;
    long m_b = 0;
};

#include "Exceptions.hpp"