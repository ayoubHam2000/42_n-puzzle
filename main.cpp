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

  // static std::vector<int> get_end_state(int size)
  // {
  //   std::vector<int> target_state;

  //   for (int i = 1; i < size * size; i++) {
  //     target_state.push_back(i);
  //   }
  //   target_state.push_back(0);
  //   return (target_state);
  // }

  static std::vector<int> get_end_state(int size)
  {
    std::vector<int> target_state(size * size, 0);
    int directions[][2] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

    int x = 0;
    int y = 0;
    int dir = 0;

    for (int i = 1; i < size * size; i++) {
      int index = x + y * size;
      target_state[index] = i;
      int next_x = x + directions[dir][0]; 
      int next_y = y + directions[dir][1];
      if (next_x < 0 || next_x >= size || next_y < 0 || next_y >= size || target_state[next_x + next_y * size] != 0)
      {
        dir = (dir + 1) % 4;
        next_x = x + directions[dir][0]; 
        next_y = y + directions[dir][1];
      }
      x = next_x;
      y = next_y;
    }
    
    return target_state;
  }

public:

  static PuzzleState init(std::istream &istream)
  {
    int nb;
    std::vector<int> list;
    int              size;
    std::string      res;

    std::getline(istream, res);
    // std::cout << res << std::endl;
    res.clear();
    istream >> size;
    while (istream >> nb) {
      list.push_back(nb);
    }
    PuzzleState::size = size;
    PuzzleState::end_state = PuzzleState(PuzzleState::get_end_state(size));
    return PuzzleState(list);
  }

};

int PuzzleState::size;
PuzzleState PuzzleState::end_state;


void  print_puzzle(const PuzzleState &state)
{
  for (int i = 0; i < PuzzleState::size; i++) {
    for (int j = 0; j < PuzzleState::size; j++) {
      int index = j + i * PuzzleState::size;
      int item = state.list[index];
      int true_item = PuzzleState::end_state.list[index];
      if (item == 0) {
        std::cout << "\033[32m " << item << " \033[0m";
      } else if (item != true_item) {
        std::cout << "\033[31m " << item << " \033[0m";
      } else {
        std::cout << " " <<  item << " ";
      }
    }
    std::cout << std::endl;
  }

}

bool  a_star(
  PuzzleState *root,
  std::function<float(const PuzzleState *, const PuzzleState *)> g,
  std::function<float(const PuzzleState *)> h,
  std::function<bool(PuzzleState *, size_t)> call_back
  )
{
  //h(n): Heuristic Estimate of the Cost from Node n to the Goal
  //g(n): Cost from the Start Node to Node n
  std::set<std::string> visited;
  typedef std::tuple<PuzzleState *, float, float> t_node_cost;
  std::vector<t_node_cost> stack;

  root->parent = nullptr;
  stack.push_back(t_node_cost(root, 0.0, h(root)));
  visited.insert(root->hash); 
  
  while (!stack.empty())
  {
    auto node_info = stack.back();
    PuzzleState* node = std::get<0>(node_info);
    float cost = std::get<1>(node_info);
    float es_cost = std::get<2>(node_info);
    //system("clear");
    //std::cout << visited.size() << " " << cost << " " << es_cost << " " << cost + es_cost << std::endl;

    bool halt = call_back(node, visited.count(node->hash));
    if (halt)
      return true;
    //visited.insert(node->hash); //? 
    stack.pop_back();

    node->propagate();

    if (visited.size() % 100 == 0) {
      std::cout << visited.size() << std::endl;
      print_puzzle(*node);
    }

    //print_puzzle(*node);
    //std::cout << std::endl;
    size_t old_size = stack.size();
    for (auto nb: node->neighbors) {
      if (visited.count(nb->hash) == 0) {
        int nb_cost = cost + g(node, nb);
        int nb_es_cost = h(nb); 
        //std::cout << "   " << nb_cost << " " << nb_es_cost << " = " << nb_cost + nb_es_cost << std::endl;
        //print_puzzle(*nb);
        //std::cout << nb->hash << std::endl;
        stack.push_back(t_node_cost(nb, nb_cost, nb_es_cost));
        visited.insert(nb->hash);
        nb->parent = node;
      }
    }
    // {
    //   int nb;
    //   std::cin >> nb;
    // }
    //exit(1);

    std::sort(stack.begin() + old_size, stack.end(), [](const t_node_cost &a, const t_node_cost &b) {
      float f_a = std::get<1>(a) + std::get<2>(a);
      float f_b = std::get<1>(b) + std::get<2>(b);
      return f_a > f_b;
    });
  }
  return false;
}

void  solve(PuzzleState &state_0)
{
  auto g_fun = [](const PuzzleState *, const PuzzleState *) -> float {
    return (0);
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
      if (item != 0)
        res += (abs(pos_x - target_pos_x) + abs(pos_y - target_pos_y));
    }
    return (res);
  };

  auto call_back = [](PuzzleState *node, size_t c) -> bool {
    //std::cout << node->hash << " ==? " << PuzzleState::end_state.hash << std::endl;
    //std::cout << node->hash << " " <<  c << std::endl;
    if (node->hash == PuzzleState::end_state.hash) {
      std::cout <<  "Solved" << std::endl;
      print_puzzle(*node);
      return true;
    }
    return false;
  };


  bool res = a_star(&state_0, g_fun, h_fun, call_back);
  std::cout << res << std::endl;
}

int main(int ac, char **av)
{
  PuzzleState state_0;

  if (ac == 2) {
    std::ifstream file(av[1]);
    if (!file.is_open()) {
      std::cerr << "Can't opening file" << std::endl;
      return (1);
    }
    state_0 = PuzzleState::init(file);
    file.close();
  } else {
    state_0= PuzzleState::init(std::cin);
  }
  //print_puzzle(PuzzleState::end_state);
  solve(state_0);
  return 0;
}