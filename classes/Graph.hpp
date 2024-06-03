# ifndef Graph_hpp
# define Graph_hpp

#include <vector>
#include <iostream>
#include <functional>

class Node
{
public:
  typedef int   node_type;
  typedef Node* node_ptr_type;
private:
  node_type                   data;
  std::vector<node_ptr_type> neighbors;
  bool                       visited;

private:
  Node();

public:
  Node(node_type data) : data(data) {}
  Node(node_type data, const std::vector<node_ptr_type> &neighbors) : data(data), neighbors(neighbors) {}

public:
  void  add_neighbor(node_ptr_type new_neighbor) {
    neighbors.push_back(new_neighbor);
  }

  bool  breath_first_search(std::function<bool(node_ptr_type)> call_back)
  {
    std::vector<node_ptr_type>  queue;

    this->reset_visited();
    queue.push_back(this);
    while (!queue.empty())
    {
      node_ptr_type node = queue[0];
      bool halt = call_back(node);
      if (halt)
        return true;
      node->visited = true;
      queue.erase(queue.begin());
      for (auto node: neighbors) {
        if (node->visited == false)
          queue.push_back(node);
      }
    }
    return false;
  }

  void  reset_visited()
  {
    this->visited = false;
    for (auto node: neighbors) {
      node->reset_visited();
    }
  }

  size_t  get_size()
  {
    size_t result = 1;
    for (auto node: neighbors) {
      result += node->get_size();
    }
    return result;
  }

  node_type get_data() const{
    return this->data;
  }

  const std::vector<node_ptr_type> &get_neighbors() const {
    return this->neighbors;
  }

};

class Graph
{

};

#endif