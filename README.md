# TransNetCalc

<h1>Transmission Line Network Modeling, Design and Analysis</h1>
Also visit the Blog:
https://aj7bf.com
The progam is called <i>TransNetCalc</i>.
  <p>  The C Code including integrated TCL interpreter is hosted on GitHub <a href="https://github.com/silicondsp/TransNetCalc">here</a>. 
<p>Prof. Sasan Ardalan (AJ7BF Extra Class Amateur Radio) has over the years developed a C program for modeling, design and analysis of complex transmission line networks. 
<p>The program is called <i>TransNetCalc</i> and is Open Source LGPL 3,</p> 
Paper on the Computer Modeling of Complex Transmission Lines (Foundation for <i>TransNetCalc</i>): 
</p>  
 <a href="Papers/Computer_Modeling_Transmission_Line_Networks.pdf"><img src="images/noun-document-103341.png" width="60" height="60" alt="paper" title="" /></a>
<p>For Modeling and Analysis of Plane Wave Propagation through dielectric sections and plasma see the paper below.
</p>    
<a href="https://vixra.org/abs/2412.0028"><img src="images/noun-document-103341.png" width="60" height="60" alt="paper" title="" /></a>
<br>  
<br> 
<br> 
<img src="images/lgplv3-147x51-1.png" width="147" height="51" alt="lgpl" title="" />
<img src="images/tcl-logo-300x180.png" width="100" height="60" alt="tcl" title="" /> 
 <br> 
  <br> <br> <br>
 <img src="images/transmission_line_types-1024x845.png" width="768" height="634" alt="transmission lines" title="" />
<p> <i>TransNetCalc</i>  also incorporates a TCL interpreter so that transmission line networks such as double stub tuners, microstrip bandpass and bandstop filters for instance can be optimized. Also the impact of parameter changes and variation on performance can be computed and plotted.
 </p>
 <p>Here is a link to the <a href="https://www.ccdsp.org/TransTopCalc_Site/userguide.htm">User Guide</a>. Note that microstrip lines are also supported.</p>
<p> Example Network (Double Stub Tuner):</p>
 <img src="images/doublestubtuner.png" width="633" height="318" alt="double stub tuner" title="" />
 <pre>n1 n2 n3
n3 n4 n5
n2
n4
n5
end
n1 coax1 30
n2 coax1 1.76 0 0
n3 coax1 2.5
n4 coax1 6.6 0 0
n5 R100_L0.0000015915 0 open </pre> 
<p>The coaxial transmission line <i>coax1</i> has characteristic impedance of 50 Ohms.</p> 
<h2>TCL Scripting</h2>
<p>The scripting capabilities of <i>TransNetCalc</i> using TCL are described <a href="https://www.ccdsp.org/TransTopCalc_Site/trantopcalc_tcl.htm">here</a> with documentation of the TCL commands.
</p>
<h2>The following is a history of the development:</h2>
<p> <i>TransNetCalc</i> has been developed by Professor Sasan Ardalan since 1987 while at NC State University and has been incorporated into <a href="https://www.ccdsp.org/">Capsim&reg;</a> since 1989. The code has been enhanced and debugged through the years. The integration with TCL was done in August 2006. Susan Alexander working with Prof. Ardalan developed the recursive solution to the binary tree representation of transmission lines. Ken Shuey provided support for modeling power transmission lines. Gary Ybarra (now a professor at Duke University) working with Professor Ardalan extended <i>TransNetCalc</i> to model plane wave propagation and turbulent Plasma’s as encountered in the re-entry or aero assist braking off the atmosphere. Joseph Hall, an outstanding programmer, developed a graphical interface to the underlying C code while at NC State back in 1988. <i>TransCalcNet</i> has been successfully used in the design of microstrip bandpass filter at 10GHz, bandstop filter at 8.3 GHz and also bandpass filters at 60 GHz. It was also highly accurate in modeling the plasma developed during re-entry on heat tiles (matching super computer models at NASA).</p>
<h2>Licensing:</h2>
<p>The <i>TransNetCalc</i> stand alone program and developed TCL commands and API is written in C and are licensed under the GNU Lesser General Public License. The TCL scripts are covered by the GNU General Public License. The scripts are separate from the <i>TransNetCalc</i> library. The intention is to provide all of the source code of <i>TransNetCalc</i> as open source code with freedom to modify and enhance the code. All modifications and enhancements to <i>theTransNetCalc</i> C API, TCL Commands must be published if distributed. Any distribution of the created code linking to the the <i>TransNetCalc</i> API and TCL commands must include a statement showing links to the original <i>TransNetCalc</i> source code. The source code linking into the <i>TransNetCalc</i> API or TCL commands does not need to be exposed.
 </p>
<a href="https://www.ccdsp.org/TransTopCalc_Site/"><img src="sd-logo-tm_sm.png" width="104" height="109" alt="sdsp logo" title="" /></a>
<p><strong>Silicon DSP Corporation</strong></p>
<p>1989-2025</p>
<p>https://www.ccdsp.org</p> 




