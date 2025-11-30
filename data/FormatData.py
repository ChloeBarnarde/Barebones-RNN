import pandas as pd
import numpy as np

np.random.seed(42)

wine_red = pd.read_csv('data/winequality-red.csv', sep=';')
wine_red['type'] = 0  # Red wine labeled as 0
wine_white = pd.read_csv('data/winequality-white.csv', sep=';')
wine_white['type'] = 1  # White wine labeled as 1

# shuffle for splitting
idx_red = wine_red.index.to_list()
np.random.shuffle(idx_red)
wine_red = wine_red.loc[idx_red].reset_index(drop=True)

idx_white = wine_white.index.to_list()
np.random.shuffle(idx_white)
wine_white = wine_white.loc[idx_white].reset_index(drop=True)

# split into train and test sets
train_portion = 0.8
test_portion = 1 - train_portion

train_red = wine_red.iloc[:int(len(wine_red) * train_portion)]
test_red = wine_red.iloc[int(len(wine_red) * train_portion):]

train_white = wine_white.iloc[:int(len(wine_white) * train_portion)]
test_white = wine_white.iloc[int(len(wine_white) * train_portion):]

train_data = pd.concat([train_red, train_white]).reset_index(drop=True)
test_data = pd.concat([test_red, test_white]).reset_index(drop=True)

# shuffle train and test data
idx_train = train_data.index.to_list()
np.random.shuffle(idx_train)
train_data = train_data.loc[idx_train].reset_index(drop=True)
idx_test = test_data.index.to_list()
np.random.shuffle(idx_test)
test_data = test_data.loc[idx_test].reset_index(drop=True)
train_data.to_csv('data/train_data.csv', index=False)
test_data.to_csv('data/test_data.csv', index=False)