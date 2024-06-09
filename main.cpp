#include "n_puzzle.h"
#include "Graph.hpp"


struct Global
{

  Global()
  {
    
  }
};


Global g;



class PuzzleState
{
public:
  static int              size;
  static PuzzleState      end_state;

public:
  std::vector<int>  list;
  std::string       hash;
  int               zero_index;
  std::vector<PuzzleState *>  neighbors;
  PuzzleState       *parent;


private:


public:
 PuzzleState() {}

  PuzzleState(const std::vector<int> &list) : list(list)
  {
    for (size_t i = 0; i < list.size(); i++)
    {
      int item = list[i];
      this->hash.append(std::to_string(item));
      this->hash.append("-");
      if (item == 0)
        zero_index = i;
    }
    this->hash.pop_back();
  }

  void propagate()
  {
    int                      size = PuzzleState::size;
    
    if (zero_index - size >= 0)
    {
      auto vec = this->list;
      std::swap(vec[zero_index], vec[zero_index - size]);
      this->neighbors.push_back(new PuzzleState(vec));
    }
    if ((zero_index % size) + 1 < size)
    {
      auto vec = this->list;
      std::swap(vec[zero_index], vec[zero_index + 1]);
      this->neighbors.push_back(new PuzzleState(vec));
    }
    if (zero_index + size < size * size)
    {
      auto vec = this->list;
      std::swap(vec[zero_index], vec[zero_index + size]);
      this->neighbors.push_back(new PuzzleState(vec));
    }
    if ((zero_index % size) - 1 >= 0)
    {
      auto vec = this->list;
      std::swap(vec[zero_index], vec[zero_index - 1]);
      this->neighbors.push_back(new PuzzleState(vec));
    }

  }

private:

  static std::vector<int> get_end_state(int size)
  {
    std::vector<int> target_state;

    for (int i = 1; i < size * size; i++) {
      target_state.push_back(i);
    }
    return (target_state);
  }

public:

  static PuzzleState init()
  {
    int nb;
    std::vector<int> list;
    int              size;
    std::string      res;

    std::getline(std::cin, res);
    std::cout << res << std::endl;
    res.clear();
    std::cin >> size;
    while (std::cin >> nb) {
      list.push_back(nb);
    }
    PuzzleState::size = size;
    PuzzleState::end_state = PuzzleState(PuzzleState::get_end_state(size));
    return PuzzleState(list);
  }

};

int PuzzleState::size;
PuzzleState PuzzleState::end_state;




bool  a_star(
  PuzzleState *root,
  std::function<float(const PuzzleState *, const PuzzleState *)> g,
  std::function<float(const PuzzleState *)> h,
  std::function<bool(PuzzleState *)> call_back
  )
{
  //h(n): Heuristic Estimate of the Cost from Node n to the Goal
  //g(n): Cost from the Start Node to Node n
  std::set<std::string> visited;
  typedef std::tuple<PuzzleState *, float, float> t_node_cost;
  std::vector<t_node_cost> stack;

  root->parent = nullptr;
  stack.push_back(t_node_cost(root, 0.0, h(root)));

  while (!stack.empty())
  {
    auto node_info = stack.back();
    PuzzleState* node = std::get<0>(node_info);
    float cost = std::get<1>(node_info);
    float es_cost = std::get<2>(node_info);
    //std::cout << cost << " " << es_cost << std::endl;

    bool halt = call_back(node);
    if (halt)
      return true;
    visited.insert(node->hash); //? 
    stack.pop_back();

    node->propagate();

    for (auto nb: node->neighbors) {
      if (visited.count(nb->hash) == 0) {
        stack.push_back(t_node_cost(nb, cost + g(node, nb), h(nb)));
        nb->parent = node;
      }
    }

    std::sort(stack.begin(), stack.end(), [](const t_node_cost &a, const t_node_cost &b) {
      float f_a = std::get<1>(a) + std::get<2>(a);
      float f_b = std::get<1>(b) + std::get<2>(b);
      return f_a > f_b;
    });

    

  }
  return false;
}



int main()
{
  PuzzleState state_0 = PuzzleState::init();


  auto g_fun = [](const PuzzleState *, const PuzzleState *) -> float {
    return (1);
  };



  auto h_fun = [](const PuzzleState *node) -> float {
    const std::vector<int> &list = node->list;
    const std::vector<int> &target_list = PuzzleState::end_state.list;

    float res = 0.0;

    for (size_t i = 0; i < list.size(); i++) {
      int item = list[i];
      int pos = i;
      auto it = std::find(target_list.begin(), target_list.end(), item);
      int target_pos = std::distance(target_list.begin(), it);
      int pos_x = pos % PuzzleState::size;
      int pos_y = pos / PuzzleState::size;
      int target_pos_x = target_pos % PuzzleState::size;
      int target_pos_y = target_pos / PuzzleState::size;
      res += (abs(pos_x - target_pos_x) + abs(pos_y - target_pos_y));
    }
    return (res);
  };

  auto call_back = [](PuzzleState *node) -> bool {
    //std::cout << node->hash << " ==? " << PuzzleState::end_state.hash << std::endl;
    if (node->hash == PuzzleState::end_state.hash) {
      return true;
    }
    return false;
  };

  a_star(&state_0, g_fun, h_fun, call_back);

  return 0;
}