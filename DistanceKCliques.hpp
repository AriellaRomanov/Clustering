#pragma once
#include "UndirectedGraph.h"
#include "Cluster.h"

template <typename T>
std::set<Cluster> DistanceKCliques(const UndirectedGraph<T>& graph)
{
  std::cout << "DistanceKCliques on graph size " << graph.GetSize() << std::endl;
  std::set<Cluster> clusters;
  return clusters;
}