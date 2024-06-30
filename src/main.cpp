#include "n_puzzle.h"

int main(int ac, char **av)
{
  PuzzleState state_0;
  Configuration conf(ac, av);

  if (!conf.file_name.empty())
  {
    std::ifstream file(conf.file_name);
    if (!file.is_open()) {
      std::cerr << "Can't opening file" << std::endl;
      return (1);
    }
    state_0 = PuzzleState::init(file);
    file.close();
  } 
  else
  {
    std::cerr << "must provide the puzzle file using -f" << std::endl;
    exit(1);
    //state_0 = PuzzleState::init(std::cin);
  }


  if (is_solvable(state_0.list, PuzzleState::end_state.list, PuzzleState::size) == true)
  {
    solve(state_0, conf);
  }
  else
  {
    std::cout << "Can't be solved" << std::endl;
  }
  return 0;
}