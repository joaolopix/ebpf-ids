
static inline int predict_C(__u64 duration,__u16 protocol,__u64 packet_counter, __u64 transmited_bytes, int current_flags) {
	int features[5] = {duration*10,protocol*10,packet_counter*10,transmited_bytes*10,current_flags*10};
	int votes[2] = {0,0};
          if (features[3] < 630) {
              if (features[0] < 5000) {
                  if (features[1] < 115) {
                      votes[0] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  votes[1] += 1;
              }
          } else {
              if (features[3] < 1125) {
                  if (features[4] < 1161660) {
                      votes[1] += 1;
                  } else {
                      votes[1] += 1;
                  }
              } else {
                  if (features[2] < 15) {
                      votes[1] += 1;
                  } else {
                      votes[1] += 1;
                  }
              }
          }
        
          if (features[3] < 630) {
              if (features[0] < 5000) {
                  if (features[3] < 515) {
                      votes[0] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  votes[1] += 1;
              }
          } else {
              if (features[4] < 1161660) {
                  if (features[3] < 1125) {
                      votes[1] += 1;
                  } else {
                      votes[1] += 1;
                  }
              } else {
                  votes[1] += 1;
              }
          }
        
          if (features[4] < 1111660) {
              if (features[2] < 15) {
                  if (features[3] < 630) {
                      votes[0] += 1;
                  } else {
                      votes[1] += 1;
                  }
              } else {
                  votes[1] += 1;
              }
          } else {
              if (features[2] < 15) {
                  if (features[4] < 1211115) {
                      votes[1] += 1;
                  } else {
                      votes[1] += 1;
                  }
              } else {
                  if (features[2] < 25) {
                      votes[1] += 1;
                  } else {
                      votes[1] += 1;
                  }
              }
          }
        
          if (features[3] < 635) {
              if (features[3] < 515) {
                  votes[0] += 1;
              } else {
                  if (features[4] < 1111660) {
                      votes[0] += 1;
                  } else {
                      votes[0] += 1;
                  }
              }
          } else {
              if (features[4] < 1161660) {
                  if (features[1] < 115) {
                      votes[1] += 1;
                  } else {
                      votes[1] += 1;
                  }
              } else {
                  votes[1] += 1;
              }
          }
        
          if (features[3] < 630) {
              if (features[1] < 115) {
                  if (features[4] < 1111660) {
                      votes[0] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  if (features[3] < 555) {
                      votes[0] += 1;
                  } else {
                      votes[0] += 1;
                  }
              }
          } else {
              if (features[4] < 1161660) {
                  if (features[4] < 1112160) {
                      votes[1] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  votes[1] += 1;
              }
          }
        
          if (features[4] < 1111660) {
              if (features[3] < 665) {
                  if (features[1] < 115) {
                      votes[0] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  if (features[3] < 1065) {
                      votes[1] += 1;
                  } else {
                      votes[1] += 1;
                  }
              }
          } else {
              if (features[4] < 1211115) {
                  if (features[0] < 15000) {
                      votes[1] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  votes[1] += 1;
              }
          }
        
          if (features[3] < 635) {
              if (features[0] < 5000) {
                  if (features[3] < 515) {
                      votes[0] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  votes[1] += 1;
              }
          } else {
              if (features[1] < 115) {
                  if (features[0] < 5000) {
                      votes[1] += 1;
                  } else {
                      votes[1] += 1;
                  }
              } else {
                  if (features[2] < 15) {
                      votes[1] += 1;
                  } else {
                      votes[1] += 1;
                  }
              }
          }
        
          if (features[0] < 5000) {
              if (features[2] < 15) {
                  if (features[1] < 115) {
                      votes[0] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  if (features[3] < 1160) {
                      votes[1] += 1;
                  } else {
                      votes[1] += 1;
                  }
              }
          } else {
              if (features[3] < 1140) {
                  if (features[3] < 1110) {
                      votes[1] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  votes[1] += 1;
              }
          }
        
          if (features[2] < 15) {
              if (features[1] < 115) {
                  if (features[4] < 1111660) {
                      votes[0] += 1;
                  } else {
                      votes[1] += 1;
                  }
              } else {
                  if (features[0] < 10000) {
                      votes[0] += 1;
                  } else {
                      votes[1] += 1;
                  }
              }
          } else {
              if (features[3] < 1140) {
                  if (features[4] < 1161660) {
                      votes[0] += 1;
                  } else {
                      votes[1] += 1;
                  }
              } else {
                  votes[1] += 1;
              }
          }
        
          if (features[4] < 1111660) {
              if (features[2] < 15) {
                  if (features[0] < 10000) {
                      votes[0] += 1;
                  } else {
                      votes[1] += 1;
                  }
              } else {
                  votes[1] += 1;
              }
          } else {
              if (features[3] < 545) {
                  if (features[4] < 1211115) {
                      votes[0] += 1;
                  } else {
                      votes[1] += 1;
                  }
              } else {
                  if (features[0] < 5000) {
                      votes[1] += 1;
                  } else {
                      votes[1] += 1;
                  }
              }
          }
        
          if (features[2] < 15) {
              if (features[3] < 635) {
                  if (features[4] < 1111660) {
                      votes[0] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  if (features[1] < 115) {
                      votes[1] += 1;
                  } else {
                      votes[1] += 1;
                  }
              }
          } else {
              if (features[3] < 1140) {
                  if (features[3] < 1110) {
                      votes[1] += 1;
                  } else {
                      votes[0] += 1;
                  }
              } else {
                  votes[1] += 1;
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
