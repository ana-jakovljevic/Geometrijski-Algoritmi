#include "ga14_coinsOnShelf.h"

CoinsOnShelf::CoinsOnShelf(QWidget *pCrtanje, int pauzaKoraka, const bool &naivni, std::string imeDatoteke, int brojDiskova)
    :AlgoritamBaza(pCrtanje, pauzaKoraka, naivni)
{
    // If disk sizes are not provided from file, than generate random sizes
    if(imeDatoteke == "") {
        for(int j = 0; j < brojDiskova; ++j) {
            Disk* noviDisk = new Disk();
            _discs.push_back(noviDisk);
        }
    }
    // Else, open file and load disk number and sizes from there
    else {
        QString fileName = QString::fromStdString(imeDatoteke);

        QFile inputFile(fileName);
        if(inputFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&inputFile);

            int discNumber = in.readLine().toInt();

            for(int j = 0; j < discNumber; ++j) {
                Disk* noviDisk = new Disk();
                noviDisk->setSize(in.readLine().toDouble());
                _discs.push_back(noviDisk);
            }
        }
        else {
            qDebug() << "ERROR OPENING FILE";
            exit(1);
        }
    }
    _n = _discs.size();
    std::sort(_discs.begin(), _discs.end(), compClass());

    // Now we have disks loaded, so we decide if go with special case or general
    float max = _discs[0]->size();
    float min = _discs[_n-1]->size();

    if(max/min < 2.0)
        _algorithm = SPECIAL;
    else
        _algorithm = GENERAL;

}

CoinsOnShelf::~CoinsOnShelf()
{
    for(auto disk : _discs)
        delete disk;
}

void CoinsOnShelf::pokreniAlgoritam()
{
    while(_i < _n) {
        if(_algorithm == SPECIAL) {

        }
        if(_algorithm == GENERAL) {

        }
    }

    emit animacijaZavrsila();
}

void CoinsOnShelf::crtajAlgoritam(QPainter *painter) const
{
    // TODO
    painter->viewport();
}

void CoinsOnShelf::pokreniNaivniAlgoritam()
{
    // TODO
}

void CoinsOnShelf::crtajNaivniAlgoritam(QPainter *painter) const
{
    // TODO
    painter->viewport();
}

Disk::Disk()
{
    _size = _randomGen.bounded(5.0);
}

double Disk::size()
{
    return _size;
}

void Disk::setSize(double newSize)
{
    _size = newSize;
}
