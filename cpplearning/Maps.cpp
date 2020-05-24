#include <iostream>
#include <map>
#include <unordered_map>

#include "classes/TestClasses.h"

void invalidIt() {
  std::cout << "=====================" << std::endl;
  std::cout << "===== invalidIt =====" << std::endl;
  std::cout << "=====================" << std::endl;
  std::map<char, TestClass> mymap;
  std::map<char, TestClass>::iterator it;

  // insert some values:
  mymap['a'] = TestClass(10);

  it = mymap.find('a');
  mymap.erase(it); // erasing by iterator

  std::cout << it->first << " => " << it->second.a() << '\n';
}

void opTesting() {
  std::cout << "=====================" << std::endl;
  std::cout << "===== opTesting =====" << std::endl;
  std::cout << "=====================" << std::endl;

  std::cout << "Constructing map" << std::endl;
  std::unordered_map<int, TestClass> umap;

  std::cout << "Constructing t1, t2, t3" << std::endl;
  TestClass t1(10);
  TestClass t2(15);
  TestClass t3(20);
  std::cout << "t1.a() = " << t1.a() << std::endl;
  std::cout << "t2.a() = " << t2.a() << std::endl;
  std::cout << "t3.a() = " << t3.a() << std::endl;
  std::cout << std::endl;

  std::cout << "=======================" << std::endl;
  std::cout << "=== The [] operator ===" << std::endl;
  std::cout << "=======================" << std::endl;

  std::cout << "umap[0]" << std::endl;
  umap[0];
  std::cout << "umap[0].a() = " << umap[0].a() << std::endl;
  std::cout << std::endl;

  std::cout << "umap[1] = t1" << std::endl;
  umap[1] = t1;
  std::cout << "umap[1].a() = " << umap[1].a() << std::endl;
  std::cout << std::endl;

  std::cout << "umap[2] = std::move(t1)" << std::endl;
  umap[2] = std::move(t1);
  std::cout << "t1.a() = " << t1.a() << std::endl;
  std::cout << "umap[1].a() = " << umap[1].a() << std::endl;
  std::cout << "umap[2].a() = " << umap[2].a() << std::endl;
  std::cout << std::endl;

  std::cout << "============================" << std::endl;
  std::cout << "=== The emplace function ===" << std::endl;
  std::cout << "============================" << std::endl;

  std::cout << "umap.emplace(3, t2)" << std::endl;
  umap.emplace(3, t2);
  std::cout << "t2.a() = " << t2.a() << std::endl;
  std::cout << "umap[3].a() = " << umap[3].a() << std::endl;
  std::cout << std::endl;

  std::cout << "umap.emplace(4, std::move(t2))" << std::endl;
  umap.emplace(4, std::move(t2));
  std::cout << "t2.a() = " << t2.a() << std::endl;
  std::cout << "umap[4].a() = " << umap[4].a() << std::endl;
  std::cout << std::endl;

  std::cout << "umap.emplace(5, TestClass{10})" << std::endl;
  umap.emplace(5, TestClass{10});
  std::cout << "umap[5].a() = " << umap[5].a() << std::endl;
  std::cout << std::endl;

  std::cout << "umap.emplace(6, 107)" << std::endl;
  umap.emplace(6, 107);
  std::cout << "umap[6].a() = " << umap[6].a() << std::endl;
  std::cout << std::endl;

  std::cout << "umap.emplace(6, 1)" << std::endl;
  auto pair = umap.emplace(6, 1);
  std::cout << "umap[6].a() = " << umap[6].a() << std::endl;
  std::cout << "pair.second = " << pair.second << std::endl;
  std::cout << std::endl;

  std::cout << "===========================" << std::endl;
  std::cout << "=== The insert function ===" << std::endl;
  std::cout << "===========================" << std::endl;

  std::cout << "umap.insert({7, t3})" << std::endl;
  umap.insert({7, t3});
  std::cout << "t3.a() = " << t3.a() << std::endl;
  std::cout << "umap[7].a() = " << umap[7].a() << std::endl;
  std::cout << std::endl;

  std::cout << "umap.insert({8, std::move(t3)})" << std::endl;
  umap.insert({8, std::move(t3)});
  std::cout << "t3.a() = " << t3.a() << std::endl;
  std::cout << "umap[8].a() = " << umap[8].a() << std::endl;
  std::cout << std::endl;

  std::cout << "umap.insert({9, TestClass{10}})" << std::endl;
  umap.insert({9, TestClass{10}});
  std::cout << "umap[9].a() = " << umap[9].a() << std::endl;
  std::cout << std::endl;

  std::cout << "umap.insert({9, TestClass{1}})" << std::endl;
  pair = umap.insert({9, TestClass{1}});
  std::cout << "umap[9].a() = " << umap[9].a() << std::endl;
  std::cout << "pair.second = " << pair.second << std::endl;
  std::cout << std::endl;
}

int main() {
  invalidIt();
  opTesting();
}
