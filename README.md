# Geometrijski algoritmi @ MATF

Projekat za predmet **Geometrijski algoritmi** na master studijama na Matematičkom fakultetu.

Profesor: [dr Predrag Janičić](www.matf.bg.ac.rs/~janicic)

Asistent: [dr Danijela Simić](www.matf.bg.ac.rs/~danijela)


## Uputstvo za studente

**Inicijalna podešavanja**

   * **Instalirati Qt i git (detaljnije na dnu)**
   * **Napraviti nalog na bitbucket-u**
   * **[Upisati nalog u tabelu:](https://docs.google.com/spreadsheets/d/1S-QJQNRnt59KoZLHLpdVfhZbjv8NLtsucuAXb112wHg/edit?usp=sharing)** (bez obzira što još uvek nemate izabrane projekte, upišite ime, prezime i korisničko ime)
   * **Napraviti folder na lokalnom računaru**, pozicionirati se u folder i klonirati repozirotijum komandom ```git clone https://bitbucket.org/geoalg2021/velikiseminarski```

[Link do šablona za projektnu dokumentaciju.](https://docs.google.com/document/d/1m8cPjdLozvlJ_rWD65epozbCq894KYWkZ6jXciUtN38/edit?usp=sharing)

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

## Osnovne komande

   * **git pull - da prevučete trenutni kod**
   * **git add \<fajlovi koje hocete da dodate>**
   * **git commit -m 'Poruka koja opisuje to sto se odradili'**
   * **git push origin master - da posaljete svoje izmene na server**

_**Svaki put kada nešto radite na projektu, odradite pull komandu pre nego što počnete (kako biste uvek radili sa najnovijom verzijom koda). **_

_**Potrebno je da podizete samo .cpp, .hpp/.h, .pro (ne .pro.user* niti bilo sta slicno) i .ui fajlove, NIKAKO ne podizati QtBuild fajlove **_

_**Da bi se pokretali Google testovi neophodno je da u root direktorijumu projekta imate klonirane googletest (git clone https://github.com/google/googletest.git) **_

## Primer podešavanja

Primer podešavanja (preuzimanja) okruženja za Ubuntu:
   * sudo apt-get install build-essential
   * sudo apt-get install qtcreator
   * sudo apt-get install qt5-default

Ako charts pravi problem:
   * sudo apt install libqt5charts5 libqt5charts5-dev

Ako okruženje prijavljuje lažna upozorenja:
   * sudo apt install libclang-common-8-dev

