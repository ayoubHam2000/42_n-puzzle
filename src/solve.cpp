#include "n_puzzle.h"

static void  construct_path(PuzzleState *end_state, const Configuration &conf)
{
  PuzzleState *node = end_state;
  std::vector<PuzzleState *> list;

  std::cout <<  "=============== Solved ===================" << std::endl;
  std::cout <<  "=============== Solved ===================" << std::endl;
  list.push_back(node);
  while (node->parent != nullptr)
  {
    list.push_back(node->parent);
    node = node->parent;
  }
  std::reverse(list.begin(), list.end());

  conf.print();
  std::cout << "Complexity in time: " << PuzzleState::complexity_in_time << std::endl;
  std::cout << "Complexity in size: " << PuzzleState::complexity_in_size << std::endl;
  std::cout << "Number of moves: " << list.size() << std::endl;

  std::cout << "Press enter to show the result ..." << std::endl;
  getchar();
  for (auto item : list) {
   std::cout << "\x1B[2J\x1B[H";
   item->print();
   std::cout << std::endl;
   getchar();
  }
}

//===================================================
// Heuristics =======================================
//===================================================


float manhattan_distance(const PuzzleState *node)
{
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
}

float euclidean_distance(const PuzzleState *node)
{
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
    {
      float dist_x = pos_x - target_pos_x;
      float dist_y = pos_y - target_pos_y;
      res += (sqrt(dist_x * dist_x + dist_y * dist_y));
    }
  }
  return (res);
}

float hamming_distance(const PuzzleState *node)
{
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
    {
      res += (pos_x != target_pos_x && pos_y != target_pos_y);
    }
  }
  return (res);
}

float linear_conflicts(const PuzzleState *node)
{
  const int size = PuzzleState::size;
  const std::vector<int> &list = node->list;
  const std::vector<int> &target_list = PuzzleState::end_state.list;
  int conflicts = 0;

  auto find_pos = [&target_list, size](int i, int item, int p) -> int
  {
    if (item == 0)
      return -1;
    for (int j = 0; j < size; j++)
    {
      int index = j + i * size;
      if (p == 1)
        index = i + j * size;
      if (item == target_list[index])
        return (j);
    }
    return -1; 
  };

  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      for (int k = j + 1; k < size; k++)
      {
        int a_pos, b_pos;

        a_pos = find_pos(i, list[j + i * size], 0);
        b_pos = find_pos(i, list[k + i * size], 0);
        if (a_pos != -1 && b_pos != -1 && a_pos > b_pos)
          conflicts++;

        a_pos = find_pos(i, list[i + j * size], 1);
        b_pos = find_pos(i, list[i + k * size], 1);
        if (a_pos != -1 && b_pos != -1 && a_pos > b_pos)
          conflicts++;
      }
    }
  }

  return manhattan_distance(node) + (conflicts * 2);
}

float zero_heuristic(const PuzzleState *)
{
  return (0.0);
}

//===================================================
// Cost =============================================
//===================================================

float normal_cost(const PuzzleState *, const PuzzleState *)
{
  return (1.0);
}

float zero_cost(const PuzzleState *, const PuzzleState *)
{
  return (0.0);
}

//===================================================
//===================================================
//===================================================

std::function<float(const PuzzleState *)> get_heuristic_function(const Configuration &conf)
{
  if (conf.search_type == Configuration::SearchType::UniformSearch)
    return (zero_heuristic);
  else if (conf.heuristics == Configuration::Heuristics::Hamming)
    return (hamming_distance);
  else if (conf.heuristics == Configuration::Heuristics::Euclidean)
    return (euclidean_distance);
  else if (conf.heuristics == Configuration::Heuristics::Conflicts)
    return (linear_conflicts);
  return (manhattan_distance);
}

std::function<float(const PuzzleState *, const PuzzleState *)> get_cost_function(const Configuration &conf)
{
  if (conf.search_type == Configuration::SearchType::GreedySearch)
    return (zero_cost);
  return (normal_cost);
}

void  solve(PuzzleState &state_0, Configuration &conf)
{
  auto call_back = [&conf](PuzzleState *node) -> bool {
    if (node->hash == PuzzleState::end_state.hash) {
      construct_path(node, conf);
      return true;
    }
    return false;
  };

  auto g_fun = get_cost_function(conf);
  auto h_fun = get_heuristic_function(conf);

  a_star(&state_0, g_fun, h_fun, call_back);
}