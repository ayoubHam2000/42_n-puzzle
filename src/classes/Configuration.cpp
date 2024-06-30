#include "n_puzzle.h"

Configuration::Configuration(int ac, char **av)
{
  if (ac == 1)
  {
    Configuration::help();
    exit(0);
  }

  this->heuristics = -1;
  this->search_type = SearchType::AStarSearch;

  for (int i = 1; i < ac; i++)
  {
    if (std::string(av[i]) == "--manhattan")
      this->heuristics = Heuristics::Manhattan;
    else if (std::string(av[i]) == "--hamming")
      this->heuristics = Heuristics::Hamming;
    else if (std::string(av[i]) == "--euclidean")
      this->heuristics = Heuristics::Euclidean;
    else if (std::string(av[i]) == "--conflicts")
      this->heuristics = Heuristics::Conflicts;
    else if (std::string(av[i]) == "-u")
      this->search_type = SearchType::UniformSearch;
    else if (std::string(av[i]) == "-g")
      this->search_type = SearchType::GreedySearch;
    else if (std::string(av[i]) == "-f")
    {
      if (i + 1 < ac)
      {
        this->file_name = av[i + 1];
        i++;
      }
      else
      {
        std::cerr << "Must provide a file name" << std::endl;
        Configuration::help();
        exit(1);
      }
    }
  }

  if (this->heuristics == -1) {
    std::cerr << "Must provide a heuristic function" << std::endl;
    Configuration::help();
    exit(1);
  }
}

void Configuration::help()
{
  std::ifstream file("./src/help.txt");
  if (!file.is_open()) {
    std::cerr << "I Can't help you" << std::endl; 
  } else {
    std::string res;
    char buffer[100];
    while (file.read(buffer, 100)) {
      res.append(buffer, 0, file.gcount());
    }
    res.append(buffer, 0, file.gcount());
    std::cout << res << std::endl;
  }
}

void Configuration::print() const
{
  const char search_type[][40] = {
    "GreedySearch",
    "UniformSearch",
    "AStarSearch",
  };
  const char heuristics[][40] = {
    "Manhattan",
    "Hamming",
    "Euclidean",
    "Conflicts"
  };
  std::cout << "Search type: " << search_type[this->search_type] << std::endl;
  std::cout << "Heuristic function: " << heuristics[this->heuristics] << std::endl;
}
