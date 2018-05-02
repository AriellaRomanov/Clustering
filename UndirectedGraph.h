#pragma once
#include "SymMatrix.h"

class UndirectedGraph
{
  private:
    long graph_size;
    SymMatrix<double> matrix;
    std::string* node_labels;
  public:
    UndirectedGraph(const long size = 1);
    UndirectedGraph(const UndirectedGraph& copy);
    UndirectedGraph(UndirectedGraph&& source);
    ~UndirectedGraph();

    inline long GetSize() const { return graph_size; }
    std::string GetNodeLabel(const long idx) const;
    long GetNodeFromLabel(const std::string& label) const;
    bool DoesEdgeExist(const std::string& node_a, const std::string& node_b) const;
    bool DoesEdgeExist(const long node_a, const long node_b) const;
    double GetEdgeWeight(const std::string& node_a, const std::string& node_b) const;
    double GetEdgeWeight(const long node_a, const long node_b) const;
};