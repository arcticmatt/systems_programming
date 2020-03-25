#include <iostream>
#include <map>
#include <unordered_map>

#include "TestClasses.h"

void it() {
  std::map<char, TestClass> mymap;
  std::map<char, TestClass>::iterator it;

  // insert some values:
  mymap['a'] = TestClass(10);

  it = mymap.find('a');
  mymap.erase(it); // erasing by iterator

  std::cout << it->first << " => " << it->second.a() << '\n';
}

int main() {
  std::cout << "Constructing map" << std::endl;
  std::unordered_map<int, TestClass> umap;

  std::cout << "Constructing t1, t2, t3" << std::endl;
  TestClass t1(10);
  TestClass t2(15);
  TestClass t3(20);
  std::cout << std::endl;

  std::cout << "umap[5] = t1" << std::endl;
  umap[5] = t1;
  std::cout << "umap[5].a() = " << umap[5].a() << std::endl;
  std::cout << std::endl;

  std::cout << "umap[6] = std::move(t1)" << std::endl;
  umap[6] = std::move(t1);
  std::cout << "umap[6].a() = " << umap[6].a() << std::endl;
  std::cout << std::endl;

  std::cout << "umap.emplace(7, std::move(t2))" << std::endl;
  umap.emplace(7, std::move(t2));
  std::cout << std::endl;

  std::cout << "umap.insert(8, std::move(t2))" << std::endl;
  umap.insert({8, std::move(t3)});
  std::cout << std::endl;

  it();
}
