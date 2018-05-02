#pragma once
#include "UndirectedGraph.h"
#include "Cluster.h"
#include <tuple>

template <typename T>
long GetCentralNode(const SymMatrix<T>& distances, const std::set<long>& node_group)
{
  long node = -1;
  long centrality = 0;

  for (const auto& n : node_group)
  {
    double closeness = 0;
    for (long i = 0; i < distances.GetSize(); i++)
      closeness += (1.0 / distances(i, n));

    if (closeness > centrality || node == -1)
    {
      centrality = closeness;
      node = n;
    }
  }

  return node;
}

template <typename T>
long ClosestNode(const SymMatrix<T>& distances, const std::set<long>& node_group, const std::set<long>& nodes_to_ignore)
{
  long closest_node = -1;
  T min_dist = -1;
  for (const auto& idx : node_group)
  {
    for (long node = 0; node < distances.GetSize(); node++)
    {
      auto itr = node_group.find(node);
      if (itr != node_group.end())
        continue;

      auto itr_d = nodes_to_ignore.find(node);
      if (itr_d != nodes_to_ignore.end())
        continue;

      auto distance = distances(idx, node);
      if (distance > 0 && (distance < min_dist || min_dist == -1))
      {
        min_dist = distance;
        closest_node = node;
      }
    }
  }
  return closest_node;
}

template <typename T>
std::pair<long, long> ClosestPair(const SymMatrix<T>& distances, const std::set<long>& nodes_to_ignore)
{
  long node_a = -1;
  long node_b = -1;
  T min_dist = -1;
  for (long row = 0; row < distances.GetSize(); row++)
  {
    auto itr_r = nodes_to_ignore.find(row);
    if (itr_r != nodes_to_ignore.end())
      continue;

    for (long col = 0; col <= row; col++)
    {
      auto itr_c = nodes_to_ignore.find(col);
      if (itr_c != nodes_to_ignore.end())
        continue;

      T distance = distances(row, col);
      if (distance > 0 && (distance < min_dist || min_dist < 0))
      {
        min_dist = distance;
        node_a = row;
        node_b = col;
      }
    }
  }
  return std::pair<long, long>(node_a, node_b);
}

template <typename T>
std::map<long, Cluster> DetermineCentroids(const UndirectedGraph<T>& graph, const SymMatrix<T>& distance_matrix, const long cluster_count)
{
  std::map<long, Cluster> centroids;
  std::set<long> deleted_nodes;
  for (long m = 0; m < cluster_count; m++)
  {
    Cluster cluster;
    auto pair = ClosestPair(distance_matrix, deleted_nodes);
    if (pair.first == -1 || pair.second == -1)
      return centroids;
    else
    {
      deleted_nodes.insert(pair.first);
      deleted_nodes.insert(pair.second);
      cluster.insert(pair.first);
      cluster.insert(pair.second);

      while(static_cast<long>(cluster.size()) < ((0.75 * graph.GetSize()) / cluster_count))
      {
        auto node = ClosestNode(distance_matrix, cluster, deleted_nodes);
        if (node == -1)
          break;
        else
        {
          deleted_nodes.insert(node);
          cluster.insert(node);
        }
      }

      auto centroid = GetCentralNode(distance_matrix, cluster);
      centroids[centroid] = cluster;
    }
  }
  return centroids;
}

template <typename T>
void AssignClusters(const UndirectedGraph<T>& graph, const SymMatrix<T>& distance_matrix, std::map<long, Cluster>& clusters)
{
  for (long i = 0; i < graph.GetSize(); i++)
  {
    long closest_centroid = -1;
    T min_dist = -1;

    for (const auto& c : clusters)
    {
      auto distance = distance_matrix(c.first, i);
      if (distance != -1 && (distance < min_dist || min_dist == -1))
      {
        min_dist = distance;
        closest_centroid = c.first;
      }
    }

    if (closest_centroid != -1)
      clusters[closest_centroid].insert(i);
  }

  bool made_change = false;
  do
  {
    made_change = false;

    for (auto itr = clusters.begin(); itr != clusters.end(); ++itr)
    {
      auto new_centroid = GetCentralNode(distance_matrix, itr->second);
      if (new_centroid != itr->first)
      {
        made_change = true;
        clusters[new_centroid] = clusters[itr->first];
        clusters.erase(itr);
        itr = clusters.begin();
      }
    }
    
    if (made_change)
    {
      for (long i = 0; i < graph.GetSize(); i++)
      {
        long old_centroid = -1;
        for (const auto& c : clusters)
        {
          auto itr = c.second.find(i);
          if (itr != c.second.end())
          {
            old_centroid = c.first;
            break;
          }
        }

        long closest_centroid = old_centroid;
        T min_dist = distance_matrix(old_centroid, i);

        for (const auto& c : clusters)
        {
          auto distance = distance_matrix(c.first, i);
          if (distance != -1 && (distance < min_dist || min_dist == -1))
          {
            min_dist = distance;
            closest_centroid = c.first;
          }
        }

        if (old_centroid != closest_centroid)
        {
          clusters[old_centroid].erase(i);
          clusters[closest_centroid].insert(i);
        }
      }
    }
  } while (made_change);
}

template <typename T>
std::map<long, Cluster> kMeans(const UndirectedGraph<T>& graph, const long cluster_count)
{
  auto distance_matrix = graph.GetDistanceMatrix();
  auto clusters = DetermineCentroids(graph, distance_matrix, cluster_count);

  for (auto itr = clusters.begin(); itr != clusters.end(); ++itr)
    itr->second.clear();

  AssignClusters(graph, distance_matrix, clusters);

  return clusters;
}