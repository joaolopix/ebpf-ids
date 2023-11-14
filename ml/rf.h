
    // !!! This file is generated using emlearn !!!

    #include <eml_trees.h>
    

EmlTreesNode rf_nodes[70] = {
  { 0, 500.0, 1, 2 },
  { 2, 33356.5, 3, 4 },
  { 5, 116166.0, 7, 5 },
  { 2, 451.5, 5, 6 },
  { 1, 11.5, 5, 6 },
  { -1, 1, -1, -1 },
  { -1, 0, -1, -1 },
  { 1, 11.5, 6, 5 },
  { 0, 500.0, 9, 10 },
  { 4, 63.5, 11, 12 },
  { 4, 96.0, 13, 14 },
  { 2, 443.5, 6, 6 },
  { 1, 11.5, 5, 5 },
  { 1, 3.5, 6, 5 },
  { 1, 3.5, 6, 5 },
  { 2, 451.5, 16, 17 },
  { 4, 60.0, 20, 21 },
  { 0, 500.0, 18, 19 },
  { 5, 111166.0, 6, 5 },
  { 0, 6500.0, 5, 5 },
  { 2, 438.5, 6, 5 },
  { 2, 23.5, 6, 5 },
  { 5, 111166.0, 23, 24 },
  { 4, 67.5, 25, 26 },
  { 4, 54.5, 27, 28 },
  { 4, 63.5, 6, 6 },
  { 1, 1.5, 6, 5 },
  { 0, 500.0, 5, 5 },
  { 0, 500.0, 5, 5 },
  { 5, 111166.0, 30, 31 },
  { 1, 11.5, 32, 33 },
  { 0, 8500.0, 34, 35 },
  { 0, 167000.0, 6, 6 },
  { 4, 67.5, 6, 5 },
  { 5, 116166.0, 5, 5 },
  { 4, 114.0, 5, 5 },
  { 2, 451.5, 37, 38 },
  { 1, 3.5, 41, 42 },
  { 5, 111166.0, 39, 40 },
  { 4, 82.5, 6, 5 },
  { 4, 56.0, 5, 5 },
  { 2, 5.5, 6, 6 },
  { 2, 51.0, 6, 5 },
  { 0, 500.0, 44, 45 },
  { 4, 63.5, 46, 47 },
  { 5, 116166.0, 48, 5 },
  { 5, 111166.0, 6, 5 },
  { 5, 111116.0, 5, 5 },
  { 4, 114.0, 6, 5 },
  { 4, 63.5, 50, 51 },
  { 2, 443.5, 52, 53 },
  { 2, 23.5, 54, 55 },
  { 4, 51.5, 6, 6 },
  { 5, 111166.0, 6, 5 },
  { 1, 1.5, 6, 6 },
  { 5, 116166.0, 5, 5 },
  { 4, 64.0, 57, 58 },
  { 1, 11.5, 61, 62 },
  { 2, 23.5, 59, 60 },
  { 2, 1.5, 5, 6 },
  { 5, 116166.0, 5, 5 },
  { 4, 56.5, 6, 6 },
  { 2, 3568.5, 6, 6 },
  { 4, 63.5, 64, 65 },
  { 5, 111166.0, 66, 67 },
  { 2, 23.5, 68, 69 },
  { 1, 3.5, 6, 6 },
  { 2, 32794.0, 5, 5 },
  { 0, 2402000.0, 6, 5 },
  { 5, 116166.0, 5, 5 } 
};

int32_t rf_tree_roots[10] = { 0, 8, 15, 22, 29, 36, 43, 49, 56, 63 };

EmlTrees rf = {
        70,
        rf_nodes,	  
        10,
        rf_tree_roots,
    };

static inline int32_t rf_predict_tree_0(const float *features, int32_t features_length) {
          if (features[0] < 500.0) {
              if (features[2] < 33356.5) {
                  if (features[2] < 451.5) {
                      return 1;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[1] < 11.5) {
                      return 1;
                  } else {
                      return 0;
                  }
              }
          } else {
              if (features[5] < 116166.0) {
                  if (features[1] < 11.5) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  return 1;
              }
          }
        }
        

static inline int32_t rf_predict_tree_1(const float *features, int32_t features_length) {
          if (features[0] < 500.0) {
              if (features[4] < 63.5) {
                  if (features[2] < 443.5) {
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
              if (features[4] < 96.0) {
                  if (features[1] < 3.5) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[1] < 3.5) {
                      return 0;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_2(const float *features, int32_t features_length) {
          if (features[2] < 451.5) {
              if (features[4] < 60.0) {
                  if (features[2] < 438.5) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[2] < 23.5) {
                      return 0;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[0] < 500.0) {
                  if (features[5] < 111166.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[0] < 6500.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_3(const float *features, int32_t features_length) {
          if (features[5] < 111166.0) {
              if (features[4] < 67.5) {
                  if (features[4] < 63.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[1] < 1.5) {
                      return 0;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[4] < 54.5) {
                  if (features[0] < 500.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[0] < 500.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_4(const float *features, int32_t features_length) {
          if (features[5] < 111166.0) {
              if (features[1] < 11.5) {
                  if (features[0] < 167000.0) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[4] < 67.5) {
                      return 0;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[0] < 8500.0) {
                  if (features[5] < 116166.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[4] < 114.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_5(const float *features, int32_t features_length) {
          if (features[2] < 451.5) {
              if (features[1] < 3.5) {
                  if (features[2] < 5.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[2] < 51.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[5] < 111166.0) {
                  if (features[4] < 82.5) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[4] < 56.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_6(const float *features, int32_t features_length) {
          if (features[0] < 500.0) {
              if (features[4] < 63.5) {
                  if (features[5] < 111166.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[5] < 111116.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[5] < 116166.0) {
                  if (features[4] < 114.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  return 1;
              }
          }
        }
        

static inline int32_t rf_predict_tree_7(const float *features, int32_t features_length) {
          if (features[4] < 63.5) {
              if (features[2] < 443.5) {
                  if (features[4] < 51.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[5] < 111166.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[2] < 23.5) {
                  if (features[1] < 1.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[5] < 116166.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_8(const float *features, int32_t features_length) {
          if (features[4] < 64.0) {
              if (features[1] < 11.5) {
                  if (features[4] < 56.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[2] < 3568.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              }
          } else {
              if (features[2] < 23.5) {
                  if (features[2] < 1.5) {
                      return 1;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[5] < 116166.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_9(const float *features, int32_t features_length) {
          if (features[4] < 63.5) {
              if (features[5] < 111166.0) {
                  if (features[1] < 3.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[2] < 32794.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[2] < 23.5) {
                  if (features[0] < 2402000.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[5] < 116166.0) {
                      return 1;
                  } else {
                      return 1;
                  }
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
    