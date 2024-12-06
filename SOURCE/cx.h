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
/*
 * cx.h
 *
 * declarations for complex math routines
 *
 * Author:
 * Joseph Nathan Hall
 *
 * Copyright 1988 by CCSP.  All rights reserved.  Do not remove this notice.
 * See .c files for the FULL text of the copyright notice.
 */



typedef struct {
                double   re, im;
        } cx_t;

#define complex cx_t


cx_t	Cx_Complex(float a, float b);
cx_t	Cx_Add(cx_t x,  cx_t y);
cx_t	Cx_Mult(cx_t x,  cx_t y);
cx_t	Cx_Sub(cx_t x,  cx_t y);
cx_t	Cx_Div(cx_t x,  cx_t y);
cx_t	Cx_Exp(cx_t x);
cx_t	Cx_Tanh(cx_t x);
cx_t	Cx_Sqrt(cx_t x);

/*
 * more convenient declarations
 */

#define	CMPLX(x, y)	Cx_Complex(x, y)
#define	ADD(x, y)	Cx_Add(x, y)
#define	MULT(x, y)	Cx_Mult(x, y)
#define	SUB(x, y)	Cx_Sub(x, y)
#define DIV(x, y)	Cx_Div(x, y)
#define EXP(x)		Cx_Exp(x)
#define TANH(x)		Cx_Tanh(x)
#define SQRT(x)		Cx_Sqrt(x)


/*
 * for debugging
 */

#define CXPRINT(f, x, str) {\
	fprintf(f, "%s ", str); \
	fprintf(f, "%.4e %c %.4ej\n", x.re, (x.im < 0 ? '-' : '+'), \
	fabs(x.im)); \
	}
