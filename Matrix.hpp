template <typename T>
Matrix<T>::Matrix(const long size)
: m_size(size), m_data(new Vector<T>[m_size]), m_zero(0)
{
  for (long i = 0; i < m_size; i++)
    m_data[i] = Vector<T>(m_size);
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T>& copy)
: m_size(copy.m_size), m_data(new Vector<T>[m_size]), m_zero(0)
{
  for (long i = 0; i < m_size; i++)
    m_data[i] = Vector<T>(copy.m_data[i]);
}

template <typename T>
Matrix<T>::Matrix(Matrix<T>&& source)
: m_size(source.m_size), m_data(source.m_data), m_zero(source.m_zero)
{
  source.m_data = nullptr;
  source.m_size = 0;
}

template <typename T>
Matrix<T>::~Matrix()
{
  //for (long i = 0; i < m_size; i++)
  //  delete m_data[i];
  delete[] m_data;
} 

template <typename T>
void Matrix<T>::SetTolerance(const T tol)
{
  if (tol < 0)
    throw RangeErr<T>(tol);
  m_zero = tol;
  for (long i = 0; i < m_size; i++)
    m_data[i].SetTolerance(m_zero);
}

template <typename T>
T Matrix<T>::GetTolerance() const
{
  return m_zero;
}

template <typename T>
T Matrix<T>::GetSize() const
{
  return m_size;
}

template <typename T>
Matrix<T> Matrix<T>::Transpose() const
{
  Matrix<T> m = *this;
  for (long i = 0; i < m_size - 1; i++)
  {
    for (long j = i + 1; j < m_size; j++)
    {
      T tmp = m.m_data[i][j];
      m.m_data[i][j] = m.m_data[j][i];
      m.m_data[j][i] = tmp;
    }
  }
  return m;
}

template <typename T>
bool Matrix<T>::IsDiagDom() const
{
  for (long i = 0; i < m_size; i++)
  {
    T sum = 0;
    for (long j = 0; j < m_size; j++)
    {
      if (i != j)
        sum += abs(m_data[i][j]);
    }
    if (sum > abs(m_data[i][i]))
      return false;
  }
  return true;
}

template <typename T>
T Matrix<T>::operator()(const long row, const long col) const
{
  return m_data[row][col];
}

template <typename T>
Matrix<T>& Matrix<T>::operator()(const long row, const long col, const T val)
{
  m_data[row][col] = val;
  return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const T rhs) const
{
  Matrix<T> m = *this;
  for (long i = 0; i < m_size; i++)
    m.m_data *= rhs;
  return m;
}

template <typename T>
Vector<T> Matrix<T>::operator*(const Vector<T>& rhs) const
{
  Vector<T> vect = Vector<T>(m_size);
  for (long i = 0; i < m_size; i++)
  {
    T sum = 0;
    for (long j = 0; j < m_size; j++)
    {
      sum += (m_data[i][j] * rhs[j]);
    }
    vect[i] = sum;
  }
  return vect;
}

template <typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T>& rhs) const
{
  Matrix<T> m(m_size);
  for (long i = 0; i < m_size; i++)
  {
    for (long j = 0; j < m_size; j++)
    {
      T sum = 0;
      for (long k = 0; k < m_size; k++)
      {
        sum += (m_data[i][k] * rhs.m_data[k][j]);
      }
      m(i, j, sum);
    }
  }
  return m;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const T rhs) const
{
  Matrix<T> m = *this;
  for (long i = 0; i < m_size; i++)
    for (long j = 0; j < m_size; j++)
      m[i][j] += rhs;
  return m;
}

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& rhs) const
{
  Matrix<T> m = *this;
  for (long i = 0; i < m_size; i++)
    for (long j = 0; j < m_size; j++)
      m[i][j] += rhs[i][j];
  return m;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const T rhs)
{
  *this = *this + rhs;
  return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& rhs)
{
  *this = *this + rhs;
  return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const T rhs) const
{
  Matrix<T> m = *this;
  return (m += (-1 * rhs));
}

template <typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T>& rhs) const
{
  Matrix<T> m = *this;
  return (m += (-1 * rhs));
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const T rhs)
{
  return (*this += (-1 * rhs));
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& rhs)
{
  return (*this += (-1 * rhs));
}