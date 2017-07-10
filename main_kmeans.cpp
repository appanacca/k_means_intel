#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>

	struct Pixel{
		float r;
		float g;
		float b;
	};


int main(int argc, char const *argv[])
{

	const std::size_t nb_point = 14740;//56; //00;
	const std::size_t nb_cluster = 12;
	const std::size_t nb_iter = 10;


	
	std::vector<Pixel> point(nb_point);
	std::vector<std::size_t> cluster(nb_point);

	std::vector<Pixel> centroid(nb_cluster);
	std::vector<std::size_t> point_per_cluster(nb_cluster);

	std::default_random_engine engine{};
	std::uniform_real_distribution<float> r_dist{0.0f, 1.0f};
	std::uniform_int_distribution<std::size_t> i_dist{0, nb_cluster - 1};


    // inizialization of the points and their cluster
	for(std::size_t k = 0; k<nb_point; ++k){
		point[k].r = r_dist(engine);
		point[k].g = r_dist(engine);
		point[k].b = r_dist(engine);
		cluster[k] = i_dist(engine);
		//myfile << point[k].r << "," << point[k].g << "," << point[k].b << "," << cluster[k] << std::endl;
	}



	auto start = std::chrono::high_resolution_clock::now();
	std::size_t iteration = 0;


		//compute the centroid of the clusters
		for (std::size_t i = 0; i < nb_cluster; ++i){
			centroid[i].r += r_dist(engine);
			centroid[i].g += r_dist(engine);
			centroid[i].b += r_dist(engine);
			point_per_cluster[i] = 0;
		}

	while (iteration < nb_iter) {


		for (std::size_t k = 0;  k < nb_cluster; ++k){
			std::size_t i = cluster[k];
			centroid[i].r += point[k].r;
			centroid[i].g += point[k].g;
			centroid[i].b += point[k].b;
			++point_per_cluster[i];
		}

		for (std::size_t i = 0; i < nb_cluster; ++i){
			std:size_t nb_point_cluster = point_per_cluster[i];
			centroid[i].r /= nb_point_cluster;
			centroid[i].g /= nb_point_cluster;
			centroid[i].b /= nb_point_cluster;
		}


		//reassign points to clusters
		for (std::size_t k = 0; k < nb_point; ++k){
			float best_distance = std::numeric_limits<float>::max();
			std::size_t best_centroid = -1;
			for (std::size_t i = 0; i < nb_cluster; ++i){
				float x = point[k].r - centroid[i].r;
				float y = point[k].g - centroid[i].g;
				float z = point[k].b - centroid[i].b;
				float distance = std::pow(x,2) + std::pow(y,2) + std::pow(z,2);
				if (distance < best_distance) {
					best_distance = distance;
					best_centroid = i;
				}
			}
			cluster[k] = best_centroid;
		}

		//exit once convergence is reached
		++iteration;

		std::cout << iteration << std::endl;

	}

	std::ofstream myfile;
    myfile.open ("initial.dat");
    //myfile << "Writing the initial distribution of points.\n";

    for(std::size_t k = 0; k<nb_point; ++k){
        myfile << point[k].r << "," << point[k].g << "," << point[k].b << "," << cluster[k] << std::endl;
    }

    myfile.close();


	auto end = std::chrono::high_resolution_clock::now();

	return 0;
}