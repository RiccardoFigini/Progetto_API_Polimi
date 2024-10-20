## Prova finale Progetto di Algoritmi e Strutture Dati - a.a - 2021-2022
Qeesta è la repository che comprende il progetto del corso di Algoritmi e Principi dell'Informatica (algoritmi e strutture dati), anno 2021/22 del Politecnico di Milano.

## Struttura della repository
Testo progetto: PDF in cui è presentata la traccia del progetto.
main.c: File .C contenente il codice del progetto.
Input output test: Cartella contenente alcuni file di test utilizzati durante lo sviluppo.
readme.md e readme_images: FIle di intestazione della repository.

## Deployment
Il codice può essere testato con un qualsiasi IDE per file.c, bisogna configurare l'IDE in modo che fornisca uno dei file nella cartella Tests al posto del file stdin. Alternativamente può essere usato anche il file stdin ma il progetto è ideato per considerare grandi quantità di parole.

## Idea alla base
Il progetto richiede di memorizzare grandi quantità di parole (anche diverse centinaia di migliaia) e soprattutto di ordinarle in ordine lessicografico. L'idea iniziale è stata quella di creare un albero rosso nero (in modo da avere una struttura bilanciata) e assegnare ad ogni parola un flag che ne indicasse la compatibilità con i vincoli appresi dai tentativi
