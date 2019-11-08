#pragma once
#include <random>
#include "../problem/Problem.h"
#include <algorithm>
#include "solution/Solution.h"

class BosmanLinkage {
public:
	class ClusterIterator2 {
		public:
			ClusterIterator2(size_t currentIndex, BosmanLinkage& linkage): currentClusterOrderingIndex(currentIndex), linkage(linkage) {
			}
			bool operator!=(const ClusterIterator2& other) const {
				return this->currentClusterOrderingIndex != other.currentClusterOrderingIndex;
			}
			std::vector<int>& operator*() const {
				return linkage.clusters[currentClusterOrderingIndex];
			}
			ClusterIterator2& operator++() {
				currentClusterOrderingIndex++;
				return *this;
			}
		private:
			size_t currentClusterOrderingIndex;
			BosmanLinkage& linkage;
	};

	class RandomClusterIterator2 {
	public:
		RandomClusterIterator2(BosmanLinkage& linkage, bool end = false) : linkage(linkage), unused(linkage.clusters.size() - 1), options(linkage.clusters.size())
			{
			if (end) {
				unused = -1;
			}
			else {
				generateNextIndex();
				std::iota(options.begin(), options.end(), 0);
			}
		}

		bool operator!=(const RandomClusterIterator2& other) const {
			return this->unused != other.unused;
		}

		std::vector<int>& operator*() const {
			return linkage.clusters[currentIndex];
		}

		RandomClusterIterator2& operator++() {
			generateNextIndex();
			return *this;
		}
	private:
		int unused;
		int currentIndex;
		void generateNextIndex() {
			if (unused > 0) {
				indexDistribution = std::uniform_int_distribution<int>(0, unused);
				int index = indexDistribution(linkage.randomGenerator);
				currentIndex = options[index];
				std::swap(options[unused - 1], options[index]);
			}
			unused--;
		}
		std::uniform_int_distribution<int> indexDistribution;
		std::vector<int> options;
		BosmanLinkage& linkage;
	};

	BosmanLinkage::ClusterIterator2 begin() {
		return { 0, *this };
	}

	BosmanLinkage::ClusterIterator2 end() {
		return { clusters.size(), *this };
	}

	BosmanLinkage::RandomClusterIterator2 randomBegin() {
		return { *this };
	}

	BosmanLinkage::RandomClusterIterator2 randomEnd() {
		return { *this, true };
	}

	BosmanLinkage(Problem* problem, std::mt19937& randomGenerator) : randomGenerator(randomGenerator), problem(problem),
		dependencyMatrix(problem->getProblemSize(), std::vector<double>(problem->getProblemSize(), 0)),
		relative(std::vector<std::vector<double>>(problem->getProblemSize(), std::vector<double>(problem->getProblemSize(), 0))),
		adjacency(std::vector<std::vector<double>>(problem->getProblemSize(), std::vector<double>(problem->getProblemSize(), 0)))
	{
		number_of_parameters = problem->getProblemSize();
		std::random_device d;
		random_seed = d();
		random_seed_changing = random_seed;
	}

	double randomRealUniform01()
	{
		int64_t n26, n27;
		double  result;

		random_seed_changing = (random_seed_changing * 0x5DEECE66DLLU + 0xBLLU) & ((1LLU << 48) - 1);
		n26 = (int64_t)(random_seed_changing >> (48 - 26));
		random_seed_changing = (random_seed_changing * 0x5DEECE66DLLU + 0xBLLU) & ((1LLU << 48) - 1);
		n27 = (int64_t)(random_seed_changing >> (48 - 27));
		result = (((int64_t)n26 << 27) + n27) / ((double)(1LLU << 53));

		return(result);
	}


	int randomInt(int maximum)
	{
		int result;

		result = (int)(((double)maximum) * randomRealUniform01());

		return(result);
	}


	int* randomPermutation(int n)
	{
		int i, j, dummy, * result;

		result = (int*)Malloc(n * sizeof(int));
		for (i = 0; i < n; i++)
			result[i] = i;

		for (i = n - 1; i > 0; i--)
		{
			j = randomInt(i + 1);
			dummy = result[j];
			result[j] = result[i];
			result[i] = dummy;
		}

		return(result);
	}

	void shuffleFOSSpecificGOMEA()
	{
		int i, * order, ** FOSs_new, * FOSs_number_of_indices_new;

		FOSs_new = (int**)Malloc(FOSs_length * sizeof(int*));
		FOSs_number_of_indices_new = (int*)Malloc(FOSs_length * sizeof(int));
		order = randomPermutation(FOSs_length);
		for (i = 0; i < FOSs_length; i++)
		{
			FOSs_new[i] = FOSs[order[i]];
			FOSs_number_of_indices_new[i] = FOSs_number_of_indices[order[i]];
		}
		free(FOSs);
		free(FOSs_number_of_indices);
		FOSs = FOSs_new;
		FOSs_number_of_indices = FOSs_number_of_indices_new;

		free(order);
	}

	void update(Solution* newSolution, int currentPopulationSize) {
		computeDependencyMatrixSpecificGOMEA(newSolution, currentPopulationSize);
		learnLTFOSSpecificGOMEA();
	}

	int** learnLTFOSSpecificGOMEA()
	{
		char     done;
		int      i, j, r0, r1, rswap, * indices, * order,
			FOSs_index, ** mpm, * mpm_number_of_indices, mpm_length,
			** mpm_new, * mpm_new_number_of_indices, mpm_new_length,
			* NN_chain, NN_chain_length, ** parent_child_relations,
			PCR_index, * FOSs_index_of_mpm_element;
		double** S_matrix, mul0, mul1;

		parent_child_relations = NULL; /* Only needed to prevent compiler warnings. */
		PCR_index = 0;    /* Only needed to prevent compiler warnings. */
		FOSs_index_of_mpm_element = NULL; /* Only needed to prevent compiler warnings. */
		mpm_new = NULL;



		/* Initialize MPM to the univariate factorization */
		order = randomPermutation(number_of_parameters);
		mpm = (int**)Malloc(number_of_parameters * sizeof(int*));
		mpm_number_of_indices = (int*)Malloc(number_of_parameters * sizeof(int));
		mpm_length = number_of_parameters;
		for (i = 0; i < number_of_parameters; i++)
		{
			indices = (int*)Malloc(1 * sizeof(int));
			indices[0] = order[i];
			mpm[i] = indices;
			mpm_number_of_indices[i] = 1;
		}
		free(order);

		/* Initialize LT to the initial MPM */
		FOSs_length = number_of_parameters + number_of_parameters - 1;
		FOSs = (int**)Malloc(FOSs_length * sizeof(int*));
		FOSs_number_of_indices = (int*)Malloc(FOSs_length * sizeof(int));
		FOSs_index = 0;
		for (i = 0; i < mpm_length; i++)
		{
			FOSs[FOSs_index] = mpm[i];
			FOSs_number_of_indices[FOSs_index] = mpm_number_of_indices[i];
			FOSs_index++;
		}

		/* Initialize similarity matrix */
		S_matrix = (double**)Malloc(number_of_parameters * sizeof(double*));
		for (i = 0; i < number_of_parameters; i++)
			S_matrix[i] = (double*)Malloc(number_of_parameters * sizeof(double));
		for (i = 0; i < mpm_length; i++)
			for (j = 0; j < mpm_length; j++)
				S_matrix[i][j] = dependencyMatrix[mpm[i][0]][mpm[j][0]];
		for (i = 0; i < mpm_length; i++)
			S_matrix[i][i] = 0;

		NN_chain = (int*)Malloc((number_of_parameters + 2) * sizeof(int));
		NN_chain_length = 0;
		done = 0;
		while (!done)
		{
			if (NN_chain_length == 0)
			{
				NN_chain[NN_chain_length] = randomInt(mpm_length);
				NN_chain_length++;
			}

			while (NN_chain_length < 3)
			{
				NN_chain[NN_chain_length] = determineNearestNeighbour(NN_chain[NN_chain_length - 1], S_matrix, mpm_number_of_indices, mpm_length);
				NN_chain_length++;
			}

			while (NN_chain[NN_chain_length - 3] != NN_chain[NN_chain_length - 1])
			{
				NN_chain[NN_chain_length] = determineNearestNeighbour(NN_chain[NN_chain_length - 1], S_matrix, mpm_number_of_indices, mpm_length);
				if (((S_matrix[NN_chain[NN_chain_length - 1]][NN_chain[NN_chain_length]] == S_matrix[NN_chain[NN_chain_length - 1]][NN_chain[NN_chain_length - 2]])) && (NN_chain[NN_chain_length] != NN_chain[NN_chain_length - 2]))
					NN_chain[NN_chain_length] = NN_chain[NN_chain_length - 2];
				NN_chain_length++;
				if (NN_chain_length > number_of_parameters)
					break;
			}
			r0 = NN_chain[NN_chain_length - 2];
			r1 = NN_chain[NN_chain_length - 1];
			if (r0 > r1)
			{
				rswap = r0;
				r0 = r1;
				r1 = rswap;
			}
			NN_chain_length -= 3;

			if (r1 < mpm_length) /* This test is required for exceptional cases in which the nearest-neighbor ordering has changed within the chain while merging within that chain */
			{
				indices = (int*)Malloc((mpm_number_of_indices[r0] + mpm_number_of_indices[r1]) * sizeof(int));

				i = 0;
				for (j = 0; j < mpm_number_of_indices[r0]; j++)
				{
					indices[i] = mpm[r0][j];
					i++;
				}
				for (j = 0; j < mpm_number_of_indices[r1]; j++)
				{
					indices[i] = mpm[r1][j];
					i++;
				}

				FOSs[FOSs_index] = indices;
				FOSs_number_of_indices[FOSs_index] = mpm_number_of_indices[r0] + mpm_number_of_indices[r1];
				FOSs_index++;

				mul0 = ((double)mpm_number_of_indices[r0]) / ((double)mpm_number_of_indices[r0] + mpm_number_of_indices[r1]);
				mul1 = ((double)mpm_number_of_indices[r1]) / ((double)mpm_number_of_indices[r0] + mpm_number_of_indices[r1]);
				for (i = 0; i < mpm_length; i++)
				{
					if ((i != r0) && (i != r1))
					{
						S_matrix[i][r0] = mul0 * S_matrix[i][r0] + mul1 * S_matrix[i][r1];
						S_matrix[r0][i] = S_matrix[i][r0];
					}
				}

				mpm_new = (int**)Malloc((mpm_length - 1) * sizeof(int*));
				mpm_new_number_of_indices = (int*)Malloc((mpm_length - 1) * sizeof(int));
				mpm_new_length = mpm_length - 1;
				for (i = 0; i < mpm_new_length; i++)
				{
					mpm_new[i] = mpm[i];
					mpm_new_number_of_indices[i] = mpm_number_of_indices[i];
				}

				mpm_new[r0] = indices;
				mpm_new_number_of_indices[r0] = mpm_number_of_indices[r0] + mpm_number_of_indices[r1];
				if (r1 < mpm_length - 1)
				{
					mpm_new[r1] = mpm[mpm_length - 1];
					mpm_new_number_of_indices[r1] = mpm_number_of_indices[mpm_length - 1];

					for (i = 0; i < r1; i++)
					{
						S_matrix[i][r1] = S_matrix[i][mpm_length - 1];
						S_matrix[r1][i] = S_matrix[i][r1];
					}

					for (j = r1 + 1; j < mpm_new_length; j++)
					{
						S_matrix[r1][j] = S_matrix[j][mpm_length - 1];
						S_matrix[j][r1] = S_matrix[r1][j];
					}
				}

				for (i = 0; i < NN_chain_length; i++)
				{
					if (NN_chain[i] == mpm_length - 1)
					{
						NN_chain[i] = r1;
						break;
					}
				}

				free(mpm);
				free(mpm_number_of_indices);
				mpm = mpm_new;
				mpm_number_of_indices = mpm_new_number_of_indices;
				mpm_length = mpm_new_length;

				if (mpm_length == 1)
					done = 1;
			}
		}

		free(NN_chain);

		free(mpm_new);
		free(mpm_number_of_indices);

		for (i = 0; i < number_of_parameters; i++)
			free(S_matrix[i]);
		free(S_matrix);

		free(FOSs_index_of_mpm_element);

		createClusters();
		return(parent_child_relations);
	}

	/**
	 * Determines nearest neighbour according to similarity values.
	 */
	int determineNearestNeighbour(int index, double** S_matrix, int* mpm_number_of_indices, int mpm_length)
	{
		int i, result;

		result = 0;
		if (result == index)
			result++;
		for (i = 1; i < mpm_length; i++)
		{
			if (((S_matrix[index][i] > S_matrix[index][result]) || ((S_matrix[index][i] == S_matrix[index][result]) && (mpm_number_of_indices[i] < mpm_number_of_indices[result]))) && (i != index))
				result = i;
		}

		return(result);
	}

	void computeDependencyMatrixSpecificGOMEA(Solution* newSolution, int currentPopulationSize)
	{
		int    i, j, k;
		double p, entropy, average_distance;
		std::vector<double>* genotype = newSolution->getGenotypePtr();
		for (i = 0; i < number_of_parameters; i++)
		{
			for (j = i + 1; j < number_of_parameters; j++)
			{
				/* Compute entropy of probability that variable i has a smaller value than variable j, use inverted entropy */
				if (genotype->at(i) < genotype->at(j)) {
					relative[i][j] += 1.0;
				}
				p = relative[i][j];
				p /= (double)currentPopulationSize;
				entropy = (p == 0) || (p == 1) ? 0 : -(p * log2(p) + (1.0 - p) * log2(1.0 - p));
				dependencyMatrix[i][j] = 1.0 - entropy;

				/* Multiply by inverted average distance between variables */
				adjacency[i][j] += (genotype->at(i) - genotype->at(j)) * (genotype->at(i) - genotype->at(j));
				average_distance = adjacency[i][j] / (double)currentPopulationSize;

				dependencyMatrix[i][j] *= 1.0 - average_distance;

				/* Create symmetric matrix */
				dependencyMatrix[j][i] = dependencyMatrix[i][j];
				//dependency_matrices[gomea_index][j][i] = dependency_matrices[gomea_index][i][j] = randomRealUniform01();
			}
		}
	}

	void createClusters() {
		clusters = std::vector<std::vector<int>>();
		for (int i = 0; i < FOSs_length && FOSs_number_of_indices[i] != number_of_parameters; i++) {
			clusters.push_back(std::vector<int>(FOSs[i], FOSs[i] + FOSs_number_of_indices[i]));
		}
	}
private:
	void* Malloc(long size)
	{
		void* result;

		result = (void*)malloc(size);
		if (!result)
		{
			printf("\n");
			printf("Error while allocating memory in Malloc( %ld ), aborting program.", size);
			printf("\n");

			exit(0);
		}

		return(result);
	}
	std::vector<std::vector<int>> clusters;
	int FOSs_length;
	int** FOSs;
	int* FOSs_number_of_indices;
	int number_of_parameters;
	Problem* problem;
	std::mt19937& randomGenerator;
	int64_t random_seed;
	int64_t random_seed_changing;
	std::vector<std::vector<double>> dependencyMatrix;
	std::vector<std::vector<double>> relative;
	std::vector<std::vector<double>> adjacency;
};