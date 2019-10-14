#if INTERFACE

#include <stdlib.h>
#include "dataset.h"
#include "information.h"

typedef struct DecisionTree {
  Dataset *dataset;
  unsigned int feature_mask;
  unsigned int feature_index;
  double split_value;
  DecisionTree *parent;
  DecisionTree *left;
  DecisionTree *right;
} DecisionTree;

#endif

#include "decision_tree.h"

// Create a new decision tree from a dataset.
DecisionTree* decision_tree_create (Dataset *dataset, unsigned int feature_mask, DecisionTree *parent) {
  DecisionTree *result = calloc(1, sizeof(DecisionTree));
  result->dataset = dataset;
  result->feature_mask = feature_mask;
  result->parent = parent;
  return result;
}

void decision_tree_train(DecisionTree *decision_tree) {
  printf("Training on %u entries.\n", decision_tree->dataset->entry_count);
  unsigned int feature_index;
  double split_value;
  printf("Trying to find best split...\n");
  double gain = information_find_best_split(decision_tree->dataset, &feature_index,&split_value);
  if (gain > 0.0001) {
    printf("Found best split at %u : %f with %f information gain\n", feature_index, split_value, gain);
    decision_tree->feature_index = feature_index;
    decision_tree->split_value = split_value;
    Dataset *left_dataset;
    Dataset *right_dataset;
    dataset_split(decision_tree->dataset, decision_tree->feature_index,  decision_tree->split_value,&left_dataset,&right_dataset);
    decision_tree->left = decision_tree_create(left_dataset, decision_tree->feature_mask, decision_tree);
    decision_tree->right = decision_tree_create(right_dataset, decision_tree->feature_mask, decision_tree);
    decision_tree_train(decision_tree->left);
    decision_tree_train(decision_tree->right);
  } else {
    printf("Unable to find a good enough split.\n");
  }
}
