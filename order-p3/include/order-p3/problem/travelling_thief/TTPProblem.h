#pragma once
#include "KnapsackItem.h"
#include "City.h"
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <limits>
#include "../Problem.h"

enum ItemSelectionPolicy {
    ProfitWeightRatio, Lightest, MostProfitable
};

class TtpProblem : public Problem {
public:
    TtpProblem() = default;
    ~TtpProblem();
	double evaluate(std::vector<int>& solution) override;

    void initialize(const std::string &filename, ItemSelectionPolicy policy);
    double selectedItemsWeight;
    double selectedItemsProfit;
    double getDistance(int firstCityIndex, int secondCityIndex);

private:
    void load(const std::string &filename);
    void selectItems(ItemSelectionPolicy policy);
    void calculateDistances();
    void addNewItem(int index, int profit, int weight, int assignedNodeIndex);
    std::vector<std::string> splitInputLine(std::string &line) const;


    static bool selectBetterWeightProfitItem(KnapsackItem *firstItem, KnapsackItem *secondItem);
    static bool selectLighterItem(KnapsackItem *firstItem, KnapsackItem *secondItem);
    static bool selectProfitableItem(KnapsackItem *firstItem, KnapsackItem *secondItem);
    int numberOfCities;
    int numberOfItems;
    int capacityOfKnapsack;
    double vMin;
    double vMax;

	std::vector<std::vector<double>> cityDistances;

    std::vector<KnapsackItem*> allItems;
    std::unordered_map<int, std::vector<KnapsackItem*>> selectedItems;
    std::unordered_map<int, double> weightsInCities;
    std::unordered_map<int, double> profitInCities;

    std::vector<City*> cities;

    double calculateDistance(City *firstCity, City *secondCity);

    void selectAllFittingItems();
};