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

  std::vector<std::pair<long, long>> merged_nodes;
  while (static_cast<long>(unmarked_nodes.size()) > 0)
  {
    long idx = rand() % static_cast<long>(unmarked_nodes.size());
    auto itr = unmarked_nodes.begin();
    std::advance(itr, idx);
    idx = *itr;

    std::set<long> neighbors;
    for (long i = 0; i < graph.GetSize(); i++)
      if (graph.DoesEdgeExist(idx, i))
        neighbors.insert(i);

    for (auto itr = neighbors.begin(); itr != neighbors.end() && static_cast<long>(neighbors.size()) > 0; ++itr)
    {
      auto n_id = *itr;
      auto umn_itr = unmarked_nodes.find(n_id);
      if (umn_itr == unmarked_nodes.end())
      {
        neighbors.erase(itr);
        itr = neighbors.begin();
      }
    }

    if (static_cast<long>(neighbors.size()) == 0)
    {
      merged_nodes.emplace_back(idx, idx);
    }
    else
    {
      long max_neighbor = -1;
      double max_eq = 0;

      for (const auto& n : neighbors)
      {
        double eq = 2 * graph.GetEdgeWeight(idx, n);
        if (eq > max_eq || max_neighbor == -1)
        {
          max_neighbor = n;
          max_eq = eq;
        }
      }

      merged_nodes.emplace_back(idx, max_neighbor);
      unmarked_nodes.erase(max_neighbor);
    }
    unmarked_nodes.erase(idx);
  }

  UndirectedGraph<T> next_graph(static_cast<long>(merged_nodes.size()));
  for (long i = 0; i < static_cast<long>(merged_nodes.size()); i++)
  {
    long mn_a = merged_nodes.at(i).first;
    long mn_b = merged_nodes.at(i).second;

    std::set<long> neighbors_a;
    for (long n = 0; n < graph.GetSize(); n++)
      if (graph.DoesEdgeExist(mn_a, n))
        neighbors_a.insert(n);

    std::set<long> neighbors_b;
    for (long n = 0; n < graph.GetSize(); n++)
      if (graph.DoesEdgeExist(mn_b, n))
        neighbors_b.insert(n);

    for (const auto& n : neighbors_a)
    {
      long new_neighbor = -1;
      for (long j = 0; j < static_cast<long>(merged_nodes.size()) && new_neighbor == -1; j++)
      {
        if (merged_nodes.at(j).first == n || merged_nodes.at(j).second == n)
          new_neighbor = j;
      }

      auto weight = graph.GetEdgeWeight(mn_a, n);
      auto curr_weight = next_graph.GetEdgeWeight(i, new_neighbor);
      next_graph.SetEdgeWeight(i, new_neighbor, curr_weight + weight);
    }

    for (const auto& n : neighbors_b)
    {
      long new_neighbor = -1;
      for (long j = 0; j < static_cast<long>(merged_nodes.size()) && new_neighbor == -1; j++)
      {
        if (merged_nodes.at(j).first == n || merged_nodes.at(j).second == n)
          new_neighbor = j;
      }

      auto weight = graph.GetEdgeWeight(mn_b, n);
      auto curr_weight = next_graph.GetEdgeWeight(i, new_neighbor);
      next_graph.SetEdgeWeight(i, new_neighbor, curr_weight + weight);
    }
  }

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
  std::cout << "Version count: " << versions.size() << " | Final size: " << versions.back().GetSize() << std::endl;
  auto clusters = kMeans(versions.back(), cluster_count);
  std::cout << "kMeans Complete." << std::endl;
  clusters = Refinement(versions, clusters);
  std::cout << "Cluster count: " << clusters.size() << std::endl;
  return clusters;
}