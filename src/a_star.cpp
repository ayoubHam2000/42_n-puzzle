#include "n_puzzle.h"

class AStarNode
{
public:
  PuzzleState *node;
  float g_cost;
  float h_cost;
public:
  AStarNode(PuzzleState *node, float g_cost, float h_cost):
    node(node), g_cost(g_cost), h_cost(h_cost) {}
};

struct AStarNodeComparator {
    bool operator()(const AStarNode &a, const AStarNode &b) const {
      float f_a = a.g_cost + a.h_cost;
      float f_b = b.g_cost + b.h_cost;
      return f_a > f_b;
    }
};

/*
  g(n): Cost from the Start Node to Node n
  h(n): Heuristic Estimate of the Cost from Node n to the Goal
*/
bool  a_star(
  PuzzleState *root,
  std::function<float(const PuzzleState *, const PuzzleState *)> g,
  std::function<float(const PuzzleState *)> h,
  std::function<bool(PuzzleState *)> call_back
  )
{

  std::set<std::string> visited;
  

  std::priority_queue<AStarNode, std::vector<AStarNode>, AStarNodeComparator> priority_queue;

  root->parent = nullptr;
  priority_queue.push(AStarNode(root, 0.0, h(root)));
  visited.insert(root->hash); 
  
  while (priority_queue.size() != 0)
  {
    size_t queue_size = priority_queue.size();
    if (PuzzleState::complexity_in_size < queue_size)
    {
      PuzzleState::complexity_in_size = queue_size;
    }

    auto node_info = priority_queue.top();
    PuzzleState* node = node_info.node;
    float cost = node_info.g_cost;

    PuzzleState::complexity_in_time = visited.size();
    if (call_back(node) == true) {
      return true;
    }
    priority_queue.pop();

    node->propagate();
    for (auto nb: node->neighbors) {
      if (visited.count(nb->hash) == 0) {
        float nb_cost = cost + g(node, nb);
        float nb_es_cost = h(nb); 
        priority_queue.push(AStarNode(nb, nb_cost, nb_es_cost));
        visited.insert(nb->hash);
        nb->parent = node;
      }
    }
  }
  return false;
}
