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

void GenerateObjectD();

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
    double x_zone = object[0] - (2 * width) * floor(object[0] / (2 * width));
    double y_zone = -(height / 2) + x_zone * height / width;
    if (object[1] > y_zone) {
      *name = string("first");
    } else {
      *name = string("zero");
    }
  } else { /* \ */
    double x_zone = object[0] - (2 * width) * floor(object[0] / (2 * width))
                  - width;
    double y_zone = height / 2 - x_zone * height / width;
    if (object[1] > y_zone) {
      *name = string("first");
    } else {
      *name = string("zero");
    }
  }
}

void NameObjectC(int cells, const vector<double>& object, string* name) {
  int horizontal = static_cast<int>(ceil(object[0] * cells));
  int vertical = static_cast<int>(ceil(object[1] * cells));
  if ((horizontal + vertical) % 2 == 0) {
    *name = string("first");
  } else {
    *name = string("zero");
  }
}

void NameObjectE(double centre_x, double centre_y, double internal_radius,
                 doubel external_radius, const vector<double>& object,
                 string* name) {
  if ((object[0] - centre_x) * (object[0] - centre_x) +
      (object[1] - centre_y) * (object[1] - centre_y) < internal_radius) {
    *name = string("first");
  } else {
    if ((object[0] - centre_x) * (object[0] - centre_x) +
        (object[1] - centre_y) * (object[1] - centre_y) > external_radius) {
      *name = string("zero");
    } else {
      *name = ((rand() % 2 == 0) ? string("zero") : string("first"));
    }
  }
}

int main() {
  return 0;
}
