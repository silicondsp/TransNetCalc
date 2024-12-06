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
#    http://www.ccdsp.org
#    
#    Raleigh, North Carolina
#


#
# load the transmission line topology file "bridge.top"
#

tload bridge.top T
#print the topology
tpr $T
# set the complex generator and generator source impedance 
cx 1.0 0.0 g
cx 100.0 0. zs
#calculate all nodes at 100 kHz and store the impedances, z0, gamma, 
#    and current for each node
tcalc $T 1e5 $g $zs timp tcur
#print total input impedance and current (complex scalars)
vpr $timp
vpr $tcur
nz0gamma $T n1 z0 gamma
puts {Characteristic Impedance node n1}
vpr $z0
puts {Propagation constant  node n1}
vpr $gamma
ninfo $T n1
set fmin 0
set fmax 1e6
set points 1024
nimp  bridge.top  src 100 $fmin $fmax $points impulseResp 






