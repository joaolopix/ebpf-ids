
    // !!! This file is generated using emlearn !!!

    #include <eml_trees.h>
    

EmlTreesNode rf_nodes[70] = {
  { 3, 1.5, 1, 2 },
  { 4, 63.5, 3, 4 },
  { 3, 2.5, 7, 6 },
  { 5, 111166.0, 5, 6 },
  { 1, 11.5, 6, 6 },
  { -1, 0, -1, -1 },
  { -1, 1, -1, -1 },
  { 4, 114.0, 6, 6 },
  { 5, 111166.0, 9, 10 },
  { 3, 1.5, 11, 12 },
  { 0, 7500.0, 13, 14 },
  { 4, 67.5, 5, 6 },
  { 4, 473.0, 6, 6 },
  { 5, 116166.0, 6, 6 },
  { 0, 9500.0, 6, 6 },
  { 5, 111166.0, 16, 17 },
  { 4, 67.5, 18, 19 },
  { 4, 54.5, 20, 21 },
  { 2, 80.5, 5, 5 },
  { 4, 106.5, 6, 6 },
  { 5, 121111.5, 6, 6 },
  { 5, 116166.0, 6, 6 },
  { 4, 63.5, 23, 24 },
  { 2, 443.5, 25, 26 },
  { 1, 11.5, 27, 28 },
  { 0, 500.0, 5, 6 },
  { 4, 49.0, 5, 5 },
  { 4, 112.5, 6, 6 },
  { 4, 106.5, 6, 6 },
  { 4, 63.5, 30, 31 },
  { 5, 111166.0, 32, 33 },
  { 1, 11.5, 34, 35 },
  { 2, 443.5, 5, 5 },
  { 2, 32798.0, 6, 6 },
  { 2, 291.0, 6, 6 },
  { 3, 1.5, 6, 6 },
  { 3, 1.5, 37, 38 },
  { 4, 63.5, 39, 40 },
  { 4, 114.0, 41, 42 },
  { 5, 111166.0, 5, 6 },
  { 1, 11.5, 6, 6 },
  { 2, 291.0, 6, 6 },
  { 5, 111116.0, 6, 6 },
  { 2, 451.5, 44, 45 },
  { 4, 60.0, 48, 49 },
  { 4, 64.0, 46, 47 },
  { 1, 11.5, 5, 5 },
  { 3, 1.5, 6, 6 },
  { 1, 11.5, 5, 5 },
  { 4, 114.0, 6, 6 },
  { 3, 1.5, 51, 52 },
  { 2, 33356.0, 53, 54 },
  { 5, 116166.0, 55, 6 },
  { 5, 111166.0, 5, 6 },
  { 4, 60.0, 5, 6 },
  { 0, 500.0, 6, 6 },
  { 2, 451.5, 57, 58 },
  { 2, 51.0, 61, 62 },
  { 5, 111166.0, 59, 60 },
  { 2, 33356.5, 5, 5 },
  { 2, 32785.5, 5, 6 },
  { 2, 24.5, 5, 5 },
  { 3, 1.5, 6, 6 },
  { 4, 63.5, 64, 65 },
  { 5, 111166.0, 66, 67 },
  { 0, 6500.0, 68, 69 },
  { 5, 111116.5, 5, 5 },
  { 5, 121111.5, 6, 6 },
  { 1, 11.5, 6, 6 },
  { 0, 9500.0, 6, 6 } 
};

int32_t rf_tree_roots[10] = { 0, 8, 15, 22, 29, 36, 43, 50, 56, 63 };

EmlTrees rf = {
        70,
        rf_nodes,	  
        10,
        rf_tree_roots,
    };

static inline int32_t rf_predict_tree_0(const float *features, int32_t features_length) {
          if (features[3] < 1.5) {
              if (features[4] < 63.5) {
                  if (features[5] < 111166.0) {
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
              if (features[3] < 2.5) {
                  if (features[4] < 114.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              } else {
                  return 1;
              }
          }
        }
        

static inline int32_t rf_predict_tree_1(const float *features, int32_t features_length) {
          if (features[5] < 111166.0) {
              if (features[3] < 1.5) {
                  if (features[4] < 67.5) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[4] < 473.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[0] < 7500.0) {
                  if (features[5] < 116166.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[0] < 9500.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_2(const float *features, int32_t features_length) {
          if (features[5] < 111166.0) {
              if (features[4] < 67.5) {
                  if (features[2] < 80.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[4] < 106.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[4] < 54.5) {
                  if (features[5] < 121111.5) {
                      return 1;
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
        

static inline int32_t rf_predict_tree_3(const float *features, int32_t features_length) {
          if (features[4] < 63.5) {
              if (features[2] < 443.5) {
                  if (features[0] < 500.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[4] < 49.0) {
                      return 0;
                  } else {
                      return 0;
                  }
              }
          } else {
              if (features[1] < 11.5) {
                  if (features[4] < 112.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[4] < 106.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_4(const float *features, int32_t features_length) {
          if (features[4] < 63.5) {
              if (features[5] < 111166.0) {
                  if (features[2] < 443.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[2] < 32798.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[1] < 11.5) {
                  if (features[2] < 291.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[3] < 1.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_5(const float *features, int32_t features_length) {
          if (features[3] < 1.5) {
              if (features[4] < 63.5) {
                  if (features[5] < 111166.0) {
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
              if (features[4] < 114.0) {
                  if (features[2] < 291.0) {
                      return 1;
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
          }
        }
        

static inline int32_t rf_predict_tree_6(const float *features, int32_t features_length) {
          if (features[2] < 451.5) {
              if (features[4] < 60.0) {
                  if (features[1] < 11.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[4] < 114.0) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[4] < 64.0) {
                  if (features[1] < 11.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[3] < 1.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_7(const float *features, int32_t features_length) {
          if (features[3] < 1.5) {
              if (features[2] < 33356.0) {
                  if (features[5] < 111166.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[4] < 60.0) {
                      return 0;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[5] < 116166.0) {
                  if (features[0] < 500.0) {
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
          if (features[2] < 451.5) {
              if (features[2] < 51.0) {
                  if (features[2] < 24.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[3] < 1.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[5] < 111166.0) {
                  if (features[2] < 33356.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[2] < 32785.5) {
                      return 0;
                  } else {
                      return 1;
                  }
              }
          }
        }
        

static inline int32_t rf_predict_tree_9(const float *features, int32_t features_length) {
          if (features[4] < 63.5) {
              if (features[5] < 111166.0) {
                  if (features[5] < 111116.5) {
                      return 0;
                  } else {
                      return 0;
                  }
              } else {
                  if (features[5] < 121111.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              }
          } else {
              if (features[0] < 6500.0) {
                  if (features[1] < 11.5) {
                      return 1;
                  } else {
                      return 1;
                  }
              } else {
                  if (features[0] < 9500.0) {
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
    