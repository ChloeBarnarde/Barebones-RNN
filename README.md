# Barebones RNN

---

This is a small project focused on how an Recurrent Neural Network functions. It was inspired by [this](https://karpathy.github.io/2015/05/21/rnn-effectiveness/) blog fromAndrej Karpathy.

## Demo

Running `make demo` in the root of the folder will compile the code. \
After that, running `./demo.out` will run the code. The first time it's run, it will have to generate a csv, which will take a bit, but it need python3, pandas and numpy for it to work.

There are some functions used which are unix exclusive. I don't have any plans to update that at the moment.

### Example Output



## Other uses

Because of the way the gradient is calculated, it only works for classification tasks.
