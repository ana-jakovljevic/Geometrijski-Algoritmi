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
                noviDisk->setRadius(in.readLine().toDouble());
                _discs.push_back(noviDisk);
            }
        }
        else {
            qDebug() << "ERROR OPENING FILE";
            exit(1);
        }
    }
    _n = _discs.size();
    std::sort(_discs.begin(), _discs.end(), compDiscs());

    // Now we have disks loaded, so we decide if go with special case or general
    float max = _discs[0]->radius();
    float min = _discs[_n-1]->radius();

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
    if(_algorithm == SPECIAL && (_n % 2 == 0)) {
        specialCaseEvenDiscs();
    }

    if(_algorithm == SPECIAL && (_n % 2 == 1)) {
        specialCaseOddDiscs();
    }

    if(_algorithm == GENERAL) {
        generalCase();
    }

    debugShelf();

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

void CoinsOnShelf::specialCaseEvenDiscs()
{
    _shelf.push_back(_discs[0]);
    int j = 1;

    while(true) {
        if(_shelf.back()->radius() == _discs[_n - j]->radius())
            break;
        _shelf.push_back(_discs[_n - j]);
        if(_shelf.back()->radius() == _discs[j]->radius())
            break;
        _shelf.push_back(_discs[j]);
        j += 2;
    }

    j = 2;
    while(true) {
        if(_shelf.front()->radius() == _discs[_n - j]->radius())
            break;
        _shelf.push_front(_discs[_n - j]);
        if(_shelf.front()->radius() == _discs[j]->radius())
            break;
        _shelf.push_front(_discs[j]);
        j += 2;
    }
}

void CoinsOnShelf::specialCaseOddDiscs()
{
    _shelf.push_back(_discs[0]);
    int j = 2;
    unsigned i = 0;

    while(true) {
        if(i < (_n/2 - 1)) {
            _shelf.push_front(_discs[_n - j]);
            ++i;
        }
        else break;
        if(i < (_n/2 - 1)) {
            _shelf.push_front(_discs[j]);
            ++i;
        }
        else break;
        j += 2;
    }

    _shelf.push_back(_discs[1]);
    j = 3;
    i = 0;
    while(true) {
        if(i < (_n/2 - 1)) {
            _shelf.push_back(_discs[_n - j]);
            ++i;
        }
        else break;
        if(i < (_n/2 - 1)) {
            _shelf.push_back(_discs[j]);
            ++i;
        }
        else break;
        j += 2;
    }

    if(_shelf.back()->radius() > _shelf.front()->radius()) {
        _shelf.push_back(_discs[_n-1]);
    }
    else {
        _shelf.push_front(_discs[_n-1]);
    }
}

void CoinsOnShelf::generalCase()
{
    // Put first two discs, adjust footprint for smaller, add gap to queue
    _shelf.push_back(_discs[0]);
    _shelf.push_back(_discs[1]);
    updateFootprintAB(_discs[0], _discs[1], true);
    MaxGap* newEntry = new MaxGap(_discs[0], _discs[1]);
    _queue.push(newEntry);

    for(unsigned i = 2; i < _n; ++i) {
        if(_queue.top()->maxGapRadius >= _discs[i]->radius()) {
            // Disc can fit into gap, add id to gap, readjust gap size
            Disk *left = _queue.top()->leftDisk();
            Disk *right = _queue.top()->rightDisk();

            // We insert disc between left and right on shelf
            list<Disk*>::iterator it = _shelf.begin();
            while((*it)->radius() != right->radius())
                it++;
            _shelf.insert(it, _discs[i]);

            // Now we update footprint distance, that this newly added disc touch smaller
            // and also update two new gaps
            if(left->radius() < right->radius()) {
                updateFootprintAB(left, _discs[i], true);

                MaxGap *newEntry = new MaxGap(left, _discs[i]);
                _queue.push(newEntry);

                MaxGap *newEntry2 = new MaxGap(_discs[i], right);
                newEntry2->setGapRadiusManual(_discs[i]->radius());
                _queue.push(newEntry2);

            }
            else {
                updateFootprintAB(right, _discs[i], false);

                MaxGap *newEntry = new MaxGap(_discs[i], right);
                _queue.push(newEntry);

                MaxGap *newEntry2 = new MaxGap(left, _discs[i]);
                newEntry2->setGapRadiusManual(_discs[i]->radius());
                _queue.push(newEntry2);

            }
            // Remove this gap becouse it's now filled
            _queue.pop();
        }
        else {
            // Disc cannot be fitted into gap, so we must add it to either side of shelf
            Disk* leftmost = _shelf.front();
            Disk* rightmost = _shelf.back();

            float leftFootprintCandidate = 2 * sqrt( leftmost->radius() * _discs[i]->radius());
            float rightFootprintCandidate = 2 * sqrt( rightmost->radius() * _discs[i]->radius());

            if(leftFootprintCandidate <= leftmost->radius()) {
                // We can fit disk I at leftmost point on shelf, without increasing span
                _shelf.push_front(_discs[i]);
                updateFootprintAB(leftmost,_discs[i], false);
                MaxGap* newEntry = new MaxGap(_discs[i], leftmost);
                _queue.push(newEntry);
            }
            else if(rightFootprintCandidate <= rightmost->radius()) {
                // We can fit disk I at rightmost point on shelf, without increasing span
                _shelf.push_back(_discs[i]);
                updateFootprintAB(rightmost, _discs[i], true);
                MaxGap* newEntry = new MaxGap(rightmost, _discs[i]);
                _queue.push(newEntry);
            }
            else {
                // We must increase span with adding disk I, so we add it to larger one
                if(leftmost->radius() > rightmost->radius()) {
                    _shelf.push_front(_discs[i]);
                    updateFootprintAB(leftmost,_discs[i], false);
                    MaxGap* newEntry = new MaxGap(_discs[i], leftmost);
                    _queue.push(newEntry);
                }
                else {
                    _shelf.push_back(_discs[i]);
                    updateFootprintAB(rightmost, _discs[i], true);
                    MaxGap* newEntry = new MaxGap(rightmost, _discs[i]);
                    _queue.push(newEntry);
                }
            }

        }
    }

}

void CoinsOnShelf::debugShelf()
{
    auto start = _shelf.begin();
    while(start != _shelf.end()) {
        qDebug() << (*start)->radius();
        start++;
    }
}

void CoinsOnShelf::updateFootprintAB(Disk *A, Disk *B, bool directionBIsRightsideA)
{
    float increment = 2 * sqrt(A->radius() * B->radius());
    if(directionBIsRightsideA) {    // Disk B is on A rightside
        B->setFootprint( A->footprint() + increment);
    }
    else {                          // Disk B is on A leftside
        B->setFootprint( A->footprint() - increment);
    }
}

Disk::Disk()
{
    _radius = _randomGen.bounded(5.0);
}

double Disk::radius()
{
    return _radius;
}

void Disk::setRadius(double newRadius)
{
    _radius = newRadius;
}

double Disk::footprint()
{
    return _footprint;
}

void Disk::setFootprint(double newFootprint)
{
    _footprint = newFootprint;
}

MaxGap::MaxGap(Disk *A, Disk *B)
{
    // Assign disk pointers
    _A = A;
    _B = B;

    // Calculate gap
    float divider = sqrt(_A->radius()) + sqrt(_B->radius());
    divider = pow(divider, 2);
    _maxGapRadius = (_A->radius() * _B->radius()) / divider;
}

Disk *MaxGap::leftDisk()
{
    return _A;
}

Disk *MaxGap::rightDisk()
{
    return _B;
}

void MaxGap::setGapRadiusManual(float newRadius)
{
    _maxGapRadius = newRadius;
}
