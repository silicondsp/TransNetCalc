
# topology file:  trantest.t

#--------------------------------------------------- 
# Title:      
# Author:      
# Date:      
# Description:      
#--------------------------------------------------- 

inform title      
inform author      
inform date      
inform descrip      

arg -1 (none)

param int 1024
star impulse0 impulse

param int 0
param int 10
param float 1e+06
param float 100
param file line1.top
param file terminal
param float 0
param float 0
star transline0 transline

param int 1024
param int 0
param file PLOT
param int 0
param int 0
param int 1
param int 1
param float 0
param int 1
param int 0
star spectrum0 spectrum

connect impulse0 0 transline0 0  	
connect transline0 0 spectrum0 0  	

