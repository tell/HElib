# -*- coding: dos -*-
# Copyright (C) 2012,2013 IBM Corp.
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
# 

INCLUDEDIRS = -I../extlibs/include

WARNOPTS = -Wall -Wextra -Wformat=2 -Wstrict-aliasing=2 -Wpointer-arith -Wbad-function-cast -Wcast-qual -Wcast-align -Wconversion -Wfloat-equal -Wredundant-decls -Wshadow -pedantic -fcolor-diagnostics


CFLAGS = -g -std=c99 $(WARNOPTS) $(INCLUDEDIRS)
CXXFLAGS = -g -std=c++11 $(WARNOPTS) $(INCLUDEDIRS)

# LD = $(CXX) -v
LDFLAGS = -L../extlibs/lib

AR = ar
ARFLAGS=ruv

GMP=-lgmp
LDLIBS = -lntl $(GMP) -lm


HEADER = EncryptedArray.h FHE.h Ctxt.h CModulus.h FHEContext.h PAlgebra.h SingleCRT.h DoubleCRT.h NumbTh.h bluestein.h IndexSet.h timing.h IndexMap.h replicate.h hypercube.h matching.h powerful.h permutations.h

SRC = KeySwitching.cpp EncryptedArray.cpp FHE.cpp Ctxt.cpp CModulus.cpp FHEContext.cpp PAlgebra.cpp SingleCRT.cpp DoubleCRT.cpp NumbTh.cpp PAlgebraMod.cpp bluestein.cpp IndexSet.cpp timing.cpp replicate.cpp hypercube.cpp matching.cpp powerful.cpp BenesNetwork.cpp permutations.cpp PermNetwork.cpp OptimizePermutations.cpp eqtesting.cpp

#OBJ = EncryptedArray.o FHE.o Ctxt.o CModulus.o FHEContext.o PAlgebra.o SingleCRT.o DoubleCRT.o NumbTh.o bluestein.o IndexSet.o timing.o KeySwitching.o PAlgebraMod.o
OBJ = NumbTh.o timing.o bluestein.o PAlgebra.o  CModulus.o FHEContext.o IndexSet.o DoubleCRT.o SingleCRT.o FHE.o KeySwitching.o Ctxt.o EncryptedArray.o replicate.o hypercube.o matching.o powerful.o BenesNetwork.o permutations.o PermNetwork.o OptimizePermutations.o eqtesting.o

#TESTPROGS = Test_PAlgebra_x Test_DoubleCRT_x Test_CModulus_x Test_FHE_x Test_Arrays_x
TESTPROGS = Test_General_x Test_Replicate_x Test_LinPoly_x Test_matmul_x Test_Powerful_x Test_Permutations_x


all: fhe.a

check: Test_General_x
	./Test_General_x R=1 m=255
	./Test_General_x R=1 k=10 p=2 r=1 d=1
	./Test_General_x R=1 k=10 p=2 r=2 d=1
	./Test_General_x R=1 k=10 p=2 r=1 d=2
	./Test_General_x R=1 k=10 p=3 r=1 d=1
	./Test_General_x R=2 k=10 p=3 r=2 d=1 c=3
	./Test_General_x R=1 k=10 p=3 r=1 d=2 c=3

test: $(TESTPROGS)

%.o: %.cpp
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

obj: $(OBJ)

fhe.a: $(OBJ)
	$(AR) $(ARFLAGS) fhe.a $(OBJ)

./%_x: %.cpp fhe.a
	$(LINK.cpp) $^ $(LOADLIBES) $(LDLIBS) -o $@

clean:
	rm -f *.o *_x *_x.exe *.a core.*
	rm -rf *.dSYM
# DO NOT DELETE

AltCRT.o: AltCRT.h NumbTh.h IndexMap.h IndexSet.h cloned_ptr.h FHEContext.h
AltCRT.o: PAlgebra.h CModulus.h bluestein.h DoubleCRT.h timing.h
CModulus.o: NumbTh.h CModulus.h PAlgebra.h cloned_ptr.h bluestein.h timing.h
Ctxt.o: FHEContext.h PAlgebra.h cloned_ptr.h CModulus.h bluestein.h
Ctxt.o: IndexSet.h Ctxt.h DoubleCRT.h NumbTh.h IndexMap.h FHE.h timing.h
DoubleCRT.o: NumbTh.h PAlgebra.h cloned_ptr.h CModulus.h bluestein.h
DoubleCRT.o: DoubleCRT.h IndexMap.h IndexSet.h FHEContext.h SingleCRT.h
DoubleCRT.o: timing.h
EncryptedArray.o: EncryptedArray.h FHE.h DoubleCRT.h NumbTh.h IndexMap.h
EncryptedArray.o: IndexSet.h cloned_ptr.h FHEContext.h PAlgebra.h CModulus.h
EncryptedArray.o: bluestein.h Ctxt.h timing.h
FHE.o: DoubleCRT.h NumbTh.h IndexMap.h IndexSet.h cloned_ptr.h FHEContext.h
FHE.o: PAlgebra.h CModulus.h bluestein.h FHE.h Ctxt.h timing.h
FHEContext.o: NumbTh.h FHEContext.h PAlgebra.h cloned_ptr.h CModulus.h
FHEContext.o: bluestein.h IndexSet.h
IndexSet.o: IndexSet.h
KeySwitching.o: FHE.h DoubleCRT.h NumbTh.h IndexMap.h IndexSet.h cloned_ptr.h
KeySwitching.o: FHEContext.h PAlgebra.h CModulus.h bluestein.h Ctxt.h
NumbTh.o: NumbTh.h
PAlgebra.o: NumbTh.h PAlgebra.h cloned_ptr.h
PAlgebraMod.o: NumbTh.h PAlgebra.h cloned_ptr.h
SingleCRT.o: NumbTh.h SingleCRT.h FHEContext.h PAlgebra.h cloned_ptr.h
SingleCRT.o: CModulus.h bluestein.h IndexSet.h IndexMap.h DoubleCRT.h
Test_General.o: FHE.h DoubleCRT.h NumbTh.h IndexMap.h IndexSet.h cloned_ptr.h
Test_General.o: FHEContext.h PAlgebra.h CModulus.h bluestein.h Ctxt.h
Test_General.o: timing.h EncryptedArray.h
Test_IO.o: FHE.h DoubleCRT.h NumbTh.h IndexMap.h IndexSet.h cloned_ptr.h
Test_IO.o: FHEContext.h PAlgebra.h CModulus.h bluestein.h Ctxt.h timing.h
Test_IO.o: EncryptedArray.h
Test_LinPoly.o: NumbTh.h
Test_PAlgebra.o: PAlgebra.h cloned_ptr.h NumbTh.h FHEContext.h CModulus.h
Test_PAlgebra.o: bluestein.h IndexSet.h
Test_Replicate.o: FHE.h DoubleCRT.h NumbTh.h IndexMap.h IndexSet.h
Test_Replicate.o: cloned_ptr.h FHEContext.h PAlgebra.h CModulus.h bluestein.h
Test_Replicate.o: Ctxt.h replicate.h EncryptedArray.h timing.h
Test_matmul.o: FHE.h DoubleCRT.h NumbTh.h IndexMap.h IndexSet.h cloned_ptr.h
Test_matmul.o: FHEContext.h PAlgebra.h CModulus.h bluestein.h Ctxt.h timing.h
Test_matmul.o: EncryptedArray.h
bluestein.o: bluestein.h timing.h
old-Test_FHE.o: FHE.h DoubleCRT.h NumbTh.h IndexMap.h IndexSet.h cloned_ptr.h
old-Test_FHE.o: FHEContext.h PAlgebra.h CModulus.h bluestein.h Ctxt.h
old-Test_FHE.o: timing.h
old2-Test_FHE.o: FHE.h DoubleCRT.h NumbTh.h IndexMap.h IndexSet.h
old2-Test_FHE.o: cloned_ptr.h FHEContext.h PAlgebra.h CModulus.h bluestein.h
old2-Test_FHE.o: Ctxt.h timing.h
powerful.o: NumbTh.h
replicate.o: replicate.h FHE.h DoubleCRT.h NumbTh.h IndexMap.h IndexSet.h
replicate.o: cloned_ptr.h FHEContext.h PAlgebra.h CModulus.h bluestein.h
replicate.o: Ctxt.h EncryptedArray.h timing.h
timing.o: timing.h
