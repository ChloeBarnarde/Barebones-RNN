import matplotlib.pyplot as plt
import pandas as pd

data = pd.read_csv('Output/output.csv')

x = range(len(data))
y = data["TrainLoss"]
plt.plot(x, y)
plt.xlabel('Epcoh')
plt.ylabel('Loss')
plt.title('Training Loss over Epochs')
plt.savefig('Output/training_loss.png')
plt.show()