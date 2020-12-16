#include "ga14_coinsOnShelf.h"

CoinsOnShelf::CoinsOnShelf(QWidget *pCrtanje, int pauzaKoraka, const bool &naivni, std::string imeDatoteke, int brojDiskova)
    :AlgoritamBaza(pCrtanje, pauzaKoraka, naivni)
{
    if(imeDatoteke == "") {
        for(int j = 0; j < brojDiskova; ++j) {
            Disk* noviDisk = new Disk();
            _diskoviSort.insert(noviDisk);
        }
    }
    else {
        QString fileName = QString::fromStdString(imeDatoteke);

        QFile inputFile(fileName);
        if(inputFile.open(QIODevice::ReadOnly)) {
            QTextStream in(&inputFile);

            int discNumber = in.readLine().toInt();

            for(int j = 0; j < discNumber; ++j) {
                Disk* noviDisk = new Disk();
                noviDisk->setSize(in.readLine().toDouble());
                _diskoviSort.insert(noviDisk);
            }
        }
        else {
            qDebug() << "ERROR OPENING FILE";
            exit(1);
        }
    }

}

CoinsOnShelf::~CoinsOnShelf()
{
    for(auto disk : _diskoviSort)
        delete disk;
}

void CoinsOnShelf::pokreniAlgoritam()
{

    // TODO
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
