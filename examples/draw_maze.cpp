#include "n_puzzle.h"
#include "./Sdl_class.hpp"
#include "./Drawer.hpp"

class Global
{
public:
  Sdl_class         &sdl;
  Drawer            &dr;
  std::vector<int>  maze;
  int               maze_size = 25;

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
void  generate_maze(std::vector<int> &maze, int size, std::function<bool(const std::vector<bool> &, int)> refresh)
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


    if (refresh(visited, cell) == true) //if quit == true
      return;
    SDL_Delay(5);

  }
}



void  draw_maze(Global *g, const std::vector<bool> &visited, int pos)
{
  int size = g->sdl.width / g->maze_size;

  for (int i = 0; i < g->maze_size; i++){
    for (int j = 0; j < g->maze_size; j++) {
      int color = 0x00ff00;
      int cell_index = j * g->maze_size + i;
      if (visited[cell_index])
        color = 0xff0000;
      if (pos == cell_index)
        color = 0x0000ff;
      g->dr.set_color(color);
      g->dr.rectangle(i * size, j * size, size, size);
    }
  }

  g->dr.set_color(0xffffff);
  for (int i = 0; i < g->maze_size; i++){
    for (int j = 0; j < g->maze_size; j++){
      g->dr.rectangle_hollow(i * size, j * size, size, size, 1, g->maze[j * g->maze_size + i]);
    }
  }

}



void  loop(Global *g)
{
  bool done = false;

  while (!g->sdl.quit)
  {
    //g->sdl.clear();

    if (done == false) 
    {
      generate_maze(g->maze, g->maze_size, [&g](const std::vector<bool> &visited, int pos) {
        //std::cout << "Called" << std::endl;
        g->sdl.clear();
        draw_maze(g, visited, pos);
        g->sdl.flush();
        g->sdl.events();
        //std::cout << g->sdl.buffer_input[0] << std::endl;
        // while (g->sdl.buffer_input[0] != 79) {
        //   g->sdl.events();
        //   SDL_Delay(2);
        // }
        return g->sdl.quit;
      });
      done = true;
    }
    g->sdl.flush();
    g->sdl.events();
  }
}

int main()
{
  Sdl_class sdl_class = Sdl_class("N_Puzzle", 800, 800);
  Drawer    dr = Drawer(sdl_class);
  Global    g = Global(sdl_class, dr);


  loop(&g);
  sdl_class.exit();
  return 0;
}