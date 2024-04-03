

static inline int predict_C(__u64 duration,__u16 protocol,__u64 packet_counter, __u64 transmited_bytes, int current_flags) {
	int features[5] = {duration*10,protocol*10,packet_counter*10,transmited_bytes*10,current_flags*10};
	int votes[5] = {0};

          if (features[4] < 1161660) {
              if (features[4] < 1111165) {
                  if (features[4] < 1111115) {
                      votes[1] += 1;
                  } else {
                      votes[2] += 1;
                  }
              } else {
                  if (features[4] < 1111660) {
                      votes[3] += 1;
                  } else {
                      votes[3] += 1;
                  }
              }
          } else {
              if (features[4] < 1211115) {
                  if (features[2] < 15) {
                      votes[4] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  votes[0] += 1;
              }
          }
        
          if (features[4] < 1161660) {
              if (features[3] < 560) {
                  if (features[3] < 515) {
                      votes[1] += 1;
                  } else {
                      votes[2] += 1;
                  }
              } else {
                  if (features[3] < 1125) {
                      votes[3] += 1;
                  } else {
                      votes[0] += 1;
                  }
              }
          } else {
              if (features[4] < 1211115) {
                  if (features[0] < 5000) {
                      votes[4] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  votes[0] += 1;
              }
          }
        
          if (features[1] < 115) {
              if (features[2] < 15) {
                  if (features[0] < 5000) {
                      votes[4] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  if (features[4] < 1161660) {
                      votes[3] += 1;
                  } else {
                      votes[0] += 1;
                  }
              }
          } else {
              if (features[2] < 15) {
                  if (features[3] < 675) {
                      votes[1] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  votes[0] += 1;
              }
          }
        
          if (features[4] < 1161660) {
              if (features[4] < 1111165) {
                  if (features[1] < 115) {
                      votes[2] += 1;
                  } else {
                      votes[1] += 1;
                  }
              } else {
                  if (features[3] < 560) {
                      votes[0] += 1;
                  } else {
                      votes[3] += 1;
                  }
              }
          } else {
              if (features[2] < 15) {
                  if (features[0] < 5000) {
                      votes[4] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  votes[0] += 1;
              }
          }
        
          if (features[4] < 1161660) {
              if (features[2] < 15) {
                  if (features[4] < 1111165) {
                      votes[2] += 1;
                  } else {
                      votes[3] += 1;
                  }
              } else {
                  if (features[1] < 115) {
                      votes[3] += 1;
                  } else {
                      votes[0] += 1;
                  }
              }
          } else {
              if (features[3] < 545) {
                  if (features[4] < 1211115) {
                      votes[4] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  votes[0] += 1;
              }
          }
        
          if (features[4] < 1161660) {
              if (features[3] < 560) {
                  if (features[3] < 515) {
                      votes[1] += 1;
                  } else {
                      votes[2] += 1;
                  }
              } else {
                  if (features[1] < 115) {
                      votes[3] += 1;
                  } else {
                      votes[0] += 1;
                  }
              }
          } else {
              if (features[3] < 545) {
                  if (features[4] < 1211115) {
                      votes[4] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  votes[0] += 1;
              }
          }
        
          if (features[1] < 115) {
              if (features[4] < 1161660) {
                  if (features[3] < 560) {
                      votes[2] += 1;
                  } else {
                      votes[3] += 1;
                  }
              } else {
                  if (features[2] < 15) {
                      votes[4] += 1;
                  } else {
                      votes[0] += 1;
                  }
              }
          } else {
              if (features[3] < 675) {
                  if (features[3] < 640) {
                      votes[1] += 1;
                  } else {
                      votes[1] += 1;
                  }
              } else {
                  if (features[2] < 15) {
                      votes[0] += 1;
                  } else {
                      votes[0] += 1;
                  }
              }
          }
        
          if (features[4] < 1161660) {
              if (features[2] < 15) {
                  if (features[4] < 1111165) {
                      votes[2] += 1;
                  } else {
                      votes[3] += 1;
                  }
              } else {
                  if (features[3] < 1200) {
                      votes[3] += 1;
                  } else {
                      votes[0] += 1;
                  }
              }
          } else {
              if (features[3] < 545) {
                  if (features[0] < 10000) {
                      votes[4] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  votes[0] += 1;
              }
          }
        
          if (features[4] < 1161660) {
              if (features[3] < 560) {
                  if (features[4] < 1111115) {
                      votes[1] += 1;
                  } else {
                      votes[2] += 1;
                  }
              } else {
                  if (features[2] < 15) {
                      votes[3] += 1;
                  } else {
                      votes[0] += 1;
                  }
              }
          } else {
              if (features[4] < 1211115) {
                  if (features[3] < 545) {
                      votes[4] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  votes[0] += 1;
              }
          }
        
          if (features[3] < 545) {
              if (features[0] < 5000) {
                  if (features[3] < 515) {
                      votes[1] += 1;
                  } else {
                      votes[4] += 1;
                  }
              } else {
                  votes[0] += 1;
              }
          } else {
              if (features[3] < 600) {
                  if (features[4] < 1161160) {
                      votes[3] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  if (features[0] < 115000) {
                      votes[0] += 1;
                  } else {
                      votes[0] += 1;
                  }
              }
          }
        
          if (features[4] < 1161660) {
              if (features[3] < 560) {
                  if (features[3] < 515) {
                      votes[1] += 1;
                  } else {
                      votes[2] += 1;
                  }
              } else {
                  if (features[4] < 1111160) {
                      votes[0] += 1;
                  } else {
                      votes[3] += 1;
                  }
              }
          } else {
              if (features[0] < 5000) {
                  if (features[2] < 15) {
                      votes[4] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  votes[0] += 1;
              }
          }
        
	int most_voted_class = -1;
	int most_voted_votes = 0;
	for (int i=0; i<sizeof(votes)/sizeof(int); i++) {
		if (votes[i] > most_voted_votes) {
			most_voted_class = i;
			most_voted_votes = votes[i];
		}
	}
return most_voted_class;
}
