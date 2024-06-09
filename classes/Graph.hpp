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
  node_ptr_type              parent;

private:
  Node();

public:
  Node(node_type data) : data(data) {}
  Node(node_type data, const std::vector<node_ptr_type> &neighbors) : 
    data(data),
    neighbors(neighbors),
    parent(nullptr)
    {}

public:
  void  add_neighbor(node_ptr_type new_neighbor) {
    neighbors.push_back(new_neighbor);
  }

  bool  breath_first_search(std::function<bool(node_ptr_type)> call_back)
  {
    std::set<node_ptr_type> visited;
    std::vector<node_ptr_type>  queue;

    queue.push_back(this);
    this->parent = nullptr;
    while (!queue.empty())
    {
      node_ptr_type node = queue[0];
      bool halt = call_back(node);
      if (halt)
        return true;
      visited.insert(node);
      queue.erase(queue.begin());
      for (auto nb: node->neighbors) {
        if (visited.count(nb) == 0) {
          queue.push_back(nb);
          nb->parent = node;
        }
      }
    }
    return false;
  }

  bool  depth_first_search(std::function<bool(node_ptr_type)> call_back)
  {
    std::set<node_ptr_type> visited;
    std::vector<node_ptr_type> stack;

    stack.push_back(this);
    this->parent = nullptr;
    while (!stack.empty())
    {
      node_ptr_type node = stack.back();
      bool halt = call_back(node);
      if (halt)
        return true;
      visited.insert(node);
      stack.pop_back();
      for (auto nb: node->neighbors) {
        if (visited.count(nb) == 0) {
          stack.push_back(nb);
          nb->parent = node;
        }
      }
    }
    return false;
  }

  bool  a_star(
    std::function<float(const Node *, const Node *)> g,
    std::function<float(const Node *)> h,
    std::function<bool(Node *)> call_back
    )
  {
    //h(n): Heuristic Estimate of the Cost from Node n to the Goal
    //g(n): Cost from the Start Node to Node n
    std::set<node_ptr_type> visited;
    typedef std::tuple<node_ptr_type, float, float> t_node_cost;
    std::vector<t_node_cost> stack;

    this->parent = nullptr;
    stack.push_back(t_node_cost(this, 0.0, h(this)));

    while (!stack.empty())
    {
      auto node_info = stack.back();
      node_ptr_type node = std::get<0>(node_info);
      float cost = std::get<1>(node_info);
      float estimate_cost = std::get<2>(node_info);
      int x = node->get_data() % MAZE_SIZE;
      int y = node->get_data() / MAZE_SIZE;
      std::cout << "node: " << x << ", " << y << " const: " << cost <<  " estimate_cost: " << estimate_cost << std::endl;

      bool halt = call_back(node);
      if (halt)
        return true;
      visited.insert(node); //? 
      stack.pop_back();


      size_t old_size = stack.size();
      for (auto nb: node->neighbors) {
        if (visited.count(nb) == 0) {
          stack.push_back(t_node_cost(nb, cost + g(node, nb), h(nb)));
          nb->parent = node;
        }
      }

      std::sort(stack.begin(), stack.end(), [](const t_node_cost &a, const t_node_cost &b) {
        float f_a = std::get<1>(a) + std::get<2>(a);
        float f_b = std::get<1>(b) + std::get<2>(b);
        return f_a > f_b;
      });

      if (stack.size() - old_size > 1) {
        std::cout << "==========================" << std::endl;
        for (auto item : stack) {
          node_ptr_type node = std::get<0>(item);
          float cost = std::get<1>(item);
          float estimate_cost = std::get<2>(item);
          int x = node->get_data() % 15;
          int y = node->get_data() / 15;
          std::cout << "\tnode: " << x << ", " << y << " const: " << cost <<  " estimate_cost: " << estimate_cost << std::endl;
        }
        std::cout << "==========================" << std::endl;
      }

    }
    return false;
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

  node_ptr_type get_parent() const {
    return this->parent;
  }

};




#endif