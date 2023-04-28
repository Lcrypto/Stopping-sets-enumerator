# Stopping-sets-enumerator
The GitHub repository includes a Linear Programming (LP) based stopping set counter for Quasi-Cyclic Low-Density Parity-Check (QC-LDPC) codes. The tool takes the QC code specification from the "testInput.txt" file and uses an efficient algorithm proposed by E. Rosnes and O. Ytrehus to find all small-size stopping sets of low-density parity-check matrices. This algorithm was published in the IEEE Transactions on Information Theory in 2009.

The tool uses the CLP/CoinUtil 1.16.11 library, which can be easily implemented by proprietary solvers such as CPLeX and Gurobi, with several time speed-up. With this tool, you can efficiently count the number of stopping sets of different sizes for QC-LDPC codes.
