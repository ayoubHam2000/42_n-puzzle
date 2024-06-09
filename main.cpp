#include "n_puzzle.h"
#include "Sdl_class.hpp"
#include "Drawer.hpp"
#include "Graph.hpp"

#include <iostream>
#include <string>
#include <bitset>

std::string toBinaryString(int number) {
    const size_t BITS = sizeof(int);  // Number of bits in an int
    std::bitset<BITS> bitset(number);
    return bitset.to_string();
}


struct Global
{
  Sdl_class         sdl;
  Drawer            dr;
  std::vector<int>  maze;
  int               maze_size = MAZE_SIZE;

  Global(): 
  sdl("N_puzzle", 800, 800), 
  dr(sdl)
  {
    this->maze.resize(maze_size * maze_size, 0b1111); //top right bottom left
  }
};


Global g;



/**
 * Generate a 2D maze with all paths traversed
 * The generated maze is an array of intergers each integer represent the active and deactivate walls
 * as top right bottom left with binary form 
*/
void  generate_maze(std::vector<int> &maze, int size)
{
  std::vector<int>  stack;
  std::vector<int>  neighbors;
  std::vector<bool> visited;

  std::mt19937 seed = std::mt19937{0};
  visited.resize(size * size, false);
  stack.reserve(size * size);

  stack.push_back(0);
  while (!stack.empty()){
    int cell;
    int next_cell;

    cell = stack.back();
    visited[cell] = true;

    neighbors.clear();
    if (cell - size >= 0 && visited[cell - size] == false)
      neighbors.push_back(cell - size);
    if ((cell % size) + 1 < size && visited[cell + 1] == false)
      neighbors.push_back(cell + 1);
    if (cell + size < size * size && visited[cell + size] == false)
      neighbors.push_back(cell + size);
    if ((cell % size) - 1 >= 0 && visited[cell - 1] == false)
      neighbors.push_back(cell - 1);
    

    //shuffle
    std::shuffle(neighbors.begin(), neighbors.end(), seed);

    if (!neighbors.empty())
    {
      stack.push_back(neighbors.back());
      next_cell = neighbors.back();

      //break the wall
      if (next_cell == cell + size){
        maze[cell] &= 0b1101;
        maze[next_cell] &= 0b0111;
      }
      else if (next_cell == cell + 1){
        maze[cell] &= 0b1011;
        maze[next_cell] &= 0b1110;
      }
      else if (next_cell == cell - size){
        maze[cell] &= 0b0111;
        maze[next_cell] &= 0b1101;
      }
      else if (next_cell == cell - 1){
        maze[cell] &= 0b1110;
        maze[next_cell] &= 0b1011;
      }
    } else {
      stack.pop_back();
    }
  }
}



void  draw_maze()
{
  int size = g.sdl.width / g.maze_size;


  g.dr.set_color(0xffffff);
  for (int i = 0; i < g.maze_size; i++){
    for (int j = 0; j < g.maze_size; j++){
      g.dr.rectangle_hollow(i * size, j * size, size, size, 1, g.maze[j * g.maze_size + i]);
    }
  }

}





void   debug(Node *node)
{
  int size = g.sdl.width / g.maze_size;


  g.sdl.clear();
  g.dr.set_color(0xffffff);
  for (int i = 0; i < g.maze_size; i++){
    for (int j = 0; j < g.maze_size; j++){
      g.dr.rectangle_hollow(i * size, j * size, size, size, 1, g.maze[j * g.maze_size + i]);
    }
  }

  {
    g.dr.set_color(0xffff00);
    int cell = node->get_data();
    int i  = cell % (g.maze_size);
    int j = cell / g.maze_size;
    g.dr.rectangle(i * size, j * size, size, size);
  }

  g.dr.set_color(0x0000ff);
  for (auto nb : node->get_neighbors()) {
    int cell = nb->get_data();
    int i  = cell % (g.maze_size);
    int j = cell / g.maze_size;
    g.dr.rectangle(i * size, j * size, size, size);
  }

  g.sdl.flush();
 
  SDL_Delay(100);
  while (true) {
    g.sdl.events();
    if (g.sdl.is_in_buffer(79)) {
      break ;
    }
    SDL_Delay(50);
    if (g.sdl.quit == true)
      std::exit(0);
  }
  if (g.sdl.quit == true)
    std::exit(0);
}

Node  *initialize_graph()
{
  std::vector<int> &maze = g.maze;
  int size = g.maze_size;
  Node **nodes = new Node*[size * size];

  for (int i = 0; i < size * size; i++)
    nodes[i] = nullptr;

  auto get_node = [&nodes](int cell) -> Node * {
    if (nodes[cell] == nullptr) {
      nodes[cell] = new Node(cell);
    }
    return nodes[cell];
  };

  for (int j = 0; j < size; j++) {
    for (int i = 0; i < size; i++) {
      int cell = i + j * size;
      std::cout << cell << std::endl;
      Node *node = get_node(cell);

      if ((~maze[cell] & 0b1000))
        node->add_neighbor(get_node(cell - size));
      if ((~maze[cell] & 0b0100))
        node->add_neighbor(get_node(cell + 1));
      if ((~maze[cell] & 0b0010))
        node->add_neighbor(get_node(cell + size));
      if ((~maze[cell] & 0b0001))
        node->add_neighbor(get_node(cell - 1));

      //debug(node);
      
      if (node->get_neighbors().size() == 0) {
        std::cout <<  "!!" << std::endl;
        delete node;
        nodes[cell] = nullptr;
      }
    }
  }

  Node *root = nodes[0];
  // for (int i = 0; i < size * size; i++) {
  //   debug(nodes[i]);
  // }
  delete nodes;

  std::cout <<  "Done" << std::endl;

  return root;
}


void  debug_breath_first(int cell)
{
  int size = g.sdl.width / g.maze_size;


  //g.sdl.clear();
  g.dr.set_color(0xffffff);
  for (int i = 0; i < g.maze_size; i++){
    for (int j = 0; j < g.maze_size; j++){
      g.dr.rectangle_hollow(i * size, j * size, size, size, 1, g.maze[j * g.maze_size + i]);
    }
  }

  g.dr.set_color(0xff0000);
  int i  = cell % (g.maze_size);
  int j = cell / g.maze_size;
  g.dr.rectangle(i * size, j * size, size, size);

  g.sdl.flush();

  SDL_Delay(100);
  while (true) {
    g.sdl.events();
    if (g.sdl.is_in_buffer(79)) {
      break ;
    }
    SDL_Delay(50);
    if (g.sdl.quit == true)
      std::exit(0);
  }
  if (g.sdl.quit == true)
    std::exit(0);
}

void  construct_the_path(Node *node)
{
  Node *parent = node->get_parent();
  g.dr.set_color(0xffff00);
  while (parent != nullptr) {
    int size = g.sdl.width / g.maze_size;
    int cell = parent->get_data();
    int i  = cell % (g.maze_size);
    int j = cell / g.maze_size;
    g.dr.rectangle(i * size, j * size, size - 2, size - 2);
    g.dr.set_color(0xff00ff);
    //SDL_Delay(100);
    g.sdl.flush();
    g.sdl.events();
    parent = parent->get_parent();
  }
}

void  breath_first_search(Node *root)
{
  std::cout << "start breath first search" << std::endl;

  Node *result = nullptr;

  
  bool is_found = root->breath_first_search([&result](Node *node) -> bool {
    int the_cell = node->get_data();
    std::cout << the_cell << " size: " << node->get_neighbors().size() << std::endl;
    //debug_breath_first(the_cell);
    if (the_cell == 49) {
      std::cout << "found !!" << std::endl;
      result = node;
      return true;
    }
    return false;
  });

  is_found = result->breath_first_search([&result](Node *node) -> bool {
    int the_cell = node->get_data();
    std::cout << the_cell << " size: " << node->get_neighbors().size() << std::endl;
    debug_breath_first(the_cell);
    if (the_cell == 624) {
      std::cout << "found !!" << std::endl;
      result = node;
      return true;
    }
    return false;
  });

  std::cout << "Is found " << is_found << std::endl;
  if (is_found)
    construct_the_path(result);
  

}

void  a_star(Node *root)
{

  

  std::cout << "start a_star search" << std::endl;

  Node *result = nullptr;

  
  auto g_fun = [](const Node *, const Node *) -> float {
    return (1.0);
  };

  int target_cell = 24 * g.maze_size + 24;

  auto h_fun = [target_cell](const Node *node) -> float {
    int x_a = node->get_data() % g.maze_size;
    int y_a = node->get_data() / g.maze_size;
    int x_b = target_cell % g.maze_size;
    int y_b = target_cell / g.maze_size;
    // std::cout << x_a << " " << y_a << " " << x_b << " " << y_b << std::endl;
    float estimate_cost = (abs(x_a - x_b) + abs(y_a - y_b));
    // std::cout << "estimate cost: " << estimate_cost << std::endl;
    return (estimate_cost);
  };

  bool is_found = root->breath_first_search([&result](Node *node) -> bool {
    int the_cell = node->get_data();
    std::cout << the_cell << " size: " << node->get_neighbors().size() << std::endl;
    //debug_breath_first(the_cell);
    if (the_cell == 49) {
      std::cout << "found !!" << std::endl;
      result = node;
      return true;
    }
    return false;
  });


  auto call_back = [&result, target_cell](Node *node) -> bool {
    int the_cell = node->get_data();
    std::cout << the_cell << " size: " << node->get_neighbors().size() << std::endl;
    debug_breath_first(the_cell);
    if (the_cell == target_cell) {
      std::cout << "found !!" << std::endl;
      result = node;
      return true;
    }
    return false;
  };

  is_found = result->a_star(g_fun, h_fun, call_back);
  std::cout << "Is found " << is_found << std::endl;
  if (is_found)
    construct_the_path(result);
  

}


int main()
{
  generate_maze(g.maze, g.maze_size);
  Node *root = initialize_graph();
  draw_maze();
  breath_first_search(root);
  //a_star(root);
  while (!g.sdl.quit)
  {
    //g.sdl.clear();

    g.sdl.flush();
    g.sdl.events();
  }
  g.sdl.exit();
  return 0;
}