/*  Transmission Line Topology Calculation and Analysis (TransTopCalc TTC) Library 
 *    Copyright (C) 1985-2017  Sasan H Ardalan
 *
 *    This library is free software; you can redistribute it and/or
 *    modify it under the terms of the GNU Lesser General Public
 *    License as published by the Free Software Foundation; either
 *    version 2.1 of the License, or (at your option) any later version.
 *
 *    This library is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public
 *    License along with this library; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    http://www.SiliconDsp.com
 *    Silicon DSP  Corporation
 */


#include <stdio.h>


#define BOOLEAN int
#define TRUE 1
#define FALSE 0
#define TOPOLOGY 5
#define SEG_NAME_BEGIN 1000
#define PI 3.14159265358979323	
#define NODE_NAME_LENGTH 80
#define TYPE_NAME_LENGTH 80

#define NODE_TYPE_BINARY 0
#define NODE_TYPE_UNARY 1
#define NODE_TYPE_LEAF 2

#define MAX_PARAMS 10


#define  SCALE  8.6858896
#define NUMBER_TYPES 2000



struct node {			
	struct node 	*parent_P;		/* parent pointer */
	struct node 	*left_P;		/* left subtree */
	struct node 	*right_P;		/* right subtree */
	int		type;
	float		x;		/* x coordinate for capnet.c */
	float		y;		/* y coordinate for capnet.c */
	char 		name[NODE_NAME_LENGTH];	/* name of node */
	char 		leftName[NODE_NAME_LENGTH];	/* name of left node */
	char 		rightName[NODE_NAME_LENGTH];	/* name of right node */
	char		typeName[NODE_NAME_LENGTH];
	complex 	leftImp;		/* left impedence */
	complex 	rightImp;		/* right impedence */
	complex		inCurrent;		/* incoming current */
	complex 	leftCurrent;		/* left current */
	complex 	rightCurrent;		/* right current */
	complex		voltage;	/* voltage */
	complex 	zLoad;		/* load impedence */
	float		r;		/* resistance per unit length */
	float		l;		/* inductance per unit length */
	float		c;		/* capacitance per unit length */
	float		g;		/* conductance per unit length */

	float		p[10];
	complex		zo;             /* characteristic impedence */
	complex		gamma;		/* propagation constant */
	float 		length;		/* length of wire */
	int		infinite;	/* indicates open circuit */
	int		lumped;
	complex		epsr;
};

struct transType  {
	char 	typeName[TYPE_NAME_LENGTH];
	float	r,l,c,g;
	float   params_A[MAX_PARAMS];
	int     count;
}; 




extern  float Trc_freq;


complex Trc_CalcCharImp(float r, float c, float l, float g, float freq, float wl);
complex Trc_CalcPropConst(float r, float c, float l, float g, float freq);
void 	Trc_BuildDistance(struct node *root_P, float distance_A[],float dt,float freq,int npts);
complex Trc_CalcPropConstPlasma(float nu, float n, float freq);
complex Trc_CalcCharImpPlasma(float nu, float n, float freq);
complex Trc_CalcCharImpPlasma(float nu, float n, float freq);
complex Trc_CalcCharImpCoax(float ri,float rs,float eps,float t,float freq);
complex Trc_CalcPropConstParall(float ri,float d,float eps,float freq);
complex Trc_CalcPropConstCoax(float ri,float rs,float eps,float t,float freq);
complex Trc_CalcCharImpParall(float ri,float d,float eps,float freq);
complex Trc_CalcPropConstWireabg(float ri,float h,float eps,float freq);
complex Trc_CalcCharImpWireabg(float ri,float h,float eps,float freq);
complex Trc_CalcPropConstBalShielded(float ri,float rs,float eps,float h,float freq);
complex Trc_CalcCharImpBalShielded(float ri,float rs,float eps,float h,float freq);


complex Trc_CalcPropConstMicrostrip(float w,float h,float er,float t,float freq);
complex Trc_CalcCharImpMicrostrip(float w,float h,float er,float t,float freq);

struct node *Trc_RecallTree(FILE 	*fp);
void Trc_TreeCharic(struct node *root_P,float freq);
void Trc_CalcCurrent(struct node *root_P, complex inputCurrent, complex inputVoltage, complex zo,complex  gamma);
void Trc_GetNodeVoltage(struct node *root_P, char *nodename,  float	*vreal, float *vimag);
void Trc_cxfft(float x[],int   *mfft);
complex	Trc_CalcImp(struct node 	*root_P, complex zo, complex gamma);
complex Trc_InitCur(struct node	*root_P, complex	gamma, complex	zo, complex	eg, complex	zs, complex	Trc_zLoad, float length, complex	totalImp);
complex Trc_LineImp(complex rcl, complex zo, complex gamma, float length);
complex Trc_CalcRcs(complex z_source, complex zo);
complex Trc_CalcRcl(complex zLoad, complex zo, int inf);
complex Trc_LineVolt(struct node	*root_P, complex inputVoltage);
complex Trc_LineCur(struct node	*root_P, complex inputCurrent,complex  zo, complex gamma, complex zLoad, float length, int inf);
complex Trc_LeftCur(complex leftImp, complex rightImp, complex inputCurrent);
complex Trc_RightCur(complex leftImp, complex rightImp, complex inputCurrent);
complex Trc_LumpedLineCur(struct node	*root_P, complex         inputCurrent);
complex Trc_ZParallel(complex leftImp, complex rightImp);
void Trc_GetNodeCurrent(struct node *root_P, char *nodename,  float	*vreal, float	*vimag);
void Trc_GetNodeVoltage(struct node *root_P, char *nodename,  float *vreal, float	*vimag);
void 	Trc_TreeCharic(struct node 	*root_P,float  freq);
void Trc_CalcCurrent(struct node *root_P,complex inputCurrent,complex inputVoltage,complex zo,complex gamma);
int Trc_NodeInit(struct node *root_P,FILE *fp);
complex Trc_TrLineCur(struct node	*root_P, complex inputCurrent, float length);
complex Trc_TrLineVoltage(struct node	*root_P, complex inputVoltage, float length);
complex Trc_TrCalcReflectionCoeff(struct node	*root_P);
int Trc_CnLoadTypes(struct transType trmTypes_A[],int	   *numTypes_P);
void Trc_NetLength(struct node *root_P);
struct node *Trc_SearchTreeName(struct node *root_P,char *name);
void Trc_PrintNode(struct node *root_P,FILE *fp);
void Trn_PrintTree(struct node *root_P, char *nodename, FILE *fp);
void Trc_CnGetLumpedValues(struct node *current_P);

void Trc_Propconst(float freq,float r,float l,float c,float g,float wL,double *alpha_P,double *beta_P);
void Trc_CirCond(float freq,float ri,float *delta_P,float *r_P,float *wL_P);


void Trc_NetImpCalc(struct node *root_P, char topologyFileName[]);
void Trc_NetAnal(struct node *root_P, char topologyFileName[]);

int Trc_NetImp(char *netFileName,char *nodeName,int frimpFlag,
              float frmin,float frdel,float frmax,int nfft,
              float sourceE,float sourceImp);



int Trc_Prompt(char *str);
void Trc_Message(char *str);
float Trc_Getval(char string[],int index);

