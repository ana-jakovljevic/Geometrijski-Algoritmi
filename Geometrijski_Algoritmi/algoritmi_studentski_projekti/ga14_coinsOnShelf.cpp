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
        // TODO: Unos diskova iz datoteke
    }

}

CoinsOnShelf::~CoinsOnShelf()
{
    for(auto disk : _diskovi)
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
