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
  int               maze_size = 15;

  Global(): 
  sdl("N_puzzle", 800, 800), 
  dr(sdl)
  {
    this->maze.resize(maze_size * maze_size, 0b1111); //top right bottom left
  }
};


Global g;

int main() {



  //g.maze_size = 15;
  //g.maze.resize(g.maze_size * g.maze_size, 0b1111);
  //generate_maze(g->maze, g->maze_size);
  //Node *root = initialize_graph();
  //draw_maze();
}