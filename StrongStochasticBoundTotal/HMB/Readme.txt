Syntax HMB_LOW -f modelname

assumes that modelname.EL.sz,  modelname.EL.Rii, modelname.EU.sz, and 
modelname.EU.Rii exist (they are elementwise lower bound and upper bound 
matrices to define a set of matrices) and creates modelname.HM.L.sz and 
modelname.HM.L.Rii to describe the strong st lower bound of all the 
matrices in the set. 

HMB_UP : idem but creates the upper bound (the files are called 
modelname.HM.U.sz and modelname.HM.U.Rii).


HBM_2UP : idem but with another implementation. 

 



