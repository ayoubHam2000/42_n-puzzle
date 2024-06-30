#ifndef N_PUZZLE_H
#define N_PUZZLE_H

# include "SDL2/SDL.h"
# include "SDL2/SDL_image.h"
# include <iostream>
# include <string>
# include <vector>
# include <map>
# include <algorithm>
# include <random>
# include <functional>
# include <cmath>
# include <set>
# include <sstream>
# include <fstream>
# include <queue>

# include "classes/PuzzleState.hpp"
# include "classes/Configuration.hpp"

bool is_solvable(const std::vector<int> &initial_state, const std::vector<int> &end_state, int puzzle_size);
bool  a_star(
  PuzzleState *root,
  std::function<float(const PuzzleState *, const PuzzleState *)> g,
  std::function<float(const PuzzleState *)> h,
  std::function<bool(PuzzleState *)> call_back
);
void  solve(PuzzleState &state_0, Configuration &conf);

#endif
