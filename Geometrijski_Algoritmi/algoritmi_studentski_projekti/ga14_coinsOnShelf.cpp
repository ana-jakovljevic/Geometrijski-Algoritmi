#include "ga14_coinsOnShelf.h"

void CoinsOnShelf::printSpan(QPainter *painter) const
{
    float span = _shelf.back()->radius() + _shelf.back()->footprint() - _shelf.front()->footprint() - _shelf.front()->radius();
    QPointF point(50, 500);
    QString spanText = "Total span: " + QString::number(span);

    painter->save();
    painter->scale(1, -1);
    painter->translate(0, -2*525);

    QPen pen = painter->pen();
    pen.setColor(Qt::black);
    QFont font;
    font.setPixelSize(24);
    painter->setFont(font);
    painter->setPen(pen);

    painter->drawText(point, spanText);
    painter->restore();

}

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
    _ended = false;
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

    //debugShelf();
    _ended = true;
    AlgoritamBaza_updateCanvasAndBlock();

    emit animacijaZavrsila();
}

void CoinsOnShelf::crtajAlgoritam(QPainter *painter) const
{
    if(!painter) return;
    painter->viewport();

    QPen pen = painter->pen();
    pen.setWidth(2);
    pen.setColor(Qt::black);
    painter->setPen(pen);

    painter->save();
    painter->scale(1,-1);
    painter->translate(0, 200);
    painter->drawText(QPointF(300,300), QString("test"));
    painter->restore();

    painter->drawLine(0, 70, 1100, 70);

    for(Disk* disc: _shelf) {
        float radius = disc->radius();
        float x = disc->footprint() + 450;
        float y = disc->radius() + 70;
        QPointF center(x,y);
        painter->drawEllipse(center, radius, radius);
    }

    if(_ended) {

    }

    if(_shelf.size() >= 2) {
        printSpan(painter);

        QPen redPen = painter->pen();
        redPen.setWidth(2);
        redPen.setColor(Qt::red);
        painter->setPen(redPen);

        float x = _shelf.front()->footprint() + 450 - _shelf.front()->radius() - 2;
        QPointF leftDown(x, 70);
        QPointF leftUp(x, 70 + _shelf.front()->radius() * 2.5);
        painter->drawLine(leftDown, leftUp);

        x = _shelf.back()->footprint() + 450 + _shelf.back()->radius() + 2;
        QPoint rightDown(x, 70);
        QPoint rightUp(x, 70 + _shelf.back()->radius() * 2.5);
        painter->drawLine(rightDown, rightUp);
    }
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
    AlgoritamBaza_updateCanvasAndBlock();

    while(true) {
        if(_shelf.back()->radius() == _discs[_n - j]->radius())
            break;
        updateFootprintAB(_shelf.back(), _discs[_n - j], true);
        _shelf.push_back(_discs[_n - j]);
        AlgoritamBaza_updateCanvasAndBlock();
        if(_shelf.back()->radius() == _discs[j]->radius())
            break;
        updateFootprintAB(_shelf.back(), _discs[j], true);
        _shelf.push_back(_discs[j]);
        AlgoritamBaza_updateCanvasAndBlock();
        j += 2;
    }

    j = 2;
    while(true) {
        if(_shelf.front()->radius() == _discs[_n - j]->radius())
            break;
        updateFootprintAB(_shelf.front(), _discs[_n - j], false);
        _shelf.push_front(_discs[_n - j]);
        AlgoritamBaza_updateCanvasAndBlock();
        if(_shelf.front()->radius() == _discs[j]->radius())
            break;
        updateFootprintAB(_shelf.front(), _discs[j], false);
        _shelf.push_front(_discs[j]);
        AlgoritamBaza_updateCanvasAndBlock();
        j += 2;
    }
}

void CoinsOnShelf::specialCaseOddDiscs()
{
    _shelf.push_back(_discs[0]);
    int j = 2;
    unsigned i = 0;
    AlgoritamBaza_updateCanvasAndBlock();

    while(true) {
        if(i < (_n/2 - 1)) {
            updateFootprintAB(_shelf.front(), _discs[_n - j], false);
            _shelf.push_front(_discs[_n - j]);
            ++i;
            AlgoritamBaza_updateCanvasAndBlock();
        }
        else break;
        if(i < (_n/2 - 1)) {
            updateFootprintAB(_shelf.front(), _discs[j], false);
            _shelf.push_front(_discs[j]);
            ++i;
            AlgoritamBaza_updateCanvasAndBlock();
        }
        else break;
        j += 2;
    }

    updateFootprintAB(_discs[0], _discs[1], true); // POTENTIAL PROBLEM
    _shelf.push_back(_discs[1]);
    AlgoritamBaza_updateCanvasAndBlock();
    j = 3;
    i = 0;
    while(true) {
        if(i < (_n/2 - 1)) {
            updateFootprintAB(_shelf.back(), _discs[_n - j], true);
            _shelf.push_back(_discs[_n - j]);
            ++i;
            AlgoritamBaza_updateCanvasAndBlock();
        }
        else break;
        if(i < (_n/2 - 1)) {
            updateFootprintAB(_shelf.back(), _discs[j], true);
            _shelf.push_back(_discs[j]);
            ++i;
            AlgoritamBaza_updateCanvasAndBlock();
        }
        else break;
        j += 2;
    }

    if(_shelf.back()->radius() > _shelf.front()->radius()) {
        updateFootprintAB(_shelf.back(), _discs[_n -1], true);
        _shelf.push_back(_discs[_n-1]);
    }
    else {
        updateFootprintAB(_shelf.front(), _discs[_n-1], false);
        _shelf.push_front(_discs[_n-1]);
    }
    AlgoritamBaza_updateCanvasAndBlock();
}

void CoinsOnShelf::generalCase()
{
    // Put first two discs, adjust footprint for smaller, add gap to queue
    _shelf.push_back(_discs[0]);
    AlgoritamBaza_updateCanvasAndBlock();
    _shelf.push_back(_discs[1]);
    updateFootprintAB(_discs[0], _discs[1], true);
    MaxGap* newEntry = new MaxGap(_discs[0], _discs[1]);
    _queue.push(newEntry);
    AlgoritamBaza_updateCanvasAndBlock();

    for(unsigned i = 2; i < _n; ++i) {
        if(_queue.top()->maxGapRadius() >= _discs[i]->radius()) {
            // Disc can fit into gap, add it to gap, readjust gap size
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

                                                // Code bellow will calculate gap size //
                float z = pow( (left->radius() + _discs[i]->radius()) , 2) - pow( (left->radius() - _discs[i]->radius()) ,2);
                z = sqrt(z);
                z = fabs(right->footprint() - left->footprint()) - z;
                z = z / (2 * (sqrt(_discs[i]->radius()) + sqrt(right->radius())));
                z = pow(z,2);
                                                //                 End                 //

                newEntry2->setGapRadiusManual(_discs[i]->radius());
                _queue.push(newEntry2);

            }
            else {
                // Right disc iz smaller, so this disk will touch right disc on its left side
                updateFootprintAB(right, _discs[i], false);

                MaxGap *newEntry = new MaxGap(_discs[i], right);
                _queue.push(newEntry);

                MaxGap *newEntry2 = new MaxGap(left, _discs[i]);

                                                // Code bellow will calculate gap size //
                float z = pow( (right->radius() + _discs[i]->radius()) , 2) - pow( (right->radius() - _discs[i]->radius()) ,2);
                z = sqrt(z);
                z = fabs(right->footprint() - left->footprint()) - z;
                z = z / (2 * (sqrt(_discs[i]->radius()) + sqrt(left->radius())));
                z = pow(z,2);
                                                //                 End                 //
                newEntry2->setGapRadiusManual(z);
                _queue.push(newEntry2);

            }
            // Remove this gap becouse it's now filled
            _queue.pop();
            AlgoritamBaza_updateCanvasAndBlock();
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
            AlgoritamBaza_updateCanvasAndBlock();
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
    _radius = _randomGen.bounded(55.0);
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

float MaxGap::maxGapRadius()
{
    return _maxGapRadius;
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
