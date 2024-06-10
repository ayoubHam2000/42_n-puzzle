#include <iostream>
#include <string>
#include <bitset>
#include <set>



int main() {

  std::set<std::string> s;

  std::string ss = "6-5-3-0-1-2-4-7-8";
  s.insert(ss);
  if (s.count(ss) == 1) {
    std::cout << "Yes" << std::endl;
  } 
}