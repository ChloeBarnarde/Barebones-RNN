import matplotlib.pyplot as plt
import pandas as pd

data = pd.read_csv('Output/output.csv')

x = range(len(data))
plt.plot(x, data['TrainLoss'], label='Train Loss')
plt.plot(x, data['TestLoss'], label='Test Loss')
plt.legend()
plt.xlabel('Epcoh')
plt.ylabel('Loss')
plt.title('Loss over Epochs')
plt.savefig('Output/training_loss.png')
plt.show()