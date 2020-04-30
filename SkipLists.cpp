#include<iostream>
#include<fstream>
#include<cstdlib>
#include <cstring>
#include<time.h>
#include<math.h>
using namespace std;
ifstream f("abce.in");
ofstream g("abce.out");
class node
{
public:
    int key;
    node **next,**back;

    node(int key, int nivel)  ///constructorul de initializare
    {
        this->key = key;
        this->next = new node*[nivel+1];
        memset(this->next, 0, sizeof(node*)*(nivel+1));   ///initializez cu 0(NULL) tot array-ul, de nivel+1 ori

        this->back = new node*[nivel+1];
        memset(this->back, 0, sizeof(node*)*(nivel+1));   ///initializez cu 0(NULL) tot array-ul, de nivel+1 ori
    }

};


class skiplist
{
    int maxlvl; //pot sa ridic maxim maxlvl nivele
    float p;    //il folosesc pentru functia random
    node *header;   //vector de poineri de start, unul pt fiecare nivel
    struct da{int max,nr;} *v;

public:
    int nivel;  //nivelul actual
    skiplist(int maxlvl);  /// nivelul maxim la care sa ajung.
    int random();
    node* CreateNode(int, int);
    void PrintSkipList();

    void InsertNode(int x); //1
    void DeleteNode(int x); //2
    void FindNode(int x);   //3 returneaza 1 daca exista, 0 daca nu. Pot sa apelez unul din CM uri si daca cel gasit =x atunci 1 altfel 0
    node* FindCMMic(int x);  //4 cel mai mic nod mai mare sau egal cu x
    node* FindCMMare(int x); //5 cel mai mare nod mai mic sau egal cu x*/
    void PrintXtoY(int X,int Y);//6 toate numerele Z, cu X<=Z<=Y in ordine crescatoare
};

skiplist::skiplist(int n)
{

    this->maxlvl = log2(n);
    this->p = 0.6;  //de p o sa ma folosesc ca sa fac functia random
    nivel = 0;

    header = new node(-1, maxlvl);  //creez headerul:aloc spatiu de maxlvl nivele si il initializez cu -1.

};

int skiplist::random()
{
    float r = (float)rand()/RAND_MAX;
    int lvl = 0;
        while (r < p && lvl < maxlvl)
        {
            lvl++;
            r = (float)rand()/RAND_MAX;

        }
    return lvl;
};

node* skiplist::CreateNode(int key, int nivel)
{
    node *n = new node(key, nivel);
    return n;
};

void skiplist::PrintSkipList()
{
    cout<<'\n'<<"Cum arata Skiplist ul meu:"<<'\n';
    for (int i=0;i<=nivel;i++)
    {
        node *n = header->next[i];
        cout << "Level " << i << ": ";
        while (n != NULL)
        {
            cout << n->key<<" ";
            n = n->next[i];
        }
        cout << "\n";
    }

    cout<<endl;

        cout << "\n";
}

void skiplist::InsertNode(int x)
{

    node *c = header; //plec din header
    node *k[maxlvl+1];  //imi tin minte legaturile pe care trebuie sa le fac pe fiecare nivel cu nodul x

    memset(k, 0, sizeof(node*)*(maxlvl+1));

    for (int i = nivel; i >= 0; i--)     //incep de pe cel mai inalt nivel si merg cat timp cheia lui next este mai mica decat x.
    {
        while (c->next[i] != NULL &&  c->next[i]->key < x)//apoi cand >=x, cobor pe urmatorul nivel.
            c = c->next[i];
        k[i] = c;
    }

    c = c->next[0];

    if (c == NULL || c->key != x) /// daca c==Null am ajuns la sfarsitul listei si inseamna ca x e cel mai mare
    {                             ///sau daca key!=x inseamna ca nu il am in lista si am gasit intre ce elemente sa il pun
        int r = random();

        if (r>nivel)
        {
            for (int i=nivel+1;i<r+1;i++)
                k[i] = header;

            nivel = r;
        }

        node* n = CreateNode(x, r); ///creez nodul n=x cu numarul de nivele=r!!
        for (int i=0;i<=r;i++) ///inserez nodul
        {
            n->next[i] = k[i]->next[i];
            n->back[i]=k[i];
            if(k[i]->next[i])
            {
                node*y=k[i]->next[i];
                y->back[i]=n;
            }
            n->back[i]=k[i];
            k[i]->next[i] = n;
        }
        cout << "Am inserat cu succes nodul " <<x << "\n";
    }
};

void skiplist::DeleteNode(int x)
{
    int i=nivel;
    node *n=header;

    while(i>=0)
    {
        while(n->next[i] and n->next[i]->key<x)
            n=n->next[i];       ///parcurg la dreapta pe nivelul i pana gasesc cel mai mic element mai mare sau egal cu x
        if(n->next[i])
            n=n->next[i];
        ///if(i==0 and n->key != x)    ///inseamna ca x nu exista in lista !!

        if(n->key==x)   ///cazul 1, e egal cu x, atunci sunt fix pe nod si nu mai trebuie sa parcurg decat strict in jos ca sa rup legaturile
        {
            while(i>=0)
                {
                    if(n->next[i])
                        n->next[i]->back[i]=n->back[i];
                    if(n->back)
                        n->back[i]->next[i]=n->next[i];

                    if(!header->next[i])    //daca mi s a golit nivelul, il sterg!
                        nivel--;
                    i--;
                } ///am terminat cautarea
        }
        else            ///daca nu e egal cu x, treb sa caut pe un nivel mai jos, de la pointerul curent la dreapta!
            {n=n->back[i];i--;}///caut un nivel mai jos, intre prev si n! e de juns sa l retin pe prev, nu o sa se depaseasca n!

    }
};

void skiplist::FindNode(int x)
{
    int i=nivel;
    bool ok=0;
    node *n=header;

    while(i>=0 and !ok)
    {
        while(n->next[i] and n->next[i]->key<x)
                n=n->next[i];       ///parcurg la dreapta pe nivelul i pana gasesc cel mai mic element mai mare sau egal cu x
        if(n->next[i])
            n=n->next[i];
        ///if(i==0 and n->key != x)    ///inseamna ca x nu exista in lista !

        if(n->key==x)   ///cazul 1, e egal cu x, atunci sunt fix pe nod si nu mai trebuie sa parcurg decat strict in jos ca sa rup legaturile
            ok=1;
        else            ///daca nu e egal cu x, treb sa caut pe un nivel mai jos, de la pointerul curent la dreapta!
            {n=n->back[i];i--;}///caut un nivel mai jos, intre prev si n! e de juns sa l retin pe prev, nu o sa se depaseasca n!

    }
    g<<ok<<endl;
}

node* skiplist::FindCMMic(int x)
{
    int i=nivel,numar=0;
    node *n=header,*p;

    while(i>=0)
    {
        while(n->next[i] and n->next[i]->key<x)
                n=n->next[i];       ///parcurg la dreapta pe nivelul i pana gasesc cel mai mic element mai mare sau egal cu x

        if(n->next[i] and n->next[i]->key==x)
            {p=n->next[i]->back[0]->next[0];i=-1;}       ///am neaparat nevoie sa returnez pointerul de pe nivelul 0
                                ///o sa imi foloseasca in functia 6!
        if(i==0)
            {p=n->next[i]->back[0]->next[0];i=-1;}
        else
            i--;

    }
    return p;
}

node* skiplist::FindCMMare(int x)
{
    int i=nivel,numar=0;
    node *n=header,*p;

    while(i>=0)
    {
        while(n->next[i] and n->next[i]->key<=x)
                n=n->next[i];       ///parcurg la dreapta pe nivelul i pana gasesc cel mai mic element mai mare sau egal cu x

        if(n->key==x)///ma opresc
            {p=n->back[0]->next[0];i=-1;} ///acelasi lucru ca mai sus, am nevoie de pointer de pe nivelul 0

        if(i==0) ///ma opresc
            {p=n;i=-1;}
        else
            {i--;}///caut un nivel mai jos, intre prev si n! e de juns sa l retin pe prev, nu o sa se depaseasca n!

    }
   return p;
}

void skiplist::PrintXtoY(int X,int Y)
 {
    node *x=FindCMMic(X);
    while( x && x->key<=Y)
    {
        g<<x->key<<" ";
        x=x->next[0];
    }
    g<<endl;

 }


int main()
{

    srand((unsigned)time(0));   // Seed random number generator este predefinit in biblioteca time.h

    int i,n,j,x,y,z;
    f>>n;

    skiplist lista(n);        //parametrul= maxlvl
    for(i=0;i<n;i++)
    {
        f>>x>>y;
        if(x==1)
            lista.InsertNode(y);
        if(x==2)
            lista.DeleteNode(y);
        if(x==3)
            lista.FindNode(y);
        if(x==4)
            {
            node*p=lista.FindCMMare(y);
            g<<p->key<<endl;
            }
        if(x==5)
            {
            node*p=lista.FindCMMic(y);
            g<<p->key<<endl;
            }
        if(x==6)
        {
            f>>z;
            lista.PrintXtoY(y,z);
        }
    }
     /*skiplist lista(100);        //parametrul= maxlvl
   lista.InsertNode(3);
    lista.InsertNode(6);
    lista.InsertNode(7);
    lista.InsertNode(9);
    lista.InsertNode(25);
    lista.InsertNode(19);
    lista.InsertNode(17);
    lista.InsertNode(26);
    lista.InsertNode(21);
    lista.InsertNode(12);
   cout<<1; lista.PrintSkipList();
    lista.DeleteNode(17);
    cout<<2;lista.PrintSkipList();
    lista.DeleteNode(9);
   cout<<3; lista.PrintSkipList();
    lista.DeleteNode(3);
    cout<<4;lista.PrintSkipList();
    lista.DeleteNode(25);
    cout<<5;lista.PrintSkipList();
   lista.InsertNode(100);
    lista.InsertNode(15);
    lista.DeleteNode(2);
    cout<<6; lista.PrintSkipList();
    lista.DeleteNode(1);
     cout<<7;lista.PrintSkipList();
    lista.DeleteNode(120);
   cout<<8; lista.PrintSkipList();
   lista.FindNode(17);
   lista.FindNode(3);
   lista.FindNode(26);
   lista.FindNode(100);
   lista.FindNode(21);
 /* // cout<<endl<<lista.FindCMMic(6)->key;
 // cout<<endl<<lista.FindCMMare(1001)->key<<endl;
   lista.PrintXtoY(2,26);*/
    return 0;
}
