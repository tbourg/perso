#include <cassert>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <ctime>
#include "R2.hpp"
namespace bamg {
using namespace std ;
template<class T> inline T Min (const T &a,const T &b)
{return a < b ? a : b ;}
template<class T> inline T Max (const T &a,const T & b)
{return a > b ? a : b ;}
template<class T> inline T Abs (const T &a){return a <0 ? -a : a ;}
template<class T> inline double Norme (const T &a){return sqrt(a*a) ;}
template<class T> inline void Exchange (T& a,T& b) {T c=a ;a=b ;b=c ;}
// définition des types pour les coordonnées
typedef double R ;
typedef int Icoor1 ; // type d’une coordonnée entière
typedef double Icoor2 ; // type des produits de coordonnées entière
const Icoor1 MaxICoor = 8388608 ; // max de coordonnées entières 223 pour
// ne pas avoir d’erreur d’arrondi
const Icoor2 MaxICoor22 // MaxICoor22 plus gros produit de coordonnées
= Icoor2(2)*Icoor2(MaxICoor) * Icoor2(MaxICoor) ; //entières
typedef P2<Icoor1,Icoor2> I2 ; // points à coordonnées entières
typedef P2<R,R> R2 ; // points à coordonnées réelles
// Gestion des erreurs
inline void MeshError(int Err){
cerr << " Fatal error in the meshgenerator " << Err << endl ;
exit(1) ; }
inline int BinaryRand(){ // OuiNon aléatoire
const long HalfRandMax = RAND_MAX/2 ;
return rand() <HalfRandMax ;
}
// déterminant entier du triangle a, b, c
Icoor2 inline det(const I2 &a,const I2 & b,const I2 &c)
{
Icoor2 bax = b.x - a.x ,bay = b.y - a.y ;
Icoor2 cax = c.x - a.x ,cay = c.y - a.y ;
return bax*cay - bay*cax ;}
// définition des numérotations dans un triangle
static const short VerticesOfTriangularEdge[3][2] = {{1,2},{2,0},{0,1}} ;
static const short EdgesVertexTriangle[3][2] = {{1,2},{2,0},{0,1}} ;
static const short OppositeVertex[3] = {0,1,2} ;
static const short OppositeEdge[3] = {0,1,2} ;
static const short NextEdge[3] = {1,2,0} ;
static const short PreviousEdge[3] = {2,0,1} ;
static const short NextVertex[3] = {1,2,0} ;
static const short PreviousVertex[3] = {2,0,1} ;
class Triangles ; // Le maillages (les triangles)
class Triangle ; // un triangle
// /////////////////////////////////////////////////////////////////////////
class Vertex {public:
I2 i ; // allow to use i.x, and i.y in long int
R2 r ; // allow to use r.x, and r.y in double
R h ; // Mesh size
int Label ;
Triangle * t ; // un triangle t contenant le sommet
short vint ; // numéro du sommet dans le triangle t
operator I2 () const {return i ;} // cast en coor. entières
operator const R2 & () const {return r ;} // cast en coor. réelles
int DelaunayOptim(int = 1) ; // optimisation Delaunay autour
friend ostream& operator <<(ostream& f, const Vertex & v)
{f << "(" << v.i << "," << v.r << ")" ; return f ;}
} ;
// /////////////////////////////////////////////////////////////////////////
class TriangleAdjacent {
public:
Triangle * t ; // le triangle
int a ; // le numéro de l’arête
TriangleAdjacent(Triangle * tt,int aa): t(tt),a(aa &3) {} ;
TriangleAdjacent() {} ;
operator Triangle * () const {return t ;}
operator Triangle & () const {return *t ;}
operator int() const {return a ;}
TriangleAdjacent operator-()
{ a= PreviousEdge[a] ;
return *this ;}
inline TriangleAdjacent Adj() const ;
inline void SetAdj2(const TriangleAdjacent& , int =0) ;
inline Vertex * EdgeVertex(const int &) const ;
inline Vertex * OppositeVertex() const ;
inline Icoor2 & det() const ;
inline int Locked() const ;
inline int GetAllFlag_UnSwap() const ;
inline void SetLock() ;
friend ostream& operator <<(ostream& f, const TriangleAdjacent & ta)
{f << "{" << ta.t << "," << ((int) ta.a) << "}" ;
return f ;}
} ; // end of Vertex class
// ///////////////////////////////////////////////////////////////////////
class Triangle {
friend class TriangleAdjacent ;
private: // les arêtes sont opposées à un sommet
Vertex * ns [3] ; // les 3 sommets
Triangle * at [3] ; // nu triangle adjacent
short aa[3] ; // les no des arêtes dans le triangle (mod 4)
// on utilise aussi aa[i] pour marquer l’arête i (i=0,1,2)
// si (aa[i] & 4 ) => arête bloquée (lock) marque de type 0
// si (aa[i] & 2n+2 => arête marquée de type n (n=0..7)
// la marque de type 1 était pour déja basculé (markunswap)
// (aa[i] & 1020 ) l’ensemble des marques
// 1020 = 1111111100 (en binaire)
// ----------------------------------
public:
Icoor2 det ; // det. du triangle (2 fois l’aire des coor. entières)
bool NonDegenere() const {return ns[0] && ns[1] && ns[2] ;}
Triangle() {}
inline void Set(const Triangle &,const Triangles &,Triangles &) ;
inline int In(Vertex *v) const
{ return ns[0]==v || ns[1]==v || ns[2]==v ;}
const Vertex & operator[](int i) const {return *ns[i] ;} ;
Vertex & operator[](int i) {return *ns[i] ;} ;
const Vertex * operator()(int i) const {return ns[i] ;} ;
Vertex * & operator()(int i) {return ns[i] ;} ;
TriangleAdjacent Adj(int i) const // triangle adjacent + arête
{ return TriangleAdjacent(at[i],aa[i]&3) ;} ;
Triangle * TriangleAdj(int i) const
{return at[i&3] ;} // triangle adjacent + arête
short NuEdgeTriangleAdj(int i) const
{return aa[i&3]&3 ;} // o de l’arête adj. dans adj tria
void SetAdjAdj(short a)
{ a &= 3 ;
Triangle *tt=at[a] ;
aa [a] &= 1015 ; // supprime les marques sauf << swap »
// (1015 == 1111110111 en binaire)
register short aatt = aa[a] & 3 ;
if(tt){
tt->at[aatt]=this ;
tt->aa[aatt]=a + (aa[a] & 1020 ) ;}} // copie toutes les marques
void SetAdj2(short a,Triangle *t,short aat)
{ at[a]=t ;aa[a]=aat ;
if(t) {t->at[aat]=this ;t->aa[aat]=a ;}}
void SetTriangleContainingTheVertex()
{ if (ns[0]) (ns[0]->t=this,ns[0]->vint=0) ;
if (ns[1]) (ns[1]->t=this,ns[1]->vint=1) ;
if (ns[2]) (ns[2]->t=this,ns[2]->vint=2) ;}
int DelaunaySwap(short a) ; // bascule le quadrilataire formé avec
// le triangle adj en a si il est non Delaunay
int DelaunayOptim(short a) ; // Met tous les quad. contenant
// le sommet a du triangle Delaunay
int Locked(int a)const // retourne si l’arête est frontière
{ return aa[a]&4 ;} // (plus d’échange d’arête)
void SetLocked(int a){ // mark l’arête comme frontière (lock)
Triangle * t = at[a] ;
t->aa[aa[a] & 3] |=4 ;
aa[a] |= 4 ;}
} ; // ----- Fin de la class Triangle -------
// ///////////////////////////////////////////////////////////////////////
class Triangles {
public:
int nbvx,nbtx ; // nombre max de sommets, de triangles
int nbv,nbt ; // nb sommet, de triangles,
int nbiv,nbtf ; // nb de triangle dégénéré (arête du bord)
int NbOfSwapTriangle,NbUnSwap ;
Vertex * vertices ; // tableau des sommets
R2 pmin,pmax ; // extrema
R coefIcoor ; // coef pour les coor. entière
Triangle * triangles ;
// end of variable
Triangles(int i) ; // constructeur
~Triangles() ;
void SetIntCoor() ; // construction des coor. entières
void RandomInit() ; // construction des sommets aléatoirement
// sauce C++
const Vertex & operator() (int i) const
{ return vertices[i] ;} ;
Vertex & operator()(int i)
{ return vertices[i] ;} ;
const Triangle & operator[] (int i) const
{ return triangles[i] ;} ;
Triangle & operator[](int i)
{ return triangles[i] ;} ;
// transformation des coordonnées ...
I2 toI2(const R2 & P) const {
return I2( (Icoor1) (coefIcoor*(P.x-pmin.x))
,(Icoor1) (coefIcoor*(P.y-pmin.y)) ) ;}
R2 toR2(const I2 & P) const {
return R2( (double) P.x/coefIcoor+pmin.x,
(double) P.y/coefIcoor+pmin.y) ;}
// ajoute sommet à un triangle
void Add( Vertex & s,Triangle * t,Icoor2 * =0) ;
void Insert() ; // insère tous les sommets
Triangle * FindTriangleContening(const I2 & B, // recherche le triangle
Icoor2 dete[3], // contenant le sommet B
Triangle *tstart) const ; // partant de tstart
void ReMakeTriangleContainingTheVertex() ;
int Number(const Triangle & t) const { return &t - triangles ;}
int Number(const Triangle * t) const { return t - triangles ;}
int Number(const Vertex & t) const { return &t - vertices ;}
int Number(const Vertex * t) const { return t - vertices ;}
private:
void PreInit(int) ;
} ; // End Class Triangles
// ///////////////////////////////////////////////////////////////////////
inline Triangles::Triangles(int i) {PreInit(i) ;}
// ///////////////////////////////////////////////////////////////////////
inline void TriangleAdjacent::SetAdj2(const TriangleAdjacent & ta, int l )
{ // set du triangle adjacent
if(t) {
t->at[a]=ta.t ;
t->aa[a]=ta.a|l ;}
if(ta.t) {
ta.t->at[ta.a] = t ;
ta.t->aa[ta.a] = a| l ;
}
}
// l’arête Locked est telle Lock (frontière)
inline int TriangleAdjacent::Locked() const
{ return t->aa[a] &4 ;}
// récupération des tous les flag (Lock)
inline int TriangleAdjacent::GetAllFlag_UnSwap() const // donne tous
{ return t->aa[a] & 1012 ;} // les marque sauf MarkUnSwap
// Construit l’ Adjacent
inline TriangleAdjacent TriangleAdjacent::Adj() const
{ return t->Adj(a) ;}
// sommet de l’arête
inline Vertex * TriangleAdjacent::EdgeVertex(const int & i) const
{return t->ns[VerticesOfTriangularEdge[a][i]] ; }
// sommet opposé à l’arête
inline Vertex * TriangleAdjacent::OppositeVertex() const
{return t->ns[bamg::OppositeVertex[a]] ; }
// det du triangle
inline Icoor2 & TriangleAdjacent::det() const
{ return t->det ;}
// Construit l’adjacent
inline TriangleAdjacent Adj(const TriangleAdjacent & a)
{ return a.Adj() ;}
// Adjacence suivante dans le triangle
inline TriangleAdjacent Next(const TriangleAdjacent & ta)
{ return TriangleAdjacent(ta.t,NextEdge[ta.a]) ;}
// Adjacence précédente dans le triangle
inline TriangleAdjacent Previous(const TriangleAdjacent & ta)
{ return TriangleAdjacent(ta.t,PreviousEdge[ta.a]) ;}
// Optimisation de Delaunay
int inline Vertex::DelaunayOptim(int i)
{
int ret=0 ;
if ( t && (vint >= 0 ) && (vint <3) )
{
ret = t->DelaunayOptim(vint) ;
if( !i)
{
t =0 ; // pour supprimer les optimisation future
vint= 0 ; }
}
return ret ;
}
// calcul de det du triangle a,b,c
Icoor2 inline det(const Vertex & a,const Vertex & b,const Vertex & c)
{
register Icoor2 bax = b.i.x - a.i.x ,bay = b.i.y - a.i.y ;
register Icoor2 cax = c.i.x - a.i.x ,cay = c.i.y - a.i.y ;
return bax*cay - bay*cax ;}
// la fonction qui fait l’échange sans aucun test
void swap(Triangle *t1,short a1,
Triangle *t2,short a2,
Vertex *s1,Vertex *s2,Icoor2 det1,Icoor2 det2) ;
// la fonction qui fait une échange pour le forçage de la frontière
int SwapForForcingEdge(Vertex * & pva ,Vertex * & pvb ,
TriangleAdjacent & tt1,Icoor2 & dets1,
Icoor2 & detsa,Icoor2 & detsb, int & nbswap) ;
// la fonction qui force l’arête a,b dans le maillage
int ForceEdge(Vertex &a, Vertex & b,TriangleAdjacent & taret) ;
// la fonction qui marque l’arête comme lock (frontière)
inline void TriangleAdjacent::SetLock(){ t->SetLocked(a) ;}
}

