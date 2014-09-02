#########
CPP 	 := icpc
CFLAGS 	 := -O3 -prec-div -no-ftz -restrict -Wshadow -MMD -MP
CFLAGS   := $(CFLAGS) -fno-inline-functions
FFTWINC  := -I $$FFTW_INC
MKLINC := -mkl

#########
MKLLIBS := -mkl=sequential
MKLTHRD := -mkl=parallel
FFTWLIB  :=  -L $$FFTW_LINK -lfftw3

#########
MPI := OMPI
$(info MPI set to $(MPI))
ifeq ($(MPI), MVAPICH)
	CPP := mpicxx
else ifeq ($(MPI), OMPI) #CPP := mpiCC
	CFLAGS := $(CFLAGS) `mpiCC -showme:compile` 
	LIBS := $(LIBS) `mpiCC -showme:link`
else ifeq ($(MPI), DV)
	CFLAGS := $(CFLAGS) `$$HOME/openmpi-1.6.3/bin/mpiCC -showme:compile` 
	LIBS := $(LIBS) `$$HOME/openmpi-1.6.3/bin/mpiCC -showme:link`
endif
$(info mpicxx = $(shell which mpicxx))

#########
.SUFFIXES:
.SUFFIXES: .cpp .o .exe .s .d
%.o: %.cpp
	$(CPP) $(EXTRNL) $(CFLAGS)  -c $<
%.s: %.cpp 
	$(CPP) $(EXTRNL) $(CFLAGS) -fno-verbose-asm  -S $< 
%.o: %.s 
	$(CPP) $(EXTRNL) $(CFLAGS) -c $< 
%.exe: %.o 
	$(CPP) $(EXTRNL) $(LFLAGS) -o $@ $(filter %.o,$^) $(LIBS) 

########
.PHONY: clean cleanx cleanxx
clean:
	rm *.o; rm *.exe; rm a.out;

cleanx:
	rm pbs*.*; 

cleanxx:
	rm *.o; rm *.exe; rm pbs*.*; rm *.d; rm DBG/outP*; rm slurm*.out;