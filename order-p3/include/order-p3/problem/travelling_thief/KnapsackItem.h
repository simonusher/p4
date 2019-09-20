#pragma once
class KnapsackItem {
public:
    KnapsackItem(int index, double profit, double weight, int assignedNodeIndex);
    int getIndex() const;
    double getProfit() const;
    double getWeight() const;
    double getProfitWeightRatio() const;

    int getAssignedNodeIndex() const;

private:
    int index;
    double profit;
    double weight;
    int assignedNodeIndex;
};