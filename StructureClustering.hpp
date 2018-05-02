#pragma once
#include "UndirectedGraph.h"

template <typename T>
void StructureClustering(const UndirectedGraph<T>& graph)
{
  std::cout << "StructureClustering on graph size " << graph.GetSize() << std::endl;
}