#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int  matrice[3][3];
    int c;int g;
}etat;

typedef struct Noeud
{
    struct Noeud *suivant;
    etat Etat;
}Noeud;

typedef struct
{
    Noeud *debut;
}liste;

liste listeNoeuds;


//////////////////////////////////////////////////
void afficher(etat e)
{
    int i,j;
    printf("\t\t _________________\n");
    for (i=0; i<3; i++)
        printf("\t\t|     |     |     |\n\t\t|  %d  |  %d  |  %d  |\n\t\t|_____|_____|_____|\n",
               e.matrice[i][0],e.matrice[i][1],e.matrice[i][2]);
    printf("\n");
}

//////////////////////////////////////////////////
int Vide(liste* l)
{
    return(l->debut==NULL);
}
//////////////////////////////////////////////////
void Inserer(etat un_etat, liste *une_liste)
{
	Noeud *element;
	element = malloc(sizeof(*element));
    if(element == NULL)
	{
		fprintf(stderr, "Probleme allocation dynamique.\n");
		exit(EXIT_FAILURE);
	}
	element->Etat = un_etat;
	element->suivant =une_liste->debut;
	une_liste->debut=element;
}
//////////////////////////////////////////////////
void Inserer_noeud(Noeud *n, liste *une_liste)
{
    Noeud *element;
	element = malloc(sizeof(*element));
    if(element == NULL)
	{
		fprintf(stderr, "Probleme allocation dynamique.\n");
		exit(EXIT_FAILURE);
	}
	element->Etat = n->Etat;
	element->suivant =une_liste->debut;
	une_liste->debut=element;
}
//////////////////////////////////////////////////
etat Extraire(liste *une_liste)
{
    if(!Vide(une_liste))
    {
        Noeud *copie=une_liste->debut;
        etat un_etat=une_liste->debut->Etat;
        une_liste->debut = une_liste->debut->suivant;
        free(copie);
        return un_etat;
    }
}
//////////////////////////////////////////////////
void supprimer_noeud(Noeud *n, liste *une_liste)
{
    Noeud *temp=une_liste->debut;
    while(temp->suivant!=NULL)
    {
        if(temp->suivant==n)
            temp->suivant=temp->suivant->suivant;
        temp=temp->suivant;
    }
}

//////////////////////////////////////////////////
int Appartient(etat e,liste* l)
{
    int cpt=0;
    Noeud* test=l->debut;
    while(test!=NULL)
    {
        for (int i=0 ; i<3 ; i++)
            for (int j= 0 ; j<3 ; j++)
                if(test->Etat.matrice[i][j] == e.matrice[i][j])
                    cpt++;
        if(cpt == 9)  return 1;
        test=test->suivant;
    }
    return 0;
}
//////////////////////////////////////////////////
int Appartient_noeud(Noeud *n,liste* l)
{
    return(Appartient(n->Etat,l));
}

///////////////
int etatSolution (etat Courant)
{
    int cpt=0;
    int solu[3][3]={{1,2,3},{8,0,4},{7,6,5}};
    for (int i=0 ; i<3 ; i++)
        for (int j= 0 ; j<3 ; j++)
            if(Courant.matrice[i][j] == solu[i][j])
                cpt++;
    return(cpt==9);
}
//////////////////////////////////////////////////

int h(etat e)
{
    int S[3][3] = {{1,2,3},{0,8,4},{7,6,5}};
    int i,j,h=0;
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            if(e.matrice[i][j]!=S[i][j])
                h++;
    return h;
}

//////////////////////////////////////////////////
int g(etat e)
{
    return e.g;
}
//////////////////////////////////////////////////
int f(etat e)
{
    return h(e) + g(e);
}
//////////////////////////////////////////////////
void tri(liste* l) //on utilise un tri par selection
{
    Noeud *temp=l->debut,*lj=l->debut,*li=l->debut;//li:represenre l[i],lj:represenre l[j]

    int len=0;
    int min;
    int c=1;
    while(temp!=NULL)
    {
        ++len;
        temp=temp->suivant;
    }
    for(int i=1; i<=len; ++i)
    {
        min=i;
        c=1;
        li=l->debut;
        while(c<i)
        {
            li=li->suivant;
            ++c;
        }
        for(int j=i+1; j<=len; ++j)
        {
            c=1;
            lj=l->debut;
            while(c<j)
            {
                lj=lj->suivant;
                ++c;
            }
            if(lj->Etat.c<li->Etat.c)
                min=j;
        }

        c=1;
        lj=l->debut;
        while(c<min)
        {
            lj=lj->suivant;
            ++c;
        }
        etat test=li->Etat;
        li->Etat=lj->Etat;
        lj->Etat=test;
    }
}
//////////////////////////////////////////////////
void right(etat e, liste* l){
    int i,j;
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            if(e.matrice[i][j] == 0)
                if(j<2)
                {
                    int aide;
                    aide=e.matrice[i][j];
                    e.matrice[i][j]=e.matrice[i][j+1];
                    e.matrice[i][j+1]=aide;
                    //e.matrice[i][j+1]=0;
                    Inserer(e,l);
                    break;
                }
}
//////////////////////////////////////////////////
void left(etat e, liste* l){
    int i,j;
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            if(e.matrice[i][j] == 0)
                if(j<3 && j!=0)
                {
                    int aide;
                    aide=e.matrice[i][j];
                    e.matrice[i][j]=e.matrice[i][j-1];
                    e.matrice[i][j-1]=aide;
                    //e.matrice[i][j-1]=0;
                    Inserer(e,l);
                    break;
                }
}
//////////////////////////////////////////////////
void up(etat e, liste* l){
    int i,j;
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
            if(e.matrice[i][j] == 0)
                if(i<3&&i !=0)
                {
                    int aide;
                    aide=e.matrice[i][j];
                    e.matrice[i][j]=e.matrice[i-1][j];
                    e.matrice[i-1][j]=aide;
                    //e.matrice[i-1][j]=0;
                    Inserer(e,l);
                    break;
                }
}
//////////////////////////////////////////////////
void down(etat e, liste* l){
    int i,j;
    for(i=0; i<3; i++)
        for(j=0; j<3; j++)
            if(e.matrice[i][j] == 0)
                if(i<2)
                {
                    int aide;
                    aide=e.matrice[i][j];
                    e.matrice[i][j]=e.matrice[i+1][j];
                    e.matrice[i+1][j]=aide;
                    //e.matrice[i+1][j]=0;
                    Inserer(e,l);
                    break;
                }
}
//////////////////////////////////////////////////
//genereSuccesseurs (etatCourant,&listeSuccesseurs);

void genereSuccesseurs(etat e, liste* l)//pointeur ou non  pour l  ?
{
    left(e,l);
    right(e, l);
    up(e,l);
    down(e,l);

}
//////////////////////////////////////////////////
int memeEtat(Noeud *n1,Noeud *n2)
{
    int c=0;
    for(int i=0; i<3; ++i)
        for(int j=0; j<3; ++j)
        {
            if(n1->Etat.matrice[i][j]==n2->Etat.matrice[i][j])
                c++;
        }
    if(c==9)
        return 1;
    else
        return 0;
}
//////////////////////////
void Recherche_en_A_etoile(etat etatInitial, etat etatFinal)
{
    liste NoeudsDejaTraites;
    etat etatCourant;
    listeNoeuds.debut  = NULL;
    Inserer(etatInitial, &listeNoeuds);
    NoeudsDejaTraites.debut  = NULL;

    while (!Vide (&listeNoeuds))
    {
        etatCourant =Extraire (&listeNoeuds);
        afficher(etatCourant);

        liste listeSuccesseurs;
        listeSuccesseurs.debut=NULL;

        int m;
        m++;

        if (etatSolution (etatCourant))
        {
            printf("\nSucces ! \nNombre de noeuds explores : %d\n",  m-1);
            exit(0);
        }
        else
        {
            genereSuccesseurs(etatCourant,&listeSuccesseurs);
            Noeud *n=listeSuccesseurs.debut;
            while(n!=NULL)
            {
                n->Etat.g=g(etatCourant)+1;
                n->Etat.c= h(n->Etat)+n->Etat.g;

                if(!Appartient_noeud(n,&listeNoeuds) && !Appartient_noeud(n,&NoeudsDejaTraites) )
                {
                    Inserer_noeud(n,&listeNoeuds);
                }
                else if(Appartient_noeud(n,&listeNoeuds))
                {
                    Noeud *n0=listeNoeuds.debut;
                    while(n0!=NULL)
                    {
                        if(memeEtat(n,n0) && n->Etat.c<n0->Etat.c)
                        {
                            n0->Etat.c==n->Etat.c;
                            break;
                        }
                        n0=n0->suivant;
                    }
                }
                else if(Appartient_noeud(n,&NoeudsDejaTraites))
                {
                    Noeud *n1=listeNoeuds.debut;
                    while(n1!=NULL)
                    {
                        if(memeEtat(n,n1) && n->Etat.c<n1->Etat.c)
                        {
                            supprimer_noeud(n1,&NoeudsDejaTraites);
                            Inserer_noeud(n,&listeNoeuds);
                            break;
                        }
                        n1=n1->suivant;
                    }
                }
                n=n->suivant;
            }
            Inserer (etatCourant, &NoeudsDejaTraites);
            tri(&listeNoeuds);
        }
    }
}

int main()
{
    etat e1,e2;
    e1.matrice[0][0]=2;e1.matrice[0][1]=8;e1.matrice[0][2]=3;
    e1.matrice[1][0]=1;e1.matrice[1][1]=6;e1.matrice[1][2]=4;
    e1.matrice[2][0]=7;e1.matrice[2][1]=0;e1.matrice[2][2]=5;

    e1.g=0;
    e1.c=5;

    e2.matrice[0][0]=1;e2.matrice[0][1]=2;e2.matrice[0][2]=3;
    e2.matrice[1][0]=8;e2.matrice[1][1]=0;e2.matrice[1][2]=4;
    e2.matrice[2][0]=7;e2.matrice[2][1]=6;e2.matrice[2][2]=5;

    Recherche_en_A_etoile (e1, e2);
return 0;
}
