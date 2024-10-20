#include <stdio.h>
#include <stdlib.h>
#define DIM 64

typedef struct parole_{
    char *string;
    char *val;
    struct parole_ *sx, *dx;
}parole_t;

typedef struct {
    char *valido[DIM][DIM];
    char *in;
    short ricorrenzaMin[DIM], ricorrenza[DIM];
    char *out[DIM];
    char ***noPosition;
}vincoli2_t;

vincoli2_t vincoli;
parole_t *tnill, *tabella[DIM][DIM];
short dimStrpiu, dimvet;
char *zeroValido, *unoValido, *zeroAltro, *unoAltro;
int dimStringa, cont;

//Sostitutivi libreria string
int fast_compare(char *s1, char *s2){
    int i;
    for(i=0; i<dimStrpiu;i++){
        if(s1[i]<s2[i])
            return 1;
        else if(s1[i]>s2[i])
            return -1;
    }
    return 0;
}
void fast_copy(char *s1, char *s2){
    int i;
    for(i=0; i<=dimStrpiu; i++)
        s1[i]=s2[i];
}

//Gestione alberi
void insertTree(parole_t **testa, parole_t *z){
    parole_t *y=tnill, *x;
    x=(*testa);
    while(x!=tnill){
        y=x;
        if(fast_compare(z->string, x->string)>0)
            x=x->sx;
        else
            x=x->dx;
    }
    if(y==tnill)
        (*testa)=z;
    else if(fast_compare(z->string, y->string)>0)
        y->sx=z;
    else
        y->dx=z;
    z->sx=tnill;
    z->dx=tnill;
}

//Inizializza la struttura del vincolo
void inizializza2(){
    int i, j, k;
    for(i=0;i<DIM; i++) {
        for (j = 0; j < DIM; j++)
            vincoli.valido[i][j] = unoValido;
        for(k=0; k<dimvet; k++)
            vincoli.noPosition[k][i]=zeroAltro;
        vincoli.ricorrenzaMin[i]=0;
        vincoli.ricorrenza[i]=0;
        vincoli.out[i]=zeroAltro;
    }
    for(i=0; i<dimStringa; i++)
        vincoli.in[i]='$';
}
void inizializza1(){
    int i;
    vincoli.in=malloc(sizeof(char)*(dimStringa+1));
    vincoli.in[dimStringa]='\0';
    vincoli.noPosition= malloc(sizeof(char**)*dimvet);
    for(i=0; i<dimvet; i++)
        vincoli.noPosition[i]=malloc(sizeof(char*)*DIM);
    inizializza2();
}

//Riempie le riccorrenze di una parola in vett
void sommaRicorrenze(short *vett, char carattere){
    if(carattere>='a'&& carattere<='z')
        vett[(carattere - 59)]++;
    else if(carattere>='A'&&carattere<='Z')
        vett[(carattere-54)]++;
    else if(carattere>='0'&& carattere<='9')
        vett[(carattere-47)]++;
    else if(carattere=='-')
        vett[0]++;
    else
        vett[37]++;
}
void riempiRicorrenze(short *vett, char *stringa){
    int i;
    for(i=0; i<DIM; i++)
        vett[i]=0;
    for(i=0; i<dimStringa; i++){
        sommaRicorrenze(vett, stringa[i]);
    }
}

//Restituisce posizione del vettore ricorrenze
int posizione(char c){
    if(c=='-')
        return 0;
    else if('0'<=c && c<='9')
        return (c-47);
    else if('A'<=c && 'Z'>=c)
        return (c-54);
    else if(c=='_')
        return 37;
    else
        return (c-59);

}

//Stampo solo le parole valide
void stampaFiltrate(parole_t *nodo){
    if(nodo!=tnill){
        stampaFiltrate(nodo->sx);
        if(*(nodo->val)=='1')
            printf("%s\n", nodo->string);
        stampaFiltrate(nodo->dx);
    }
}

//costruisco stringa sim
void stringaSim(char *string, char *res, char *sim,  short *ricRes){
    int i;
    int riccpy[DIM];
    int shift;
    char carattere;
    for(i=0; i<DIM; i++)
        riccpy[i]=ricRes[i];
    for(i=0; i<dimStringa ; i++){
        sim[i]='/';
        if(res[i]==string[i]) {
            sim[i] = '+';
            carattere=res[i];
            if(carattere>='a'&& carattere<='z')
                riccpy[(carattere - 59) ]--;
            else if(carattere>='A'&&carattere<='Z')
                riccpy[(carattere-54) ]--;
            else if(carattere>='0'&& carattere<='9')
                riccpy[(carattere-47) ]--;
            else if(carattere=='-')
                riccpy[0]--;
            else
                riccpy[37]--;
        }
    }
    for(i=0; i<dimStringa ;i++) {
        if(sim[i]!='+') {
            shift=posizione(string[i]);
            if (riccpy[shift]> 0){
                sim[i] = '|';
                riccpy[shift]--;
            }
        }
    }
}

//Cerco parola all'interno di tutti gli alberi
parole_t* cercaPut(parole_t *nodo, char *stringa){
    while(nodo!=tnill){
        if(fast_compare(nodo->string, stringa)==0)
            return nodo;
        else if(fast_compare(nodo->string, stringa)<0)
            nodo=nodo->sx;
        else
            nodo=nodo->dx;
    }
    return tnill;
}
parole_t *cerca(char *stringa){
    int i, j;
    parole_t *tmp = tnill;
    for(i=0; tmp== tnill && i<DIM; i++)
        for(j=0; tmp== tnill && j<DIM; j++)
            tmp=cercaPut(tabella[i][j], stringa);
    return tmp;
}

//Aggiorno la struttura del vincolo
void gestioneTabella(char *sim, short *ricRes, parole_t *nodo) {
    int i, j, k, shift;
    for(i=0; i<2; i++){
        shift= posizione(nodo->string[i]);
        if(sim[i]=='+'){
            if(i==0) {
                for (j = 0; j < DIM; j++) {
                    for (k = 0; j!= shift && k < DIM; k++) {
                        vincoli.valido[j][k]=zeroValido;
                    }
                }
            }
            else{
                for (j = 0; j < DIM; j++) {
                    for (k = 0; k < DIM; k++) {
                        if(k!=shift)
                            vincoli.valido[j][k]=zeroValido;
                    }
                }
            }
        }
        else if(sim[i]=='/' && ricRes[shift]==0){
            vincoli.out[shift]=unoAltro;
            for(j=0; j<DIM; j++)
                vincoli.valido[shift][j]=zeroValido;
            for(j=0; j<DIM; j++)
                vincoli.valido[j][shift]=zeroValido;
        }
        else{
            if(i==0){
                for(j=0; j<DIM; j++)
                    vincoli.valido[shift][j]=zeroValido;
            }
            else {
                for (j = 0; j < DIM; j++)
                    vincoli.valido[j][shift] = zeroValido;
            }
        }
    }
}
void gestioneNormale(char *string, char *sim, char *res, short *ricRes){
    int i, shift, j;
    short ric[DIM], differenza[DIM];
    riempiRicorrenze(ric, string);
    for(i=0; i<DIM; i++)
        differenza[i]=ricRes[i]-ric[i];
    for(i=0; i<dimStringa; i++){
        shift= posizione(string[i]);
        if(ric[shift]!=dimStrpiu && ricRes[shift]!=0 && vincoli.ricorrenza[shift]==0){
            if(differenza[shift]>=0 && vincoli.ricorrenzaMin[shift]<ric[shift] && ricRes[shift]>0)
                vincoli.ricorrenzaMin[shift]=ric[shift];
            else if(differenza[shift]<0 && ricRes[shift]>0) {
                vincoli.ricorrenza[shift] = ricRes[shift];
                if(sim[i]=='/') {
                    if (i == 0) {
                        for (j = 0; j < DIM; j++)
                            vincoli.valido[shift][j] = zeroValido;
                    } else if (i == 1) {
                        for (j = 0; j < DIM; j++)
                            vincoli.valido[j][shift] = zeroValido;
                    } else
                        vincoli.noPosition[i - 2][shift] = unoAltro;
                }
            }
            ric[shift]=dimStrpiu;
        }
    }
    for(j=2; j<dimStringa; j++){
        shift= posizione(string[j]);
        if(sim[j]=='+')
            vincoli.in[j]=string[j];
        else if(sim[j]=='/' && ricRes[shift] == 0) {
            vincoli.out[shift] = unoAltro;
            for(i=0; i<DIM; i++) {
                vincoli.valido[i][shift] = zeroValido;
                vincoli.valido[shift][i]= zeroValido;
            }
        }
        else
            vincoli.noPosition[j-2][shift] = unoAltro;
    }
}
void aggiornaStrutturaVincolo(parole_t *nodo, char *sim, char *res, short *ricRes){
    gestioneTabella(sim, ricRes, nodo);
    gestioneNormale(nodo->string, sim, res, ricRes);
}

//Applicazione vincolo per aggiornamento parole
char aggiornoParolaSingola(parole_t *nodo) {
    int i, shift, j;
    short ric[DIM];
    riempiRicorrenze(ric, nodo->string);
    for (i = 2, j=0; i < dimStringa;i++, j++) {
        shift = posizione(nodo->string[i]);
        if ((vincoli.in[i] != '$' && vincoli.in[i] != nodo->string[i])||(*vincoli.out[shift] == '1')||(*vincoli.noPosition[j][shift] == '1')){
            nodo->val = zeroValido;
            return '0';
        }
    }
    for (i = 0; i < DIM; i++) {
        if ((vincoli.ricorrenza[i] != 0 && vincoli.ricorrenza[i] != ric[i]) ||
            (vincoli.ricorrenza[i] == 0 && vincoli.ricorrenzaMin[i] > 0 &&
             ric[i] < vincoli.ricorrenzaMin[i])) {
            nodo->val = zeroValido;
            return '0';
        }
    }
    return '1';
}
void aggiornoParoleValide(parole_t * nodo){
    if(nodo!=tnill){
        aggiornoParoleValide(nodo->sx);
        if( *(nodo->val)=='1' && aggiornoParolaSingola(nodo)=='1')
            cont++;
        aggiornoParoleValide(nodo->dx);
    }
}

int main() {
    char np[]="+nuova_partita";
    char ii[]="+inserisci_inizio";
    char iF[]="+inserisci_fine";
    char sf[]="+stampa_filtrate";

    short i, j, ricRes[DIM], SCI, shiftR, shiftC;
    int tentativi, dim;
    char *stringaIngresso, *sim, *res;
    parole_t *nodo;

    tnill= malloc(sizeof(parole_t));
    zeroValido=malloc(sizeof(char));
    zeroAltro=malloc(sizeof(char));
    unoAltro= malloc(sizeof(char));
    unoValido=malloc(sizeof(char));
    *zeroValido=*zeroAltro='0';
    *unoValido=*unoAltro='1';

    for(i=0; i<DIM; i++)
        for(j=0; j<DIM; j++)
            tabella[i][j]=tnill;

    SCI=scanf("%d", &dimStringa);
    if(dimStringa<18)
        dim=18;
    else
        dim=dimStringa+1;
    dimStrpiu=dimStringa+1;
    dimvet=dimStringa-2;

    stringaIngresso=malloc(sizeof(char)*dim);
    res=malloc(sizeof(char)*dimStrpiu);
    sim=malloc(sizeof(char)*dimStrpiu);
    sim[dimStringa]='\0';
    inizializza1();
    SCI=scanf("%s", stringaIngresso);
    while(fast_compare(stringaIngresso, np)!=0) {
        if (fast_compare(stringaIngresso, ii) == 0) {
            SCI = scanf("%s", stringaIngresso);
            while (fast_compare(stringaIngresso, iF) != 0) {
                nodo = malloc(sizeof(parole_t));
                nodo->string = malloc(sizeof(char) * (dimStrpiu));
                nodo->string[dimStringa] = '\0';
                fast_copy(nodo->string, stringaIngresso);
                shiftR = posizione(stringaIngresso[0]);
                shiftC = posizione(stringaIngresso[1]) ;
                nodo->val = unoValido;
                insertTree(&(tabella[shiftR][shiftC]), nodo);
                SCI = scanf("%s", stringaIngresso);
            }
            SCI = scanf("%s", stringaIngresso);
        } else {
            nodo = malloc(sizeof(parole_t));
            nodo->string = malloc(sizeof(char) * dimStrpiu);
            nodo->string[dimStringa] = '\0';
            fast_copy(nodo->string, stringaIngresso);
            shiftR = posizione(stringaIngresso[0]);
            shiftC = posizione(stringaIngresso[1]);
            nodo->val = unoValido;
            insertTree(&(tabella[shiftR][shiftC]), nodo);
            SCI = scanf("%s", stringaIngresso);
        }
    }

    SCI= scanf("%s", res);
    SCI= scanf("%d", &tentativi);
    riempiRicorrenze(ricRes, res);
    SCI= scanf("%s", stringaIngresso);
    while(1){
        while (1){
            if(fast_compare(stringaIngresso, np)==0){
                SCI= scanf("%s", res);
                SCI= scanf("%d", &tentativi);
                riempiRicorrenze(ricRes, res);
                for(i=0; i<DIM; i++) {
                    vincoli.ricorrenza[i] = 0;
                    vincoli.ricorrenzaMin[i] = 0;
                }
                for(i=0; i<dimStringa; i++)
                    vincoli.in[i]='$';
                *zeroValido='1';
                zeroValido= malloc(sizeof(char));
                *zeroValido='0';
                *unoAltro='0';
                unoAltro=malloc(sizeof(char));
                *unoAltro='1';
            }
            else if(fast_compare(stringaIngresso, ii)==0){
                SCI= scanf("%s", stringaIngresso);
                while(fast_compare(stringaIngresso, iF)!=0){
                    nodo=malloc(sizeof(parole_t));
                    nodo->string= malloc(sizeof(char)*(dimStrpiu));
                    nodo->string[dimStringa]='\0';
                    fast_copy(nodo->string, stringaIngresso);
                    shiftR= posizione(stringaIngresso[0]) ;
                    shiftC= posizione(stringaIngresso[1]) ;
                    nodo->val=unoValido;
                    insertTree(&(tabella[shiftR][shiftC]), nodo);
                    if(*vincoli.valido[shiftR][shiftC]=='1')
                        aggiornoParolaSingola(nodo);
                    SCI= scanf("%s", stringaIngresso);
                }
            }
            else if(fast_compare(stringaIngresso, sf)==0){
                for(i=0; i<DIM; i++) {
                    for (j = 0; j < DIM; j++) {
                        if (*vincoli.valido[i][j] == '1' && tabella[i][j]!=tnill)
                            stampaFiltrate(tabella[i][j]);
                    }
                }
            }
            else{
                nodo=cerca(stringaIngresso);
                if(nodo==tnill)
                    printf("not_exists\n");
                else if(fast_compare(nodo->string, res)==0){
                    printf("ok\n");
                    break;
                }
                else{
                    stringaSim(stringaIngresso, res, sim, ricRes);
                    printf("%s\n", sim);
                    aggiornaStrutturaVincolo(nodo, sim, res, ricRes);
                    cont=0;
                    for(i=0; i<DIM; i++) {
                        for (j = 0; j < DIM; j++) {
                            if (*vincoli.valido[i][j] == '1' && tabella[i][j]!=tnill) {
                                aggiornoParoleValide(tabella[i][j]);
                            }
                        }
                    }
                    printf("%d\n", cont);
                    tentativi--;
                    if(tentativi==0){
                        printf("ko\n");
                        break;
                    }
                }
            }
            SCI= scanf("%s", stringaIngresso);
        }
        SCI= scanf("%s", stringaIngresso);
        if(SCI==-1)
            break;
    }
    return 0;
}
