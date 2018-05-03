#pragma once
#include <map>
#include <set>
using Cluster = std::set<long>;

template <typename T>
long GetCentralNode(const SymMatrix<T>& distances, const Cluster& cluster)
{
  long node = -1;
  long centrality = 0;

  for (const auto& n : cluster)
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
double ClusterDensity(const UndirectedGraph<T>& graph, const Cluster& cluster)
{
  double edge_count = 0;
  for (const auto& c1 : cluster)
    for (const auto& c2 : cluster)
      if (graph.DoesEdgeExist(c1, c2))
        edge_count++;

  return edge_count / (cluster.size() * (cluster.size() - 1));
}

template <typename T>
T MaxShortestPath(const UndirectedGraph<T>& graph, const Cluster& cluster)
{
  auto distances = graph.GetDistanceMatrix();
  T max_path = 0;
  for (const auto& c1 : cluster)
    for (const auto& c2 : cluster)
      if (distances(c1, c2) > max_path)
        max_path = distances(c1, c2);
  
  return max_path;
}

template <typename T>
void OutputClusterInformation(const UndirectedGraph<T>& graph, const std::map<long, Cluster>& clusters)
{
  std::cout << "Cluster count: " << clusters.size() << std::endl << std::endl;
  
  long count = 0;
  for (const auto& cl : clusters)
  {
    std::cout << "Cluster ID: " << count++ << std::endl;
    std::cout << "Centroid Node: " << cl.first << ", Centroid Node Label: " << graph.GetNodeLabel(cl.first) << std::endl;
    std::cout << "Cluster Size: " << cl.second.size() << std::endl;
    std::cout << "Cluster Density: " << ClusterDensity(graph, cl.second) << std::endl;
    std::cout << "Cluster Maximum Shortest Path: " << MaxShortestPath(graph, cl.second) << std::endl;
    std::cout << "Cluster Nodes (ID/Label):" << std::endl;
    for (const auto& n : cl.second)
      std::cout << "  " << n << "/" << graph.GetNodeLabel(n) << std::endl;
    std::cout << std::endl << std::endl;
  }
}