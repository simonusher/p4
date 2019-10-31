#ifndef HIERARCHICAL_CLUSTERING_H
#define HIERARCHICAL_CLUSTERING_H

#define HIERARCHICAL_CLUSTERING_ONLY_ROOT_REMAINS_COUNT 1


#include <cstdint>
#include <utility>
#include <vector>

using namespace std;

namespace Clustering
{
	class CHierarchicalClustering
	{
	public:
		CHierarchicalClustering(uint32_t iNumberOfElements, double** ppdDistances);

		~CHierarchicalClustering();

		void vRun();

		vector<vector<uint32_t>*>* pvGetClusters() { return &v_clusters; }
		vector<pair<uint32_t, uint32_t>*>* pvGetClusterIndexes() { return &v_cluster_indexes; }

	private:
		void v_init_simple_clusters();
		void v_clear_clusters();

		void v_clear_cluster_indexes();

		void v_clear();

		void v_init_remaining_elements(vector<uint32_t>* pvRemainingElements);

		double** ppd_clone_distances();

		uint32_t* pi_create_initial_mapping();

		void v_update_distances(double** ppdDistances, vector<uint32_t>* pvRemainingElements, uint32_t iClosestElement0, uint32_t iClosestElement1, uint32_t* piMapping);

		static void v_find_closest_elements(double** ppdDistances, vector<uint32_t>* pvRemainingElements, uint32_t* piElement0, uint32_t* piElement1);

		uint32_t i_number_of_elements;
		double** ppd_distances;

		vector<vector<uint32_t>*> v_clusters;
		vector<pair<uint32_t, uint32_t>*> v_cluster_indexes;

	};//class CHierarchicalClustering
}//namespace Clustering

#endif//HIERARCHICAL_CLUSTERING_H