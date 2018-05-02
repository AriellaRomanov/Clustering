
// Desc: default constructor for the SubscriptErr class
// Pre: None
// Post: an instance of the SubscriptErr class
// will be created
template <typename T>
RangeErr<T>::RangeErr(){}

// Desc: constructor for the SubscriptErr class
// Pre: None
// Post: an instance of the SubscriptErr class
// will be created, with the index set to i
template <typename T>
RangeErr<T>::RangeErr(const T i)
: index(i)
{}

// Desc: returns the value of the subscript
// this error was set to
// Pre: None
// Post: returns the value of the subscript
// this error was set to
template <typename T>
T& RangeErr<T>::BadRange() const
{
  return index;
}

// Desc: returns a string description of the error
// Pre: None
// Post: returns a string description of the error
template <typename T>
string RangeErr<T>::what() const
{
  string err = "Value out of range: " +  to_string(index);
  return err;
}

// Desc: default constructor for the DiagDomErr class
// Pre: None
// Post: an instance of the DiagDomErr class
// will be created
DiagDomErr::DiagDomErr(){}

// Desc: returns a string description of the error
// Pre: None
// Post: returns a string description of the error
string DiagDomErr::what() const
{
  return "Matrix not diagonal dominant";
}

// Desc: default constructor for the PosSemiDefErr class
// Pre: None
// Post: an instance of the PosSemiDefErr class
// will be created
PosSemiDefErr::PosSemiDefErr(){}

// Desc: returns a string description of the error
// Pre: None
// Post: returns a string description of the error
string PosSemiDefErr::what() const
{
  return "Matrix not positive semi-definite";
}

// Desc: default constructor for the SubscriptErr class
// Pre: None
// Post: an instance of the SubscriptErr class
// will be created
SubscriptErr::SubscriptErr(){}

// Desc: constructor for the SubscriptErr class
// Pre: None
// Post: an instance of the SubscriptErr class
// will be created, with the index set to i
SubscriptErr::SubscriptErr(const long i)
: index(i)
{}

// Desc: returns the value of the subscript
// this error was set to
// Pre: None
// Post: returns the value of the subscript
// this error was set to
long& SubscriptErr::BadSubscript()
{
  return index;
}

// Desc: returns a string description of the error
// Pre: None
// Post: returns a string description of the error
string SubscriptErr::what() const
{
  string err = "Bad subscript: " +  to_string(index);
  return err;
}

// Desc: default constructor for DivByZeroErr class
// Pre: None
// Post: creates an instance of the DivByZeroErr class
DivByZeroErr::DivByZeroErr(){}

// Desc: returns a string description of the error
// Pre: None
// Post: returns a string description of the error
string DivByZeroErr::what() const
{
  return "Illegal division by zero.";
}

// Desc: default constructor for the SizeErr class
// Pre: None
// Post: an instance of the SizeErr class
// will be created
SizeErr::SizeErr(){}

// Desc: constructor for the SizeErr class
// Pre: None
// Post: an instance of the SizeErr class
// will be created, with the two sizes
// set to a and b
SizeErr::SizeErr(const long a, const long b)
: m_a(a), m_b(b)
{}

// Desc: returns a string description of the error
// Pre: None
// Post: returns a string description of the error
string SizeErr::what() const
{
  string err = "Non-matching sizes: " +  to_string(m_a) + " and " + to_string(m_b);
  return err;
}