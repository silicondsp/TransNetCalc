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
#include <math.h>
#include <stdio.h>
#include <string.h>

#define MU 12.566371e-7
#define EPS0 8.854e-12
#define SIG 58.0e6
#define SIGMA 58.0e6
#define SPEED_OF_LIGHT 299792458.
/*
 * for miles
#define SCALE 1609.35
 */
#define SCALE 1.0
#define INFINITY 1.0e22

/*
 * The function CalcCharImp calculates the characteristic
 * impedence of the network in terms of the cable parameters.
 */
complex Trc_CalcCharImp(float r, float c, float l, float g, float freq,
                        float wl)
//  float	r,		/* resistance per unit length */
//  	c,		/* capacitance per unit length */
//  	l,		/* inductance per unit length */
//  	g,		/* conductance per unit length */
//  	freq;		/* frequency */
//  float	wl;		/* intrinsic inductance term */
{
  double w, x, y, zomag, theta;
  double wL;
  complex zo;

  /*
   * characteristic impedence zo = (r+jwl)/(g+jwc)
   */

  w = 2.0 * PI * freq;
  wL = w * l + wl;
  x = g * g + (w * w * c * c);
  y = r * r + (wL * wL);
  zomag = sqrt(y / x);
  zomag = sqrt(zomag);
  if (g)
    theta = atan(wL / r) - atan(w * c / g);
  else
    theta = atan(wL / r) - PI / 2;
  theta = theta / 2.0;
  zo.re = zomag * cos(theta);
  zo.im = zomag * sin(theta);
  return (zo);
}

/*
 * The function CalcPropConst calculates the propagation constant
 * of the network in terms of the cable parameters.
 */
complex Trc_CalcPropConst(float r, float c, float l, float g, float freq)
//   float	r,		/* resistance per unit length */
//           c,		/* capacitance per unit length */
//           l,		/* inductance per unit length */
//           g,		/* conductance per unit length */
//           freq;		/* frequency */

{
  float v, w, x, y, z, t;
  complex gamma;

  /* propagation constant gamma = sqrt[(r+jwl)(g+jwc)] =
   * sqrt[1/2{sqrt((r2+w2l2)(g2+w2c2))+rg-w2lc}]
   * +j sqrt[1/2{sqrt(r2+w2l2)(g2+w2c2))-rg+w2lc}]
   */

  w = 2.0 * PI * freq;
  t = w * w;
  v = r * r + (t * l * l);
  z = g * g + (t * c * c);
  y = sqrt(v * z);
  y = sqrt(y);
  if (r)
    v = atan2(w * l, r);
  else
    v = PI / 2;
  if (g)
    z = atan2(w * c, g);
  else
    z = PI / 2;
  x = 0.5 * (v + z);
  gamma.re = y * cos(x);
  gamma.im = y * sin(x);
  /*
          x = g*g+(w*w*c*c);
          y = r*r+(w*w*l*l);
          z = sqrt(x*y);
          v = r*g-(w*w*l*c);
          gamma.re = sqrt((v+z)/2.0);
          gamma.im = sqrt((-v+z)/2.0);
  */
  return (gamma);
}

/*
 * This routine is a special routine that creates
 * an array of distance corresponding to lengths of
 * different segments. The calculation adjusts the length for
 * the speed of light in the medium.
 * This routine ignores the right nodes and goes all the way to the
 * load
 */
void Trc_BuildDistance(struct node *root_P, float distance_A[], float dt,
                       float freq, int npts) {
  float n, epsr, eps, nu;
  float dist;
  float time, accTime, totalTime, totalLength;
  float velocity;
  int plasmaFlag = 0;
  int i, j, jj, numSlabs;
  int ii;
  int ns;
  int totalSlabs;
  float tt;
  struct node *current_P; /* pointer to node */
  FILE *profile_F;
  int getOut;

  if (root_P == (struct node *)NULL)
    return;
  // fprintf(stderr,"dt freq npts  %e %e %d\n",dt,freq,npts);
  /*
   * calculate total length
   */
  totalLength = 0;
  current_P = root_P;
  totalSlabs = 0;
  do {
    totalLength += current_P->length;
    totalSlabs++;
  } while ((current_P = current_P->left_P));
  printf("Total length = %e total slabs = %d \n", totalLength, totalSlabs);

  profile_F = fopen("profile.dat", "w");
  current_P = root_P;
  dist = 0;
  i = 0;
  for (ii = 0; ii < npts; ii++) {
    if (dist >= totalLength * 100)
      break;
    numSlabs = 0;
    totalTime = 0;
    current_P = root_P;
    do {
      numSlabs++;

      /*
       * check if plasma
       */
      if (strncmp(current_P->typeName, "plasma", 6) == 0) {
        plasmaFlag = 1;
        nu = current_P->r;
        n = current_P->l;
        epsr = 1.0 - 81e6 * n / (freq * freq);
        if (epsr > 0) {
          eps = EPS0 * epsr;
          current_P->epsr.re = epsr;
          current_P->epsr.im = 0;
        } else {

          eps = EPS0 * epsr;
          current_P->epsr.re = epsr;
          current_P->epsr.im = 0;
          epsr = 1.0;
        }
      } else {
        eps = current_P->c;
        epsr = eps / EPS0;
        current_P->epsr.re = epsr;
        current_P->epsr.im = 0;
      }

      if (ii == 0)
        printf("%s epsr = %e \n", current_P->name, epsr);
      if (!plasmaFlag)
        velocity = SPEED_OF_LIGHT / sqrt(epsr);
      else
        velocity = SPEED_OF_LIGHT * sqrt(epsr);

      totalTime += 2.0 * current_P->length / velocity;
      if (totalTime > ii * dt)
        break;
    } while ((current_P = current_P->left_P));
#if 1
    printf("Number of slabs at %d = %d \n", ii, numSlabs);
#endif
    ns = numSlabs;
    distance_A[ii] = 0;
    {
      numSlabs = 0;
      current_P = root_P;
      distance_A[ii] = 0;
      do {
        numSlabs++;
        distance_A[ii] += current_P->length * 100;
        /*
        fprintf(profile_F,"%e %e\n",distance_A[ii],current_P->epsr.re);
        */
      } while ((current_P = current_P->left_P) && numSlabs < ns);
    }
    dist = distance_A[ii];
  }

  if (ii < npts) {
    for (j = ii; j < npts; j++) {
      dist += dt * SPEED_OF_LIGHT * 100.0;
      distance_A[j] = dist;
      /*
                 fprintf(profile_F,"%e %e \n",dist,eps/EPS0);
      fprintf(profile_F,"%e  1.0\n",distance_A[j]);
      */
    }
  }
  for (i = 0; i < npts; i++) {
    if (distance_A[i] >= totalLength * 100.0)
      fprintf(profile_F, "%e  1.0\n", distance_A[i]);
    else {
      current_P = root_P;
      tt = 0;
      getOut = FALSE;
      do {
        tt += current_P->length;
        if (tt * 100.0 > distance_A[i]) {
          fprintf(profile_F, "%e  %e\n", distance_A[i], current_P->epsr.re);
          getOut = TRUE;
        }

      } while ((current_P = current_P->left_P) && !getOut);
    }
  }

  fclose(profile_F);
}

/*
 * The function CalcPropConstPlasma calculates the propagation constant
 * of the network in terms of the plasma parameters.
 */
complex Trc_CalcPropConstPlasma(float nu, float n, float freq)
//      float	nu,		/* Collision frequency */
//              n,		/* electron density */
//              freq;		/* frequency */
{
  double v, w, x, y, z, t;
  double delta, omega;
  double wp, wpsqr;
  double nu1;
  double mag, angle;
  complex gamma;

  w = 2.0 * PI * freq;
  wp = 2 * PI * 9000 * sqrt(n);
  nu1 = nu * 2 * PI;
  wpsqr = wp * wp;

  t = w * w + nu1 * nu1;
  delta = wpsqr / t;
  omega = wpsqr * nu1 / (w * t);

  mag = (1 - delta) * (1 - delta) + omega * omega;
  mag = sqrt(sqrt(mag));
  mag = mag * w / 3e8;

  if (1 - delta)
    if (omega)
      angle = 0.5 * atan2(-omega, 1 - delta);
    else if (1 - delta > 0) {
      gamma.re = 0.0;
      gamma.im = mag;
      return (gamma);
    } else {
      gamma.re = -mag;
      gamma.im = 0.0;
      return (gamma);
    }
  else
    angle = -PI / 4;

  angle += PI / 2;
  gamma.re = mag * cos(angle);
  gamma.im = mag * sin(angle);
#if 0
fprintf(stderr,"gamma %e %e \n",gamma.re,gamma.im);
#endif
  return (gamma);
}

/*
 * The function CalcCharImpPlasma calculates the characteristic
 * impedence of the network in terms of the plasma parameters.
 */
complex Trc_CalcCharImpPlasma(float nu, float n, float freq)
//    float	nu,		/* Collision frequency */
//            n,		/* electron density */
//            freq;		/* frequency */
{
  double w, x, y, t, zomag, theta;
  complex zo;
  double delta, omega;
  double wp, wpsqr;
  double nu1;
  double mag, angle;

  w = 2.0 * PI * freq;
  wp = 2 * PI * 9000 * sqrt(n);
  nu1 = nu * 2 * PI;
  wpsqr = wp * wp;

  t = w * w + nu1 * nu1;
  delta = wpsqr / t;
  omega = wpsqr * nu1 / (w * t);

  mag = (1 - delta) * (1 - delta) + omega * omega;
  mag = sqrt(sqrt(mag));
  mag = 1.0 / mag;

  zomag = mag * 376.7343;
  if (1 - delta)
    if (omega)
      angle = 0.5 * atan2(-omega, 1 - delta);
    else if (1 - delta < 0) {
      zo.re = 0.0;
      zo.im = zomag;
      return (zo);
    } else
      angle = 0;
  else
    angle = -PI / 4;

  angle = -angle;

  zo.re = zomag * cos(angle);
  zo.im = zomag * sin(angle);
  return (zo);
}

/*
 * The function CalcPropConstCoax calculates the propagation constant
 * of the network in terms of the coaxial transmission line  parameters.
 */
complex Trc_CalcPropConstCoax(float ri, float rs, float eps, float t,
                              float freq)
//    float	ri,		/* Coax inner cond. radius */
//            rs,		/* Coax outer cond. inside radius */
//            eps,		/* dielectric constant */
//            t,		/* shield thickness */
//            freq;		/* frequency */
{
  double v, w, x, y, z;
  float wl, r, l, c, g, delta;
  double mag, angle;
  complex gamma;
  double alpha, beta;
  float rShield;

  // fprintf(stderr,"ri rs eps t freq %e %e %e %e %e \n",ri,rs,eps,t,freq);

  Trc_CirCond(freq, ri, &delta, &r, &wl);
  l = sqrt(MU * EPS0) * 138 * log10(rs / ri);
  c = (MU * EPS0 * eps) / l;
  g = 0.0;
  if (delta > t)
    delta = t;
  rShield = 1.0 / (SIGMA * PI * delta * (2 * rs + delta));
  r += rShield;
  Trc_Propconst(freq, r, l, c, g, wl, &alpha, &beta);
  gamma.re = alpha;
  gamma.im = beta;
  return (gamma);
}

/*
 * The function CalcCharImpCoax calculates the characteristic impedance
 * of the network in terms of the coaxial transmission line  parameters.
 */
complex Trc_CalcCharImpCoax(float ri, float rs, float eps, float t, float freq)
//   float	ri,		/* Coax inner cond. radius */
//           rs,		/* Coax outer cond. inside radius */
//           eps,		/* dielectric constant */
//           t,		/* shield thickness */
//           freq;		/* frequency */
{
  double v, w, x, y, z;
  float delta, omega;
  double mag, angle;
  float r, l, c, g, wl;
  complex zo;
  float rShield;

  Trc_CirCond(freq, ri, &delta, &r, &wl);
  if (delta > t)
    delta = t;
  rShield = 1.0 / (SIGMA * PI * delta * (2 * rs + delta));
  r += rShield;
  l = sqrt(MU * EPS0) * 138 * log10(rs / ri);
  c = (MU * EPS0 * eps) / l;
  g = 0.0;
  // fprintf(stderr,"r l c g freq %e %e %e %e %e \n",r,l,c,g,freq);
  zo = Trc_CalcCharImp(r, c, l, g, freq, wl);

  return (zo);
}

/*
 * The function CalcPropConstParall calculates the propagation constant
 * of the network in terms of two parallel wire
 * transmission line  parameters.
 */
complex Trc_CalcPropConstParall(float ri, float d, float eps, float freq)
//   float	ri,		/* wire radius */
//           d,		/* distance between conductors */
//           eps,		/* dielectric constant */
//           freq;		/* frequency */
{
  double v, w, x, y, z, t;
  float wl, r, l, c, g, delta;
  double mag, angle;
  complex gamma;
  double alpha, beta;
  float rShield;

  Trc_CirCond(freq, ri, &delta, &r, &wl);
  l = sqrt(MU * EPS0 * eps) * 120.0 * log(d / ri);
  c = MU * EPS0 * eps / l;
  g = 0.0;
  r = 2 * r;
  wl = 2 * wl;
  Trc_Propconst(freq, r, l, c, g, wl, &alpha, &beta);
  gamma.re = alpha;
  gamma.im = beta;
  return (gamma);
}

/*
 * The function CalcCharImpParall calculates the characteristic impedance
 * of the network in terms of two parallel wire
 * transmission line  parameters.
 */
complex Trc_CalcCharImpParall(float ri, float d, float eps, float freq)
//     float	ri,		/* wire radius */
//             d,		/* distance between conductors */
//             eps,		/* dielectric constant */
//             freq;		/* frequency */
{
  double v, w, x, y, z, t;
  float delta, omega;
  double mag, angle;
  float r, l, c, g, wl;
  complex zo;

  Trc_CirCond(freq, ri, &delta, &r, &wl);
  l = sqrt(MU * EPS0 * eps) * 120.0 * log(d / ri);
  c = MU * EPS0 * eps / l;
  g = 0.0;
  r = 2 * r;
  wl = 2 * wl;

  zo = Trc_CalcCharImp(r, c, l, g, freq, wl);

  return (zo);
}

/*
 * The function CalcPropConstWireabg calculates the propagation constant
 * of the network in terms of single wire  above ground plane
 * transmission line  parameters.
 */
complex Trc_CalcPropConstWireabg(float ri, float h, float eps, float freq)
// float	ri,		/* wire radius */
//         h,		/* height above ground */
//         eps,		/* dielectric constant */
//         freq;		/* frequency */
{
  double v, w, x, y, z, t;
  float wl, r, l, c, g, delta;
  double mag, angle;
  complex gamma;
  double alpha, beta;

  Trc_CirCond(freq, ri, &delta, &r, &wl);
  l = sqrt(MU * EPS0) * 138 * log10(2 * h / ri);
  c = MU * EPS0 * eps / l;
  g = 0.0;
  Trc_Propconst(freq, r, l, c, g, wl, &alpha, &beta);
  gamma.re = alpha;
  gamma.im = beta;
  return (gamma);
}

/*
 * The function CalcCharImpWireabg calculates the  characteristic impedance
 * of the network in terms of single wire  above ground plane
 * transmission line  parameters.
 */
complex Trc_CalcCharImpWireabg(float ri, float h, float eps, float freq)
// float	ri,		/* wire radius */
//         h,		/* height above ground */
//         eps,		/* dielectric constant */
//         freq;		/* frequency */
{
  double v, w, x, y, z, t;
  double delta, omega;
  double mag, angle;
  float r, l, c, g, wl;
  complex zo;

  Trc_CirCond(freq, ri, &delta, &r, &wl);
  l = sqrt(MU * EPS0) * 138 * log10(2 * h / ri);
  c = MU * EPS0 * eps / l;
  g = 0.0;

  zo = Trc_CalcCharImp(r, c, l, g, freq, wl);

  return (zo);
}

/*
 * The function CalcPropConstBalShielded calculates the propagation constant
 * of the network in terms of the balance shielded transmission line parameters.
 */
complex Trc_CalcPropConstBalShielded(float ri, float rs, float eps, float h,
                                     float freq)
//   float	ri,		/* Coax inner cond. radius */
//           rs,		/* Coax outer cond. inside radius */
//           eps,		/* dielectric constant */
//           h,		/* distance between two inner cond. */
//           freq;		/* frequency */
{

  double v, w, x, y, z;
  float wl, r, l, c, g, delta;
  double mag, angle;
  complex gamma;
  double alpha, beta;
  float rShield;
  float zchar;

  Trc_CirCond(freq, ri, &delta, &r, &wl);
  v = 0.5 * h / ri;
  x = 0.5 * h / rs;
  x = x * x;
  zchar = 120 * log(2 * v * ((1 - x) / (1 + x)));
  l = sqrt(MU * EPS0) * zchar;
  c = (MU * EPS0 * eps) / l;
  g = 0.0;
  r = 2 * r;
  wl = 2 * wl;
  Trc_Propconst(freq, r, l, c, g, wl, &alpha, &beta);
  gamma.re = alpha;
  gamma.im = beta;
  return (gamma);
}

/*
 * The function CalcCharImpCoax calculates the characteristic impedance
 * of the network in terms of the balance shielded transmission line parameters.
 */
complex Trc_CalcCharImpBalShielded(float ri, float rs, float eps, float h,
                                   float freq)
//  float	ri,		/* Coax inner cond. radius */
//          rs,		/* Coax outer cond. inside radius */
//          eps,		/* dielectric constant */
//          h,		/* distance between two inner cond. */
//          freq;		/* frequency */
{
  double v, w, x, y, z;
  float delta, omega;
  double mag, angle;
  float r, l, c, g, wl;
  complex zo;
  float zchar;

  Trc_CirCond(freq, ri, &delta, &r, &wl);
  v = 0.5 * h / ri;
  x = 0.5 * h / rs;
  x = x * x;
  zchar = 120 * log(2 * v * ((1 - x) / (1 + x)));
  l = sqrt(MU * EPS0) * zchar;
  c = (MU * EPS0 * eps) / l;
  g = 0.0;
  r = 2 * r;
  wl = 2 * wl;
  zo = Trc_CalcCharImp(r, c, l, g, freq, wl);

  return (zo);
}

/*
 * The function CalcPropConstWireabg calculates the propagation constant
 * of the network in terms of single wire  above ground plane
 * transmission line  parameters.
 */
complex Trc_CalcPropConstMicrostrip(float w, float h, float er, float t,
                                    float freq)
// float	w,		/* microstrip width */
//         h,		/* microstrip height */
//         er,		/* dielectric constant */
//          t,          /* microstrip thickness  */
//         freq;		/* frequency */
{
  float v, x, y, z;
  float wl, r, l, c, g, delta;
  double mag, angle;
  complex gamma;
  double alpha, beta;
  double erE;
  double k0;

#define MU 12.566371e-7
#define EPS0 8.854e-12

  k0 = 2 * PI * freq * sqrt(MU * EPS0);

  erE = (er + 1) / 2 + (er - 1) / (2 * sqrt(1 + 12 * h / w));

  beta = sqrt(erE) * k0;
  alpha = 0; // for now

  gamma.re = alpha;
  gamma.im = beta;
  return (gamma);
}

/*
 * The function CalcCharImpWireabg calculates the  characteristic impedance
 * of the network in terms of single wire  above ground plane
 * transmission line  parameters.
 */
complex Trc_CalcCharImpMicrostrip(float w, float h, float er, float t,
                                  float freq)
// float	w,		/* microstrip width */
//         h,		/* microstrip height */
//         er,		/* dielectric constant */
//          t,          /* microstrip thickness  */
//         freq;		/* frequency */
{
  float v, x, y, z;
  double delta, omega;
  double mag, angle;
  float r, l, c, g, wl;
  complex z0;
  double ratio;
  double ereSqrt;
  double z0r;
  double ere;

  ere = (er + 1) / 2 + (er - 1) / (2 * sqrt(1 + 12 * h / w));
  ereSqrt = sqrt(ere);
  ratio = w / h;

  if (ratio < 1) {

    z0r = 60 / ereSqrt * log(8 / ratio + ratio / 4.0);

  } else {

    z0r = 120 * PI / (ereSqrt * (ratio + 1.393 + 0.667 * log(ratio + 1.444)));
  }

  z0.re = z0r;
  z0.im = 0.0;

  return (z0);
}
