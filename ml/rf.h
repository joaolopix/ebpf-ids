
    // !!! This file is generated using emlearn !!!

    #include <eml_trees.h>
    

EmlTreesNode rf_nodes[74] = {
  { 2, 1.5, 1, 2 },
  { 3, 63.0, 3, 4 },
  { 3, 118.0, 7, 8 },
  { 4, 111166.0, 5, 5 },
  { 1, 11.5, 6, 6 },
  { -1, 0, -1, -1 },
  { -1, 1, -1, -1 },
  { 4, 116166.0, 5, 6 },
  { 0, 2500.0, 6, 6 },
  { 2, 1.5, 10, 11 },
  { 3, 63.5, 12, 13 },
  { 3, 118.0, 14, 15 },
  { 3, 51.5, 5, 5 },
  { 1, 11.5, 6, 6 },
  { 0, 11500.0, 6, 6 },
  { 4, 111116.0, 6, 6 },
  { 2, 1.5, 17, 18 },
  { 3, 63.0, 19, 20 },
  { 2, 2.5, 21, 22 },
  { 0, 500.0, 5, 6 },
  { 3, 81.5, 6, 6 },
  { 0, 9500.0, 6, 6 },
  { 4, 111116.0, 6, 6 },
  { 3, 63.5, 24, 25 },
  { 1, 11.5, 27, 28 },
  { 4, 116166.0, 26, 6 },
  { 1, 11.5, 6, 6 },
  { 4, 111166.0, 5, 5 },
  { 3, 55.5, 5, 5 },
  { 3, 63.0, 30, 31 },
  { 4, 111166.0, 32, 33 },
  { 1, 11.5, 34, 35 },
  { 1, 11.5, 5, 5 },
  { 0, 500.0, 5, 6 },
  { 2, 1.5, 6, 6 },
  { 3, 106.5, 6, 6 },
  { 2, 1.5, 37, 38 },
  { 3, 63.5, 39, 40 },
  { 4, 116166.0, 41, 6 },
  { 4, 111166.0, 5, 5 },
  { 1, 11.5, 6, 6 },
  { 3, 122.0, 5, 6 },
  { 4, 111166.0, 43, 44 },
  { 0, 500.0, 47, 48 },
  { 3, 54.5, 45, 46 },
  { 0, 500.0, 5, 6 },
  { 4, 116166.0, 5, 6 },
  { 2, 1.5, 5, 6 },
  { 2, 9.0, 6, 5 },
  { 3, 63.5, 50, 51 },
  { 3, 51.5, 5, 52 },
  { 1, 11.5, 53, 54 },
  { 4, 111166.0, 5, 5 },
  { 3, 112.5, 6, 6 },
  { 2, 1.5, 6, 6 },
  { 3, 63.5, 56, 57 },
  { 1, 11.5, 59, 60 },
  { 4, 116166.0, 58, 6 },
  { 4, 111216.0, 6, 5 },
  { 0, 500.0, 5, 6 },
  { 3, 55.5, 5, 5 },
  { 4, 111166.0, 62, 63 },
  { 3, 66.5, 64, 65 },
  { 2, 1.5, 66, 67 },
  { 1, 11.5, 5, 5 },
  { 3, 106.5, 6, 6 },
  { 3, 54.5, 5, 6 },
  { 3, 114.0, 6, 6 },
  { 2, 1.5, 69, 70 },
  { 4, 111166.0, 71, 72 },
  { 4, 116166.0, 73, 6 },
  { 1, 11.5, 5, 5 },
  { 4, 121111.5, 6, 6 },
  { 3, 122.0, 5, 6 } 
};

int32_t rf_tree_roots[11] = { 0, 9, 16, 23, 29, 36, 42, 49, 55, 61, 68 };

EmlTrees rf = {
        74,
        rf_nodes,	  
        11,
        rf_tree_roots,
    };

static inline int32_t rf_predict_tree_0(const float *features, int32_t features_length) {
          if (features[2] < 1.5) {
              if (features[3] < 63.0) {
                  if (features[4] < 111166.0) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[1] < 11.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[3] < 118.0) {
                  if (features[4] < 116166.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[0] < 2500.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_1(const float *features, int32_t features_length) {
          if (features[2] < 1.5) {
              if (features[3] < 63.5) {
                  if (features[3] < 51.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[1] < 11.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[3] < 118.0) {
                  if (features[0] < 11500.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[4] < 111116.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_2(const float *features, int32_t features_length) {
          if (features[2] < 1.5) {
              if (features[3] < 63.0) {
                  if (features[0] < 500.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[3] < 81.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[2] < 2.5) {
                  if (features[0] < 9500.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[4] < 111116.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_3(const float *features, int32_t features_length) {
          if (features[3] < 63.5) {
              if (features[1] < 11.5) {
                  if (features[4] < 111166.0) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[3] < 55.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              }
          } else {
              if (features[4] < 116166.0) {
                  if (features[1] < 11.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              } else {
                  return 1;
              }
          }
        }
        

static inline int32_t rf_predict_tree_4(const float *features, int32_t features_length) {
          if (features[3] < 63.0) {
              if (features[4] < 111166.0) {
                  if (features[1] < 11.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[0] < 500.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[1] < 11.5) {
                  if (features[2] < 1.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[3] < 106.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_5(const float *features, int32_t features_length) {
          if (features[2] < 1.5) {
              if (features[3] < 63.5) {
                  if (features[4] < 111166.0) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[1] < 11.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[4] < 116166.0) {
                  if (features[3] < 122.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  return 1;
              }
          }
        }
        

static inline int32_t rf_predict_tree_6(const float *features, int32_t features_length) {
          if (features[4] < 111166.0) {
              if (features[0] < 500.0) {
                  if (features[2] < 1.5) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[2] < 9.0) {
                      return 1;
                  } else {
                      return 0;
                  }
              }
          } else {
              if (features[3] < 54.5) {
                  if (features[0] < 500.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[4] < 116166.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_7(const float *features, int32_t features_length) {
          if (features[3] < 63.5) {
              if (features[3] < 51.5) {
                  return 0;
              } else {
                  if (features[4] < 111166.0) {
                      return 0;
                  } else {
                      return 0;
                  }
              }
          } else {
              if (features[1] < 11.5) {
                  if (features[3] < 112.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[2] < 1.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_8(const float *features, int32_t features_length) {
          if (features[3] < 63.5) {
              if (features[1] < 11.5) {
                  if (features[0] < 500.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[3] < 55.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              }
          } else {
              if (features[4] < 116166.0) {
                  if (features[4] < 111216.0) {
                      return 1;
                  } else {
                      return 0;
                  }
              } else {
                  return 1;
              }
          }
        }
        

static inline int32_t rf_predict_tree_9(const float *features, int32_t features_length) {
          if (features[4] < 111166.0) {
              if (features[3] < 66.5) {
                  if (features[1] < 11.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[3] < 106.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[2] < 1.5) {
                  if (features[3] < 54.5) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[3] < 114.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_10(const float *features, int32_t features_length) {
          if (features[2] < 1.5) {
              if (features[4] < 111166.0) {
                  if (features[1] < 11.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[4] < 121111.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[4] < 116166.0) {
                  if (features[3] < 122.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  return 1;
              }
          }
        }
        

int32_t rf_predict(const float *features, int32_t features_length) {

        int32_t votes[2] = {0,};
        int32_t _class = -1;

        _class = rf_predict_tree_0(features, features_length); votes[_class] += 1;
    _class = rf_predict_tree_1(features, features_length); votes[_class] += 1;
    _class = rf_predict_tree_2(features, features_length); votes[_class] += 1;
    _class = rf_predict_tree_3(features, features_length); votes[_class] += 1;
    _class = rf_predict_tree_4(features, features_length); votes[_class] += 1;
    _class = rf_predict_tree_5(features, features_length); votes[_class] += 1;
    _class = rf_predict_tree_6(features, features_length); votes[_class] += 1;
    _class = rf_predict_tree_7(features, features_length); votes[_class] += 1;
    _class = rf_predict_tree_8(features, features_length); votes[_class] += 1;
    _class = rf_predict_tree_9(features, features_length); votes[_class] += 1;
    _class = rf_predict_tree_10(features, features_length); votes[_class] += 1;
    
        int32_t most_voted_class = -1;
        int32_t most_voted_votes = 0;
        for (int32_t i=0; i<2; i++) {

            if (votes[i] > most_voted_votes) {
                most_voted_class = i;
                most_voted_votes = votes[i];
            }
        }
        return most_voted_class;
    }
    