#include "UndirectedGraph.h"

UndirectedGraph::UndirectedGraph(const long size)
: graph_size(size), node_labels(new std::string[size]), matrix(size)
{
}

UndirectedGraph::UndirectedGraph(const UndirectedGraph& copy)
: graph_size(copy.GetSize()), node_labels(new std::string[copy.GetSize()]), matrix(copy.GetSize())
{
  for (long i = 0; i < graph_size; i++)
    node_labels[i] = copy.GetNodeLabel(i);
}

UndirectedGraph::UndirectedGraph(UndirectedGraph&& source)
: graph_size(source.graph_size), node_labels(source.node_labels), matrix(source.matrix)
{
  source.node_labels = nullptr;
  source.graph_size = 0;
}

UndirectedGraph::~UndirectedGraph()
{
  delete[] node_labels;
  graph_size = 0;
}

std::string UndirectedGraph::GetNodeLabel(const long idx) const
{
  if (idx > graph_size)
    throw SubscriptErr(idx);
  return node_labels[idx];
}

long UndirectedGraph::GetNodeFromLabel(const std::string& label) const
{
  for (long i = 0; i < graph_size; i++)
    if (label == GetNodeLabel(i))
      return i;
  throw NodeNameErr(label);
}

bool UndirectedGraph::DoesEdgeExist(const std::string& node_a, const std::string& node_b) const
{
  return (GetEdgeWeight(node_a, node_b) != 0);
}

bool UndirectedGraph::DoesEdgeExist(const long node_a, const long node_b) const
{
  return (GetEdgeWeight(node_a, node_b) != 0);
}

double UndirectedGraph::GetEdgeWeight(const std::string& node_a, const std::string& node_b) const
{
  long a = GetNodeFromLabel(node_a);
  long b = GetNodeFromLabel(node_b);
  return GetEdgeWeight(a, b);
}

double UndirectedGraph::GetEdgeWeight(const long node_a, const long node_b) const
{
  return matrix[node_a][node_b];
}