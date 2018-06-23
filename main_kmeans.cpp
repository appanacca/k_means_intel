#include <chrono>
#include <fstream>
#include <iostream>
#include <omp.h>
#include <random>
#include <vector>
#include <tuple>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

using tuint = std::size_t;

class PixelVector {
public:
  std::vector<float> r;
  std::vector<float> g;
  std::vector<float> b;
  PixelVector(const std::ptrdiff_t n) : r(n), g(n), b(n) {}
};

std::tuple<const tuint,const  tuint,const  tuint> initialize_inputs(int ac, char const *av[]){

    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "produce help message")
    ("nb_point", po::value<tuint>(), "number of points")
            ("nb_cluster", po::value<tuint>(), "number of clusters")
    ("nb_iter", po::value<tuint>(), "number of iterain");

     po::variables_map vm;
     po::store(po::parse_command_line(ac, av, desc), vm);
     po::notify(vm);



     const tuint nb_point = vm["nb_point"].as<tuint>();
     const tuint nb_cluster = vm["nb_cluster"].as<tuint>();
     const tuint nb_iter = vm["nb_iter"].as<tuint>();

    return {nb_point, nb_cluster, nb_iter};
}


int main(int ac, char const *av[]) {

  auto [nb_point, nb_cluster, nb_iter] = initialize_inputs(ac, av);

  PixelVector point(nb_point);
  std::vector<tuint> cluster(nb_point);

  PixelVector centroid(nb_cluster);
  std::vector<tuint> point_per_cluster(nb_cluster);

  std::default_random_engine engine{};
  std::uniform_real_distribution<float> r_dist{0.0f, 1.0f};
  std::uniform_int_distribution<uint> i_dist{0, nb_cluster - 1};

  // inizialization of the points and their cluster
  for (int k = 0; k < nb_point; ++k) {
    point.r[k] = r_dist(engine);
    point.g[k] = r_dist(engine);
    point.b[k] = r_dist(engine);
    cluster[k] = i_dist(engine);
    // myfile << point[k].r << "," << point[k].g << "," << point[k].b << "," <<
    // cluster[k] << std::endl;
  }

  auto start = std::chrono::high_resolution_clock::now();
  int iteration = 0;

  // compute the centroid of the clusters
  for (int i = 0; i < nb_cluster; ++i) {
    centroid.r[i] += r_dist(engine);
    centroid.g[i] += r_dist(engine);
    centroid.b[i] += r_dist(engine);
    point_per_cluster[i] = 0;
  }

  while (iteration < nb_iter) {

    for (int k = 0; k < nb_cluster; ++k) {
      int i = cluster[k];
      centroid.r[i] += point.r[k];
      centroid.g[i] += point.g[k];
      centroid.b[i] += point.b[k];
      ++point_per_cluster[i];
    }

    for (int i = 0; i < nb_cluster; ++i) {
      int nb_point_cluster = point_per_cluster[i];
      centroid.r[i] /= nb_point_cluster;
      centroid.g[i] /= nb_point_cluster;
      centroid.b[i] /= nb_point_cluster;
    }

// reassign points to clusters
#pragma omp parallel for num_threads(6)
    for (int k = 0; k < nb_point; ++k) {
      float best_distance = std::numeric_limits<float>::max();
      int best_centroid = -1;
      for (int i = 0; i < nb_cluster; ++i) {
        float x = point.r[k] - centroid.r[i];
        float y = point.g[k] - centroid.g[i];
        float z = point.b[k] - centroid.b[i];
        float distance = x * x + y * y + z * z;
        if (distance < best_distance) {
          best_distance = distance;
          best_centroid = i;
        }
      }
      cluster[k] = best_centroid;
    }

    // exit once convergence is reached
    ++iteration;

    // std::cout << iteration << std::endl;
  }
  auto end = std::chrono::high_resolution_clock::now();

  std::ofstream myfile;
  myfile.open("initial.dat");

  for (int k = 0; k < nb_point; ++k) {
    myfile << point.r[k] << "," << point.g[k] << "," << point.b[k] << ","
           << cluster[k] << std::endl;
  }

  myfile.close();


  std::chrono::duration<double> diff = end - start;
  std::cout << "Elapsed time: " << diff.count() << std::endl;

  return 0;
}
