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

tload t.top T
#print the topology
tpr $T
# set the complex generator and generator source impedance 
cx 1.0 0.0 g
cx 50.0 0. zs
#calculate all nodes at 10MHz and store the impedances, z0, gamma, and current for each node
tcalc $T 1e7 $g $zs timp tcur
#print total input impedance and current (complex scalars)
vpr $timp
vpr $tcur
# get the length of the stub n2
ngetlength $T n2 n4len
#calculate +-10% and dlength
set lenBegin [ expr $n4len -0.1*$n4len]
set lenend [ expr $n4len + 0.1*$n4len ]
set dlen [expr ($lenend-$lenBegin)/100.0 ]
#create complex and real vectors to store results (much more efficient than lists)
vcxcr  zvlen 100
vcr zmag 100
# get z0 and gamma (propagation constant) to use in VSWR calculation
nz0gamma $T n1 z0 gamma
# initialize list to store results (in addition to storing in vectors)
set results [list {}]

puts "Iterating over length"
for { set i 0 } { $i<100 } {incr i} {
    set len [expr $lenBegin +$i*$dlen ]
    #change the length of the stub
    nsetlength $T n2 $len
    #recalculate the network for new length
    tcalc $T 1e7 $g $zs
    # store all the computed values for node n1 (left right impedance, current )
    # the variables are automatically created
    nvalues $T n1
    # compute total inpedance at node n1 (variables $n1_zl and $n1_zr were automatically created in the previous command)
    zpar $n1_zl $n1_zr zt
    vpr $zt
    #store the input impedance at n1 in the complex vector $zvlen (z versus length)
    vcxset $zvlen  $zt $i
    #get the magnitude of the impedance
    cxgetpolar $zt zmagval theta
    
    #compute reflection coefficient
    rflc $zt $z0 rflc_n1
    #compute vswr
    vswr $rflc_n1 vswr_n1
    #store in real vector
    vset $zmag $zmagval $i
    
#    lappend results [list $len $zmagval ]
    #store vswr in list with length
    lappend results [list $len $vswr_n1 ]
}
#print resulst of input impedance
vpr $zvlen 14.6f zin_vs_len.dat -twocol

#
# create a file to store the vswr vs length
#
puts "Storing results in vswr.dat"
set out [ open vswr.dat w ]

# puts $out [ llength $results ] 
#puts -nonewline $out "#Length" 
#puts -nonewline $out "\t"  
#puts    $out "MagZin"  

foreach value $results {
   puts  -nonewline $out [lindex $value 0] 
   puts  -nonewline $out "\t" 
   puts   $out [lindex $value  1] 

}

close $out






