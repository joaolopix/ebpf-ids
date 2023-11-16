import pandas as pd
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import confusion_matrix
from sklearn.utils import resample
import emlearn
import matplotlib.pyplot as plt
import seaborn as sn
import numpy as np
import rf_gen_PY
import rf_gen_C

def convert_bytes(size):
    if 'M' in size:
        return float(size.replace('M', '')) * 1024 * 1024  # Convert from megabytes to bytes
    return size

def convert_falgs(flags):
    flags_ = 0
    for flag in flags:
        if flag != '.':
            flags_ = flags_ + 2
            flags_ = flags_ * 10
        if flag == '.':
            flags_ = flags_ + 1
            flags_ = flags_ * 10
    flags_ = flags_/10
    return int(flags_)

def convert_proto(proto):
    if 'UDP' in proto:
        return 17
    elif 'TCP' in proto:
        return 6
    elif 'IGMP' in proto:
        return 2
    elif 'ICMP' in proto:
        return 1

def dataset_processing():
    # Read the CSV file
    data = pd.read_csv("week2.csv", delimiter=",",low_memory=False)

    # Drop rows where 'label' column has the value 'victim'
    data = data[data['label'] != 'victim']

    # Replace all rows in the 'Dst Pt' column that have float values with NaN and Drop rows where 'Dst Pt' is NaN
    data['Dst Pt'] = pd.to_numeric(data['Dst Pt'], errors='coerce')
    data = data.dropna(subset=['Dst Pt'])

    class_counts = data['label'].value_counts()
    minority_class = class_counts.idxmin()
    majority_class = class_counts.idxmax()
    majority_data = data[data['label'] == majority_class]
    minority_data = data[data['label'] == minority_class]
    # Undersample the majority class
    undersampled_majority = resample(majority_data, replace=False, n_samples=len(minority_data), random_state=42)
    # Concatenate the undersampled majority class with the minority class
    undersampled_df = pd.concat([undersampled_majority, minority_data])
    # Shuffle the dataframe to ensure randomness
    undersampled_df = undersampled_df.sample(frac=1, random_state=42).reset_index(drop=True)
    data = undersampled_df

    # Get Target data
    Y = data['label']

    # Get X data
    X = data.drop(['label', 'Date first seen', 'Src IP Addr', 'Dst IP Addr','Src Pt', 'attackType','attackID',
                   'attackDescription','Flows','Tos'], axis=1)

    X['Bytes'] = X['Bytes'].apply(convert_bytes).astype(int)
    X['Flags'] = X['Flags'].apply(convert_falgs).astype(int)
    X['Proto'] = X['Proto'].apply(convert_proto).astype(int)
    X['Dst Pt'] = X['Dst Pt'].astype(int)
    X['Duration'] = X['Duration'] * 1e6
    X['Duration'] = X['Duration'].astype(int)
    X['Packets'] = X['Packets'].astype(int)

    return X,Y

def train_model(X,Y):

    X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.2, random_state=10)

    rf_Model = RandomForestClassifier()
    rf_Model.set_params(n_estimators=10,max_depth=3,max_leaf_nodes=1000)
    rf_Model.fit(X_train, Y_train)
    print(f'Train Accuracy - : {rf_Model.score(X_train, Y_train):.3f}')
    print(f'Test Accuracy - : {rf_Model.score(X_test, Y_test):.3f}')

    Y_predicted = rf_Model.predict(X_test)
    cm = confusion_matrix(Y_test,Y_predicted,labels=rf_Model.classes_)
    cm_norm = cm/cm.sum(axis=1)[:,np.newaxis]
    class_labels = rf_Model.classes_

    plt.figure(figsize=(10, 7))
    sn.heatmap(cm_norm, annot=True, fmt='.2f', xticklabels=class_labels, yticklabels=class_labels)
    plt.xlabel('Predicted')
    plt.ylabel('Actual')
    plt.show()

    return rf_Model

def model_to_C(rf_Model,file_name):

    cmodel = emlearn.convert(rf_Model)
    cmodel.save(file=file_name, name='rf')

def main():

    n_trees = 10
    max_depth = 3
    emlearn_file = 'rf.h'
    X,Y = dataset_processing()
    rf_Model = train_model(X,Y)
    model_to_C(rf_Model,emlearn_file)
    rf_gen_C.generate_C_code(emlearn_file,'rf_mode.h',n_trees)
    rf_gen_PY.generate_PY_code(emlearn_file,'rf_mode.py',max_depth)

if __name__ == "__main__":
     main()