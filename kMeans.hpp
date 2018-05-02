#pragma once
#include "UndirectedGraph.h"
#include "Cluster.h"

template <typename T>
std::set<Cluster> kMeans(const UndirectedGraph<T>& graph)
{
  std::cout << "kMeans on graph size " << graph.GetSize() << std::endl;
  std::set<Cluster> clusters;
  return clusters;
}