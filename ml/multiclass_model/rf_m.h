
    // !!! This file is generated using emlearn !!!

    #include <eml_trees.h>
    

EmlTreesNode rf_nodes[72] = {
  { 4, 116166.0, 1, 2 },
  { 4, 111116.5, 3, 4 },
  { 4, 121111.5, 7, 8 },
  { 4, 111111.5, 5, 6 },
  { 4, 111166.0, 10, 10 },
  { -1, 1, -1, -1 },
  { -1, 2, -1, -1 },
  { 2, 1.5, 9, 8 },
  { -1, 0, -1, -1 },
  { -1, 4, -1, -1 },
  { -1, 3, -1, -1 },
  { 4, 116166.0, 12, 13 },
  { 3, 56.0, 14, 15 },
  { 4, 121111.5, 16, 8 },
  { 3, 51.5, 5, 6 },
  { 3, 112.5, 10, 8 },
  { 0, 500.0, 9, 8 },
  { 1, 11.5, 18, 19 },
  { 2, 1.5, 20, 21 },
  { 2, 1.5, 22, 8 },
  { 0, 500.0, 9, 8 },
  { 4, 116166.0, 10, 8 },
  { 3, 67.5, 5, 8 },
  { 4, 116166.0, 24, 25 },
  { 4, 111116.5, 26, 27 },
  { 2, 1.5, 28, 8 },
  { 1, 11.5, 6, 5 },
  { 3, 56.0, 8, 10 },
  { 0, 500.0, 9, 8 },
  { 4, 116166.0, 30, 31 },
  { 2, 1.5, 33, 34 },
  { 3, 54.5, 32, 8 },
  { 4, 121111.5, 9, 8 },
  { 4, 111116.5, 6, 10 },
  { 1, 11.5, 10, 8 },
  { 4, 116166.0, 36, 37 },
  { 3, 56.0, 38, 39 },
  { 3, 54.5, 40, 8 },
  { 3, 51.5, 5, 6 },
  { 1, 11.5, 10, 8 },
  { 4, 121111.5, 9, 8 },
  { 1, 11.5, 42, 43 },
  { 4, 116166.0, 44, 45 },
  { 3, 67.5, 46, 47 },
  { 3, 56.0, 6, 10 },
  { 2, 1.5, 9, 8 },
  { 3, 64.0, 5, 5 },
  { 2, 1.5, 8, 8 },
  { 4, 116166.0, 49, 50 },
  { 2, 1.5, 52, 53 },
  { 3, 54.5, 51, 8 },
  { 0, 1000.0, 9, 8 },
  { 4, 111116.5, 6, 10 },
  { 3, 120.0, 10, 8 },
  { 4, 116166.0, 55, 56 },
  { 3, 56.0, 57, 58 },
  { 4, 121111.5, 59, 8 },
  { 4, 111111.5, 5, 6 },
  { 2, 1.5, 10, 8 },
  { 3, 54.5, 9, 8 },
  { 3, 54.5, 61, 62 },
  { 0, 500.0, 65, 8 },
  { 3, 60.0, 63, 64 },
  { 4, 116116.0, 10, 8 },
  { 0, 11500.0, 8, 8 },
  { 3, 51.5, 5, 9 },
  { 4, 116166.0, 67, 68 },
  { 3, 56.0, 69, 70 },
  { 0, 500.0, 71, 8 },
  { 3, 51.5, 5, 6 },
  { 4, 111116.0, 8, 10 },
  { 2, 1.5, 9, 8 } 
};

int32_t rf_tree_roots[11] = { 0, 11, 17, 23, 29, 35, 41, 48, 54, 60, 66 };

EmlTrees rf = {
        72,
        rf_nodes,	  
        11,
        rf_tree_roots,
    };

static inline int32_t rf_predict_tree_0(const float *features, int32_t features_length) {
          if (features[4] < 116166.0) {
              if (features[4] < 111116.5) {
                  if (features[4] < 111111.5) {
                      return 1;
                  } else {
                      return 2;
                  }
              } else {
                  if (features[4] < 111166.0) {
                      return 3;
                  } else {
                      return 3;
                  }
              }
          } else {
              if (features[4] < 121111.5) {
                  if (features[2] < 1.5) {
                      return 4;
                  } else {
                      return 0;
                  }
              } else {
                  return 0;
              }
          }
        }
        

static inline int32_t rf_predict_tree_1(const float *features, int32_t features_length) {
          if (features[4] < 116166.0) {
              if (features[3] < 56.0) {
                  if (features[3] < 51.5) {
                      return 1;
                  } else {
                      return 2;
                  }
              } else {
                  if (features[3] < 112.5) {
                      return 3;
                  } else {
                      return 0;
                  }
              }
          } else {
              if (features[4] < 121111.5) {
                  if (features[0] < 500.0) {
                      return 4;
                  } else {
                      return 0;
                  }
              } else {
                  return 0;
              }
          }
        }
        

static inline int32_t rf_predict_tree_2(const float *features, int32_t features_length) {
          if (features[1] < 11.5) {
              if (features[2] < 1.5) {
                  if (features[0] < 500.0) {
                      return 4;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[4] < 116166.0) {
                      return 3;
                  } else {
                      return 0;
                  }
              }
          } else {
              if (features[2] < 1.5) {
                  if (features[3] < 67.5) {
                      return 1;
                  } else {
                      return 0;
                  }
              } else {
                  return 0;
              }
          }
        }
        

static inline int32_t rf_predict_tree_3(const float *features, int32_t features_length) {
          if (features[4] < 116166.0) {
              if (features[4] < 111116.5) {
                  if (features[1] < 11.5) {
                      return 2;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[3] < 56.0) {
                      return 0;
                  } else {
                      return 3;
                  }
              }
          } else {
              if (features[2] < 1.5) {
                  if (features[0] < 500.0) {
                      return 4;
                  } else {
                      return 0;
                  }
              } else {
                  return 0;
              }
          }
        }
        

static inline int32_t rf_predict_tree_4(const float *features, int32_t features_length) {
          if (features[4] < 116166.0) {
              if (features[2] < 1.5) {
                  if (features[4] < 111116.5) {
                      return 2;
                  } else {
                      return 3;
                  }
              } else {
                  if (features[1] < 11.5) {
                      return 3;
                  } else {
                      return 0;
                  }
              }
          } else {
              if (features[3] < 54.5) {
                  if (features[4] < 121111.5) {
                      return 4;
                  } else {
                      return 0;
                  }
              } else {
                  return 0;
              }
          }
        }
        

static inline int32_t rf_predict_tree_5(const float *features, int32_t features_length) {
          if (features[4] < 116166.0) {
              if (features[3] < 56.0) {
                  if (features[3] < 51.5) {
                      return 1;
                  } else {
                      return 2;
                  }
              } else {
                  if (features[1] < 11.5) {
                      return 3;
                  } else {
                      return 0;
                  }
              }
          } else {
              if (features[3] < 54.5) {
                  if (features[4] < 121111.5) {
                      return 4;
                  } else {
                      return 0;
                  }
              } else {
                  return 0;
              }
          }
        }
        

static inline int32_t rf_predict_tree_6(const float *features, int32_t features_length) {
          if (features[1] < 11.5) {
              if (features[4] < 116166.0) {
                  if (features[3] < 56.0) {
                      return 2;
                  } else {
                      return 3;
                  }
              } else {
                  if (features[2] < 1.5) {
                      return 4;
                  } else {
                      return 0;
                  }
              }
          } else {
              if (features[3] < 67.5) {
                  if (features[3] < 64.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[2] < 1.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_7(const float *features, int32_t features_length) {
          if (features[4] < 116166.0) {
              if (features[2] < 1.5) {
                  if (features[4] < 111116.5) {
                      return 2;
                  } else {
                      return 3;
                  }
              } else {
                  if (features[3] < 120.0) {
                      return 3;
                  } else {
                      return 0;
                  }
              }
          } else {
              if (features[3] < 54.5) {
                  if (features[0] < 1000.0) {
                      return 4;
                  } else {
                      return 0;
                  }
              } else {
                  return 0;
              }
          }
        }
        

static inline int32_t rf_predict_tree_8(const float *features, int32_t features_length) {
          if (features[4] < 116166.0) {
              if (features[3] < 56.0) {
                  if (features[4] < 111111.5) {
                      return 1;
                  } else {
                      return 2;
                  }
              } else {
                  if (features[2] < 1.5) {
                      return 3;
                  } else {
                      return 0;
                  }
              }
          } else {
              if (features[4] < 121111.5) {
                  if (features[3] < 54.5) {
                      return 4;
                  } else {
                      return 0;
                  }
              } else {
                  return 0;
              }
          }
        }
        

static inline int32_t rf_predict_tree_9(const float *features, int32_t features_length) {
          if (features[3] < 54.5) {
              if (features[0] < 500.0) {
                  if (features[3] < 51.5) {
                      return 1;
                  } else {
                      return 4;
                  }
              } else {
                  return 0;
              }
          } else {
              if (features[3] < 60.0) {
                  if (features[4] < 116116.0) {
                      return 3;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[0] < 11500.0) {
                      return 0;
                  } else {
                      return 0;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_10(const float *features, int32_t features_length) {
          if (features[4] < 116166.0) {
              if (features[3] < 56.0) {
                  if (features[3] < 51.5) {
                      return 1;
                  } else {
                      return 2;
                  }
              } else {
                  if (features[4] < 111116.0) {
                      return 0;
                  } else {
                      return 3;
                  }
              }
          } else {
              if (features[0] < 500.0) {
                  if (features[2] < 1.5) {
                      return 4;
                  } else {
                      return 0;
                  }
              } else {
                  return 0;
              }
          }
        }
        

int32_t rf_predict(const float *features, int32_t features_length) {

        int32_t votes[5] = {0,};
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
        for (int32_t i=0; i<5; i++) {

            if (votes[i] > most_voted_votes) {
                most_voted_class = i;
                most_voted_votes = votes[i];
            }
        }
        return most_voted_class;
    }
    