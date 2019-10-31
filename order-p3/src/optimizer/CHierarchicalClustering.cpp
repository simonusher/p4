#include "../../include/order-p3/optimizer/CHierarchicalClustering.h"

#include <algorithm>
#include <cfloat>

using namespace Clustering;

CHierarchicalClustering::CHierarchicalClustering(uint32_t iNumberOfElements, double** ppdDistances)
{
	i_number_of_elements = iNumberOfElements;
	ppd_distances = ppdDistances;


	if (iNumberOfElements > 0)
	{
		v_clusters.reserve(2 * iNumberOfElements - 1);
		v_cluster_indexes.reserve(iNumberOfElements - 1);
	}//if (iNumberOfElements > 0)
}//CHierarchicalClustering::CHierarchicalClustering(uint32_t iNumberOfElements, double **ppdDistances, CLog *pcLog)

CHierarchicalClustering::~CHierarchicalClustering()
{
	v_clear();
}//CHierarchicalClustering::~CHierarchicalClustering()

void CHierarchicalClustering::vRun()
{
	v_init_simple_clusters();
	v_clear_cluster_indexes();

	vector<uint32_t> v_remaining_elements;
	v_init_remaining_elements(&v_remaining_elements);

	double** ppd_cloned_distances = ppd_clone_distances();

	uint32_t* pi_mapping = pi_create_initial_mapping();

	uint32_t i_closest_element_0, i_closest_element_1;
	uint32_t i_mapping_0, i_mapping_1;

	vector<uint32_t>* pv_cluster;
	pair<uint32_t, uint32_t>* pp_cluster_indexes;

	while (v_remaining_elements.size() > HIERARCHICAL_CLUSTERING_ONLY_ROOT_REMAINS_COUNT)
	{
		v_find_closest_elements(ppd_cloned_distances, &v_remaining_elements, &i_closest_element_0, &i_closest_element_1);

		i_mapping_0 = *(pi_mapping + i_closest_element_0);
		i_mapping_1 = *(pi_mapping + i_closest_element_1);


		pv_cluster = new vector<uint32_t>();
		pv_cluster->reserve(v_clusters.at(i_mapping_0)->size() + v_clusters.at(i_mapping_1)->size());
		pv_cluster->insert(pv_cluster->end(), v_clusters.at(i_mapping_0)->begin(), v_clusters.at(i_mapping_0)->end());
		pv_cluster->insert(pv_cluster->end(), v_clusters.at(i_mapping_1)->begin(), v_clusters.at(i_mapping_1)->end());

		v_clusters.push_back(pv_cluster);


		pp_cluster_indexes = new pair<uint32_t, uint32_t>(i_mapping_0, i_mapping_1);

		v_cluster_indexes.push_back(pp_cluster_indexes);


		v_update_distances(ppd_cloned_distances, &v_remaining_elements, i_closest_element_0, i_closest_element_1, pi_mapping);

		*(pi_mapping + i_closest_element_0) = v_clusters.size() - 1;

		v_remaining_elements.erase(find(v_remaining_elements.begin(), v_remaining_elements.end(), i_closest_element_1));
	}//while (v_remaining_elements.size() > HIERARCHICAL_CLUSTERING_ONLY_ROOT_REMAINS_COUNT)


	for (uint32_t i = 0; i < i_number_of_elements; i++)
	{
		delete* (ppd_cloned_distances + i);
	}//for (uint32_t i = 0; i < i_number_of_elements; i++)

	delete ppd_cloned_distances;


	delete pi_mapping;
}//void CHierarchicalClustering::vRun()

void CHierarchicalClustering::v_init_simple_clusters()
{
	v_clear_clusters();

	vector<uint32_t>* pv_cluster;

	for (uint32_t i = 0; i < i_number_of_elements; i++)
	{
		pv_cluster = new vector<uint32_t>();
		pv_cluster->reserve(1);
		pv_cluster->push_back(i);

		v_clusters.push_back(pv_cluster);
	}//for (uint32_t i = 0; i < i_number_of_elements; i++)
}//void CHierarchicalClustering::v_init_simple_clusters()

void CHierarchicalClustering::v_clear_clusters()
{
	for (size_t i = 0; i < v_clusters.size(); i++)
	{
		delete v_clusters.at(i);
	}//for (size_t i = 0; i < v_clusters.size(); i++)

	v_clusters.clear();
}//void CHierarchicalClustering::v_clear_clusters()

void CHierarchicalClustering::v_clear_cluster_indexes()
{
	for (size_t i = 0; i < v_cluster_indexes.size(); i++)
	{
		delete v_cluster_indexes.at(i);
	}//for (size_t i = 0; i < v_cluster_indexes.size(); i++)

	v_cluster_indexes.clear();
}//void CHierarchicalClustering::v_clear_cluster_indexes()

void CHierarchicalClustering::v_clear()
{
	v_clear_clusters();
	v_clear_cluster_indexes();
}//void CHierarchicalClustering::v_clear()

void CHierarchicalClustering::v_init_remaining_elements(vector<uint32_t>* pvRemainingElements)
{
	pvRemainingElements->clear();
	pvRemainingElements->reserve(i_number_of_elements);

	for (uint32_t i = 0; i < i_number_of_elements; i++)
	{
		pvRemainingElements->push_back(i);
	}//for (uint32_t i = 0; i < i_number_of_elements; i++)
}//void CHierarchicalClustering::v_init_remaining_elements(vector<uint32_t> *pvRemainingElements)

double** CHierarchicalClustering::ppd_clone_distances()
{
	double** ppd_cloned_distances = new double* [i_number_of_elements];

	for (uint32_t i = 0; i < i_number_of_elements; i++)
	{
		*(ppd_cloned_distances + i) = new double[i_number_of_elements];

		for (uint32_t j = 0; j < i_number_of_elements; j++)
		{
			*(*(ppd_cloned_distances + i) + j) = *(*(ppd_distances + i) + j);
		}//for (uint32_t j = 0; j < i_number_of_elements; j++)
	}//for (uint32_t i = 0; i < i_number_of_elements; i++)

	return ppd_cloned_distances;
}//double ** CHierarchicalClustering::ppd_clone_distances()

uint32_t* CHierarchicalClustering::pi_create_initial_mapping()
{
	uint32_t* pi_initial_mapping = new uint32_t[i_number_of_elements];

	for (uint32_t i = 0; i < i_number_of_elements; i++)
	{
		*(pi_initial_mapping + i) = i;
	}//for (uint32_t i = 0; i < i_number_of_elements; i++)

	return pi_initial_mapping;
}//uint32_t * CHierarchicalClustering::pi_create_initial_mapping()

void CHierarchicalClustering::v_update_distances(double** ppdDistances, vector<uint32_t>* pvRemainingElements, uint32_t iClosestElement0, uint32_t iClosestElement1, uint32_t* piMapping)
{
	uint32_t i_mapping_closest_element_0 = *(piMapping + iClosestElement0);
	uint32_t i_mapping_closest_element_1 = *(piMapping + iClosestElement1);

	uint32_t i_size_0 = (uint32_t)v_clusters.at(i_mapping_closest_element_0)->size();
	uint32_t i_size_1 = (uint32_t)v_clusters.at(i_mapping_closest_element_1)->size();

	double d_distance_0, d_distance_1;

	uint32_t i_element, i_mapping_element;

	for (uint32_t i = 0; i < (uint32_t)pvRemainingElements->size(); i++)
	{
		i_element = pvRemainingElements->at(i);

		if (i_element != iClosestElement0 && i_element != iClosestElement1)
		{
			i_mapping_element = *(piMapping + i_element);

			d_distance_0 = *(*(ppdDistances + iClosestElement0) + i_element) * (double)i_size_0;
			d_distance_1 = *(*(ppdDistances + iClosestElement1) + i_element) * (double)i_size_1;

			*(*(ppdDistances + iClosestElement0) + i_element) = (d_distance_0 + d_distance_1) / (double)(i_size_0 + i_size_1);
			*(*(ppdDistances + i_element) + iClosestElement0) = *(*(ppdDistances + iClosestElement0) + i_element);
		}//if (i_element != iClosestElement0 && i_element != iClosestElement1)
	}//for (uint32_t i = 0; i < (uint32_t)pvRemainingElements->size(); i++)
}//void CHierarchicalClustering::v_update_distances(double **ppdDistances, vector<uint32_t> *pvRemainingElements, uint32_t iClosestElement0, uint32_t iClosestElement1, uint32_t *piMapping)

void CHierarchicalClustering::v_find_closest_elements(double** ppdDistances, vector<uint32_t>* pvRemainingElements, uint32_t* piElement0, uint32_t* piElement1)
{
	double d_min_distance = DBL_MAX;
	double d_distance;

	uint32_t i_index_0, i_index_1;

	for (uint32_t i = 0; i < (uint32_t)pvRemainingElements->size(); i++)
	{
		i_index_0 = pvRemainingElements->at(i);

		for (uint32_t j = i + 1; j < (uint32_t)pvRemainingElements->size(); j++)
		{
			i_index_1 = pvRemainingElements->at(j);

			d_distance = *(*(ppdDistances + i_index_0) + i_index_1);

			if (d_distance < d_min_distance)
			{
				*piElement0 = i_index_0;
				*piElement1 = i_index_1;

				d_min_distance = d_distance;
			}//if (d_distance < d_min_distance)
		}//for (uint32_t j = i + 1; j < (uint32_t)pvRemainingElements->size(); j++)
	}//for (uint32_t i = 0; i < (uint32_t)pvRemainingElements->size(); i++)
}//void CHierarchicalClustering::v_find_closest_elements(double **ppdDistances, vector<uint32_t> *pvRemainingElements, uint32_t *piElement0, uint32_t *piElement1)