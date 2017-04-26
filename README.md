# sparse-coding

## Training
The values in the training dataset are integers and distributed in the range of [-15871; 16213] (please note, that the ranges are very different for the different sequences, but we estimate the largest range for all sequences). Thus, they may be encoded as the 15-bit variables (1 sign bit, and 14 data bits representing a number upto 16384)

## Example command
sc_train.exe D:\Data\Raw_6_sensors 6 16 1 512 2000 1000 0.01f D:\dictionary.dic

## Testing
