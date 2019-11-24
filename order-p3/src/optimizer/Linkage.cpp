
#include "../../include/order-p3/optimizer/Linkage.h"


Linkage::Linkage(int problemSize, std::mt19937& randomGenerator) :
	randomGenerator(randomGenerator),
	relativeOrderingInformationSum(problemSize, vector<double>(problemSize, 0)),
	adjacencyInformationSum(problemSize, vector<double>(problemSize, 0)),
	distanceMeasureMatrix(problemSize, vector<double>(problemSize, -1)),
	clusters(2 * problemSize - 1),
	clusterOrdering(2 * problemSize - 1),
	problemSize(problemSize)
	{
	for (size_t i = 0; i < problemSize; i++) {
		clusters[i].push_back(i);
	}

	for (size_t i = 0; i < clusterOrdering.size(); i++) {
		clusterOrdering[i] = i;
	}

	distances.resize(clusters.size(), vector<double>(clusters.size(), -1));
}

void Linkage::update(Solution* newSolution, int currentPopulationSize) {
	updateLinkageInformation(newSolution, currentPopulationSize);
	rebuildTree();
}

Linkage::ClusterIterator::ClusterIterator(size_t currentIndex, Linkage& linkage) :
	currentClusterOrderingIndex(currentIndex), linkage(linkage) {
}

bool Linkage::ClusterIterator::operator!=(const ClusterIterator& other) const {
	return this->currentClusterOrderingIndex != other.currentClusterOrderingIndex;
}

std::vector<int>& Linkage::ClusterIterator::operator*() const {
	return linkage.clusters[linkage.clusterOrdering[currentClusterOrderingIndex]];
}

Linkage::ClusterIterator& Linkage::ClusterIterator::operator++() {
	currentClusterOrderingIndex++;
	return *this;
}

Linkage::RandomClusterIterator::RandomClusterIterator(Linkage& linkage, bool end)
: linkage(linkage), unused(linkage.clusterOrdering.size() - 1), options(linkage.clusterOrdering)
{
	if(end) {
		unused = -1;
	} else {
		generateNextIndex();
	}
}

bool Linkage::RandomClusterIterator::operator!=(const RandomClusterIterator& other) const {
	return this->unused != other.unused;
}

std::vector<int>& Linkage::RandomClusterIterator::operator*() const {
	return linkage.clusters[currentIndex];
}

Linkage::RandomClusterIterator& Linkage::RandomClusterIterator::operator++() {
	generateNextIndex();
	return *this;
}

void Linkage::RandomClusterIterator::generateNextIndex() {
	if(unused > 0) {
		indexDistribution = std::uniform_int_distribution<int>(0, unused);
		int index = indexDistribution(linkage.randomGenerator);
		currentIndex = options[index];
		std::swap(options[unused - 1], options[index]);
	}
	unused--;
}

Linkage::ClusterIterator Linkage::begin() {
	return { 0, *this };
}

Linkage::ClusterIterator Linkage::end() {
	return { clusterOrdering.size(), *this };
}

Linkage::RandomClusterIterator Linkage::randomBegin() {
	return { *this };
}

Linkage::RandomClusterIterator Linkage::randomEnd() {
	return { *this, true };
}

double Linkage::getDistance(int firstIndex, int secondIndex) {
	if (firstIndex > secondIndex) {
		std::swap(firstIndex, secondIndex);
	}
	return distanceMeasureMatrix[firstIndex][secondIndex];
}

void Linkage::rebuildTree() {
	// usable keeps track of which clusters can still be merged
	vector<size_t> usable(problemSize);
	// initialize it to just the clusters of size 1
	std::iota(usable.begin(), usable.end(), 0);
	// useful keeps track of clusters that should be used by crossover
	vector<bool> useful(clusters.size(), true);
	// Shuffle the single variable clusters
	std::shuffle(clusters.begin(), clusters.begin() + problemSize, randomGenerator);

	// Find the initial distances between the clusters
	for (size_t i = 0; i < problemSize - 1; i++) {
		for (size_t j = i + 1; j < problemSize; j++) {
			distances[i][j] = getDistance(clusters[i][0], clusters[j][0]);
			// make it symmetric
			distances[j][i] = distances[i][j];
		}
	}
	// The indices of the merging clusters
	size_t first, second;
	// Used to find which two clusters are closest together
	size_t final, best_index;
	// Each iteration we add some amount to the path, and remove the last
	// two elements.  This keeps track of how much of usable is in the path.
	size_t end_of_path = 0;

	// rebuild all clusters after the single variable clusters
	for (size_t index = problemSize; index < clusters.size(); index++) {
		// Shuffle everything not yet in the path
		std::shuffle(usable.begin() + end_of_path, usable.end(), randomGenerator);

		// if nothing in the path, just add a random usable node
		if (end_of_path == 0) {
			end_of_path++;
		}

		while (end_of_path < usable.size()) {
			// last node in the path
			final = usable[end_of_path - 1];

			// best_index stores the location of the best thing in usable
			best_index = end_of_path;
			float min_dist = distances[final][usable[best_index]];
			// check all options which might be closer to "final" than "usable[best_index]"
			for (size_t option = end_of_path + 1; option < usable.size(); option++) {
				if (distances[final][usable[option]] < min_dist) {
					min_dist = distances[final][usable[option]];
					best_index = option;
				}
			}

			// If the current last two in the path are minimally distant
			if (end_of_path > 1
				and min_dist >= distances[final][usable[end_of_path - 2]]) {
				break;
			}

			// move the best to the end of the path
			std::swap(usable[end_of_path], usable[best_index]);
			end_of_path++;
		}

		// Last two elements in the path are the clusters to join
		first = usable[end_of_path - 2];
		second = usable[end_of_path - 1];

		// If the distance between the joining clusters is zero, only keep them
		// if configured to do so
		if (-1e-10 <= distances[first][second] <= 1e-10) {
			useful[first] = false;
			useful[second] = false;
		}

		// Remove things from the path
		end_of_path -= 2;

		// create new cluster
		clusters[index] = clusters[first];
		// merge the two clusters
		clusters[index].insert(clusters[index].end(), clusters[second].begin(),
			clusters[second].end());
      
		// Calculate distances from all clusters to the newly created cluster
		int i = 0;
		int end = usable.size() - 1;
		while (i <= end) {
			auto x = usable[i];
			// Removes 'first' and 'second' from usable
			if (x == first or x == second) {
				std::swap(usable[i], usable[end]);
				end--;
				continue;
			}
			// Use the previous distances to calculate the joined distance
			float first_distance = distances[first][x];
			first_distance *= clusters[first].size();
			float second_distance = distances[second][x];
			second_distance *= clusters[second].size();
			distances[x][index] = ((first_distance + second_distance)
				/ (clusters[first].size() + clusters[second].size()));
			// make it symmetric
			distances[index][x] = distances[x][index];
			i++;
		}
		// Shorten usable by 1, insert the new cluster
		usable.pop_back();
		usable.back() = index;
	}

	// Now that we know what clusters exist, determine their ordering
	clusterOrdering.resize(clusters.size());
	std::iota(clusterOrdering.begin(), clusterOrdering.end(), 0);
	
	// The last cluster contains all variables and is always useless
	useful.back() = false;

	// remove not useful clusters
	size_t kept = 0;
	for (size_t i = 0; i < clusterOrdering.size(); i++) {
		if (useful[clusterOrdering[i]]) {
			std::swap(clusterOrdering[i], clusterOrdering[kept]);
			kept++;
		}
	}
	clusterOrdering.resize(kept);
}

void Linkage::updateLinkageInformation(Solution* solution, int currentPopulationSize) {
	vector<double>* genotype = solution->getGenotypePtr();
	for (int firstGeneIndex = 0; firstGeneIndex < problemSize - 1; firstGeneIndex++) {
		const double firstGeneValue = genotype->at(firstGeneIndex);
		for (int secondGeneIndex = firstGeneIndex + 1; secondGeneIndex < problemSize; secondGeneIndex++) {
			const double secondGeneValue = genotype->at(secondGeneIndex);
			relativeOrderingInformationSum[firstGeneIndex][secondGeneIndex] += calculateRelativeOrderingInformation(firstGeneValue, secondGeneValue);
			adjacencyInformationSum[firstGeneIndex][secondGeneIndex] += calculateAdjacencyInformation(firstGeneValue, secondGeneValue);

			double linkageValue = calculateDependencyBetweenGenes(firstGeneIndex, secondGeneIndex, currentPopulationSize);
			const double distanceBetweenGenes = 1 - linkageValue;
			distanceMeasureMatrix[firstGeneIndex][secondGeneIndex] = distanceBetweenGenes;
			distanceMeasureMatrix[secondGeneIndex][firstGeneIndex] = distanceBetweenGenes;
		}
	}
}

double Linkage::calculateDependencyBetweenGenes(const int firstGeneIndex, const int secondGeneIndex, const int currentPopulationSize) {
	const double relativeOrderingMeasure = calculateRelativeOrderingMeasure(firstGeneIndex, secondGeneIndex, currentPopulationSize);
	const double adjacencyMeasure = calculateAdjacencyMeasure(firstGeneIndex, secondGeneIndex, currentPopulationSize);
	return relativeOrderingMeasure * adjacencyMeasure;
}

double Linkage::calculateRelativeOrderingMeasure(const int firstGeneIndex, const int secondGeneIndex, const int currentPopulationSize) {
	const double pij = relativeOrderingInformationSum[firstGeneIndex][secondGeneIndex] / static_cast<double>(currentPopulationSize);
	double entropy;
	if (pij == 0 || pij == 1) {
		entropy = 0;
	}
	else {
		entropy = -(pij * log2(pij) + (1.0 - pij) * log2(1.0 - pij));
	}
	return 1.0 - entropy;
}

double Linkage::calculateAdjacencyMeasure(const int firstGeneIndex, const int secondGeneIndex, const int currentPopulationSize) {
	const double adjacencyMeasure = adjacencyInformationSum[firstGeneIndex][secondGeneIndex] / static_cast<double>(currentPopulationSize);
	return 1.0 - adjacencyMeasure;
}

double Linkage::calculateRelativeOrderingInformation(const double firstGeneValue, const double secondGeneValue) {
	if (firstGeneValue < secondGeneValue) {
		return 1.0;
	}
	else {
		return 0.0;
	}
}

double Linkage::calculateAdjacencyInformation(const double firstGeneValue, const double secondGeneValue) {
	const double valuesDifference = firstGeneValue - secondGeneValue;
	return valuesDifference * valuesDifference;
}