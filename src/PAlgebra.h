/* Copyright (C) 2012,2013 IBM Corp.
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#ifndef _PAlgebra_H_
#define _PAlgebra_H_
/**
 * @file PAlgebra.h
 * @brief Declatations of the classes PAlgebra
 *
 * @class PAlgebra
 * @brief The structure of (Z/mZ)* /(p)
 *
 * A PAlgebra object is determined by an integer m and a prime p, where p does
 * not divide m. It holds information descrtibing the structure of (Z/mZ)^*,
 * which is isomorphic to the Galois group over A = Z[X]/Phi_m(X)). 
 *
 * We represent (Z/mZ)^* as (Z/mZ)^* = (p) x (g1,g2,...) x (h1,h2,...)
 * where the group generated by g1,g2,... consists of the elements that
 * have the same order in (Z/mZ)^* as in (Z/mZ)^* /(p,g_1,...,g_{i-1}), and
 * h1,h2,... generate the remaining quotient group (Z/mZ)^* /(p,g1,g2,...). 
 *
 * We let T subset (Z/mZ)^* be a set of representatives for the quotient
 * group (Z/mZ)^* /(p), defined as T={ prod_i gi^{ei} * prod_j hj^{ej} }
 * where the ei's range over 0,1,...,ord(gi)-1 and the ej's range over
 * 0,1,...ord(hj)-1 (these last orders are in (Z/mZ)^* /(p,g1,g2,...)).
 *
 * Phi_m(X) is factored as Phi_m(X)= prod_{t in T} F_t(X) mod p,
 * where the F_t's are irreducible modulo p. An arbitrary factor
 * is chosen as F_1, then for each t in T we associate with the index t the
 * factor F_t(X) = GCD(F_1(X^t), Phi_m(X)).
 *
 * Note that fixing a representation of the field R=(Z/pZ)[X]/F_1(X) 
 * and letting z be a root of F_1 in R (which
 * is a primitive m-th root of unity in R), we get that F_t is the minimal
 * polynomial of z^{1/t}.
 */

#include <vector>
#include <NTL/ZZX.h>
#include <NTL/GF2X.h>
#include <NTL/vec_GF2.h>
#include <NTL/GF2EX.h>
#include <NTL/lzz_pEX.h>
#include "cloned_ptr.h"
NTL_CLIENT

class PAlgebra {
  unsigned long m;   // the integer m defines (Z/mZ)^*, Phi_m(X), etc.
  unsigned long p;   // the prime base of the plaintext space

  unsigned long phiM; // phi(m)
  unsigned long ordP; // the order of p in (Z/mZ)^*
  unsigned long nSlots; // phi(m)/ordP = # of plaintext slots

  vector<unsigned long> gens; // Our generators for (Z/mZ)^* (other than p)
  vector<long> ords; // ords[i] is the order of gens[i] in quotient group kept
                     // with a negative sign if different than order in (Z/mZ)*


  vector<long> prods; // \prods[i] = \prod_{j=i}^{gens.size()-1} |ords[i]|

  ZZX PhimX;   // Holds the integer polynomial Phi_m(X)
  double cM;   // the ring constant c_m for Z[X]/Phi_m(X)

  vector<unsigned long> T; // The representatives for the quotient group (Z/mZ)^*/(p)
  vector<long> Tidx;  // i=Tidx[t] is the index i s.t. T[i]=t. 
                      // Tidx[t]==-1 if t notin T

  vector<long> zmsIdx; // if t is the i'th element in (Z/mZ)* then zmsIdx[t]=i
                       // zmsIdx[t]==-1 if t notin (Z/mZ)*

  vector<int> dLogT; 
  // holds the discrete-logarithms for elements in T: If (z/mZ)^*/(p)
  // has n generators then dLogT is an array of n*nSlots interest, where
  // the entries [in, in+1,...,(i+1)n-1] hold the discrete-logarithms for
  // the i'th element of (z/mZ)^*/(p). 
  // Namely, for i<nSlots we have dLogT[in,...,(i+1)n-1] = [e1,...,en]
  // s.t. T[i] = prod_{i=1}^n gi^{ei} mod m (with n=gens.size())

 public:

  PAlgebra(unsigned long mm, unsigned long pp = 2);  // constructor

  bool operator==(const PAlgebra& other) const;
  bool operator!=(const PAlgebra& other) const {return !(*this==other);}
  // comparison

  /* I/O methods */

  //! Prints the structure in a readable form
  void printout() const;

  /* Access methods */

  //! Returns m
  unsigned long getM() const { return m; }

  //! Returns p
  unsigned long getP() const { return p; }

  //! Returns phi(m)
  unsigned long getPhiM() const { return phiM; }

  //! The order of p in (Z/mZ)^*
  unsigned long getOrdP() const { return ordP; }

  //! The number of plaintext slots = phi(m) = ord(p)
  unsigned long getNSlots() const { return nSlots; }

  //! The cyclotomix polynomial Phi_m(X)
  const ZZX& getPhimX() const { return PhimX; }

  //! The number of generators in (Z/mZ)^* /(p)
  unsigned long numOfGens() const { return gens.size(); }

  //! the i'th generator in (Z/mZ)^* /(p) (if any)
  unsigned long ZmStarGen(unsigned long i) const
  {  return (i<gens.size())? gens[i] : 0; }

  //! The order of i'th generator (if any)
  unsigned long OrderOf(unsigned long i) const
  {  return (i<ords.size())? abs(ords[i]) : 0; }

  //! Is ord(i'th generator) the same as its order in (Z/mZ)^*? 
  bool SameOrd(unsigned long i) const
  {  return (i<ords.size())? (ords[i]>0) : false; }

  //! @name Translation between index, represnetatives, and exponents

  //! Returns the i'th element in T
  unsigned long ith_rep(unsigned long i) const
  {  return (i<nSlots)? T[i]: 0; }

  //! Returns the index of t in T
  long indexOfRep(unsigned long t) const
  {  return (t>0 && t<m)? Tidx[t]: -1; }

  //! Is t in T?
  bool isRep(unsigned long t) const
  {  return (t>0 && t<m && Tidx[t]>-1); }

  //! Returns the index of t in (Z/mZ)*
  long indexInZmstar(unsigned long t) const
  {  return (t>0 && t<m)? zmsIdx[t]: -1; }

  //! Is t in [0,m-1] with (t,m)=1?
  bool inZmStar(unsigned long t) const
  {  return (t>0 && t<m && zmsIdx[t]>-1); }

  //! @brief Returns ith coordinate of index k along the i'th dimension.
  //! See Section 2.4 in the design document.
  long coordinate(long i, long k) const;
 
  //! @brief adds offset to index k in the i'th dimension
  long addCoord(long i, long k, long offset) const;

  //! @brief Returns prod_i gi^{exps[i]} mod m. If onlySameOrd=true,
  //! use only generators that have the same order as in (Z/mZ)^*.
  unsigned long exponentiate(const vector<unsigned long>& exps, 
			      bool onlySameOrd=false) const;

  //! Inverse of exponentiate
  const int* dLog(unsigned long t) const {
    long i = indexOfRep(t);
    if (i<0) return NULL;
    return &(dLogT[i*gens.size()]); // bug: this should be an iterator
  }

  /* Miscellaneous */

  //! The order of the quoteint group (Z/mZ)^* /(p) (if flag=false), or the
  //! subgroup of elements with the same order as in (Z/mZ)^* (if flag=true)
  unsigned long qGrpOrd(bool onlySameOrd=false) const { 
    if (gens.size()<=0) return 1;
    unsigned long ord = 1;
    for (unsigned long i=0; i<ords.size(); i++)
      if (!onlySameOrd || SameOrd(i)) ord *= abs(ords[i]);
    return ord;
  }

  //! exps is an array of exponents (the dLog of some t in T), this function
  //! incerement exps lexicographic order, reutrn false if it cannot be
  //! incremented (because it is at its maximum value)
  bool nextExpVector(vector<unsigned long>& exps) const;
};


enum PA_tag { PA_GF2_tag, PA_zz_p_tag };

/**
@class: PAlgebraMod 
@brief The structure of Z[X]/(Phi_m(X), p)

An object of type PAlgebraMod stores information about a PAlgebra object
zMStar, and an integer r. It also provides support for encoding and decoding
plaintext slots.

the PAlgebra object zMStar defines (Z/mZ)^* /(0), and the PAlgebraMod object
stores various tables related to the polynomial ring Z/(p^r)[X].  To do this
most efficiently, if p == 2 and r == 1, then these polynomials are represented
as GF2X's, and otherwise as zz_pX's. Thus, the types of these objects are not
determined until run time. As such, we need to use a class heirarchy, as
follows.

\li PAlgebraModBase is a virtual class

\li PAlegbraModDerived<type> is a derived template class, where
  type is either PA_GF2 or PA_zz_p.

\li The class PAlgebraMod is a simple wrapper around a smart pointer to a
  PAlgebraModBase object: copying a PAlgebra object results is a "deep copy" of
  the underlying object of the derived class. It provides dDirect access to the
  virtual methods of PAlgebraModBase, along with a "downcast" operator to get a
  reference to the object as a derived type, and also == and != operators.
**/

//! \cond FALSE (make doxygen ignore these classes)
class DummyBak {
// placeholder class used in GF2X impl

public:
  void save() {}
  void restore() const {}
};

class DummyContext {
// placeholder class used in GF2X impl

public:
  void save() {}
  void restore() const {}
  DummyContext() {}
  DummyContext(long) {}
};

class PA_GF2 {
// typedefs for algebraic structires built up from GF2

public:
  static const PA_tag tag = PA_GF2_tag;
  typedef GF2X RX;
  typedef vec_GF2X vec_RX;
  typedef GF2XModulus RXModulus;
  typedef DummyBak RBak;
  typedef DummyContext RContext;
  typedef GF2E RE;
  typedef vec_GF2E vec_RE;
  typedef GF2EX REX;
  typedef GF2EBak REBak;
  typedef vec_GF2EX vec_REX;
  typedef GF2EContext REContext;
};


class PA_zz_p {
// typedefs for algebraic structires built up from zz_p

public:
  static const PA_tag tag = PA_zz_p_tag;
  typedef zz_pX RX;
  typedef vec_zz_pX vec_RX;
  typedef zz_pXModulus RXModulus;
  typedef zz_pBak RBak;
  typedef zz_pContext RContext;
  typedef zz_pE RE;
  typedef vec_zz_pE vec_RE;
  typedef zz_pEX REX;
  typedef zz_pEBak REBak;
  typedef vec_zz_pEX vec_REX;
  typedef zz_pEContext REContext;
};
//! \endcond


//! Virtual base class for PAlgebraMod
class PAlgebraModBase {

public:

  virtual ~PAlgebraModBase() {}
  virtual PAlgebraModBase* clone() const = 0;

  //! Returns the type tag: PA_GF2_tag or PA_zz_p_tag
  virtual PA_tag getTag() const = 0;

  //! Returns reference to underlying PAlgebra object
  virtual const PAlgebra& getZMStar() const = 0;

  //! Returns reference to the factorization of Phi_m(X) mod p^r, but as ZZX's
  virtual const vector<ZZX>& getFactorsOverZZ() const = 0;

  //! The value r
  virtual long getR() const  = 0;

  //! The value p^r
  virtual long getPPowR() const = 0;

  //! Restores the NTL context for p^r
  virtual void restoreContext() const = 0;

  /**
     @brief Generates the "mask table" that is used to support rotations
     
     maskTable[i][j] is a polynomial representation of a mask that is 1 in
     all slots whose i'th coordinate is at least j, and 0 elsewhere. We have:
     \verbatim
       maskTable.size() == zMStar.numOfGens()     // # of generators
       for i = 0..maskTable.size()-1:
         maskTable[i].size() == 1 + zMStar.OrderOf(i) // 1 + order of gen i
     \endverbatim
  **/
  virtual void genMaskTable() const = 0;

  /**

    generates CRT table to speed up mask encoding

  **/
  virtual void genCrtTable() const = 0;
};

#ifndef DOXYGEN_IGNORE
#define PA_INJECT(typ)\
  static const PA_tag tag = typ::tag;  \
  typedef typename typ::RX RX;  \
  typedef typename typ::vec_RX vec_RX;  \
  typedef typename typ::RXModulus RXModulus;  \
  typedef typename typ::RBak RBak;  \
  typedef typename typ::RContext RContext;  \
  typedef typename typ::RE RE;  \
  typedef typename typ::vec_RE vec_RE;  \
  typedef typename typ::REX REX;  \
  typedef typename typ::REBak REBak;  \
  typedef typename typ::vec_REX vec_REX;  \
  typedef typename typ::REContext REContext;  \

#endif

template<class type> class PAlgebraModDerived;
// forward declaration

//! Auxilliary structure to support encoding/decoding slots.
template<class type> class MappingData {

public:
  PA_INJECT(type)

  friend class PAlgebraModDerived<type>;

private:
  RX G; // the polynomial defining the field extension
  long degG; // the degree of the polynomial

  /* the remaining fields are visible only to PAlgebraModDerived */

  vector<RX> maps;
  REContext contextForG;
  vector<REX> rmaps;

public:
  const RX& getG() const { return G; }
  long getDegG() const { return degG; } 
};

//! A concrete instantiation of the virtual class
template<class type> class PAlgebraModDerived : public PAlgebraModBase {
public:
  PA_INJECT(type)

private:
  const PAlgebra& zMStar;
  long r;
  long pPowR;
  RContext pPowRContext;

  RXModulus PhimXMod;

  vec_RX factors;
  vector<ZZX> factorsOverZZ;
  vec_RX crtCoeffs;
  vector< vector< RX > > maskTable;
  vector<RX> crtTable;


public:

  PAlgebraModDerived(const PAlgebra& zMStar, long r);

  PAlgebraModDerived(const PAlgebraModDerived& other) // copy constructor
  : zMStar(other.zMStar), r(other.r), pPowR(other.pPowR), 
    pPowRContext(other.pPowRContext)
  {
    RBak bak; bak.save(); restoreContext();
    PhimXMod = other.PhimXMod;
    factors = other.factors;
    maskTable = other.maskTable;
    crtTable = other.crtTable;
  }

  PAlgebraModDerived& operator=(const PAlgebraModDerived& other) // assignment
  {
    if (this == &other) return *this;

    assert(&zMStar == &other.zMStar);
    r = other.r;
    pPowR = other.pPowR;
    pPowRContext = other.pPowRContext;

    RBak bak; bak.save(); restoreContext();
    PhimXMod = other.PhimXMod;
    factors = other.factors;
    maskTable = other.maskTable;
    crtTable = other.crtTable;

    return *this;
  }

  //! Returns a pointer to a "clone"
  virtual PAlgebraModBase* clone() const { return new PAlgebraModDerived(*this); }

  //! Returns the type tag: PA_GF2_tag or PA_zz_p_tag
  virtual PA_tag getTag() const { return tag; }

  //! Returns reference to underlying PAlgebra object
  virtual const PAlgebra& getZMStar() const { return zMStar; }

  //! Returns reference to the factorization of Phi_m(X) mod p^r, but as ZZX's
  virtual const vector<ZZX>& getFactorsOverZZ() const { return factorsOverZZ; }

  //! The value r
  virtual long getR() const { return r; }

  //! The value p^r
  virtual long getPPowR() const { return pPowR; }

  //! Restores the NTL context for p^r
  virtual void restoreContext() const { pPowRContext.restore(); }

  /**
     @brief Generates the "mask table" that is used to support rotations
     
     maskTable[i][j] is a polynomial representation of a mask that is 1 in
     all slots whose i'th coordinate is at least j, and 0 elsewhere. We have:
     \verbatim
       maskTable.size() == zMStar.numOfGens()     // # of generators
       for i = 0..maskTable.size()-1:
         maskTable[i].size() == zMStar.OrderOf(i) // order of generator i
     \endverbatim
  **/
  virtual void genMaskTable() const; // logically, but not really, const

  /**

    generates CRT table to speed up mask encoding

  **/

  virtual void genCrtTable() const; // logically, but not really, const

  /* In all of the following functions, it is expected that the caller 
     has already restored the relevant modulus (p^r), which
     can be done by invoking the method restoreContext()
   */


  //! Returns reference to an RXModulus representing Phi_m(X) (mod p^r)
  const RXModulus& getPhimXMod() const { return PhimXMod; }

  //! Returns reference to the factors of Phim_m(X) modulo p^r
  const vec_RX& getFactors() const { return factors; }

  //! @brief Returns the CRT coefficients: 
  //! element i contains (prod_{j!=i} F_j)^{-1} mod F_i,
  //! where F_0 F_1 ... is the factorization of Phi_m(X) mod p^r
  const vec_RX& getCrtCoeffs() const { return crtCoeffs; }


  /**
     @brief Returns ref to maskTable, which is used to implement rotations
     (in the EncryptedArray module).

     maskTable[i][j] is a polynomial representation of a mask that is 1 in
     all slots whose i'th coordinate is at least j, and 0 elsewhere. We have:
     \verbatim
       maskTable.size() == zMStar.numOfGens()     // # of generators
       for i = 0..maskTable.size()-1:
         maskTable[i].size() == zMStar.OrderOf(i) // order of generator i
     \endverbatim
  **/
  const vector< vector< RX > >& getMaskTable() const // logically, but not really, const
  {
    if (maskTable.size() == 0) 
      genMaskTable();
    return maskTable;
  }

  /** 

    returns ref to CRT table
 
  **/

  const vector< RX >& getCrtTable() const // logically, but not really, const
  {
    if (crtTable.size() == 0) 
      genCrtTable();
    return crtTable;
  }

  ///@{
  //! @name Embedding in the plaintext slots and decoding back
  //! In all the functions below, G must be irredicible mod p, 
  //! and the order of G must divide the order of p modulo m
  //! (as returned by zMStar.getOrdP()).
  //! In addition, when r > 1, G must be the monomial X (RX(1, 1))

  //! @brief Returns a vector crt[] such that crt[i] = H mod Ft (with t = T[i])
  void CRT_decompose(vector<RX>& crt, const RX& H) const;

  //! @brief Returns H in R[X]/Phi_m(X) s.t. for every i<nSlots and t=T[i],
  //! we have H == crt[i] (mod Ft)
  void CRT_reconstruct(RX& H, vector<RX>& crt) const;

  //! @brief Compute the maps for all the slots.
  //! In the current implementation, we if r > 1, then
  //! we must have either deg(G) == 1 or G == factors[0]
  void mapToSlots(MappingData<type>& mappingData, const RX& G) const;

  //! @brief Returns H in R[X]/Phi_m(X) s.t. for every t in T, the element
  //! Ht = (H mod Ft) in R[X]/Ft(X) represents the same element as alpha
  //! in R[X]/G(X).
  //!
  //! Must have deg(alpha)<deg(G). The mappingData argument should contain
  //! the output of mapToSlots(G).
  void embedInAllSlots(RX& H, const RX& alpha, 
                       const MappingData<type>& mappingData) const;

  //! @brief Returns H in R[X]/Phi_m(X) s.t. for every t in T, the element
  //! Ht = (H mod Ft) in R[X]/Ft(X) represents the same element as alphas[i]
  //! in R[X]/G(X).
  //!
  //! Must have deg(alpha[i])<deg(G). The mappingData argument should contain
  //! the output of mapToSlots(G).
  void embedInSlots(RX& H, const vector<RX>& alphas, 
                    const MappingData<type>& mappingData) const;

  //! @brief Return an array such that alphas[i] in R[X]/G(X) represent the
  //! same element as rt = (H mod Ft) in R[X]/Ft(X) where t=T[i].
  //!
  //! The mappingData argument should contain the output of mapToSlots(G).
  void decodePlaintext(vector<RX>& alphas, const RX& ptxt,
		       const MappingData<type>& mappingData) const;

  //! @brief Returns a coefficient vector C for the linearized polynomial
  //! representing M.
  //!
  //! For h in Z/(p^r)[X] of degree < d,
  //! \f[ M(h(X) mod G) = sum_{i=0}^{d-1} (C[j] mod G) * (h(X^{p^j}) mod G).\f]
  //! G is assumed to be defined in mappingData, with d = deg(G).
  //! L describes a linear map M by describing its action on the standard
  //! power basis: M(x^j mod G) = (L[j] mod G), for j = 0..d-1.  
  void buildLinPolyCoeffs(vector<RX>& C, const vector<RX>& L,
                          const MappingData<type>& mappingData) const;
  ///@}
private:
  /* internal functions, not for public consumption */

  static void SetModulus(long p) {
    RContext context(p);
    context.restore();
  }

  //! w in R[X]/F1(X) represents the same as X in R[X]/G(X)
  void mapToF1(RX& w, const RX& G) const { mapToFt(w,G,1); }

  //! Same as above, but embeds relative to Ft rather than F1. The
  //! optional rF1 contains the output of mapToF1, to speed this operation.
  void mapToFt(RX& w, const RX& G, unsigned long t, const RX* rF1=NULL) const;
};

//! Builds a table, of type PA_GF2 if p == 2 and r == 1, and PA_zz_p otherwise
PAlgebraModBase *buildPAlgebraMod(const PAlgebra& zMStar, long r);

// A simple wrapper for a pointer to an object of type PAlgebraModBase.
//
// Direct access to the virtual methods of PAlgebraModBase is provided,
// along with a "downcast" operator to get a reference to the object
// as a derived type, and == and != operators.
class PAlgebraMod {

private:
  cloned_ptr<PAlgebraModBase> rep;

public:
  // copy constructor: default
  // assignment: default
  // destructor: default
  // NOTE: the use of cloned_ptr ensures that the default copy constructor,
  // assignment operator, and destructor will work correctly.

  explicit
  PAlgebraMod(const PAlgebra& zMStar, long r) 
  : rep( buildPAlgebraMod(zMStar, r) )
  { }
  // constructor

  //! Downcast operator
  //! example: const PAlgebraModDerived<PA_GF2>& rep = alMod.getDerived(PA_GF2());
  template<class type> 
  const PAlgebraModDerived<type>& getDerived(type) const
  { return dynamic_cast< const PAlgebraModDerived<type>& >( *rep ); }
  
  
  bool operator==(const PAlgebraMod& other) const
  {
    return getZMStar() == getZMStar() && getR() == other.getR();
  }
  // comparison

  bool operator!=(const PAlgebraMod& other) const
  {
    return !(*this == other);
  }
  // comparison

  /* direct access to the PAlgebraModBase methods */

  //! Returns the type tag: PA_GF2_tag or PA_zz_p_tag
  PA_tag getTag() const { return rep->getTag(); }
  //! Returns reference to underlying PAlgebra object
  const PAlgebra& getZMStar() const { return rep->getZMStar(); }
  //! Returns reference to the factorization of Phi_m(X) mod p^r, but as ZZX's
  const vector<ZZX>& getFactorsOverZZ() const { return rep->getFactorsOverZZ(); }
  //! The value r
  long getR() const { return rep->getR(); } 
  //! The value p^r
  long getPPowR() const { return rep->getPPowR(); }
  //! Restores the NTL context for p^r
  void restoreContext() const { rep->restoreContext(); }

  /**
     @brief Generates the "mask table" that is used to support rotations
     
     maskTable[i][j] is a polynomial representation of a mask that is 1 in
     all slots whose i'th coordinate is at least j, and 0 elsewhere. We have:
     \verbatim
       maskTable.size() == zMStar.numOfGens()     // # of generators
       for i = 0..maskTable.size()-1:
         maskTable[i].size() == zMStar.OrderOf(i) // order of generator i
     \endverbatim
  **/
  void genMaskTable() const { rep->genMaskTable(); }

  /** 

    generates CRT table to speed up mask encoding
 
  **/

  void genCrtTable() const { rep->genCrtTable(); }
};

#endif // #ifdef _PAlgebra_H_
