#pragma once
#include "UndirectedGraph.h"

template <typename T>
void kMeans(const UndirectedGraph<T>& graph)
{
  std::cout << "kMeans on graph size " << graph.GetSize() << std::endl;
}