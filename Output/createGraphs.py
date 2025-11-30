import matplotlib.pyplot as plt
import pandas as pd
import itertools
import numpy as np

data = pd.read_csv('Output/output.csv')

x = range(len(data))
plt.plot(x, data['TrainLoss'], label='Train Loss')
plt.plot(x, data['TestLoss'], label='Test Loss')
plt.legend()
plt.xlabel('Epcoh')
plt.ylabel('Loss')
plt.title('Loss over Epochs')
plt.savefig('Output/training_loss.png')
# plt.show()

plt.clf()

# Big thanks to https://how2matplotlib.com/how-can-i-plot-a-confusion-matrix-in-matplotlib for showing 
# how to plot a confusion matrix in matplotlib, couldn't have done it without you guys

confusion_data = pd.read_csv('Output/ConfusionMatrixValues.csv')

cm = np.array([[confusion_data.iloc[0,0], confusion_data.iloc[0,3]],
               [confusion_data.iloc[0,1], confusion_data.iloc[0,2]]])
cm_noramalized = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]

classes = ['Red Wine', 'White Wine']

plt.imshow(cm_noramalized, interpolation='nearest', cmap=plt.cm.Wistia)
plt.title("Normalized Confusion Matrix for Wine Classification")
plt.colorbar()
tick_marks = np.arange(len(classes))
plt.xticks(tick_marks, classes, rotation=45)
plt.yticks(tick_marks, classes)

# fmt = '.2f' if normalize else 'd'
thresh = cm_noramalized.max() / 2.
for i, j in itertools.product(range(cm_noramalized.shape[0]), range(cm_noramalized.shape[1])):
    plt.text(j, i, "{:0.2f}".format(cm_noramalized[i, j]),
              horizontalalignment="center",
              color="white" if cm_noramalized[i, j] > thresh else "black")

plt.tight_layout()
plt.ylabel('True label')
plt.xlabel('Predicted label')
plt.savefig('Output/confusionMatrix.png')
plt.show()