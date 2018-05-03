#pragma once
#include "SymMatrix.h"

template <typename T>
class UndirectedGraph
{
  private:
    long graph_size;
    std::string* node_labels;
    SymMatrix<T> matrix;
  public:
    UndirectedGraph(const long size = 1);
    UndirectedGraph(const UndirectedGraph<T>& copy);
    UndirectedGraph(UndirectedGraph<T>&& source);
    ~UndirectedGraph();
    UndirectedGraph<T>& operator=(const UndirectedGraph<T>& copy);

    inline long GetSize() const { return graph_size; }
    std::string GetNodeLabel(const long idx) const;
    long GetNodeFromLabel(const std::string& label) const;
    bool DoesEdgeExist(const std::string& node_a, const std::string& node_b) const;
    bool DoesEdgeExist(const long node_a, const long node_b) const;
    double GetEdgeWeight(const std::string& node_a, const std::string& node_b) const;
    double GetEdgeWeight(const long node_a, const long node_b) const;
    void SetEdgeWeight(const long node_a, const long node_b, const T weight);
    long GetDegree(const long idx) const;
    SymMatrix<T> GetDistanceMatrix() const;

    friend ostream& operator<<(ostream& os, const UndirectedGraph& graph)
    {
      os << "[Labels]" << std::endl;
      for (long i = 0; i < graph.graph_size; i++)
        os << "   " << graph.node_labels[i] << std::endl;
      os << "[Edges]" << std::endl << graph.matrix << std::endl;
      return os;
    }

    friend ifstream& operator>>(ifstream& is, UndirectedGraph<T>& graph)
    {
      long old_size = graph.graph_size;
      is >> graph.graph_size;
      if (old_size != graph.graph_size)
      {
        delete[] graph.node_labels;
        graph.node_labels = new std::string[graph.graph_size];
      }
      for (long i = 0; i < graph.graph_size; i++)
        is >> graph.node_labels[i];

      graph.matrix = SymMatrix<T>(graph.graph_size);
      for (long row = 0; row < graph.graph_size; row++)
      {
        for (long col = 0; col < graph.graph_size; col++)
        {
          T value(0);
          is >> value;
          graph.matrix(row, col, value);
        }
      }

      return is;
    }
};

#include "UndirectedGraph.hpp"