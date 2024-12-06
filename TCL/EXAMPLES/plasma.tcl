#  Transmission Line Topology Calculation and Analysis (TranTopCalc) Command Tool Scripts
#    Copyright (C) 2006  Sasan H Ardalan
#
#    This script is free software; you can redistribute it and/or
#    modify it under the terms of the GNU  General Public
#    License as published by the Free Software Foundation; either
#    version 2 of the License, or (at your option) any later version.
#
#   This script is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#     General Public License for more details.
#
#    You should have received a copy of the GNU  General Public
#    License along with this library; if not, write to the Free Software
#    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#    http://www.xcad.com
#    XCAD Corporation
#    Raleigh, North Carolina
#

#
# This is a double stub tuner. At node n1 we have a match. 
# node n2 is the second stub near the generator
# At 10MHz the network is matched.
#
# in the following script we change the length of the stub n2 
# by +- 10% around the length where a match is acheived.
# 100 points are calculated.
# results are stored as follows.
# The complex input impedance is stored in a complex vector
# It is stored in a filex as complex numbers (a+bi)
# The VSWR and Length are stored in a list.
# This list is later stored in a file as two columns
#

#
# load the transmission line topology file "t.top"
#

tload plasmaplate.top T
#print the topology
tpr $T
# set the complex generator and generator source impedance 
cx 1.0 0.0 g
cx 377 0. zs
set frequency 40e9
tcalc $T $frequency $g $zs 


#set fmin 150e9
#set fmin 400e9
set fmin 100e9
set points 1024
set fmax  [expr 20e6*512+$fmin]
nimp  plasmaplate.top  n0 377  $fmin $fmax $points impResp 







