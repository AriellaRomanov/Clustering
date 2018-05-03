#pragma once
#include "kMeans.hpp"
#include <time.h>
#include <stdlib.h>
#include <vector>

template<typename T>
std::map<long, Cluster> Refinement(std::vector<UndirectedGraph<T>> versions, std::map<long, Cluster> starting_clusters)
{
  return starting_clusters;
}

template <typename T>
std::vector<UndirectedGraph<T>> Coarsening(UndirectedGraph<T> graph, const long cluster_count)
{
  std::vector<UndirectedGraph<T>> versions;
  versions.emplace_back(graph);

  std::set<long> unmarked_nodes;
  for (long i = 0; i < graph.GetSize(); i++)
    unmarked_nodes.insert(i);

  std::set<std::pair<long, long>> merged_nodes;
  while (static_cast<long>(unmarked_nodes.size()) > 0)
  {
    
  }

  UndirectedGraph<T> next_graph(static_cast<long>(merged_nodes.size()));

  if (next_graph.GetSize() <= 4 * cluster_count)
    versions.emplace_back(next_graph);
  else
  {
    auto next_versions = Coarsening(next_graph, cluster_count);
    for (auto& v : next_versions)
      versions.emplace_back(v);
  }

  return versions;
}

template <typename T>
std::map<long, Cluster> KernelClustering(const UndirectedGraph<T>& graph, const long cluster_count)
{
  std::cout << "KernelClustering on graph size " << graph.GetSize() << std::endl;
  auto versions = Coarsening(graph, cluster_count);
  auto clusters = kMeans(versions.back(), cluster_count);
  clusters = Refinement(versions, clusters);
  return clusters;
}