#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define FAKTOR_BAKETIRANJA 5
#define BROJ_BAKETA 7
#define FIKSNI_KORAK 1
#define MAX_NAZIV_PIVA 15
#define MAX_NAZIV_PROIZVODJACA 25
#define MAX_IME_DATOTEKE 20

#define LAGER 1
#define EJL 2
#define AKTUELAN 1
#define NEAKTUELAN 2
#define SLOBODNA_LOKACIJA 0



typedef struct{
    int rbrBaketa;
    int lokacijaUBaketu;
    int statusSloga;
}InfoOBaketu;

typedef struct{
    int dan;
    int mesec;
    int godina;
}Datum;

typedef struct{
    int evidencioniBroj;
    char nazivPiva[MAX_NAZIV_PIVA];
    Datum datumProizvodnje;
    Datum datumIsticanjaRokaTrajanja;
    float procenatAlkohola;
    int vrstaPiva;
    float cenaPiva;
    char nazivProizvodjaca[MAX_NAZIV_PROIZVODJACA];
    int statusSloga;
}Pivo;

typedef struct{
    Pivo pivo[FAKTOR_BAKETIRANJA];
}Baket;


//glavne metode iz menija
void formiranjeDatoteke();
void izborAktivneDatoteke(FILE **aktivnaDatoteka,char *imeAktivneDatoteke);
void prizakSvihPiva(FILE **aktivnaDatoteka);
void upisNovogSloga(FILE **aktuelnaDatoteka);
void potraziPivo(FILE **aktuelnaDatoteka);
void obrisiPivo(FILE **aktuelnaDatoteka);
void promeniProcenatAlkohola(FILE **aktuelnaDatoteka);





//metode koje ce se koristiti u drugim metodama
int metodPreklapanja(int evidencioniBroj);
void ucitajBaket(FILE **aktuelnaDatoteka,Baket* baket,int brojBaketa);
int unesiBaket(FILE **aktuelnaDatoteka,Baket *baket,int brojBaketa);
InfoOBaketu daLiPivoPostoji(FILE **aktuelnaDatoteka,int evidencioniBroj);



int main()
{
    int odabir=0;
    FILE *aktivnaDatoteka=NULL;
    char imeAktivneDatoteke[20];

    /*
        Ispis menija i omogucavanje odabira izvrsavanja odredjene funkcionalnosti
    */

    do{
        printf("* * * PROGRAM ZA EVIDENTIRANJE PIVA NA NOVOSADSKOM OKTOBERFESTU* * * * * * * * *\n");
        printf("*                                                                              *\n");
        printf("*                                                                              *\n");
        printf("*                                                                              *\n");
        printf("*\t\t   1. Formiranje prazne dadoteke                               *\n");
        printf("*\t\t   2. Izbor aktivne dadoteke                                   *\n");
        printf("*\t\t   3. Prikaz naziva aktivne dadoteke                           *\n");
        printf("*\t\t   4. Upis novog piva u atkivnu datoteku                       *\n");
        printf("*\t\t   5. Trazenje piva u aktivnoj datoteci                        *\n");
        printf("*\t\t   6. Prikaz svih piva aktivne datoteke                        *\n");
        printf("*\t\t   7. Obrisi aktuelni slog piva                                *\n");
        printf("*\t\t   8. Promeni procenat alkohola u slogu piva aktivne datoteke  *\n");
        printf("*\t\t   9. Izlaz iz programa                                        *\n");
        printf("*                                                                              *\n");
        printf("*                                                                              *\n");
        printf("* * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * \n");
        printf("Izaberite zeljenu opciju:");


        fflush(stdin);
        scanf("%d", &odabir);

        printf("\n\n");

        switch(odabir){
            case 1: {
                        formiranjeDatoteke();
                        break;
                    }
            case 2: {   izborAktivneDatoteke(&aktivnaDatoteka,imeAktivneDatoteke);
                        break;
                    }
            case 3: {   if(aktivnaDatoteka==NULL)
                            printf("\nNe postoji nijedna trenutno aktivna datoteka!\n\n");
                        else
                            printf("\nTrenutna aktivna datoteka je: ' %s '\n\n",imeAktivneDatoteke);
                        break;
                    }
            case 4: {   upisNovogSloga(&aktivnaDatoteka);
                        break;
                    }
            case 5: {   potraziPivo(&aktivnaDatoteka);
                        break;
                    }
            case 6: {   prizakSvihPiva(&aktivnaDatoteka);
                        break;
                    }
            case 7: {   obrisiPivo(&aktivnaDatoteka);
                        break;
                    }
            case 8: {   promeniProcenatAlkohola(&aktivnaDatoteka);
                        break;
                    }
            case 9: {   printf("\nUspesno zatvaranje programa!\n");
                        if(aktivnaDatoteka!=NULL){
                            fflush(aktivnaDatoteka);
                            fclose(aktivnaDatoteka);
                        }
                        exit(0);}
            default : { printf("Unesi ispravnu vrednost.\n\n");break;}
        }

    }
    while(1);


    return 0;
}



/*
    FORMIRANJE PRAZNE DATOTEKE
*/
void formiranjeDatoteke(){
    FILE *datoteka;
    char ime[20];

    printf("***********************\n");
    printf("* Formiranje datoteke *\n");
    printf("***********************\n");

    printf("\nIme datoteke: ");
    fflush(stdin);
    scanf("%[^\n]s", ime);

    datoteka=fopen(ime,"w+b");
    if(datoteka== NULL)
        printf("\nKreiranje nove datoteke ' %s ' nije uspelo.\n\n",ime);
    else{
        /*
            Ako je datoteka uspesno kreirana ubacujemo u nju
            bakete koji imaju u sebi neke pocetne vrednosti
        */

        int i;
        Baket baket;

        for(i=0;i<FAKTOR_BAKETIRANJA;i++){
            baket.pivo[i].cenaPiva=0;

            baket.pivo[i].datumIsticanjaRokaTrajanja.dan=0;
            baket.pivo[i].datumIsticanjaRokaTrajanja.mesec=0;
            baket.pivo[i].datumIsticanjaRokaTrajanja.godina=0;

            baket.pivo[i].datumProizvodnje.dan=0;
            baket.pivo[i].datumProizvodnje.mesec=0;
            baket.pivo[i].datumProizvodnje.godina=0;

            baket.pivo[i].evidencioniBroj=0;

            strncpy(baket.pivo[i].nazivPiva,"Nema",20);
            strncpy(baket.pivo[i].nazivProizvodjaca,"Nema",20);
            baket.pivo[i].procenatAlkohola=0.0;

            baket.pivo[i].vrstaPiva=0;
            baket.pivo[i].statusSloga=SLOBODNA_LOKACIJA;
        }

        /*
            Kada je baket inicijalizovan sledi njegov upis u datoteku
            tj niza baketa (kopija ovog inicijalizovanog) ciji je broj
            zadat kao BROJ_BAKETA
        */

        for(i=1;i<=BROJ_BAKETA;i++){
           unesiBaket(&datoteka,&baket,i);
        }


        printf("\nDatoteka ' %s ' je formirana!\n\n",ime);

    }
    fclose(datoteka);

}
/*
    Metoda koja ce od korisnika traziti da unese zeljeno ime
    datoteke koja ce biti aktivna i na osnovu imena cemo proveriti da li ona
    uopste postoji kako bi bila aktivna , ako postoji
    tu datoteku imenujemo sa aktivnom
*/

void izborAktivneDatoteke(FILE **aktivnaDatoteka,char *imeAktivneDatoteke){
    FILE *novaAktivnaDatoteka;
    char novoIme[MAX_IME_DATOTEKE];

    printf("**************************\n");
    printf("* Izbor aktivne datoteke *\n");
    printf("**************************\n");

    printf("\nUnesite ime datoteke za koju zelite da bude aktivna:");
    fflush(stdin);
    scanf("%[^\n]s",novoIme);

    novaAktivnaDatoteka=fopen(novoIme,"rb+");

    if(novaAktivnaDatoteka==NULL){
        printf("\nNije uspeo otvoriti zadatu datoteku\n\n");
        return;
    }
    else{
        //ako imamo neku predhodnu aktivnu datoteku, mi cemo je zatvoriti
        if((*aktivnaDatoteka)!=NULL)
            fclose((*aktivnaDatoteka));

        (*aktivnaDatoteka)=novaAktivnaDatoteka;
        strncpy((imeAktivneDatoteke),novoIme,MAX_IME_DATOTEKE);

        printf("\nDatoteka ' %s ' je sada nova aktivna datoteka!\n\n",novoIme);

    }
}


void prizakSvihPiva(FILE **aktivnaDatoteka){


    if((*aktivnaDatoteka)== NULL){
        printf("\nNema sta da se prikaze jer nemas aktivnu datoteku!\n\n");
        return;
    }
    printf("*********************\n");
    printf("* Prikaz liste piva *\n");
    printf("*********************\n");

    int pocetak=1;
    Baket baket;
    int i;

    for(pocetak =1;pocetak<=BROJ_BAKETA;pocetak=pocetak+ FIKSNI_KORAK){
        printf("\n\nBAKET %d : \n\n",pocetak);
        ucitajBaket(aktivnaDatoteka,&baket,pocetak);
        for(i=0;i<FAKTOR_BAKETIRANJA;i++){
            if(baket.pivo[i].statusSloga == AKTUELAN){
                printf("Lokacija %d\n",i);
                printf("Evidencioni broj: %d\n",baket.pivo[i].evidencioniBroj);
                printf("Naziv piva: %s \n",baket.pivo[i].nazivPiva);
                printf("Datum proizvodnje: %d.%d.%d.\n",baket.pivo[i].datumProizvodnje.dan,baket.pivo[i].datumProizvodnje.mesec,baket.pivo[i].datumProizvodnje.godina);
                printf("Datum isticanja roka: %d.%d.%d.\n",baket.pivo[i].datumIsticanjaRokaTrajanja.dan,baket.pivo[i].datumIsticanjaRokaTrajanja.mesec,baket.pivo[i].datumIsticanjaRokaTrajanja.godina);
                printf("Procenat alkohola: %f\n",baket.pivo[i].procenatAlkohola);
                if(baket.pivo[i].vrstaPiva==1)
                    printf("Vrsta piva: LAGER\n");
                else
                    printf("Vrsta piva: EJL\n");
                printf("Cena piva: %f\n",baket.pivo[i].cenaPiva);
                printf("Proizvodjac: %s\n\n",baket.pivo[i].nazivProizvodjaca);

            }
       }
    }
}





void upisNovogSloga(FILE **aktuelnaDatoteka){
    /*
      Pre svega prvo da proverimo da li postoji aktivna datoteka
    */

    if((*aktuelnaDatoteka)==NULL){
        printf("Ne postoji nijedna aktuelna datoteka, postavi neku datoteku za aktuelnu pa probaj ponovo.\n\n");
        return;
    }

    printf("*******************\n");
    printf("* Upis novog piva *\n");
    printf("*******************\n");

    Pivo pivo;

    /*
     Deo koda koji ispituje tacnost unesenog evidencionog broja
     kao i njegov upis u slog Pivo, ako je tacan
    */
    int ispravan=1;
    int pokusajPonovo=0;
    InfoOBaketu info;
    do{
        do{
            ispravan=1;
            printf("\nUnesi sestocifreni evidencioni broj:");
            fflush(stdin);
            scanf("%d",&pivo.evidencioniBroj);
            char str[10];
            sprintf(str, "%d", pivo.evidencioniBroj);
            if(pivo.evidencioniBroj<100000 || pivo.evidencioniBroj>999999 || str[0]=='0'){
                printf("\nBroj mora biti sestocifren!(Prva cifra mora biti nenula vrednost)\n\n");
                ispravan=0;
            }
        }while(ispravan==0);
        /*
            Ako je ispravan evidencioni broj, sada moramo da proverimo da li vec postoji
        */
         info=daLiPivoPostoji(aktuelnaDatoteka,pivo.evidencioniBroj);
         if(info.statusSloga == AKTUELAN){
            printf("\n\nSlog sa tim evidencionim brojem(%d) odgovara vec jednom postojecem slogu.\n",pivo.evidencioniBroj);
            printf("\nDa li zelis opet da pokusas uneti evidencioni broj?\n\n(1-DA,(OSTALA DUGMAD)-NE) : ");
            fflush(stdin);
            scanf("%d",&pokusajPonovo);
            if(pokusajPonovo==0)
                return;
         }
    }while(pokusajPonovo==1);

    /*
        Ubacivanje naziva piva u slog Pivo
    */
    char unetiString[100];
    do{
        ispravan=1;
        printf("\nUnesite ime piva: ");
        fflush(stdin);
        scanf("%[^\n]s",unetiString);
        if(strlen(unetiString) > MAX_NAZIV_PIVA ){
            printf("\nPrekoracio si maksimalni naziv za pivo,probaj opet.\n\n");
            ispravan=0;
        }
    }while(ispravan==0);
    strcpy(pivo.nazivPiva,unetiString);

    /*
        Ubacivanje datuma proizvodnje
    */
    int mesec=-1;
    int dan=-1;
    int godina=-1;

    ispravan=1;
    printf("\nUnos datuma proizvodnje:\n");
    do{
        ispravan=1;
        printf("\nUnesi dan(1-31):");
        fflush(stdin);
        scanf("%d",&dan);
        if(dan<1 || dan > 31){
            printf("\nPrekoracio si opseg za vrednost dana u mesecu!(Opseg je 1-31)\n");
            ispravan=0;
        }
    }while(ispravan==0);
    pivo.datumProizvodnje.dan=dan;

    do{
        ispravan=1;
        printf("\nUnesi mesec(1-12):");
        fflush(stdin);
        scanf("%d",&mesec);
        if(mesec<1 || mesec > 12){
            printf("\nPrekoracio si opseg za vrednost meseca u godini!(Opseg je 1-12)\n");
            ispravan=0;
        }
    }while(ispravan==0);
    pivo.datumProizvodnje.mesec=mesec;

    do{
        ispravan=1;
        printf("\nUnesi godinu(1980+):");
        fflush(stdin);
        scanf("%d",&godina);
        if(godina<1980){
            printf("\nOtisao si ispod opsega,pivo je previse staro!\n\n");
            ispravan=0;
        }
    }while(ispravan==0);
    pivo.datumProizvodnje.godina=godina;


    /*
        Upisivanje datuma roka isticanja trajanja
    */

    ispravan=1;
    printf("\nUnos datuma roka isticanja:\n");

    do{
        ispravan=1;
        printf("\nUnesi godinu(1980+):");
        fflush(stdin);
        scanf("%d",&godina);
        if(godina<1980){
            printf("\nOtisao si ispod opsega,pivo je previse staro!\n\n");
            ispravan=0;
        }
        if(godina<pivo.datumProizvodnje.godina){
            printf("\nNe moze rok isticanja roka da bude manji od datuma proizvodnje!\n\n");
            ispravan=0;
        }
    }while(ispravan==0);
    pivo.datumIsticanjaRokaTrajanja.godina=godina;


    do{
        ispravan=1;
        printf("\nUnesi mesec(1-12):");
        fflush(stdin);
        scanf("%d",&mesec);
        if(mesec<1 || mesec > 12){
            printf("\nPrekoracio si opseg za vrednost meseca u godini!(Opseg je 1-12)\n");
            ispravan=0;
        }
        if(pivo.datumIsticanjaRokaTrajanja.godina==pivo.datumProizvodnje.godina && mesec<pivo.datumProizvodnje.mesec){
            printf("\nNe moze rok isticanja roka da bude manji od datuma proizvodnje!\n\n");
            ispravan=0;
        }
    }while(ispravan==0);
    pivo.datumIsticanjaRokaTrajanja.mesec=mesec;

    do{
        ispravan=1;
        printf("\nUnesi dan(1-31):");
        fflush(stdin);
        scanf("%d",&dan);
        if(dan<1 || dan > 31){
            printf("\nPrekoracio si opseg za vrednost dana u mesecu!(Opseg je 1-31)\n");
            ispravan=0;
        }
        if(pivo.datumIsticanjaRokaTrajanja.godina==pivo.datumProizvodnje.godina && pivo.datumIsticanjaRokaTrajanja.mesec==pivo.datumProizvodnje.mesec && dan<pivo.datumProizvodnje.dan){
            printf("\nNe moze rok isticanja roka da bude manji od datuma proizvodnje!\n\n");
            ispravan=0;
        }
    }while(ispravan==0);
    pivo.datumIsticanjaRokaTrajanja.dan=dan;

    /*
        Unos procenta alkohola
    */

    float procenatAlkohola=0.0;
    do{
        ispravan=1;
        printf("\nUnesi procenat alkohola:");
        fflush(stdin);
        scanf("%f.2",&procenatAlkohola);
        if(procenatAlkohola<0.0 || procenatAlkohola>100.0){
            printf("Uneli ste pogresan procenat alkohola!(procenti se krecu 1-100)\n\n");
            ispravan=0;
        }
    }while(ispravan==0);
    pivo.procenatAlkohola=procenatAlkohola;

    /*
        Unos vrste piva
    */
    int vrsta=0;
    do{
        ispravan=1;
        printf("\nUnesi vrstu piva: 1.LAGER, 2.EJL  *");
        fflush(stdin);
        scanf("%d",&vrsta);
        if(vrsta!=1 && vrsta!=2){
            printf("\nNiste uneli pravilnu vrednost, unesite 1 ili 2!");
            ispravan=0;
        }
    }while(ispravan==0);
    pivo.vrstaPiva=vrsta;

    /*
        Unos cene piva
    */
    int cena=-1;
    do{
        ispravan=1;
        printf("\nUnesi cenu piva:");
        fflush(stdin);
        scanf("%d",&cena);
        if(cena<0){
            printf("\nNe moze cena biti manja od 0!Pokusaj ponovo");
            ispravan=0;
        }
    }while(ispravan==0);
    pivo.cenaPiva=cena;


    /*
        Ubacivanje naziva proizvodjaca u slog Pivo
    */
    do{
        ispravan=1;
        printf("\nUnesite ime proizvodjaca piva: ");
        fflush(stdin);
        scanf("%[^\n]s",unetiString);
        if(strlen(unetiString) > MAX_NAZIV_PROIZVODJACA ){
            printf("\nPrekoracio si maksimalni naziv za proizvodjaca piva, probaj opet.\n\n");
            ispravan=0;
        }
    }while(ispravan==0);
    strcpy(pivo.nazivProizvodjaca,unetiString);


    /*
        Postavljanje statusa sloga pivo na SLOBODNU_LOKACIJU
    */

    pivo.statusSloga=AKTUELAN;

    int maticniBaket;
    int tekuciBaket;
    Baket baket;
    int i=0;
    int uspeoUpisati=0;

    maticniBaket = tekuciBaket = metodPreklapanja(pivo.evidencioniBroj);

    do{
        ucitajBaket(aktuelnaDatoteka,&baket,tekuciBaket);

        /*
            Ako smo u prosloj pretragi nasli slog sa vrednosti kljuca
            od naseg piva, a da je on neaktuelan dovoljno je smao da ubacimo
            vrednosti piva u taj slog ako se evidencioni brojevi poklapaju
        */
        if(info.statusSloga==NEAKTUELAN){
                if(baket.pivo[info.lokacijaUBaketu].evidencioniBroj==pivo.evidencioniBroj){
                    baket.pivo[info.lokacijaUBaketu]=pivo;
                    uspeoUpisati=1;
                }
        }else{
          /*
            Ako slog u onoj pretrazi nije bio neaktuelan
            on je onda slobodan pa moramo da vrsimo pretragu
            na prvo slobodno mesto od naseg maticnog baketa
          */
          for(i=0;i<FAKTOR_BAKETIRANJA;i++){
            if(baket.pivo[i].statusSloga==SLOBODNA_LOKACIJA){
                baket.pivo[i]=pivo;
                info.lokacijaUBaketu=i;
                uspeoUpisati=1;
                break;
            }
          }
        }

        if(uspeoUpisati==0) {
            /*
                Ako nije uspeo upisati znaci da u maticnom baketu nema mesta
                te je nas slog PREKORACILAC, i posto je nama korak K==1 mi gledamo odma
                naredni baket, a ako je nas baket zadnji redni broj mi skacemo na prvi u nizu
            */
            tekuciBaket=tekuciBaket+FIKSNI_KORAK;

            if(tekuciBaket>BROJ_BAKETA)
                tekuciBaket=tekuciBaket%BROJ_BAKETA;
        }
    }while(uspeoUpisati==0 && tekuciBaket!=maticniBaket);
    //ako dodje do maticnog baketa znaci da nigde nema mesta i da upis nije uspeo

    int uspesnoUpisanBaket=1;


    if(uspeoUpisati !=0){
        uspesnoUpisanBaket=unesiBaket(aktuelnaDatoteka,&baket,tekuciBaket);

       if(uspesnoUpisanBaket==1){
        if(tekuciBaket!=maticniBaket)
            printf("\nPivo je uspesno upisano u baket sa rbr %d na lokaciji %d , dok je njegov maticni baket  sa rbr %d\n\n",tekuciBaket,info.lokacijaUBaketu,maticniBaket);
        else
            printf("\nPivo je uspesno upisano u baket sa rbr %d na lokaciji %d\n\n",tekuciBaket,info.lokacijaUBaketu);
       }else
            printf("\nNeuspeli upis piva u baket!\n\n");


    }else
        printf("\nDatoteka je puna pa zato slog ne moze da se upise\n\n");


}


void potraziPivo(FILE **aktuelnaDatoteka){
    /*
        Prvo proveravamo da li uopste postoji aktuelna datoteka
    */
    if((*aktuelnaDatoteka) == NULL){
        printf("Ne postoji nijedna aktuelna datoteka, postavi neku datoteku za aktuelnu pa probaj ponovo.\n\n");
        return;
    }

    printf("******************\n");
    printf("* Trazenje sloga *\n");
    printf("******************\n");
    printf("\n\nUnesi evidencioni broj: ");

    int evidencioniBroj;
    scanf("%d",&evidencioniBroj);
    char str[10];
    sprintf(str, "%d", evidencioniBroj);
    if(evidencioniBroj<100000 || evidencioniBroj>999999 || str[0]=='0'){
        printf("Neispravan evidencioni broj!\n\n");
        return;
    }

    InfoOBaketu info;
    Baket baket;

    info=daLiPivoPostoji(aktuelnaDatoteka,evidencioniBroj);
    if(info.statusSloga==1){
        printf("BLOK %d\n",info.rbrBaketa);
        printf("Lokacija %d\n",info.lokacijaUBaketu);
        ucitajBaket(aktuelnaDatoteka,&baket,info.rbrBaketa);

        printf("Evidencioni broj: %d\n",baket.pivo[info.lokacijaUBaketu].evidencioniBroj);
        printf("Naziv piva: %s \n",baket.pivo[info.lokacijaUBaketu].nazivPiva);
        printf("Datum proizvodnje: %d.%d.%d.\n",baket.pivo[info.lokacijaUBaketu].datumProizvodnje.dan,baket.pivo[info.lokacijaUBaketu].datumProizvodnje.mesec,baket.pivo[info.lokacijaUBaketu].datumProizvodnje.godina);
        printf("Datum isticanja roka: %d.%d.%d.\n",baket.pivo[info.lokacijaUBaketu].datumIsticanjaRokaTrajanja.dan,baket.pivo[info.lokacijaUBaketu].datumIsticanjaRokaTrajanja.mesec,baket.pivo[info.lokacijaUBaketu].datumIsticanjaRokaTrajanja.godina);
        printf("Procenat alkohola: %f\n",baket.pivo[info.lokacijaUBaketu].procenatAlkohola);
        if(baket.pivo[info.lokacijaUBaketu].vrstaPiva==1)
            printf("Vrsta piva: LAGER\n");
        else
            printf("Vrsta piva: EJL\n");
        printf("Cena piva: %f\n",baket.pivo[info.lokacijaUBaketu].cenaPiva);
        printf("Proizvodjac: %s\n\n",baket.pivo[info.lokacijaUBaketu].nazivProizvodjaca);

        return;
    }else{
        printf("\nZadato pivo nije pronadjeno !!\n\n");
        return;
    }

}

void obrisiPivo(FILE **aktuelnaDatoteka){
    /*
        Prvo proveravamo da li ima uopste aktivne datoteke
    */

    if((*aktuelnaDatoteka)==NULL){
        printf("Ne postoji nijedna aktuelna datoteka, postavi neku datoteku za aktuelnu pa probaj ponovo.\n\n");
        return;
    }

    printf("******************\n");
    printf("* Brisanje piva  *\n");
    printf("******************\n");
    printf("\n\nUnesi evidencioni broj piva koje bi obrisao: ");

    int evidencioniBroj;
    scanf("%d",&evidencioniBroj);
    char str[10];
    sprintf(str, "%d", evidencioniBroj);
    if(evidencioniBroj<100000 || evidencioniBroj>999999 || str[0]=='0'){
        printf("Neispravan evidencioni broj!\n\n");
        return;
    }


    InfoOBaketu info;
    Baket baket;

    info=daLiPivoPostoji(aktuelnaDatoteka,evidencioniBroj);

    if(info.statusSloga==1){
        ucitajBaket(aktuelnaDatoteka,&baket,info.rbrBaketa);
        baket.pivo[info.lokacijaUBaketu].statusSloga=NEAKTUELAN;
        unesiBaket(aktuelnaDatoteka,&baket,info.rbrBaketa);
        printf("\nPivo sa evidencionim brojem: %d je uspesno logicki obrisano!\n\n",evidencioniBroj);
        return;
    }else{
        printf("\nNema zadatog piva za brisanje!\n\n");
        return;
    }


}


void promeniProcenatAlkohola(FILE **aktuelnaDatoteka){
    /*
        Prvo proveravamo da li ima uopste aktivne datoteke
    */
    if((*aktuelnaDatoteka)==NULL){
        printf("Ne postoji nijedna aktuelna datoteka, postavi neku datoteku za aktuelnu pa probaj ponovo.\n\n");
        return;
    }

    printf("****************************\n");
    printf("* Izmena procenta alkohola *\n");
    printf("****************************\n");

    printf("\n\nUnesi evidencioni broj piva kojem bi promenio procenat alkohola: ");

    int evidencioniBroj;
    scanf("%d",&evidencioniBroj);
    char str[10];
    sprintf(str, "%d", evidencioniBroj);
    if(evidencioniBroj<100000 || evidencioniBroj>999999 || str[0]=='0'){
        printf("Neispravan evidencioni broj!\n\n");
        return;
    }

    InfoOBaketu info;
    Baket baket;

    info=daLiPivoPostoji(aktuelnaDatoteka,evidencioniBroj);

    if(info.statusSloga==1){
        ucitajBaket(aktuelnaDatoteka,&baket,info.rbrBaketa);
        printf("\n\nUnesi zeljeni procenat alkohola za pivo sa evidencionim brojem ' %d ' : ",evidencioniBroj);
        float procenat = 0.0;
        scanf("%f",&procenat);
        if(procenat<0.0 || procenat>100.0){
            printf("\nNije ispravan procenat alkohola!Nista od izmene!\n\n");
            return;
        }
        baket.pivo[info.lokacijaUBaketu].procenatAlkohola=procenat;
        unesiBaket(aktuelnaDatoteka,&baket,info.rbrBaketa);
        printf("\n\nUspesno je promenjen procenat alkohola na pivu sa evidencionim brojem: %d \n\n",evidencioniBroj);
        return;
    }else{
        printf("\nNema zadatog piva za izmenu procenta alkohola!\n\n");
        return;
    }
}






//FUNKCIJE KOJE SLUZE KAO POTPOMOC GLAVNIM FUNKCIONALNOSTIMA
int metodPreklapanja(int evidencioniBroj){
    /* posto je 6ocifreni broj onda je p=6, a posto je decimalni broj
        v=10, n=1 (jer je broj baketa 7)
        p/n=6, znaci trebace mi 6 segmenata kako bih mogao raspodeliti
        medju bakete,i nece biti obrcanja redosleda svakog drugo, jer je duzina segmenta samo
        1 pa cemo samo sumirati.
    */
    int suma=0;
    int broj=evidencioniBroj;
    while(broj != 0){
        suma +=broj%10;
        broj/=10;
    }
    int T=suma%10;
    int adresaBaketa=1 + (BROJ_BAKETA*T/10);

    return adresaBaketa;

}

void ucitajBaket(FILE **aktuelnaDatoteka,Baket* baket,int brojBaketa) {
	int greska_seek=-1;
	int greska_read=-1;

	greska_seek = fseek((*aktuelnaDatoteka), (brojBaketa-1)*sizeof(Baket), SEEK_SET);
	if(greska_seek != 0) {
		printf("Greska kod seekovanja datoteke\n\n");
		return;
	}

	greska_read = fread(baket, sizeof(Baket), 1, (*aktuelnaDatoteka));
	if(greska_read != 1) {
		printf("Greska kod citanja baketa iz datoteke\n\n");
		return;
	}
}

int unesiBaket(FILE **aktuelnaDatoteka,Baket *baket,int brojBaketa){

    int greska_seek=-1;
    int greska_write=-1;

    greska_seek= fseek((*aktuelnaDatoteka),(brojBaketa-1)*sizeof(Baket),SEEK_SET);
    if(greska_seek != 0){
        printf("Greska kod seekovanja datoteke\n\n");
        return 0;
    }

    greska_write=fwrite(baket,sizeof(Baket),1,(*aktuelnaDatoteka));
    if(greska_write !=1){
        printf("Greska kod citanja baketa iz datoteke\n\n");
        return 0;
    }
    return 1;
}




InfoOBaketu daLiPivoPostoji(FILE **aktuelnaDatoteka,int evidencioniBroj) {

    int maticniBaket=metodPreklapanja(evidencioniBroj);
    int tekuciBaket=metodPreklapanja(evidencioniBroj);

    InfoOBaketu info;

    Baket baket;
    int i;
    do {
        ucitajBaket(aktuelnaDatoteka,&baket, tekuciBaket);
        for(i=0; i<FAKTOR_BAKETIRANJA; i++) {
            if(baket.pivo[i].statusSloga == NEAKTUELAN && baket.pivo[i].evidencioniBroj == evidencioniBroj) {
                info.rbrBaketa= tekuciBaket;
                info.lokacijaUBaketu= i;
                info.statusSloga=NEAKTUELAN;
                return info;
            }
            else if(baket.pivo[i].statusSloga  == AKTUELAN && baket.pivo[i].evidencioniBroj == evidencioniBroj) {
                info.rbrBaketa= tekuciBaket;
                info.lokacijaUBaketu= i;
                info.statusSloga=AKTUELAN;
                return info;
            }
        }

        tekuciBaket += FIKSNI_KORAK;

        if(tekuciBaket > BROJ_BAKETA)
            tekuciBaket = tekuciBaket % BROJ_BAKETA;

    }  while(maticniBaket != tekuciBaket);

    info.rbrBaketa=maticniBaket;
    info.lokacijaUBaketu=0;
    info.statusSloga=SLOBODNA_LOKACIJA;

    return info;

}





