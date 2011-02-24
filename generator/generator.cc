#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>

using std::cout;
using std::endl;
using std::ofstream;
using std::pair;
using std::string;
using std::stringstream;
using std::vector;

const int MIN_COUNT = 500;
const int MAX_COUNT = 10000;
const int STEP_COUNT = 500;
const int MIN_RADIUS = 1;
const int MAX_RADIUS = 5;
const double STEP_RADIUS = 0.1;
const double CENTER_X = 0.5;
const double CENTER_Y = 0.5;
const int MIN_WIDTH = 1;
const int MAX_WIDTH = 5;
const double STEP_WIDTH = 0.1;
const int MIN_HEIGHT = 1;
const int MAX_HEIGHT = 5;
const double STEP_HEIGHT = 0.1;
const int MIN_CELLS = 2;
const int MAX_CELLS = 20;
const double M1_ZERO = 0.55;
const double M2_ZERO = 0.45;
const double D1_ZERO = 0.3;
const double COV12_ZERO = 0.35;
const double D2_ZERO = 0.7;
const double M1_FIRST = 0.45;
const double M2_FIRST = 0.55;
const double D1_FIRST = 1;
const double COV12_FIRST = 0.15;
const double D2_FIRST = 0.1;

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

void GenerateObjectD(double M1, double M2, double D1, double Cov12, double D2,
                     const string& class_name, pair<vector<double>, string>* object) {
  double rand_x = Random();
  double rand_y = Random();
  object->first.assign(2, 0);
  object->first[0] = M1 + sqrt(D1) * rand_x;
  object->first[1] = M2 + Cov12 / sqrt(D1) * rand_x + sqrt(D2 - Cov12 * Cov12 / D1) * rand_y;
  object->second = class_name;
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
    double x_zone = object[0] - (2 * width) * floor(object[0] / (2 * width));
    double y_zone = 0.5 - height / 2 + x_zone * height / width;
    if (object[1] > y_zone) {
      *name = string("first");
    } else {
      *name = string("zero");
    }
  } else { /* \ */
    double x_zone = object[0] - (2 * width) * floor(object[0] / (2 * width))
                  - width;
    double y_zone = 0.5 + height / 2 - x_zone * height / width;
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
                 double external_radius, const vector<double>& object,
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

void CreateArffFile(const string& file_name,
                    const vector<pair<vector<double>, string> >& objects) {
  string real_file_name = file_name + string(".arff");
  ofstream arff(real_file_name.c_str());
  arff << "\% BEGIN OF ARFF FILE" << endl;
  arff << "\% Automatically generated ARFF file" << endl;
  arff << "\% Author: Pavel Zhernov" << endl;
  arff << "@RELATION " << file_name << endl;
  arff << endl;
  arff << "@ATTRIBUTE x\t NUMERIC" << endl;
  arff << "@ATTRIBUTE y\t NUMERIC" << endl;
  arff << "@ATTRIBUTE class\t {zero,first}" << endl;
  arff << endl;
  arff << "@DATA" << endl;
  for (int index = 0; index < objects.size(); ++index) {
    arff << objects[index].first[0] << ','
         << objects[index].first[1] << ','
         << objects[index].second << endl;
  }
  arff << "\% END OF ARFF FILE" << endl;
  arff.close();
  cout << real_file_name << " has been generated" << endl;
}

void CreateObjectsA(int count, double centre_x, double centre_y, double radius,
                    vector<pair<vector<double>, string> >* objects) {
  objects->clear();
  for (int index = 0; index < count; ++index) {
    vector<double> object;
    GenerateObject(&object);
    string class_name;
    NameObjectA(centre_x, centre_y, radius, object, &class_name);
    objects->push_back(make_pair(object, class_name));
  }
}

void CreateObjectsB(int count, double width, double height,
                    vector<pair<vector<double>, string> >* objects) {
  objects->clear();
  for (int index = 0; index < count; ++index) {
    vector<double> object;
    GenerateObject(&object);
    string class_name;
    NameObjectB(width, height, object, &class_name);
    objects->push_back(make_pair(object, class_name));
  }
}

void CreateObjectsC(int count, int cells,
                    vector<pair<vector<double>, string> >* objects) {
  objects->clear();
  for (int index = 0; index < count; ++index) {
    vector<double> object;
    GenerateObject(&object);
    string class_name;
    NameObjectC(cells, object, &class_name);
    objects->push_back(make_pair(object, class_name));
  }
}

void CreateObjectsD(int count_zero, int count_first,
                    double M1_zero, double M2_zero, double D1_zero,
                    double Cov12_zero, double D2_zero,
                    double M1_first, double M2_first, double D1_first,
                    double Cov12_first, double D2_first,
                    vector<pair<vector<double>, string> >* objects) {
  objects->clear();
  for (int index = 0; index < count_zero; ++index) {
    pair<vector<double>, string> object;
    GenerateObjectD(M1_zero, M2_zero, D1_zero, Cov12_zero, D2_zero, string("zero"), &object);
    objects->push_back(object);
  }
  for (int index = 0; index < count_first; ++index) {
    pair<vector<double>, string> object;
    GenerateObjectD(M1_first, M2_first, D1_first, Cov12_first, D2_first, string("first"), &object);
    objects->push_back(object);
  }
}

void CreateObjectsE(int count, double centre_x, double centre_y,
                    double internal_radius, double external_radius,
                    vector<pair<vector<double>, string> >* objects) {
  objects->clear();
  for (int index = 0; index < count; ++index) {
    vector<double> object;
    GenerateObject(&object);
    string class_name;
    NameObjectE(centre_x, centre_y, internal_radius, external_radius, object, &class_name);
    objects->push_back(make_pair(object, class_name));
  }
}

void GenerateFilesA() {
  for (int count = MIN_COUNT; count <= MAX_COUNT; count += STEP_COUNT) {
    for (int multiplier = MIN_RADIUS; multiplier <= MAX_RADIUS; ++multiplier) {
      vector<pair<vector<double>, string> > objects;
      CreateObjectsA(count, CENTER_X, CENTER_Y, multiplier * STEP_RADIUS, &objects);
      stringstream print_file_name;
      print_file_name << "data/circle/circle_" << count << "_objects_"
                      << multiplier * STEP_RADIUS << "_radius";
      string file_name;
      print_file_name >> file_name;
      CreateArffFile(file_name, objects);
    }
  }
}

void GenerateFilesB() {
  for (int count = MIN_COUNT; count <= MAX_COUNT; count += STEP_COUNT) {
    for (int multiplier_width = MIN_WIDTH; multiplier_width <= MAX_WIDTH; ++multiplier_width) {
      for (int multiplier_height = MIN_HEIGHT; multiplier_height <= MAX_HEIGHT; ++multiplier_height) {
        vector<pair<vector<double>, string> > objects;
        CreateObjectsB(count, multiplier_width * STEP_WIDTH, multiplier_height * STEP_HEIGHT, &objects);
        stringstream print_file_name;
        print_file_name << "data/saw/saw_" << count << "_objects_"
                        << multiplier_width * STEP_WIDTH << "_width_"
                        << multiplier_height * STEP_HEIGHT << "_height";
        string file_name;
        print_file_name >> file_name;
        CreateArffFile(file_name, objects);
      }
    }
  }
}

void GenerateFilesC() {
  for (int count = MIN_COUNT; count <= MAX_COUNT; count += STEP_COUNT) {
    for (int cells = MIN_CELLS; cells <= MAX_CELLS; ++cells) {
      vector<pair<vector<double>, string> > objects;
      CreateObjectsC(count, cells, &objects);
      stringstream print_file_name;
      print_file_name << "data/chess/chess_" << count << "_objects_"
                      << cells << "_cells";
      string file_name;
      print_file_name >> file_name;
      CreateArffFile(file_name, objects);
    }
  }
}

void GenerateFilesD() {
  for (int count = MIN_COUNT; count <= MAX_COUNT; count += STEP_COUNT) {
    vector<pair<vector<double>, string> > objects;
    CreateObjectsD(count / 2, count / 2, M1_ZERO, M2_ZERO, D1_ZERO, COV12_ZERO, D2_ZERO,
                   M1_FIRST, M2_FIRST, D1_FIRST, COV12_FIRST, D2_FIRST, &objects);
    stringstream print_file_name;
    print_file_name << "data/normal_distribution/normal_distribution_"
                    << count << "_objects";
    string file_name;
    print_file_name >> file_name;
    CreateArffFile(file_name, objects);
  }
}

void GenerateFilesE() {
  for (int count = MIN_COUNT; count <= MAX_COUNT; count += STEP_COUNT) {
    for (int multiplier_internal = MIN_RADIUS; multiplier_internal <= MAX_RADIUS; ++multiplier_internal) {
      for (int multiplier_external = MIN_RADIUS; multiplier_external <= MAX_RADIUS; ++multiplier_external) {
        if (multiplier_internal < multiplier_external) {
          vector<pair<vector<double>, string> > objects;
          CreateObjectsE(count, CENTER_X, CENTER_Y, multiplier_internal * STEP_RADIUS,
                         multiplier_external * STEP_RADIUS, &objects);
          stringstream print_file_name;
          print_file_name << "data/two_circles/two_circles_" << count << "_objects_"
                          << multiplier_internal * STEP_RADIUS << "_internal_radius_"
                          << multiplier_external * STEP_RADIUS << "_external_radius";
          string file_name;
          print_file_name >> file_name;
          CreateArffFile(file_name, objects);
        }
      }
    }
  }
}

void GenerateAllFiles() {
  GenerateFilesA();
  GenerateFilesB();
  GenerateFilesC();
  GenerateFilesD();
  GenerateFilesE();
}

int main() {
  InitRandomSeed(239);
  mkdir("data", 0755);
  mkdir("data/circle", 0755);
  mkdir("data/saw", 0755);
  mkdir("data/chess", 0755);
  mkdir("data/normal_distribution", 0755);
  mkdir("data/two_circles", 0755);
  GenerateAllFiles();
  return 0;
}
