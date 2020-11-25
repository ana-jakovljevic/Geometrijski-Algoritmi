# Geometrijski algoritmi @ MATF

Projekat za predmet **Geometrijski algoritmi** na master studijama na Matematičkom fakultetu.

Profesor: [dr Predrag Janičić](www.matf.bg.ac.rs/~janicic)

Asistent: [dr Danijela Simić](www.matf.bg.ac.rs/~danijela)


## Uputstvo za studente

**Inicijalna podešavanja**

   * **Instalirati Qt i git (detaljnije na dnu)**
   * **Napraviti nalog na bitbucket-u**
   * **Upisati nalog u [tabelu](https://docs.google.com/spreadsheets/d/1S-QJQNRnt59KoZLHLpdVfhZbjv8NLtsucuAXb112wHg/edit?usp=sharing)**, bez obzira na to što još uvek nemate izabrane projekte, upišite ime, prezime i korisničko ime
   * **Napraviti folder na lokalnom računaru**, pozicionirati se u folder i klonirati repozitorijum komandom ```git clone https://bitbucket.org/geoalg2021/velikiseminarski```
   * Sifra za `knjige.zip` je _geometrijskialgoritmi_

[Link do šablona za projektnu dokumentaciju.](https://docs.google.com/document/d/1m8cPjdLozvlJ_rWD65epozbCq894KYWkZ6jXciUtN38/edit?usp=sharing)

**Pre početka potrebno je napraviti granu na kojoj će biti projekat.** To se može postići na sledeći način:
```
git checkout -b ime_grane
```
Naziv `ime_grane` treba da odgovara nazivu projekta, npr. `ga03_ImeProjekta`. Nakon napravljenih izmena, izmene se mogu podići na repozitorijum:
```
git add <files>
git commit -m 'opis izmena'
git push origin ime_grane:ime_grane
```

**Struktura direktorijuma**

   * algoritmi_sa_vezbi [Sadrži primere sa časova vežbi]
      * ga00_alg1.cpp
      *  ga00_alg1.h
      *  ...
      *  ...
      *  gaXY_algXY.cpp
      *  gaXY_algXY.h
   * algoritmi_studentski_projekti [Sadrži studentske projekte]
      *  ga00_emptyProject.cpp
      *  ga00_emptyProject.h
      *  ga01_temaPrvogStudentaIzTabele.cpp
      *  ga01_temaPrvogStudentaIzTabele.h
      *  ga02_temaDrugogStudentaIzTabele.cpp
      *  ga02_temaDrugogStudentaIzTabele.h
      *  ...
      *  ...
   * docs [Sadrži projektnu dokumentaciju]
      *  ga00_convexHull.docx
      *  ga00_convexHull.pdf
      *  ga01_temaPrvogStudenta.docx
      *  ga01_temaPrvogStudenta.pdf
      *  ...
      *  ...
      *  ga25_temaPoslednjegStudenta.docx
      *  ga25_temaPoslednjegStudenta.pdf
   * input_files [Sadrži foldere sa raznim unapred zadatim ulazima za Vaše algoritme]
      *  ga01_temaPrvogStudenta
      *  ...
      *  ga25_temaPoslednjegStudenta

## Primer podešavanja

Primer podešavanja (preuzimanja) git-a za Ubuntu:

  * sudo apt install git

Preuzimanje repozitorijuma i Google testova:

  * git clone https://bitbucket.org/geoalg2021/velikiseminarski
  * cd velikiseminarski
  * git clone https://github.com/google/googletest

Prezentacija o jediničnim testovima:

  * http://poincare.matf.bg.ac.rs/~asimic/2017/ga/7/UnitTests.pdf

Povezivanje sa Atlassian nalogom:

  * git config user.name "korisničko ime iz tabele"
  * git config user.email "povezani mejl iz tabele"

Primer podešavanja (preuzimanja) Qt-a za Ubuntu:

   * sudo apt install build-essential
   * sudo apt install qtcreator
   * sudo apt install qt5-default

Ako charts pravi problem:

   * sudo apt install libqt5charts5 libqt5charts5-dev

Ako okruženje prijavljuje lažna upozorenja:

   * sudo apt install libclang-common-8-dev

