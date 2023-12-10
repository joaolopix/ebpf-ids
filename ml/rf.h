
    // !!! This file is generated using emlearn !!!

    #include <eml_trees.h>
    

EmlTreesNode rf_nodes[66] = {
  { 3, 63.5, 1, 2 },
  { 0, 500.0, 6, 5 },
  { 1, 11.5, 3, 4 },
  { 4, 116166.0, 5, 5 },
  { 2, 1.5, 5, 5 },
  { -1, 1, -1, -1 },
  { 1, 11.5, 7, 7 },
  { -1, 0, -1, -1 },
  { 3, 63.0, 9, 10 },
  { 0, 500.0, 12, 5 },
  { 4, 116166.0, 11, 5 },
  { 3, 112.5, 5, 5 },
  { 3, 51.5, 7, 7 },
  { 4, 111166.0, 14, 15 },
  { 2, 1.5, 16, 5 },
  { 2, 1.5, 17, 18 },
  { 3, 67.5, 7, 5 },
  { 4, 121111.5, 5, 5 },
  { 2, 2.5, 5, 5 },
  { 3, 63.0, 20, 21 },
  { 3, 51.5, 7, 22 },
  { 4, 116166.0, 23, 5 },
  { 4, 111166.0, 7, 7 },
  { 1, 11.5, 5, 5 },
  { 3, 63.0, 25, 26 },
  { 1, 11.5, 27, 28 },
  { 4, 116166.0, 29, 5 },
  { 4, 111166.0, 7, 7 },
  { 3, 55.5, 7, 7 },
  { 4, 111216.0, 5, 7 },
  { 4, 111166.0, 31, 32 },
  { 3, 66.5, 33, 34 },
  { 4, 121111.5, 35, 5 },
  { 1, 11.5, 7, 7 },
  { 3, 106.5, 5, 5 },
  { 0, 1500.0, 5, 7 },
  { 3, 63.5, 37, 38 },
  { 0, 500.0, 41, 5 },
  { 1, 11.5, 39, 40 },
  { 0, 11500.0, 5, 5 },
  { 2, 1.5, 5, 5 },
  { 3, 51.5, 7, 7 },
  { 0, 500.0, 43, 44 },
  { 2, 1.5, 45, 46 },
  { 3, 114.0, 47, 5 },
  { 1, 11.5, 7, 7 },
  { 3, 116.0, 5, 5 },
  { 3, 111.0, 5, 7 },
  { 2, 1.5, 49, 50 },
  { 1, 11.5, 51, 52 },
  { 3, 114.0, 53, 5 },
  { 4, 111166.0, 7, 5 },
  { 0, 1000.0, 7, 5 },
  { 4, 116166.0, 7, 5 },
  { 4, 111166.0, 55, 56 },
  { 2, 1.5, 59, 5 },
  { 3, 54.5, 57, 58 },
  { 4, 121111.5, 7, 5 },
  { 0, 500.0, 5, 5 },
  { 3, 67.5, 7, 5 },
  { 2, 1.5, 61, 62 },
  { 3, 63.5, 63, 64 },
  { 3, 114.0, 65, 5 },
  { 4, 111166.0, 7, 7 },
  { 1, 11.5, 5, 5 },
  { 3, 111.0, 5, 7 } 
};

int32_t rf_tree_roots[11] = { 0, 8, 13, 19, 24, 30, 36, 42, 48, 54, 60 };

EmlTrees rf = {
        66,
        rf_nodes,	  
        11,
        rf_tree_roots,
    };

static inline int32_t rf_predict_tree_0(const float *features, int32_t features_length) {
          if (features[3] < 63.5) {
              if (features[0] < 500.0) {
                  if (features[1] < 11.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  return 1;
              }
          } else {
              if (features[1] < 11.5) {
                  if (features[4] < 116166.0) {
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
        

static inline int32_t rf_predict_tree_1(const float *features, int32_t features_length) {
          if (features[3] < 63.0) {
              if (features[0] < 500.0) {
                  if (features[3] < 51.5) {
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
        

static inline int32_t rf_predict_tree_2(const float *features, int32_t features_length) {
          if (features[4] < 111166.0) {
              if (features[2] < 1.5) {
                  if (features[3] < 67.5) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  return 1;
              }
          } else {
              if (features[2] < 1.5) {
                  if (features[4] < 121111.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[2] < 2.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_3(const float *features, int32_t features_length) {
          if (features[3] < 63.0) {
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
              if (features[4] < 121111.5) {
                  if (features[0] < 1500.0) {
                      return 1;
                  } else {
                      return 0;
                  }
              } else {
                  return 1;
              }
          }
        }
        

static inline int32_t rf_predict_tree_6(const float *features, int32_t features_length) {
          if (features[3] < 63.5) {
              if (features[0] < 500.0) {
                  if (features[3] < 51.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  return 1;
              }
          } else {
              if (features[1] < 11.5) {
                  if (features[0] < 11500.0) {
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
        

static inline int32_t rf_predict_tree_7(const float *features, int32_t features_length) {
          if (features[0] < 500.0) {
              if (features[2] < 1.5) {
                  if (features[1] < 11.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[3] < 116.0) {
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
        

static inline int32_t rf_predict_tree_8(const float *features, int32_t features_length) {
          if (features[2] < 1.5) {
              if (features[1] < 11.5) {
                  if (features[4] < 111166.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[0] < 1000.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[3] < 114.0) {
                  if (features[4] < 116166.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  return 1;
              }
          }
        }
        

static inline int32_t rf_predict_tree_9(const float *features, int32_t features_length) {
          if (features[4] < 111166.0) {
              if (features[2] < 1.5) {
                  if (features[3] < 67.5) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  return 1;
              }
          } else {
              if (features[3] < 54.5) {
                  if (features[4] < 121111.5) {
                      return 0;
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
        

static inline int32_t rf_predict_tree_10(const float *features, int32_t features_length) {
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
    