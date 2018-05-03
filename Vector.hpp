template <typename T>
Vector<T>::Vector(const long size)
: m_size(size), m_data(new T[size]), zero_tol(0)
{
  for (long i = 0; i < m_size; i++)
    m_data[i] = 0;
}

template <typename T>
Vector<T>::Vector(const Vector<T>& copy)
: m_size(copy.m_size), m_data(new T[copy.m_size]), zero_tol(copy.zero_tol)
{
  T *p = m_data + m_size;
  T *q = copy.m_data + m_size;
  while (p > m_data)
    *--p = *--q;
}

template <typename T>
Vector<T>::Vector(Vector<T>&& copy)
: m_size(copy.m_size), m_data(copy.m_data), zero_tol(copy.zero_tol)
{
  copy.m_data = nullptr;
  copy.m_size = 0;
}

template <typename T>
Vector<T>::~Vector()
{
  delete[] m_data;
  m_size = 0;
}

template <typename T>
void Vector<T>::SetTolerance(const T tol)
{
  if (tol < 0)
    throw RangeErr<T>(tol);
  zero_tol = tol;
  return;
}

template <typename T>
long Vector<T>::GetSize() const
{
  return m_size;
}

template <typename T>
T Vector<T>::mag() const
{
  // intialize the magnitude to 0
  T mag = 0;
  // for each element of the Vector
  // add the square of the element to the magnitude
  for (long i = 0; i < m_size; i++)
    mag += (m_data[i] * m_data[i]);
  // take the square root of the sum of squares
  mag = sqrt(mag);
  return mag;
}

template <typename T>
T Vector<T>::dot(const Vector<T>& v) const
{
  // check for errors
  if (m_size != v.m_size)
    throw SizeErr(m_size, v.m_size);
  // initialize the product to 0
  T dot = 0;
  // iterate through the elements
  // add to dot the product of the corresponding
  // elements from each Vector
  for (long i = 0; i < m_size; i++)
    dot += (m_data[i] * v.m_data[i]);
  // return the inner (dot) product of the Vectors
  return dot;
}

template <typename T>
T& Vector<T>::operator[](const long i) const
{
  if (i < 0 || i >= m_size)
    throw SubscriptErr(i);
  return m_data[i];
}

template <typename T>
template <typename Y>
Vector<T> Vector<T>::operator*(const Y& d) const
{
  // make a copy of the current Vector
  auto vect = *this;
  // for each element of the Vector
  // multiply the element by d
  for (long i = 0; i < vect.m_size; i++)
    vect.m_data[i] *= static_cast<T>(d);
  // return the modified Vector
  return vect;
}

template <typename T>
template <typename Y>
Vector<T> Vector<T>::operator/(const Y& d) const
{
  // check for errors
  T d_t = static_cast<T>(d);
  if (d_t < zero_tol)
    throw DivByZeroErr();
  // make a copy of the current Vector
  auto vect = *this;
  // for each element of the Vector
  // divided the element by d
  for (long i = 0; i < vect.m_size; i++)
    vect.m_data[i] /= d_t;
  // return the modified Vector
  return vect;
}

template <typename T>
Vector<T> Vector<T>::operator+(const Vector<T>& v) const
{
  // check for errors
  if (m_size != v.m_size)
    throw SizeErr(m_size, v.m_size);
  // copy the calling Vector
  auto vect = *this;
  // iterate through the Vector
  // for each element, add the corresponding element in v
  for (long i = 0; i < vect.m_size; i++)
    vect.m_data[i] += v.m_data[i];
  // return the new Vector
  return vect;
}

template <typename T>
Vector<T> Vector<T>::operator-(const Vector<T>& v) const
{
  // check for errors
  if (m_size != v.m_size)
    throw SizeErr(m_size, v.m_size);
  // copy the calling Vector
  auto vect = *this;
  // multiply the Vector v by -1 and pass to the add operator
  // return the new Vector
  return (vect + (v * -1));
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& v)
{
  // delete the data array
  delete[] m_data;
  // set the new size, dimension
  m_size = v.m_size;
  // allocate new memory for the new data array
  m_data = new T[m_size];
  // set the new zero threshold
  zero_tol = v.zero_tol;

  // iterate through data members and set to new value
  T *p = m_data + m_size;
  T *q = v.m_data + m_size;
  while (p > m_data)
    *--p = *--q;

  return *this;
}

// Desc: operator to divide and assign a Vector by a number
// Pre: the parameter d must be able to be static casted to type T
// Post: the calling Vector will have each element in it
// divided by the parameter d casted to T
template <typename T>
template <typename Y>
Vector<T>& Vector<T>::operator/=(const Y& d)
{
  // check for error
  T d_t = static_cast<T>(d);
  if (d_t < zero_tol)
    throw DivByZeroErr();
  // iterate through the Vector and divide
  // each element by the casted value d
  for (long i = 0; i < m_size; i++)
    m_data[i] /= d_t;
  return *this;
}

// Desc: operator to add and assign a Vector with a number
// Pre: the parameter d must be able to be static casted to type T
// Post: the calling Vector will have each element in it
// added with the parameter d casted to T
template <typename T>
template <typename Y>
Vector<T>& Vector<T>::operator+=(const Y& d)
{
  // iterate through the Vector and add each
  // element with the casted value d
  for (long i = 0; i < m_size; i++)
    m_data[i] += static_cast<T>(d);
  return *this;
}

// Desc: operator to subtract and assign a Vector with a number
// Pre: the parameter d must be able to be static casted to type T
// Post: the calling Vector will subtract the parameter d casted
// to T from each element in it
template <typename T>
template <typename Y>
Vector<T>& Vector<T>::operator-=(const Y& d)
{
  // multiply d by -1 and pass to += operator
  return (this += (-1 * static_cast<T>(d)));
}

// Desc: operator to add and assign a Vector with a Vector
// Pre: the parameter v must be of the same dimension
// as the calling Vector
// Post: the calling Vector will have each element in it
// added with the corresponding element in v
template <typename T>
Vector<T>& Vector<T>::operator+=(const Vector<T>& v)
{
  // check for errors
  if (m_size != v.m_size)
    throw SizeErr(m_size, v.m_size);
  // iterate through the Vector
  // add each corresponding element in v to the calling
  // Vector element
  for (long i = 0; i < m_size; i++)
    m_data[i] += v.m_data[i];
  return *this;
}

// Desc: operator to subtract and assign a Vector with a Vector
// Pre: the parameter v must be of the same dimension
// as the calling Vector
// Post: the calling Vector will have each element in it
// added with the corresponding element in v
template <typename T>
Vector<T>& Vector<T>::operator-=(const Vector<T>& v)
{
  // check for errors
  if (m_size != v.m_size)
    throw SizeErr(m_size, v.m_size);
  // iterate through the Vector
  // subtract each corresponding element in v
  // from the calling Vector element
  for (long i = 0; i < m_size; i++)
    m_data[i] -= v.m_data[i];
  return *this;
}
