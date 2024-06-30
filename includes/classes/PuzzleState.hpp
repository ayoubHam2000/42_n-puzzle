#ifndef PuzzleState_HPP
# define PuzzleState_HPP

class PuzzleState
{
public:
  static int              size;
  static PuzzleState      end_state;
  static size_t              complexity_in_time;
  static size_t              complexity_in_size;

public:
  std::vector<int>  list;
  std::string       hash;
  int               zero_index;
  std::vector<PuzzleState *>  neighbors;
  PuzzleState       *parent;

public:
  PuzzleState();
  PuzzleState(const std::vector<int> &list);
  void  propagate();
  void  print() const;

private:
  static std::vector<int> get_end_state(int size);

public:
  static PuzzleState init(std::istream &istream);

};

#endif
