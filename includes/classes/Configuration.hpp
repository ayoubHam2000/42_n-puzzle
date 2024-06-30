#ifndef Configuration_hpp
#define Configuration_hpp

struct Configuration
{
public:
  enum Heuristics {
    Manhattan,
    Hamming, //tiles out of place
    Euclidean,
    Conflicts
  };
  
  enum SearchType {
    GreedySearch,
    UniformSearch,
    AStarSearch,
  };

public:
  int   heuristics;
  int   search_type;
  bool  display_solution;
  std::string file_name;

public:
  Configuration(int ac, char **av);

public:
  void print() const;

public:
  static void help();

};

#endif