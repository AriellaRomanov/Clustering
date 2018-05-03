#pragma once
#include "kMeans.hpp"
#include <time.h>
#include <stdlib.h>
#include <vector>

template <typename T>
struct gVersion
{
  UndirectedGraph<T> graph;
  std::vector<std::pair<long, long>> merges;
};

template <typename T>
void RefinementAlgorithm(const UndirectedGraph<T>& graph, std::map<long, Cluster>& starting_clusters)
{
  const long max_iterations = 10;
  const T sigma = 1;

  SymMatrix<T> identity(graph.GetSize());
  for (long i = 0; i < identity.GetSize(); i++)
    identity(i, i, sigma);

  SymMatrix<T> kernel_matrix(graph.GetAdjacencyMatrix());
  kernel_matrix += identity;

  long iteration = 0;
  while (iteration++ < max_iterations)
  {
    std::map<long, std::vector<double>> distances;
    for (const auto& cl : starting_clusters)
    {
      std::vector<double> dists;
      for (long i = 0; i < kernel_matrix.GetSize(); i++)
      {
        double wj = cl.second.size();

        double kij = 0;
        for (const auto& j : cl.second)
          kij += kernel_matrix(i, j);
        kij *= 2;

        double kjl = 0;
        for (const auto& j : cl.second)
          for (const auto& l : cl.second)
            kjl += kernel_matrix(j, l);

        double d = kernel_matrix(i, i) - (kij / wj) + (kjl / (wj * wj));
        dists.push_back(d);
      }
      distances[cl.first] = dists;
    }

    std::map<long, Cluster> new_clusters = starting_clusters;
    for (auto& cl : new_clusters)
      cl.second.clear();

    for (long i = 0; i < kernel_matrix.GetSize(); i++)
    {
      long min_cluster = -1;
      double min_dist = 0;
      for (const auto& dists : distances)
      {
        if (dists.second.at(i) < min_dist || min_cluster == -1)
        {
          min_cluster = dists.first;
          min_dist = dists.second.at(i);
        }
      }
      new_clusters[min_cluster].insert(i);
    }

    if (new_clusters == starting_clusters)
      iteration = max_iterations;
    starting_clusters = new_clusters;
  }
}

template <typename T>
std::map<long, Cluster> Refinement(std::vector<gVersion<T>> versions, std::map<long, Cluster> starting_clusters)
{
  auto current_graph = versions.back();
  versions.pop_back();
  RefinementAlgorithm(current_graph.graph, starting_clusters);

  while (static_cast<long>(versions.size()) > 0)
  {
    // expand graph
    auto old_graph = current_graph;
    current_graph = versions.back();
    versions.pop_back();

    auto new_clusters = starting_clusters;
    if (static_cast<long>(old_graph.merges.size()) > 0)
    {
      for (auto& cl : new_clusters)
        cl.second.clear();

      for (long i = 0; i < current_graph.graph.GetSize(); i++)
      {
        // get the node id of the supernode it was in
        long supernode = -1;
        for (long j = 0; j < static_cast<long>(old_graph.merges.size()) && supernode == -1; j++)
          if (i == old_graph.merges.at(j).first || i == old_graph.merges.at(j).second)
            supernode = j;

        // get the cluster id for the supernode
        for (const auto& cl : starting_clusters)
        {
          auto itr = cl.second.find(supernode);
          if (itr != cl.second.end())
            new_clusters[cl.first].insert(i);
        }
      }
    }
    starting_clusters = new_clusters;

    // refine expanded graph clusters
    RefinementAlgorithm(current_graph.graph, starting_clusters);
  }

  return starting_clusters;
}

template <typename T>
std::vector<gVersion<T>> Coarsening(const UndirectedGraph<T>& graph, const long cluster_count)
{
  std::vector<gVersion<T>> versions;
  gVersion<T> _v;
  _v.graph = graph;
  versions.emplace_back(_v);

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
  {
    gVersion<T> ver;
    ver.graph = next_graph;
    ver.merges = merged_nodes;
    versions.emplace_back(ver);
  }
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
  auto versions = Coarsening(graph, cluster_count);
  auto clusters = kMeans(versions.back().graph, cluster_count);
  clusters = Refinement(versions, clusters);

  for (auto& cl : clusters)
    cl.second.insert(cl.first);

  return clusters;
}