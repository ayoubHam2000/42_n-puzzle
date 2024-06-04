#include "n_puzzle.h"
#include "Sdl_class.hpp"
#include "Drawer.hpp"
#include "Graph.hpp"

class Global
{
public:
  Sdl_class         &sdl;
  Drawer            &dr;
  std::vector<int>  maze;
  int               maze_size = 25;
  //Node              &gh;

private:
  Global();

public:
  Global(Sdl_class &sdl_class, Drawer &drawer): sdl(sdl_class), dr(drawer) {
    this->maze.resize(maze_size * maze_size, 0b1111); //top right bottom left
  }
};

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

  std::mt19937 seed = std::mt19937{std::random_device{}()};
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



void  draw_maze(Global *g)
{
  int size = g->sdl.width / g->maze_size;


  g->dr.set_color(0xffffff);
  for (int i = 0; i < g->maze_size; i++){
    for (int j = 0; j < g->maze_size; j++){
      g->dr.rectangle_hollow(i * size, j * size, size, size, 1, g->maze[j * g->maze_size + i]);
    }
  }

}


#include <iostream>
#include <string>
#include <bitset>
std::string toBinaryString(int number) {
    const size_t BITS = sizeof(int);  // Number of bits in an int
    std::bitset<BITS> bitset(number);
    return bitset.to_string();
}


void   debug(Global *g, int cell)
{
  int size = g->sdl.width / g->maze_size;


  g->sdl.clear();
  g->dr.set_color(0xffffff);
  for (int i = 0; i < g->maze_size; i++){
    for (int j = 0; j < g->maze_size; j++){
      g->dr.rectangle_hollow(i * size, j * size, size, size, 1, g->maze[j * g->maze_size + i]);
    }
  }
  g->dr.set_color(0xff0000);
  int i  = cell % (g->maze_size);
  int j = cell / g->maze_size;
  g->dr.rectangle(i * size, j * size, size, size);

      
  g->sdl.flush();
  int up = ~g->maze[cell] & 0b1000;
  int right = ~g->maze[cell] & 0b0100;
  int down = ~g->maze[cell] & 0b0010;
  int left = ~g->maze[cell] & 0b0001;
  std::cout << "cell " << cell << " values " << toBinaryString(g->maze[cell]) << " up " << up << " right " << right << " down " << down << "left " << left << std::endl;
  
  SDL_Delay(100);
  while (true) {
    g->sdl.events();
    if (g->sdl.is_in_buffer(79)) {
      break ;
    }
    SDL_Delay(50);
  }
  if (g->sdl.quit == true)
    std::exit(0);
}

void   initialize_graph_rec(Global *g, Node *node, std::vector<int> &maze, std::vector<bool> &visited, int size)
{
  int cell = node->get_data();
  visited[cell] = true;

  //debug(g, cell);

  int a = maze[cell];
  if ((~maze[cell] & 0b1000) && visited[cell - size] == false)
    node->add_neighbor(new Node(cell - size));
  if ((~maze[cell] & 0b0100) && visited[cell + 1] == false)
    node->add_neighbor(new Node(cell + 1));
  if ((~maze[cell] & 0b0010) && visited[cell + size] == false)
    node->add_neighbor(new Node(cell + size));
  if ((~maze[cell] & 0b0001) && visited[cell - 1] == false)
    node->add_neighbor(new Node(cell - 1));
  for (auto n : node->get_neighbors()) {
    //if (visited[n->get_data()] == false) {
      std::cout << "explore for: " << n->get_data() << std::endl;
      initialize_graph_rec(g, n, maze, visited, size);
    //}
  }
}

Node  *initialize_graph(Global *g)
{
  Node *root = new Node(0);
  std::vector<bool> visited;
  
  visited.resize(g->maze_size * g->maze_size, false);
  initialize_graph_rec(g, root, g->maze, visited, g->maze_size);
  return root;
}

void  loop(Global *g)
{
  //bool done = false;

  generate_maze(g->maze, g->maze_size);
  Node *root = initialize_graph(g);
  while (!g->sdl.quit)
  {
    g->sdl.clear();
    draw_maze(g);
    g->sdl.flush();
    g->sdl.events();
  }
}

int main()
{
  std::vector<int> v;
  v.reserve(10);
  v.push_back(2222222);

  int *a = &v[0];

  std::string str = "AYOUB";
  

  Sdl_class sdl_class = Sdl_class("N_Puzzle", 800, 800);
  Drawer    dr = Drawer(sdl_class);
  Global    g = Global(sdl_class, dr);


  loop(&g);
  sdl_class.exit();
  return 0;
}