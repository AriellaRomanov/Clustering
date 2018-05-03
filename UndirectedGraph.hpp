#include "UndirectedGraph.h"

template <typename T>
UndirectedGraph<T>::UndirectedGraph(const long size)
: graph_size(size), node_labels(new std::string[size]), matrix(size)
{
}

template <typename T>
UndirectedGraph<T>::UndirectedGraph(const UndirectedGraph<T>& copy)
: graph_size(copy.GetSize()), node_labels(new std::string[copy.GetSize()]), matrix(copy.matrix)
{
  for (long i = 0; i < graph_size; i++)
    node_labels[i] = copy.GetNodeLabel(i);
}

template <typename T>
UndirectedGraph<T>::UndirectedGraph(UndirectedGraph<T>&& source)
: graph_size(source.graph_size), node_labels(source.node_labels), matrix(source.matrix)
{
  source.node_labels = nullptr;
  source.graph_size = 0;
}

template <typename T>
UndirectedGraph<T>::~UndirectedGraph()
{
  delete[] node_labels;
  graph_size = 0;
}

template <typename T>
UndirectedGraph<T>& UndirectedGraph<T>::operator=(const UndirectedGraph<T>& copy)
{
  delete[] node_labels;
  graph_size = copy.graph_size;
  node_labels = new std::string[graph_size];
  for (long i = 0; i < graph_size; i++)
    node_labels[i] = copy.node_labels[i];
  matrix = copy.matrix;
  return *this;
}

template <typename T>
std::string UndirectedGraph<T>::GetNodeLabel(const long idx) const
{
  if (idx > graph_size)
    throw SubscriptErr(idx);
  return node_labels[idx];
}

template <typename T>
long UndirectedGraph<T>::GetNodeFromLabel(const std::string& label) const
{
  for (long i = 0; i < graph_size; i++)
    if (label == GetNodeLabel(i))
      return i;
  throw NodeNameErr(label);
}

template <typename T>
bool UndirectedGraph<T>::DoesEdgeExist(const std::string& node_a, const std::string& node_b) const
{
  return (GetEdgeWeight(node_a, node_b) != 0);
}

template <typename T>
bool UndirectedGraph<T>::DoesEdgeExist(const long node_a, const long node_b) const
{
  return (GetEdgeWeight(node_a, node_b) != 0);
}

template <typename T>
double UndirectedGraph<T>::GetEdgeWeight(const std::string& node_a, const std::string& node_b) const
{
  long a = GetNodeFromLabel(node_a);
  long b = GetNodeFromLabel(node_b);
  return GetEdgeWeight(a, b);
}

template <typename T>
double UndirectedGraph<T>::GetEdgeWeight(const long node_a, const long node_b) const
{
  return matrix(node_a, node_b);
}

template <typename T>
void UndirectedGraph<T>::SetEdgeWeight(const long node_a, const long node_b, const T weight)
{
  matrix(node_a, node_b, weight);
}

template <typename T>
long UndirectedGraph<T>::GetDegree(const long idx) const
{
  long degree = 0;
  for (long col = 0; col < graph_size; col++)
    degree += ((matrix(idx, col) != 0) ? 1 : 0);
  return degree;
}

template <typename T>
SymMatrix<T> UndirectedGraph<T>::GetDistanceMatrix() const
{
  const T infinity = -1;

  SymMatrix<T> distances(graph_size);
  for (long row = 0; row < graph_size; row++)
  {
    for (long col = 0; col <= row; col++)
    {
      T value = matrix(row, col);
      if (col == row)
        value = 0;
      else if (value == 0)
        value = infinity;
      distances(row, col, value);
    }
  }

  for (long k = 0; k < graph_size; k++)
  {
    for (long i = 0; i < graph_size; i++)
    {
      for (long j = 0; j < graph_size; j++)
      {
        if ((distances(i, j) > distances(i, k) + distances(k, j) || distances(i, j) == infinity)
            && distances(i, k) != infinity
            && distances(k, j) != infinity)
        {
          distances(i, j, (distances(i, k) + distances(k, j)));
        }
      }
    }
  }

  return distances;
}

template <typename T>
SymMatrix<T> UndirectedGraph<T>::GetAdjacencyMatrix() const
{
  return matrix;
}
