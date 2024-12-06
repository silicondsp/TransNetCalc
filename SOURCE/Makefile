#
# TranTopCalc library makefile
# Sasan Ardalan 
#
CFLAGS = -g    
CC=  cc

all: libtrc.a

OBJECTS = trc_traverse.o trc_netanal.o trc_netcalc.o trc_subs.o \
		trc_length.o trc_getnode.o trc_netimp.o  trc_hybrid.o \
		trc_getlumped.o trc_trantypes.o  trc_graph.o \
		cx_complex.o trc_param.o trc_bessel.o    

trc_traverse.o : trc_traverse.c trantopcalc.h
	$(CC) -c $(CFLAGS)  trc_traverse.c

trc_netcalc.o : trc_netcalc.c trantopcalc.h
	$(CC) -c $(CFLAGS)  trc_netcalc.c

trc_netanal.o : trc_netanal.c trantopcalc.h
	$(CC) -c $(CFLAGS)  trc_netanal.c

trc_subs.o : trc_subs.c trantopcalc.h
	$(CC) -c $(CFLAGS)  trc_subs.c

trc_getnode.o : trc_getnode.c trantopcalc.h
	$(CC) -c $(CFLAGS)  trc_getnode.c

trc_netimp.o : trc_netimp.c trantopcalc.h kiss_fft.h
	$(CC) -c $(CFLAGS)  trc_netimp.c

trc_length.o : trc_length.c trantopcalc.h
	$(CC) -c $(CFLAGS)  trc_length.c

trc_hybrid.o : trc_hybrid.c 
	$(CC) -c $(CFLAGS)  trc_hybrid.c

trc_getlumped.o : trc_getlumped.c trantopcalc.h 
	$(CC) -c $(CFLAGS)  trc_getlumped.c

cx_complex.o : cx_complex.c cx.h
	$(CC) -c $(CFLAGS)  cx_complex.c

trc_param.o : trc_param.c  tr.h
	$(CC) -c $(CFLAGS)  trc_param.c

trc_bessel.o : trc_bessel.c  tr.h
	$(CC) -c $(CFLAGS)  trc_bessel.c

trc_interimp.o : trc_interimp.c trantopcalc.h
	$(CC) -c $(CFLAGS)  trc_interimp.c

trc_trantypes.o : trc_trantypes.c trantopcalc.h
	$(CC) -c $(CFLAGS)  trc_trantypes.c

trc_draw.o : trc_draw.c trantopcalc.h
	$(CC) -c $(CFLAGS)  trc_draw.c


trc_graph.o : trc_graph.c trantopcalc.h
	$(CC) -c $(CFLAGS)  trc_graph.c

kiss_fft.o : kiss_fft.c kiss_fft.h
	$(CC) -c $(CFLAGS)  kiss_fft.c






libtrc.a :   $(OBJECTS) 
	ar rv libtrc.a   $(OBJECTS) 
	


trc : trc_main.o trc_interimp.o   libtrc.a kiss_fft.o
	$(CC) -o trc   trc_main.o  trc_interimp.o libtrc.a kiss_fft.o   -lm


