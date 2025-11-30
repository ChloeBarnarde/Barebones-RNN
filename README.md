# Barebones RNN

This is a small project focused on how a Recurrent Neural Network functions. It was inspired by [this](https://karpathy.github.io/2015/05/21/rnn-effectiveness/) blog fromAndrej Karpathy.

## Demo

Running `make demo` in the root of the folder will compile the code. \
After that, running `./demo.out` will run the code. 

There are some functions used which are unix exclusive, so it won't run on windows. 

### Output

It will train on the datset for 100 epochs (this is most likely going to cause overfitting), a progress bar for each epoch pops up, and the smoothloss is displayed with it.

### Limitations

Currently the data isn't being split between a training set and testing set, so it is most likely overfiting the data. \
None of the variables have been whitened or noramlized, so there are most likely some issues because of that.


## Other uses

Because of the way the Gradient and Loss is calculated, it only works for classification tasks.
