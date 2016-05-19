class Vecteur:
	def __init__(self,x,y):
		self._x=x
		self._y=y
		
	def base(self,v):
		v._x=self._y
		v._y=self._x
		return v
		
	def eg(self,v):
		if self._x==v._x and self._y==v._y:
			eq=True
		else:
			eq=False
		return eq
			
	def nul(self):
		n=False
		if self._x==0 and self._y==0:
			n=True
		return n
		
	def col(self,v):
		co=False
		if self._x*v._y==self._y*v._x:
			co=True
		return co
	
	def add(self,v):
		self._x += v._x
		self._y += v._y
		
	def chg(self,x,y):
		self._x=x
		self._y=y
		
	def affiche(self):
		print("x=",self._x)
		print("y=",self._y)

	def __str__(self):
		return "vecteur"+self.__class__.__name__+"\n x:"+str(self._x)+"\n y:"+str(self._y)
		
v1=Vecteur(0,1)
v2=Vecteur(0,0)
v3=Vecteur(0,1)
v4=Vecteur(0,0)
v2=v1.base(v2)
print(v1)
v1.affiche()
print("v2")
v2.affiche()
print(v1.eg(v4))
print(v3.nul())
v1.add(v2)
print("v1")
v1.affiche()
print(v1.col(v3))
