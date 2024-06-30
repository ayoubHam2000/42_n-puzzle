#include "n_puzzle.h"

int PuzzleState::size;
PuzzleState PuzzleState::end_state;
size_t PuzzleState::complexity_in_time;
size_t PuzzleState::complexity_in_size;

PuzzleState::PuzzleState() {}

PuzzleState::PuzzleState(const std::vector<int> &list) : list(list)
{
  for (size_t i = 0; i < list.size(); i++)
  {
    int item = list[i];
    this->hash.append(std::to_string(item));
    this->hash.append("-");
    if (item == 0)
      this->zero_index = i;
  }
  this->hash.pop_back();
}

void PuzzleState::propagate()
{
  int size = PuzzleState::size;
  
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

std::vector<int> PuzzleState::get_end_state(int size)
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


PuzzleState PuzzleState::init(std::istream &istream)
{
  int nb;
  std::vector<int> list;
  int              size;
  std::string      res;

  std::getline(istream, res);
  res.clear();
  istream >> size;
  while (istream >> nb) {
    list.push_back(nb);
  }
  PuzzleState::size = size;
  PuzzleState::end_state = PuzzleState(PuzzleState::get_end_state(size));
  return PuzzleState(list);
}

void  PuzzleState::print() const
{
  int buffer_size = std::to_string(PuzzleState::size * PuzzleState::size - 1).size() + 2;

  auto print_center = [](const std::string &s, int buffer)
  {
    int m = buffer / 2;
    int i = 0;
    for (; i < m; i++)
      std::cout <<  ' ';
    std::cout << s;
    i += s.size();
    for (; i < buffer; i++)
      std::cout <<  ' ';
  };

  for (int i = 0; i < PuzzleState::size; i++) {
    for (int j = 0; j < PuzzleState::size; j++) {
      int index = j + i * PuzzleState::size;
      int item = this->list[index];
      print_center(std::to_string(item), buffer_size);
      //std::cout << " " <<  item << " ";
     
    }
    std::cout << std::endl;
  }
}

// void  PuzzleState::print()
// {
//   for (int i = 0; i < PuzzleState::size; i++) {
//     for (int j = 0; j < PuzzleState::size; j++) {
//       int index = j + i * PuzzleState::size;
//       int item = this->list[index];
//       int true_item = PuzzleState::end_state.list[index];
//       if (item == 0) {
//         std::cout << "\033[32m " << item << " \033[0m";
//       } else if (item != true_item) {
//         std::cout << "\033[31m " << item << " \033[0m";
//       } else {
//         std::cout << " " <<  item << " ";
//       }
//     }
//     std::cout << std::endl;
//   }
// }
