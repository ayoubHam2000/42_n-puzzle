#include "n_puzzle.h"

int count_inversions(const std::vector<int> &list)
{
  int nb_inversions = 0;

  for (size_t i = 0; i < list.size(); i++)
  {
    int a = list[i];
    for (size_t j = i + 1; j < list.size(); j++) {
      int b = list[j];
      if (a > b && a != 0 && b != 0) {
        nb_inversions++;
      }
    }
  }

  return nb_inversions;
}

int get_puzzle_blank_from_bottom(const std::vector<int> &list, int puzzle_size)
{
  for (size_t i = 0; i < list.size(); i++)
  {
    if (list[i] == 0)
    {
      return (puzzle_size - i / puzzle_size);
    }
  }
  return (0);
}


bool is_solvable(const std::vector<int> &initial_state, const std::vector<int> &end_state, int puzzle_size)
{
  int initial_state_inversions = count_inversions(initial_state);
  int end_state_inversions = count_inversions(end_state);
  int initial_state_puzzle_blank_from_bottom = get_puzzle_blank_from_bottom(initial_state, puzzle_size);
  int end_state_puzzle_blank_from_bottom = get_puzzle_blank_from_bottom(end_state, puzzle_size);
  
  int diff = abs(end_state_puzzle_blank_from_bottom - initial_state_puzzle_blank_from_bottom);
  if (puzzle_size % 2 != 0)
  {
    return (initial_state_inversions % 2 == end_state_inversions % 2);
  }
  else
  {
    return ((initial_state_inversions + diff) % 2 == end_state_inversions % 2);
  }
}
