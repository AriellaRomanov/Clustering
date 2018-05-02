#pragma once
#include "UndirectedGraph.h"
#include "Cluster.h"

template <typename T>
std::set<Cluster> KernelClustering(const UndirectedGraph<T>& graph)
{
  std::cout << "KernelClustering on graph size " << graph.GetSize() << std::endl;
  std::set<Cluster> clusters;
  return clusters;
}