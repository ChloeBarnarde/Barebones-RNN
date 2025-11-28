import pandas as pd
import numpy as np

allTokens = {}
limit = 5000

df = pd.read_csv('data/TheExaminer_SpamClickbaitCatalog/examiner_date_tokens_short.csv', sep='|')

def oneHotArrayToString(array):
    s = ""
    for i in range(len(array)):
        s+=str(int(array[i]))
    return s

# go through values to add to dictionary
limitIter = 0 # limit for the amount of characters
for row in df["headline_tokens"]:
    for ch in row:
        limitIter+=1
        if limitIter >= limit:
            break
        if (ch in allTokens.keys()):
            continue
        allTokens[ch] = np.zeros(1) # placeholder value


# get one hot encodings 
i = 0;
for ch in allTokens.keys():
    allTokens[ch] = np.zeros(len(allTokens))
    allTokens[ch][i] = 1
    i+=1

# for translating one-hot values to ch's
with open("data/TheExaminer_SpamClickbaitCatalog/oneHotRep.csv", mode='w', newline='') as file:
    for ch in allTokens.keys():
        file.write(ch)
        file.write(",")
        file.write(oneHotArrayToString(allTokens[ch]))
        file.write("\n")

# for writing the one hot pairs to the csv
f = open("data/TheExaminer_SpamClickbaitCatalog/examiner_date_tokens_short_onehot.csv", 'w')
rows = df['headline_tokens'].count()
rowi = 0
limitIter = 0
for row in df["headline_tokens"]:
    for i in range(len(row)-1):
        limitIter += 1
        if limitIter > limit:
            break
        f.write('{0}|{1}'.format(oneHotArrayToString(allTokens[row[i]]), 
                                   oneHotArrayToString(allTokens[row[i+1]])))
        if (i < len(row)-2 and limitIter < limit):
            f.write('\n')
    if limitIter >= limit-1:
        break # limit reached
    if rowi >= len(df['headline_tokens'])-1:
        continue # at the last row
    f.write('\n')
    f.write('{0}|{1}\n'.format(oneHotArrayToString(allTokens[row[len(row)-1]]), 
                               oneHotArrayToString(allTokens[df['headline_tokens'][rowi][0]])))
    # progresbar
    rowi+=1
    print(f"[{u'█'*round((rowi/rows)*60)}{'.'*(round(((rows-rowi)/rows)*60))}]", end='\r', flush=True)

print('') #for new line
f.close()
