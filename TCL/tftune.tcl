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
# in the following script we change the frequncy of the stub n2 
# by +- 10% around the frequency where a match is acheived.
# 100 points are calculated.
# results are stored as follows.
# The VSWR and freq are stored in a list.
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
set freq 1e7
#calculate +-10% of tuned frequency 10MHz 
set fBegin [ expr $freq -0.1*$freq]
set fEnd [ expr $freq + 0.1*$freq ]
set df [expr ($fEnd-$fBegin)/100.0 ]

# initialize list to store results (in addition to storing in vectors)
set results [list {}]

puts "Iterating over length"
for { set i 0 } { $i<100 } {incr i} {
    set f [expr $fBegin +$i*$df ]
    #recalculate the network for new frequency
    tcalc $T $f $g $zs
    # get z0 and gamma (propagation constant) to use in VSWR calculation
    nz0gamma $T n1 z0 gamma

    # store all the computed values for node n1 (left right impedance, current )
    # the variables are automatically created
    nvalues $T n1
    # compute total inpedance at node n1 (variables $n1_zl and $n1_zr were automatically created in the previous command)
    zpar $n1_zl $n1_zr zt
    
    #compute reflection coefficient
    rflc $zt $z0 rflc_n1
    #compute vswr
    vswr $rflc_n1 vswr_n1
    
    #store vswr in list with freq
    set fMHz [expr $f*1e-6]
    lappend results [list $fMHz $vswr_n1 ]
}

#
# create a file to store the vswr vs length
#
puts "Storing results in vswr_tune.dat"
set out [ open vswr_tune.dat w ]
foreach value $results {
   puts  -nonewline $out [lindex $value 0] 
   puts  -nonewline $out "\t" 
   puts   $out [lindex $value  1] 

}

close $out






