#pragma once
#include "UndirectedGraph.h"
#include "Cluster.h"

template <typename T>
std::set<Cluster> StructureClustering(const UndirectedGraph<T>& graph)
{
  std::cout << "StructureClustering on graph size " << graph.GetSize() << std::endl;
  std::set<Cluster> clusters;
  return clusters;
}