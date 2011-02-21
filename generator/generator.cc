#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using std::string;
using std::vector;

void InitRandomSeed(int seed) {
  srand(seed);
}

double Random() {
  return static_cast<double>(rand()) / (1.0 + static_cast<double>(RAND_MAX));
}

void GenerateObject(vector<double>* object) {
  object->assign(2, 0);
  object->at(0) = Random();
  object->at(1) = Random();
}

void NameObjectA(double centre_x, double centre_y, double radius,
                 const vector<double>& object, string* name) {
  if ((object[0] - centre_x) * (object[0] - centre_x) +
      (object[1] - centre_y) * (object[1] - centre_y) < radius) {
    *name = string("first");
  } else {
    *name = string("zero");
  }
}

void NameObjectB(double width, double height,
                 const vector<double>& object, string* name) {
  if (static_cast<int>(ceil(object[0] / (2 * width))) % 2 == 1) { /* / */
  } else { /* \ */
  }
}

int main() {
  return 0;
}
