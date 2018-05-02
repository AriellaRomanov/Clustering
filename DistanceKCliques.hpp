#pragma once
#include "UndirectedGraph.h"
#include "Cluster.h"

template <typename T>
std::map<long, Cluster> InitialClustering(const UndirectedGraph<T>& graph)
{
  std::set<long> node_ids;
  for (long i = 0; i < graph.GetSize(); i++)
    node_ids.insert(i);

  std::map<long, Cluster> clusters;
  long cluster_count = 0;

  while (static_cast<long>(node_ids.size()) > 0)
  {
    cluster_count++;

    long node = *node_ids.begin();
    long node_degree = graph.GetDegree(node);
    for (const auto& n : node_ids)
    {
      long deg = graph.GetDegree(n);
      if (deg > node_degree)
      {
        node_degree = deg;
        node = n;
      }
    }

    clusters[cluster_count] = {node};
    node_ids.erase(node);

    for (long i = 0; i < graph.GetSize(); i++)
    {
      if (i != node && graph.DoesEdgeExist(node, i))
      {
        clusters[cluster_count].insert(i);
        node_ids.erase(i);
      }
    }
    
  }

  return clusters;
}

template <typename T>
std::map<long, std::set<long>> GetBridgeNodes(const UndirectedGraph<T>& graph, const std::map<long, Cluster>& clusters)
{
  std::map<long, std::set<long>> bridges; // <node_id, cluster_list>

  for (const auto& c : clusters)
  {
    for (const auto& node : c.second)
    {
      bool is_bridge = false;
      for (long i = 0; i < graph.GetSize() && !is_bridge; i++)
      {
        if (i != node && graph.DoesEdgeExist(node, i))
        {
          auto itr = c.second.find(i);
          if (itr == c.second.end())
          {
            std::set<long> cluster_list;
            for (const auto& c2 : clusters)
            {
              auto c2_itr = c2.second.find(i);
              if (c2_itr != c2.second.end())
                cluster_list.insert(c2.first);
            }
            is_bridge = true;
            bridges[node] = cluster_list;
          }
        }
      } 
    }
  }

  return bridges;
}

template <typename T>
T GetClusterDiameter(const UndirectedGraph<T>& graph, const Cluster& cluster)
{
  T diameter = 0;
  auto distances = graph.GetDistanceMatrix();
  for (const auto& n1 : cluster)
  {
    for (const auto& n2 : cluster)
    {
      auto dist = distances(n1, n2);
      if (dist > diameter)
        diameter = dist;
    }
  }
  return diameter;
}

template <typename T>
std::map<long, Cluster> DistanceKCliques(const UndirectedGraph<T>& graph, const T distance_k)
{
  auto clusters = InitialClustering(graph);
  auto bridges = GetBridgeNodes(graph, clusters);

  bool cluster_list_non_empty = false;
  do
  {
    for (auto itr = clusters.begin(); itr != clusters.end(); ++itr)
    {
      if (itr->second.size() == 0)
      {
        clusters.erase(itr->first);
        for (auto& bridge : bridges)
          bridge.second.erase(itr->first);
        itr = clusters.begin();
      }
    }

    cluster_list_non_empty = false;
    for (auto& bridge : bridges)
      cluster_list_non_empty = cluster_list_non_empty || (bridge.second.size() > 0);

    if (cluster_list_non_empty)
    {
      long bridge_node = (*bridges.begin()).first;
      long bridge_size = 0;
      for (auto& bridge : bridges)
      {
        long size = 0;
        for (const auto& li : bridge.second)
          size += clusters[li].size();
        
        if (size > bridge_size)
        {
          bridge_size = size;
          bridge_node = bridge.first;
        }
      }

      auto cluster_list = bridges[bridge_node];
      long bridge_node_cluster = -1;
      for (const auto& cl : clusters)
      {
        auto itr = cl.second.find(bridge_node);
        if (itr != cl.second.end())
          bridge_node_cluster = cl.first;
      }
      cluster_list.insert(bridge_node_cluster);

      Cluster combined_cluster;
      for (const auto& cl : cluster_list)
        for (const auto& node : clusters[cl])
          combined_cluster.insert(node);

      long cluster_one = -1;
      T diam_one = 0;
      long cluster_two = -1;
      T diam_two = 0;
      for (const auto& cl : cluster_list)
      {
        auto diameter = GetClusterDiameter(graph, clusters[cl]);
        if (cluster_one < 0)
        {
          cluster_one = cl;
          diam_one = diameter;
        }
        else if (cluster_two < 0)
        {
          cluster_two = cl;
          diam_two = diameter;
        }
        else
        {
          if (diam_one < diam_two)
          {
            if (diameter > diam_one)
            {
              cluster_one = cl;
              diam_one = diameter;
            }
          }
          else
          {
            if (diameter > diam_two)
            {
              cluster_two = cl;
              diam_two = diameter;
            }
          }
        }
      }

      if (diam_two > diam_one)
      {
        long _temp_c = cluster_one;
        long _temp_d = diam_one;
        cluster_one = cluster_two;
        diam_one = diam_two;
        cluster_two = _temp_c;
        diam_two = _temp_d;
      }

      T modifier = (bridge_node_cluster == cluster_one || bridge_node_cluster == cluster_two) ? 1 : 2;
      T combined_diameter = diam_one + diam_two + modifier;

      if (combined_diameter <= distance_k)
      {
        long cluster_count = clusters.size() + 1;
        clusters[cluster_count] = combined_cluster;
        for (auto& cl : cluster_list)
          clusters.erase(cl);

        for (auto& bridge : bridges)
        {
          bool removal_made = false;
          for (auto& cl : cluster_list)
          {
            auto itr = bridge.second.find(cl);
            if (itr != bridge.second.end())
            {
              bridge.second.erase(cl);
              removal_made = true;
            }
          }

          if (removal_made && combined_diameter < distance_k)
            bridge.second.insert(cluster_count);
        }

        if (combined_diameter == distance_k)
        {
          for (const auto& u : combined_cluster)
            bridges.erase(u);
        }
      }
      else
      {
        long rem_cluster = cluster_two;
        if (bridge_node_cluster != cluster_one)
          rem_cluster = cluster_one;

        if (cluster_one == bridge_node_cluster ||
            cluster_two == bridge_node_cluster)
        {
          for (const auto& u : combined_cluster)
          {
            auto b_itr = bridges.find(u);
            if (b_itr != bridges.end())
              bridges[u].erase(rem_cluster);
          }

          for (auto& u : clusters[rem_cluster])
          {
            auto b_itr = bridges.find(u);
            if (b_itr != bridges.end())
            {
              for (const auto& clist : cluster_list)
                bridges[u].erase(clist);
            }
          }
        }
        else
        {
          bridges[bridge_node_cluster].erase(rem_cluster);
        }
      }
    }

  } while (cluster_list_non_empty);

  for (auto itr = clusters.begin(); itr != clusters.end(); ++itr)
  {
    if (itr->second.size() == 0)
    {
      clusters.erase(itr->first);
      itr = clusters.begin();
    }
  }

  return clusters;
}