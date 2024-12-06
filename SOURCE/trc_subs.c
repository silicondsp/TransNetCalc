
/*  Transmission Line Topology Calculation and Analysis (TransTopCalc TTC)
 * Library Copyright (C) 1985-2017  Sasan H Ardalan
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

#include "cx.h"
#include "trantopcalc.h"
#include <stdio.h>

#define MAX_STRING 80
#define MAX_INT 50
#define MAXLINE 80
#define PARENT 0
#define TERMINATION 1

#if 0
// OLD GKS 
/*
 * trmdraw() draws a transmission line segment and uses a 
 * menu to ask the user whether the node is a parent or 
 * a termination(leaf).
 * The beginning coordinates of the transmission line are
 * (xin,yin) and the returned ending coordinates are 
 * (xn,yn).
 * If the node is a parent then term=FALSE
 *
 */
	trmdraw(ws_id,xin,yin,xn,yn,term)
	int ws_id;
	float	xin,yin,*xn,*yn;
	BOOLEAN	*term;
{
	int		data_record[1],
     			error_status,
     			input_mode,echo_flag,record_buffer_length,record_size,
     			input_status,initial_choice,device_num,xform,
     			initial_status,prompt_echo_type;
	float		echo_area[4],world_x,world_y;
	float		x[2],y[2];
	int		np;
	int		action;
	int		act;
	static char  *selection[] = {
		"Parent     ",
		"Termination",
		0
	};
	int row_start=2;
	int col_start=68;
	int menu();
	device_num = 1;

/*	record_buffer_length = 4;	*/
	gks$inq_locator_state(&ws_id,&device_num,
     	        &GKS$K_VALUE_REALIZED,&error_status,
     		&input_mode,&echo_flag,&xform,&world_x,&world_y,
     		&prompt_echo_type, echo_area,
     		data_record, &record_buffer_length, &record_size);

	prompt_echo_type = 1;
	record_buffer_length =0;
	world_x=xin;
	world_y=yin;

	gks$init_locator(&ws_id,&device_num,&world_x,
     	  	&world_y,&xform,
     		&prompt_echo_type,
     		echo_area, data_record, &record_buffer_length );

	gks$set_locator_mode (&ws_id,&device_num,
     		&GKS$K_INPUT_MODE_REQUEST, &GKS$K_ECHO);

	gks$request_locator (&ws_id, &device_num, &input_status,
     		&xform,&world_x,&world_y );
	x[0]=xin;
	x[1]=world_x;
	y[0]=yin;
	y[1]=world_y;
	np=2;
	gks$polyline(&np,x,y);
	action=menu(selection,1,row_start,col_start);
	*term = FALSE;
	if (action == TERMINATION ) *term= TRUE;
	clear();
	*xn=world_x;
	*yn=world_y; 

}

/*
 * draw_line() simply draws a transmission line using locator to
 * specify the end point.
 *
 */
	draw_line(ws_id,xb,yb,xe,ye)
	int ws_id;
	float	xb,yb,xe,ye;
{
	float		x[2],y[2];
	int		np;


	x[0]=xb;
	x[1]=xe;
	y[0]=yb;
	y[1]=ye;
	np=2;
	gks$polyline(&np,x,y);

}


/* 
 * clears the workstation
 */

clear_ws(ws_id)
int ws_id;
{
	gks$clear_ws(&ws_id,&GKS$K_CLEAR_CONDITIONALLY );
}

#endif
/*
 * prompt the user. the string is printed in the
 * lower left corner of the screen and is highlighted.
 */
int Trc_Prompt(char *str) { fprintf(stderr, "%s \n", str); }
/*
 * print a message in the lower left corner and highlight and blink
 */
void Trc_Message(char *str) { fprintf(stderr, "%s \n", str); }
