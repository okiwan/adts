#include <iostream.h>
#include <stdlib.h>
#include <time.h>

#ifdef PERFPROFILE
	#include <google/heap-profiler.h>
#endif
	
#define NELEMENTSDEMO		5

template <class C, class I>

class ABC {
public:
	
	ABC(void) : arrel(NULL), numelements(0), altura(0) {}
	~ABC(void);
	void inserir(C pclau, I pinfo);
	void inordre(void);
	I consultar(C pclau);
	C minim(void);
	C maxim(void);
	void esborrar(C pclau);
	int elements(void);
	
private:
	
	class Node {
	public:
		C clau;
		I info;
		Node *fesq;
		Node *fdre;

		Node(C pclau, I pinfo, Node *pfesq = NULL, Node *pfdre = NULL) : clau(pclau), info(pinfo), fesq(pfesq), fdre(pfdre) {}			
	};
	
	Node *arrel;
	Node *actual;
	int numelements;
	int altura;
	
	void inserir(C pclau, I pinfo, Node **node);
	void inordre(Node **node);
	I consultar(C pclau, Node **node);
	C minim(Node **node);
	C maxim(Node **node);
	void esborrar(C pclau, Node **node);
	Node * buscaTreuIRetornaMinim(Node **node);
};

template <class C, class I> ABC<C, I>::~ABC(void) {
	delete arrel;
	delete actual;
}

template <class C, class I> void ABC<C, I>::inserir(C pclau, I pinfo) {
	this->inserir(pclau, pinfo, &arrel);
}

template <class C, class I> void ABC<C, I>::inordre(void) {
	this->inordre(&arrel);
}

template <class C, class I> I ABC<C, I>::consultar(C pclau) {
	return consultar(pclau, &arrel);
}

template <class C, class I> C ABC<C, I>::minim(void) {
	return minim(&arrel);
}

template <class C, class I> C ABC<C, I>::maxim(void) {
	return maxim(&arrel);
}

template <class C, class I> int ABC<C, I>::elements(void) {
	return numelements;
}

template <class C, class I> void ABC<C, I>::esborrar(C pclau) {
	esborrar(pclau, &arrel);	
}

template <class C, class I> void ABC<C, I>::inserir(C pclau, I pinfo, Node **node) {
	if(*node == NULL) {
		*node = new Node(pclau, pinfo);
		numelements++;
	}
	else if(pclau > (*node)->clau) inserir(pclau, pinfo, &(*node)->fdre);
	else if(pclau < (*node)->clau) inserir(pclau, pinfo, &(*node)->fesq);
	else {
		(*node)->clau = pclau;
		(*node)->info = pinfo;
	}
}

template <class C, class I> void ABC<C, I>::inordre(Node **node) {
	if(*node == NULL) return;
	inordre(&(*node)->fesq);
	cout << "[" << (*node)->clau << ", " << (*node)->info << "]" << endl;
	inordre(&(*node)->fdre);
}

template <class C, class I> I ABC<C, I>::consultar(C pclau, Node **node) {
	if(*node == NULL) throw "consultar: element no trobat";
	if((*node)->clau > pclau) return consultar(pclau, &(*node)->fesq);
	if((*node)->clau < pclau) return consultar(pclau, &(*node)->fdre);
	return (*node)->info;
}

template <class C, class I> C ABC<C, I>::minim(Node **node) {
	if(*node == NULL) throw "minim: arbre buit";
	if((*node)->fesq != NULL) return minim(&(*node)->fesq);
	return (*node)->clau;
}

template <class C, class I> C ABC<C, I>::maxim(Node **node) {
	if(*node == NULL) throw "maxim: arbre buit";
	if((*node)->fdre != NULL) return maxim(&(*node)->fdre);
	return (*node)->clau;
}

template <class C, class I> void ABC<C, I>::esborrar(C pclau, Node **node) {
	if(*node == NULL) return;
	if((*node)->clau > pclau) return esborrar(pclau, &(*node)->fesq);
	if((*node)->clau < pclau) return esborrar(pclau, &(*node)->fdre);
	else {
		Node *q = *node;
		
		if((*node)->fdre == NULL)
			*node = (*node)->fesq;
		else if((*node)->fesq == NULL)
			*node = (*node)->fdre;
		else {
			Node *min = buscaTreuIRetornaMinim(&(*node)->fdre);
			min->fesq = (*node)->fesq;
			min->fdre = (*node)->fdre;
			*node = min;
		}
		
		delete q;
		numelements--;
	}
}

template <class C, class I> typename ABC<C, I>::Node * ABC<C, I>::buscaTreuIRetornaMinim(Node **node) {
	if(*node == NULL) return NULL;
	if((*node)->fesq != NULL) return buscaTreuIRetornaMinim(&(*node)->fesq);
	Node *q = *node;
	*node = (*node)->fdre;
	return q;
}

int main(int argc, char **argv) {
	
	int unnumero;

#ifdef PERFPROFILE
	HeapProfilerStart("/Users/Sergio/Desktop/volcado");
	HeapProfilerDump("**Heap a l'inici**");
#endif

	cout << "> Demostració d'ABCs" << endl;
	
	cout << "> Inicialització de variables de l'entorn" << endl;
	srand(time(NULL));
	
	cout << "> Creem un arbre, buit per defecte" << endl;
	ABC<int, int> arbre;
	
	cout << "> Afegim un total de " << NELEMENTSDEMO << " element(s), cadascun un natural de l'1 al 100" << endl;
	for(int i=0; i<NELEMENTSDEMO; ++i) {
		int tmp = (rand() % 100) + 1;
		unnumero = tmp;
		arbre.inserir(tmp, tmp);
	}
	
	cout << "> Recorregut en inordre de l'arbre" << endl;
	arbre.inordre();

	cout << "> Cerca d'un element inexistent" << endl;
	try {
		arbre.consultar(-1);
	} catch (char *m) {
		cout << m << endl;
	}
	
	cout << "> Cerca d'un element existent" << endl;
	cout << arbre.consultar(unnumero) << endl;
	
	cout << "> Mínim i màxim de l'arbre" << endl;
	cout << "min = " << arbre.minim() << ", max = " << arbre.maxim() << endl;
	
	cout << "> Esborrat d'un element (" << unnumero << ") i recorregut en inordre" << endl;
	arbre.esborrar(unnumero);
	arbre.inordre();

	cout << "> Buidat de l'arbre" << endl;
	while(arbre.elements() > 0) {
		cout << ">> Treiem el..." << arbre.minim() << endl;
		arbre.esborrar(arbre.minim());
		arbre.inordre();
	}

#ifdef PERFPROFILE
	HeapProfilerDump("**Heap al final**");
	HeapProfilerStop();
#endif

	return 0;
}
