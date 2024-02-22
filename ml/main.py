import pandas as pd
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score, precision_score, recall_score, f1_score, confusion_matrix, precision_recall_curve, auc
from sklearn.preprocessing import LabelEncoder
from sklearn.utils import resample
import emlearn
import matplotlib.pyplot as plt
import seaborn as sn
import numpy as np
import rf_gen_PY
import rf_gen_C

def convert_bytes(size):
    if 'M' in size:
        return int(float(size.replace('M', '')) * 10**6)  # Convert from megabytes to bytes
    return int(size)

def convert_flags(flags):
    flags_ = 0
    for flag in flags:
        if flag != '.':
            flags_ = flags_ + 2
            flags_ = flags_ * 10
        if flag == '.':
            flags_ = flags_ + 1
            flags_ = flags_ * 10
    flags_ = flags_ / 10
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

def convert_attack_desc_to_proto(desc):
    if '-sU' in desc:
        return 17
    elif 'sF' in desc or 'sS' in desc or 'sA' in desc:
        return 6
    elif 'sP' in desc:
        return 1
    elif "---" in desc:
        return 0

def convert_attack_desc(desc):
    if 'sU' in desc:
        return 1
    elif 'sF' in desc:
        return 2
    elif 'sS' in desc:
        return 3
    elif 'sA' in desc:
        return 4
    elif 'sP' in desc:
        return 5
    elif "---" in desc:
        return 0

def convert_attack_desc_number_to_proto(desc):
    if 1 == desc:
        return 17
    elif 2 == desc or 3 == desc or 4 == desc:
        return 6
    elif 5 == desc:
        return 1
    elif 0 == desc:
        return 0

def dataset_statistics(file_name,file2 = None):
    if file2 == None:
        # Read the CSV file
        data = pd.read_csv(file_name, delimiter=",", low_memory=False)
    else:
        df1 = pd.read_csv(file_name, delimiter=",", low_memory=False)
        df2 = pd.read_csv(file2, delimiter=",", low_memory=False)
        data = pd.concat([df1, df2], ignore_index=True)

    data = data[data['label'] != 'victim']
    data['Proto'] = data['Proto'].apply(convert_proto).astype(int)
    label_counts = data['label'].value_counts()
    print(f"Default Dataset\nnormal: {label_counts['normal']}\nattacker: {label_counts['attacker']}\n",)

    proto_counts = data[(data['label'] == 'attacker')]
    proto_counts = proto_counts['Proto'].value_counts()
    print(f"Protocols used by attacker\nTCP: {proto_counts[6]}\nUDP: {proto_counts[17]}\nICMP: {proto_counts[1]}\n")

    proto_counts = data[(data['label'] == 'normal')]
    proto_counts = proto_counts['Proto'].value_counts()
    print(f"Protocols used by normal activity\nTCP: {proto_counts[6]}\nUDP: {proto_counts[17]}\nICMP: {proto_counts[1]}\nIGMP: {proto_counts[2]}\n")

    attack_counts = data['attackDescription'].apply(convert_attack_desc).astype(int)
    attack_counts = attack_counts.value_counts()
    print(f"Attacks\nsU: {attack_counts[1]}\nsF: {attack_counts[2]}\nsS: {attack_counts[3]}\nsA: {attack_counts[4]}\nsP: {attack_counts[5]}\n")

    attack_convert = data['attackDescription'].apply(convert_attack_desc_to_proto).astype(int)
    attack_convert = attack_convert.value_counts()
    print(f"Attacks by Protocol\nTCP: {attack_convert[6]}\nUDP: {attack_convert[17]}\nICMP: {attack_convert[1]}\n")

    data['attackDescription'] = data['attackDescription'].apply(convert_attack_desc).astype(int)
    miss_classified_icmp = data[(data['Proto'] == 1) & (data['attackDescription'] != 5) & (data['label'] == 'attacker')]
    miss_classified_tcp = data[(data['Proto'] == 6) & (data['attackDescription'] != 2) & (data['attackDescription'] != 3) & (data['attackDescription'] != 4) & (data['label'] == 'attacker')]
    miss_classified_udp = data[(data['Proto'] == 17) & (data['attackDescription'] != 1) & (data['label'] == 'attacker')]
    print(f"Miss classified ICMP {len(miss_classified_icmp)}")
    print(f"Miss classified TCP {len(miss_classified_tcp)}")
    print(f"Miss classified UDP {len(miss_classified_udp)}\n")

    print(f"Attacks with the ICMP proto: {miss_classified_icmp['attackDescription'].value_counts()} \n"
          f"Attacks with the TCP proto {miss_classified_tcp['attackDescription'].value_counts()} \n"
          f"Attacks with the UDP proto {miss_classified_udp['attackDescription'].value_counts()} \n")

    test = data[(data['label'] == 'attacker')]
    print()
    print(data['Tos'].value_counts())
    print("ToS by Attacker")
    print(test['Tos'].value_counts())

    print()
    print(data['Flows'].value_counts())

    data = data[(data['Proto'] != 2) & (data['Proto'] != 1)]
    data = data[~((data['attackDescription'] == 5))]
    data = data[~((data['Proto'] == 6) & (data['attackDescription'] != 2) & (data['attackDescription'] != 3) & (data['attackDescription'] != 4) & (data['label'] == 'attacker'))]
    data = data[~(((data['Proto'] == 17) & (data['attackDescription'] != 1) & (data['label'] == 'attacker')))]

    udp = data[(data['attackDescription'] == 1)]
    fin = data[(data['attackDescription'] == 2)]
    syn = data[(data['attackDescription'] == 3)]
    ack = data[(data['attackDescription'] == 4)]
    normal = data[(data['attackDescription'] == 0)]
    print(udp['Flags'].value_counts())
    print(fin['Flags'].value_counts())
    print(syn['Flags'].value_counts())
    print(ack['Flags'].value_counts())
    print(normal['Flags'].value_counts())

    print()
    print("Number of packets in attacks")
    npackets = data[(data['label'] == 'attacker')]
    print(npackets['Packets'].value_counts())

    sample_syn = data[(data['label'] == 'attacker') & (data['Packets'] == 2)]
    print(sample_syn['attackDescription'].value_counts())

def dataset_processing(multiclass=False):

    # Read the CSV file
    df1 = pd.read_csv("week1.csv", delimiter=",",low_memory=False)
    df2 = pd.read_csv("week2.csv", delimiter=",", low_memory=False)

    data = pd.concat([df1, df2], ignore_index=True)

    # Drop rows where 'label' column has the value 'victim'
    data = data[data['label'] != 'victim']

    # Remove labels with IGMP AND ICMP Protocol
    data['Proto'] = data['Proto'].apply(convert_proto).astype(int)
    data = data[(data['Proto'] != 2) & (data['Proto'] != 1)]

    # Remove Ping scan
    data['attackDescription'] = data['attackDescription'].apply(convert_attack_desc).astype(int)
    data = data[~((data['attackDescription'] == 5))]

    # Remove missidentidied UDP and TCP
    data = data[~((data['Proto'] == 6) & (data['attackDescription'] != 2) & (data['attackDescription'] != 3) & (data['attackDescription'] != 4) & (data['label'] == 'attacker'))]
    data = data[~(((data['Proto'] == 17) & (data['attackDescription'] != 1) & (data['label'] == 'attacker')))]

    if multiclass:
        print(data['attackDescription'].value_counts())
        # Oversample
        sU = data[data['attackDescription'] == 1]
        sS = data[data['attackDescription']==3]
        sA = data[data['attackDescription'] == 4]
        osU = sU[:int(len(sU)/4)]
        osA = pd.concat([sA, sA[:int(len(sA)/2)]])
        data = pd.concat([data, osU, sS,osA])
        print(data['attackDescription'].value_counts())
        # Undersample
        majority_data = data[data['attackDescription'] == 0]
        minority_data = data[data['attackDescription'] != 0]
        class_counts = data['attackDescription'].value_counts()
        minority_class = class_counts.idxmin()
        minority_sub_data = data[data['attackDescription'] == minority_class]
        undersampled_majority = resample(majority_data, replace=False, n_samples=len(minority_sub_data), random_state=42)
        undersampled_df = pd.concat([undersampled_majority, minority_data])
        # Shuffle the dataframe to ensure randomness
        undersampled_df = undersampled_df.sample(frac=1, random_state=42).reset_index(drop=True)
        data = undersampled_df
        print(data['attackDescription'].value_counts())

        # Get Target data
        Z = data['attackDescription']

        Z.to_csv('Z_data.csv', index=False)

        # Get K data
        K = data.drop(
            ['label', 'Date first seen', 'Src IP Addr', 'Dst IP Addr', 'Dst Pt', 'Src Pt', 'attackType', 'attackID',
             'attackDescription', 'Flows', 'Tos'], axis=1)

        K['Bytes'] = K['Bytes'].apply(convert_bytes).astype('Int64')
        K['Flags'] = K['Flags'].apply(convert_flags).astype('Int64')
        # X['Dst Pt'] = X['Dst Pt'].astype('Int64')
        # X['Src Pt'] = X['Src Pt'].astype('Int64')
        K['Duration'] = K['Duration'] * 1e6
        K['Duration'] = K['Duration'].round().astype('Int64')
        K['Packets'] = K['Packets'].astype('Int64')

        print(K.head())

        print("\nData processing completed\n")

        K.to_csv('K_data.csv', index=False)

    else:
        # Undersample
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

        Y.to_csv('Y_data.csv', index=False)

        # Get X data
        X = data.drop(['label', 'Date first seen', 'Src IP Addr', 'Dst IP Addr','Dst Pt','Src Pt','attackType','attackID','attackDescription','Flows','Tos'], axis=1)

        X['Bytes'] = X['Bytes'].apply(convert_bytes).astype('Int64')
        X['Flags'] = X['Flags'].apply(convert_flags).astype('Int64')
        #X['Dst Pt'] = X['Dst Pt'].astype('Int64')
        #X['Src Pt'] = X['Src Pt'].astype('Int64')
        X['Duration'] = X['Duration'] * 1e6
        X['Duration'] = X['Duration'].round().astype('Int64')
        X['Packets'] = X['Packets'].astype('Int64')

        print(X.head())

        print("\nData processing completed\n")

        X.to_csv('X_data.csv', index=False)

def train_model(X,Y,n_trees,max_depth,rs,multiclass=False):

    X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.2)

    rf_Model = RandomForestClassifier()
    if multiclass:
        class_weights = {0: 1, 1: 1, 2: 2,3: 2,4: 3}
        rf_Model.set_params(class_weight=class_weights,n_estimators=n_trees,max_depth=max_depth,max_leaf_nodes=1000,oob_score=True,random_state=rs) # 1122 binary ; 123456789 multi
    else:
        rf_Model.set_params(n_estimators=n_trees, max_depth=max_depth, max_leaf_nodes=1000,oob_score=True, random_state=rs)  # 1122 binary ; 123456789 multi

    rf_Model.fit(X_train, Y_train)

    Y_predicted = rf_Model.predict(X_test)

    # Evaluate performance metrics
    if multiclass:
        accuracy= accuracy_score(Y_test, Y_predicted)
        precision = precision_score(Y_test, Y_predicted, average='macro')
        recall = recall_score(Y_test, Y_predicted, average='macro')
        f1 = f1_score(Y_test, Y_predicted, average='macro')
        oob_score = rf_Model.oob_score_
    else:
        label_encoder = LabelEncoder()
        Y_test_binary = label_encoder.fit_transform(Y_test)
        Y_predicted_binary = label_encoder.transform(Y_predicted)
        accuracy = accuracy_score(Y_test_binary,  Y_predicted_binary)
        precision = precision_score(Y_test_binary,  Y_predicted_binary)
        recall = recall_score(Y_test_binary,  Y_predicted_binary)
        f1 = f1_score(Y_test_binary,  Y_predicted_binary)
        precision_curve, recall_curve, _ = precision_recall_curve(Y_test_binary, Y_predicted_binary)
        area_under_curve = auc(recall_curve, precision_curve)
        oob_score = rf_Model.oob_score_

    # Displaying the results
    print(f"Accuracy: {accuracy:.3f}")
    print(f"Precision: {precision:.3f}")
    print(f"Recall: {recall:.3f}")
    print(f"F1 Score: {f1:.3f}")
    #print(f"Area Under Precision-Recall Curve: {area_under_curve:.3f}")
    print(f"OOB Score: {oob_score:.3f}")

    cm = confusion_matrix(Y_test,Y_predicted,labels=rf_Model.classes_)
    cm_norm = cm/cm.sum(axis=1)[:,np.newaxis]
    if multiclass:
        class_labels = ["normal","sU","sF","sS","sA"]
    else:
        class_labels = rf_Model.classes_
    plt.figure(figsize=(10, 7))
    sn.heatmap(cm_norm, annot=True, fmt='.2f', xticklabels=class_labels, yticklabels=class_labels)
    plt.xlabel('Predicted')
    plt.ylabel('Actual')
    plt.show()

    # Plotting
    feature_importances = rf_Model.feature_importances_
    sorted_indices = np.argsort(feature_importances)[::-1]
    X_labels = ["Duration","Proto","Packets","Bytes","Flags"]
    X_sorted = [X_labels[i] for i in sorted_indices]
    plt.figure(figsize=(8, 6))
    plt.bar(range(X.shape[1]), feature_importances[sorted_indices], align="center")
    plt.xticks(range(X.shape[1]), X_sorted, rotation=45)
    plt.xlabel("Feature Index")
    plt.ylabel("Feature Importance")
    plt.title("Random Forest Feature Importance")
    plt.show()

    return rf_Model

def model_to_C(rf_Model,file_name):

    cmodel = emlearn.convert(rf_Model)
    cmodel.save(file=file_name, name='rf')

def evaluate_ntrees(X, Y, min_trees, max_trees, step_size,multiclass = False):
    X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.2, random_state=10)

    n_trees_range = range(min_trees, max_trees + 1, step_size)
    accuracy_scores, precision_scores, recall_scores, f1_scores, oob_scores = [], [], [], [], []

    for n_trees in n_trees_range:
        rf_Model = RandomForestClassifier(n_estimators=n_trees, max_leaf_nodes=1000, oob_score=True)
        rf_Model.fit(X_train, Y_train)

        Y_predicted = rf_Model.predict(X_test)

        # Evaluate performance metrics
        if multiclass:
            accuracy_scores.append(accuracy_score(Y_test, Y_predicted))
            precision_scores.append(precision_score(Y_test, Y_predicted, average='macro'))
            recall_scores.append(recall_score(Y_test, Y_predicted, average='macro'))
            f1_scores.append(f1_score(Y_test, Y_predicted, average='macro'))
            oob_scores.append(rf_Model.oob_score_)
        else:
            label_encoder = LabelEncoder()
            Y_test_binary = label_encoder.fit_transform(Y_test)
            Y_predicted_binary = label_encoder.transform(Y_predicted)
            accuracy_scores.append(accuracy_score(Y_test_binary, Y_predicted_binary))
            precision_scores.append(precision_score(Y_test_binary, Y_predicted_binary))
            recall_scores.append(recall_score(Y_test_binary, Y_predicted_binary))
            f1_scores.append(f1_score(Y_test_binary, Y_predicted_binary))
            oob_scores.append(rf_Model.oob_score_)


    # Plot the results
    plt.figure(figsize=(10, 6))

    # You can plot different metrics on the same graph or create multiple subplots for each metric
    plt.plot(n_trees_range, accuracy_scores, label='Accuracy')
    plt.plot(n_trees_range, precision_scores, label='Precision')
    plt.plot(n_trees_range, recall_scores, label='Recall')
    plt.plot(n_trees_range, f1_scores, label='F1 Score')
    plt.plot(n_trees_range, oob_scores, label='OOB Score')

    plt.xticks([0,5,11,20,30,40,50,60,70,80,90,100])
    plt.xlabel('Number of Trees')
    plt.ylabel('Score')
    plt.title('Random Forest Performance vs. Number of Trees')
    plt.legend()
    plt.show()

def evaluate_depth(X, Y, min_d, max_d, step_size,n_trees,multiclass = False):
    X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=0.2, random_state=10)

    d_range = range(min_d, max_d + 1, step_size)
    accuracy_scores, precision_scores, recall_scores, f1_scores, oob_scores = [], [], [], [], []

    for d in d_range:
        rf_Model = RandomForestClassifier(n_estimators=n_trees,max_depth=d, max_leaf_nodes=1000, oob_score=True)
        rf_Model.fit(X_train, Y_train)

        Y_predicted = rf_Model.predict(X_test)

        # Evaluate performance metrics
        if multiclass:
            accuracy_scores.append(accuracy_score(Y_test, Y_predicted))
            precision_scores.append(precision_score(Y_test, Y_predicted, average='macro'))
            recall_scores.append(recall_score(Y_test, Y_predicted, average='macro'))
            f1_scores.append(f1_score(Y_test, Y_predicted, average='macro'))
            oob_scores.append(rf_Model.oob_score_)
        else:
            label_encoder = LabelEncoder()
            Y_test_binary = label_encoder.fit_transform(Y_test)
            Y_predicted_binary = label_encoder.transform(Y_predicted)

            accuracy_scores.append(accuracy_score(Y_test_binary, Y_predicted_binary))
            precision_scores.append(precision_score(Y_test_binary, Y_predicted_binary))
            recall_scores.append(recall_score(Y_test_binary, Y_predicted_binary))
            f1_scores.append(f1_score(Y_test_binary, Y_predicted_binary))
            oob_scores.append(rf_Model.oob_score_)

    # Plot the results
    plt.figure(figsize=(10, 6))

    # You can plot different metrics on the same graph or create multiple subplots for each metric
    plt.plot(d_range, accuracy_scores, label='Accuracy')
    plt.plot(d_range, precision_scores, label='Precision')
    plt.plot(d_range, recall_scores, label='Recall')
    plt.plot(d_range, f1_scores, label='F1 Score')
    plt.plot(d_range, oob_scores, label='OOB Score')

    plt.xticks([0, 1, 3, 10, 20, 30, 40, 50])
    plt.xlabel('Tree Depth')
    plt.ylabel('Score')
    plt.title('Random Forest Performance vs. Tree Depth')
    plt.legend()
    plt.show()

def evaluate_balance(file_path):

    with open(file_path, 'r') as src:
        lines = src.readlines()

    votes = [0,0,0,0,0]

    for line in lines:
        if "return 0;" in line:
            votes[0] += 1
        if "return 1;" in line:
            votes[1] += 1
        if "return 2;" in line:
            votes[2] += 1
        if "return 3;" in line:
            votes[3] += 1
        if "return 4;" in line:
            votes[4] += 1
    print(votes)

def main_b():

    #dataset_processing(multiclass=False)

    X = pd.read_csv("X_data.csv", delimiter=",")
    Y = pd.read_csv("Y_data.csv").values.ravel()  # label

    #evaluate_ntrees(X, Y, min_trees=5, max_trees=100, step_size=5,multiclass=True)
    #evaluate_depth(X, Y, min_d=1, max_d=50, step_size=5,n_trees=11,multiclass=True)

    n_trees = 11
    max_depth = 3
    rf_Model = train_model(X, Y, n_trees, max_depth, 1122,multiclass=False)

    model_to_C(rf_Model,'binary_model/rf_b.h')
    rf_gen_C.generate_C_code('binary_model/rf_b.h', 'binary_model/rf_model_b.h', n_trees)
    rf_gen_PY.generate_PY_code('binary_model/rf_b.h', 'binary_model/rf_model_b.py', max_depth)

    evaluate_balance('binary_model/rf_b.h')

def main_m():

    #dataset_processing(multiclass=True)

    K = pd.read_csv("K_data.csv", delimiter=",")
    Z = pd.read_csv("Z_data.csv").values.ravel()  # attack_d

    #evaluate_ntrees(K, Z, min_trees=5, max_trees=100, step_size=5,multiclass=True)
    #evaluate_depth(K, Z, min_d=1, max_d=50, step_size=5,n_trees=11,multiclass=True)

    n_trees = 11
    max_depth = 3
    rf_Model = train_model(K, Z, n_trees, max_depth, 123456789,multiclass=True)

    model_to_C(rf_Model, 'multiclass_model/rf_m.h')
    rf_gen_C.generate_C_code('multiclass_model/rf_m.h', 'multiclass_model/rf_model_m.h', n_trees)
    rf_gen_PY.generate_PY_code('multiclass_model/rf_m.h', 'multiclass_model/rf_model_m.py', max_depth)

    evaluate_balance('multiclass_model/rf_m.h')

def main():

    #dataset_statistics("week1.csv")
    #dataset_statistics("week2.csv")
    #dataset_statistics("week1.csv","week2.csv")

    #main_b()
    main_m()


if __name__ == "__main__":
     main()