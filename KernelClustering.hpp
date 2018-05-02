#pragma once
#include "UndirectedGraph.h"

template <typename T>
void KernelClustering(const UndirectedGraph<T>& graph)
{
  std::cout << "KernelClustering on graph size " << graph.GetSize() << std::endl;
}