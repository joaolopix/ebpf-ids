
    // !!! This file is generated using emlearn !!!

    #include <eml_trees.h>
    

EmlTreesNode rf_nodes[70] = {
  { 0, 500.0, 1, 2 },
  { 4, 111166.0, 3, 4 },
  { 3, 114.0, 7, 6 },
  { 2, 1.5, 5, 6 },
  { 4, 121111.5, 6, 6 },
  { -1, 0, -1, -1 },
  { -1, 1, -1, -1 },
  { 3, 111.0, 6, 5 },
  { 3, 63.5, 9, 10 },
  { 1, 11.5, 11, 12 },
  { 0, 9500.0, 13, 14 },
  { 4, 111166.0, 5, 5 },
  { 3, 55.5, 5, 5 },
  { 4, 116166.0, 6, 6 },
  { 3, 114.0, 6, 6 },
  { 3, 63.5, 16, 17 },
  { 1, 11.5, 18, 19 },
  { 4, 116166.0, 20, 6 },
  { 3, 56.5, 5, 5 },
  { 3, 55.5, 5, 5 },
  { 1, 11.5, 6, 6 },
  { 3, 64.0, 22, 23 },
  { 4, 111166.0, 24, 25 },
  { 1, 11.5, 26, 27 },
  { 3, 60.0, 5, 5 },
  { 0, 500.0, 5, 6 },
  { 3, 112.5, 6, 6 },
  { 3, 106.5, 6, 6 },
  { 3, 63.0, 29, 30 },
  { 4, 111166.0, 31, 32 },
  { 4, 116166.0, 33, 6 },
  { 1, 11.5, 5, 5 },
  { 0, 500.0, 5, 6 },
  { 4, 111216.0, 6, 5 },
  { 3, 63.5, 35, 36 },
  { 4, 111166.0, 37, 38 },
  { 4, 116166.0, 39, 6 },
  { 4, 111116.5, 5, 5 },
  { 4, 121111.5, 5, 6 },
  { 1, 11.5, 6, 6 },
  { 0, 500.0, 41, 42 },
  { 2, 1.5, 43, 44 },
  { 3, 114.0, 45, 6 },
  { 3, 63.0, 5, 6 },
  { 1, 11.5, 6, 6 },
  { 3, 111.0, 6, 5 },
  { 0, 500.0, 47, 48 },
  { 3, 63.0, 49, 50 },
  { 2, 2.5, 51, 6 },
  { 3, 51.5, 5, 5 },
  { 3, 81.5, 6, 6 },
  { 0, 6500.0, 6, 6 },
  { 2, 1.5, 53, 54 },
  { 0, 500.0, 57, 6 },
  { 0, 9500.0, 55, 56 },
  { 0, 1500.0, 6, 6 },
  { 3, 114.0, 6, 6 },
  { 4, 111166.0, 5, 6 },
  { 4, 111166.0, 59, 60 },
  { 3, 66.5, 61, 62 },
  { 3, 54.5, 63, 64 },
  { 3, 60.0, 5, 5 },
  { 3, 106.5, 6, 6 },
  { 4, 121111.5, 5, 6 },
  { 4, 116166.0, 6, 6 },
  { 3, 63.5, 66, 67 },
  { 0, 500.0, 69, 6 },
  { 4, 116166.0, 68, 6 },
  { 3, 112.5, 6, 6 },
  { 4, 111166.0, 5, 5 } 
};

int32_t rf_tree_roots[11] = { 0, 8, 15, 21, 28, 34, 40, 46, 52, 58, 65 };

EmlTrees rf = {
        70,
        rf_nodes,	  
        11,
        rf_tree_roots,
    };

static inline int32_t rf_predict_tree_0(const float *features, int32_t features_length) {
          if (features[0] < 500.0) {
              if (features[4] < 111166.0) {
                  if (features[2] < 1.5) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[4] < 121111.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[3] < 114.0) {
                  if (features[3] < 111.0) {
                      return 1;
                  } else {
                      return 0;
                  }
              } else {
                  return 1;
              }
          }
        }
        

static inline int32_t rf_predict_tree_1(const float *features, int32_t features_length) {
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
              if (features[0] < 9500.0) {
                  if (features[4] < 116166.0) {
                      return 1;
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
        

static inline int32_t rf_predict_tree_2(const float *features, int32_t features_length) {
          if (features[3] < 63.5) {
              if (features[1] < 11.5) {
                  if (features[3] < 56.5) {
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
        

static inline int32_t rf_predict_tree_3(const float *features, int32_t features_length) {
          if (features[3] < 64.0) {
              if (features[4] < 111166.0) {
                  if (features[3] < 60.0) {
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
                  if (features[3] < 112.5) {
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
        

static inline int32_t rf_predict_tree_5(const float *features, int32_t features_length) {
          if (features[3] < 63.5) {
              if (features[4] < 111166.0) {
                  if (features[4] < 111116.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[4] < 121111.5) {
                      return 0;
                  } else {
                      return 1;
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
        

static inline int32_t rf_predict_tree_6(const float *features, int32_t features_length) {
          if (features[0] < 500.0) {
              if (features[2] < 1.5) {
                  if (features[3] < 63.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[1] < 11.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[3] < 114.0) {
                  if (features[3] < 111.0) {
                      return 1;
                  } else {
                      return 0;
                  }
              } else {
                  return 1;
              }
          }
        }
        

static inline int32_t rf_predict_tree_7(const float *features, int32_t features_length) {
          if (features[0] < 500.0) {
              if (features[3] < 63.0) {
                  if (features[3] < 51.5) {
                      return 0;
                  } else {
                      return 0;
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
                  if (features[0] < 6500.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              } else {
                  return 1;
              }
          }
        }
        

static inline int32_t rf_predict_tree_8(const float *features, int32_t features_length) {
          if (features[2] < 1.5) {
              if (features[0] < 500.0) {
                  if (features[4] < 111166.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  return 1;
              }
          } else {
              if (features[0] < 9500.0) {
                  if (features[0] < 1500.0) {
                      return 1;
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
        

static inline int32_t rf_predict_tree_9(const float *features, int32_t features_length) {
          if (features[4] < 111166.0) {
              if (features[3] < 66.5) {
                  if (features[3] < 60.0) {
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
              if (features[3] < 54.5) {
                  if (features[4] < 121111.5) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[4] < 116166.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_10(const float *features, int32_t features_length) {
          if (features[3] < 63.5) {
              if (features[0] < 500.0) {
                  if (features[4] < 111166.0) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  return 1;
              }
          } else {
              if (features[4] < 116166.0) {
                  if (features[3] < 112.5) {
                      return 1;
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
    