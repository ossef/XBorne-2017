# XBorne-2017
We present the last version of XBorne a software tool for the probabilistic
modeling with Markov chains. The tool which has been developed initially as a test-
bed for the algorithmic stochastic comparisons of stochastic matrices and Markov
chains, is now a general purpose framework which can be used for the Markovian
modelling in education and research, further details in [1].

Xborne is organized as a set of programs which exchange files to analyse
a model. 
Files contain data (the matrices, the description of states) and 
programs in C (definition of state-dependant probabilities, definition of 
transitions, definition of rewards). Some programms must be compiled
with this new functions to analyse a new model. 

--> User's guide : view details of utilisation in file `xbornV1-3.pdf`

--> Directories :

 - SrongStochasticBoundTotal : Various implementation of Vincent algoritms available.
   IMSUB is only available with a band structure for internal representation 
   of the matrix (thus it can use a lot of memory and be slow). Also 
   the FQ algorithm with a banded form for the matrix. 

 - CensorBounds : DPY and Truffet methods are available. New methods will be 
   available soon. 

 - BlocksGeneration : not available anymore (a new release 
   to be consistent with the other tools)

 - NablaAlgorithms: some of the algorithms are here (INABLAU, INABLAL, SLUB). 

 - Format : a set of file descriptions and format to make the programs run. 

 - MarkovGeneration : generMarkov.c (replace newgen.c, some bugs fixed, some improvements) 

 - Reward : 
   You can easily use "rewardIndice.c" which computes
   typical rewards while "reawrdComponents.c" needs that you provide a 
   description of your own rewards by a C file called funReward.c. In that 
   case, your model must also have a "model.cd" file for state description. 
   An example is provided but the comments are in French. 

 - Solve: GTH (also sparse GTH), Power, GaussSeidel, and SOR. Not the best methods but you 
   can transform your model into a MM form and use state of the art solver
   available on the web. 

 - BirthDeath: Solves any classical Birth-Death process numerically. However, one should inform
   the main parameters as arrival rates and service rates in each state (adapt Taux() function to your model).
   Examples of M/M/1/B and M/M/C/B are given.

 - MatrixOPerations: some programs to transform the matrices: transpose for 
   instance. Also possible to export.
   
 - MitraniDemo and MitraniDemo2 : Exemple of building, solving and calcuting 
   Rewards of Mitran's model.
   matrix to Latex, Scilab, Matrix Market format. 
   Uniformisation and the T-transform are also available. 

 - Lumpabillity: Tarjan's algorithm to find best partition for lumpable and 
   quasi-lumpable matrix will be available soon. Ana's version of Limsub
   (with LMSUB, LIMSUB and LL as possible options) is in this directory. 

 - AutomataGeneration: to build an automata instead of the Markov chain. 
   The output is a tgf file which can be used by yed to find a nice drawing. 
   See also Lam2Tgf which transform the .Rii and .cd files into a tgf files 
   for drawing. 

 - Simulation: create a simulator of the DTMC from the fun.c file instead 
   of generating the matrix. Use the same fun.c file but it neeeds
   extra information in const.h.  2 versions here: with or without 
   information on the coverage of the state space. 
   
 - LR : some programs to calculate the steady-state distribution and transient
   distribution for Low Rank and Class G Markov chains. 
   
 - Util : Construction of the two tables of Walker's algorithm. To use in alais method for a simulation.
   as input : a probabilistic distribution.
   output : two vectors in the same file the size is in the .sz file.
   
 - Muntz : Lower and Upper algorithm for the steady state distribution.

 - Properties : verifications algorithms of some proprieties of Markov chain graphs.

##  Contributors & Copyright

- [Youssef AIT EL MAHJOUB](https://github.com/ossef)
- Jean-Michel Fourneau
- Franck Quessette
- Dimitris Vekris
- This code is open source. However, one can cite the original document [1] published in ISCIS 2016.

[1] "XBorne 2016: A Brief Introduction", Youssef AIT EL MAHJOUB, Jean-Michel FOURNEAU, Franck QUESSETTE, Dimitris VEKRIS". in ISCIS - International Symposium on Computer and Information Sciences, 2016.
