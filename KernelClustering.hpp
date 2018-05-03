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
    //std::cout << "-----REFINING-----" << std::endl;
    // expand graph
    auto old_graph = current_graph;
    current_graph = versions.back();
    versions.pop_back();

    auto new_clusters = starting_clusters;
    if (static_cast<long>(old_graph.merges.size()) > 0)
    {
      for (auto& cl : new_clusters)
        cl.second.clear();

      for (long supernode = 0; supernode < static_cast<long>(old_graph.merges.size()); supernode++)
      {
        auto node_a = old_graph.merges.at(supernode).first;
        auto node_b = old_graph.merges.at(supernode).second;
        //std::cout << supernode << "->(" << node_a << "," << node_b << ")" << std::endl;

        // get the cluster id for the supernode
        for (const auto& cl : starting_clusters)
        {
          auto itr = cl.second.find(supernode);
          if (itr != cl.second.end())
          {
            new_clusters[cl.first].insert(node_a);
            new_clusters[cl.first].insert(node_b);
          }
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
  std::cout << "-----COARSENING(" << graph.GetSize() << ")-----" << std::endl;
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
    auto n_itr = unmarked_nodes.begin();
    std::advance(n_itr, idx);
    long node = *n_itr;

    std::set<long> node_neighbors;
    for (long i = 0; i < graph.GetSize(); i++)
    {
      if (graph.DoesEdgeExist(node, i))
      {
        auto itr = unmarked_nodes.find(i);
        if (itr != unmarked_nodes.end())
          node_neighbors.insert(i);
      }
    }

    //std:cout << "node_neighbors: " << node_neighbors.size() << std::endl;
    if (static_cast<long>(node_neighbors.size()) == 0)
    {
      //std::cout << "(" << node << "," << node << ")->" << merged_nodes.size() << std::endl;
      merged_nodes.emplace_back(node, node);
    }
    else
    {
      long max_neighbor = -1;
      double max_eq = 0;

      for (const auto& n : node_neighbors)
      {
        double eq = graph.GetEdgeWeight(node, n);
        if (eq > max_eq || max_neighbor == -1)
        {
          max_neighbor = n;
          max_eq = eq;
        }
      }

      //std::cout << "(" << node << "," << max_neighbor << ")->" << merged_nodes.size() << std::endl;
      merged_nodes.emplace_back(node, max_neighbor);
      unmarked_nodes.erase(max_neighbor);
    }
    unmarked_nodes.erase(node);
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

  if (next_graph.GetSize() <= 2 * cluster_count)
  {
    gVersion<T> ver;
    ver.graph = next_graph;
    ver.merges = merged_nodes;
    versions.emplace_back(ver);
  }
  else
  {
    auto next_versions = Coarsening(next_graph, cluster_count);
    for (long i = 0; i < static_cast<long>(next_versions.size()); i++)
    {
      if (i == 0)
        next_versions.at(i).merges = merged_nodes;
      versions.emplace_back(next_versions.at(i));
    }
  }

  return versions;
}

template <typename T>
std::map<long, Cluster> KernelClustering(const UndirectedGraph<T>& graph, const long cluster_count)
{
  auto versions = Coarsening(graph, cluster_count);
  auto clusters = kMeans(versions.back().graph, cluster_count);
  clusters = Refinement(versions, clusters);

  long _nnodes = 0;
  for (const auto& _cl_ : clusters)
    for (const auto& _nn : _cl_.second)
      _nnodes++;
  //std::cout << "R1: " << _nnodes << std::endl;

  // for (auto itr = clusters.begin(); itr != clusters.end(); ++itr)
  // {
  //   if (itr->second.size() == 0)
  //   {
  //     clusters.erase(itr);
  //     itr = clusters.begin();
  //   }
  // }
  for (auto& cl : clusters)
    cl.second.insert(cl.first);

  _nnodes = 0;
  for (const auto& _cl_ : clusters)
    for (const auto& _nn : _cl_.second)
      _nnodes++;
  //std::cout << "R2: " << _nnodes << std::endl;

  return clusters;
}