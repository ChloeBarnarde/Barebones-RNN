Took the first 100,000 entires from the orignal db found [here](https://www.kaggle.com/datasets/therohk/examine-the-examiner/data)

FormatData.py takes the first 100,000 entires, and turns it into a csv with one hot encodings of X and Y.
This is so that the demo can use it, but even then, it limits it to the first 20,000 characters so that the memory doesn't get eaten up, since the file the format data script produces has over 5 million lines.

You don't have to run the python script manually, since its automatically run by the demo if the file doesn't exist.
