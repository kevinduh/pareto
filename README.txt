===============================
Pareto Frontier Computation

Kevin Duh <kevinduh@is.naist.jp>
===============================

Compile: 
>> make
>> make clean


Usage:
>> ./pareto -l file1 -s file2 ... -l fileK 

   This computes the pareto frontier given N K-dimensional datapoints. 
   There are K>1 files, each consisting of a column of N numbers. 
   The flag -l indicates larger number is better (maximize); 
   alternatively, the -s flag means smaller is better (minimize). 
   The output consists of N labels, where 1=pareto and 0=not. 


Example:
>> ./pareto -l example.file1 -l example.file2 > example.result
- this should give: #pareto: 6 of 20 total, by the stablesort algorithm 
- see example.png for visualization of this example

------------------------------------------------------------------
Note:
  Currently, the underlying pareto algorithm uses
   - an O(NlogN) stablesort method for K=2, and
   - an O(KN^2) pairwise method for K>2. 
  Faster ones for K>2 may be implemented in the future.
  Reference: P. Godfrey, R. Shipley, J Gryz, "Algorithms and Analyses for Maximal Vector Computation", VLDB Journal, 2007 vol. 16(5). 

