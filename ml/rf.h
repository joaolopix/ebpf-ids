
    // !!! This file is generated using emlearn !!!

    #include <eml_trees.h>
    

EmlTreesNode rf_nodes[67] = {
  { 3, 63.0, 1, 2 },
  { 1, 11.5, 3, 4 },
  { 3, 112.5, 6, 7 },
  { 3, 57.5, 5, 5 },
  { 3, 55.5, 5, 5 },
  { -1, 0, -1, -1 },
  { 0, 500.0, 8, 8 },
  { 1, 11.5, 8, 8 },
  { -1, 1, -1, -1 },
  { 3, 63.0, 10, 11 },
  { 0, 500.0, 13, 8 },
  { 4, 116166.0, 12, 8 },
  { 3, 112.5, 8, 8 },
  { 3, 51.5, 5, 5 },
  { 4, 111166.0, 15, 16 },
  { 2, 1.5, 17, 8 },
  { 2, 1.5, 18, 19 },
  { 3, 63.0, 5, 8 },
  { 4, 121111.5, 8, 8 },
  { 2, 2.5, 8, 8 },
  { 3, 63.0, 21, 22 },
  { 3, 51.5, 5, 23 },
  { 4, 116166.0, 24, 8 },
  { 4, 111166.0, 5, 5 },
  { 1, 11.5, 8, 8 },
  { 3, 63.0, 26, 27 },
  { 1, 11.5, 28, 29 },
  { 4, 116166.0, 30, 8 },
  { 4, 111166.0, 5, 5 },
  { 3, 55.5, 5, 5 },
  { 4, 111216.0, 8, 5 },
  { 4, 111166.0, 32, 33 },
  { 3, 67.5, 34, 35 },
  { 4, 121111.5, 36, 8 },
  { 1, 11.5, 5, 5 },
  { 3, 106.5, 8, 8 },
  { 0, 500.0, 8, 5 },
  { 3, 63.0, 38, 39 },
  { 0, 500.0, 42, 8 },
  { 1, 11.5, 40, 41 },
  { 0, 500.0, 8, 8 },
  { 2, 1.5, 8, 8 },
  { 3, 51.5, 5, 5 },
  { 0, 500.0, 44, 45 },
  { 2, 1.5, 46, 47 },
  { 3, 114.0, 48, 8 },
  { 1, 11.5, 5, 5 },
  { 3, 116.0, 8, 8 },
  { 3, 111.0, 8, 5 },
  { 2, 1.5, 50, 51 },
  { 1, 11.5, 52, 53 },
  { 3, 114.0, 54, 8 },
  { 4, 111166.0, 5, 8 },
  { 0, 1000.0, 5, 8 },
  { 4, 116166.0, 5, 8 },
  { 4, 111166.0, 56, 57 },
  { 2, 1.5, 60, 8 },
  { 3, 54.5, 58, 59 },
  { 4, 121111.5, 5, 8 },
  { 0, 500.0, 8, 8 },
  { 0, 1000.0, 5, 8 },
  { 2, 1.5, 62, 63 },
  { 3, 64.0, 64, 65 },
  { 3, 114.0, 66, 8 },
  { 4, 111166.0, 5, 5 },
  { 1, 11.5, 8, 8 },
  { 3, 111.0, 8, 5 } 
};

int32_t rf_tree_roots[11] = { 0, 9, 14, 20, 25, 31, 37, 43, 49, 55, 61 };

EmlTrees rf = {
        67,
        rf_nodes,	  
        11,
        rf_tree_roots,
    };

static inline int32_t rf_predict_tree_0(const float *features, int32_t features_length) {
          if (features[3] < 63.0) {
              if (features[1] < 11.5) {
                  if (features[3] < 57.5) {
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
              if (features[3] < 112.5) {
                  if (features[0] < 500.0) {
                      return 1;
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
                  if (features[3] < 63.0) {
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
              if (features[3] < 67.5) {
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
                  if (features[0] < 500.0) {
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
              if (features[1] < 11.5) {
                  if (features[0] < 500.0) {
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
                  if (features[0] < 1000.0) {
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
              if (features[3] < 64.0) {
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
    