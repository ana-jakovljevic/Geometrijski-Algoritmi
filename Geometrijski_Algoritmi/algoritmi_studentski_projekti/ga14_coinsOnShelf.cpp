#include "ga14_coinsOnShelf.h"

void CoinsOnShelf::printSpan(QPainter *painter, bool naive) const
{
    /* Calculate span that we need to display. Naive boolean value is
     * used to differ shelfs when calculating */
    double span;
    double x, y;
    if(!naive) {
        x = _shelf.back()->footprint() + _shelf.back()->radius();
        y = _shelf.front()->footprint() - _shelf.front()->radius();
    }
    else {
        x = _shelfNaive.back()->footprint() + _shelfNaive.back()->radius();
        y = _shelfNaive.front()->footprint() - _shelfNaive.front()->radius();
    }
    span = x - y;

    // Now print it on screen
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

void CoinsOnShelf::printSpanFinal(QPainter *painter) const
{
    // This method is only used at end of naive algorithm
    QPointF point(50, 500);
    QString spanText = "Total span: " + QString::number(_naiveMinSpan);

    painter->save();
    painter->scale(1, -1);
    painter->translate(0, -2*525);

    QPen pen = painter->pen();
    pen.setColor(Qt::red);
    QFont font;
    font.setPixelSize(24);
    painter->setFont(font);
    painter->setPen(pen);

    painter->drawText(point, spanText);
    painter->restore();
}

double CoinsOnShelf::calculateSpan()
{
    /* Span in this case couldn't be calculated only from first and last disc
     * One example: if we have very large disc that's second on shelf and first disc is very very
     * small. In that case, left edge of that large disc is what we want, not left edge of first (small)
     * disc
     */
    double x = 0;
    for(auto disc: _shelfNaive)
        if(disc->footprint() + disc->radius() > x)
            x = disc->footprint() + disc->radius();

    double y = 99999;
    for(auto disc: _shelfNaive)
        if(disc->footprint() - disc->radius() < y)
            y = disc->footprint() - disc->radius();

    return x-y;
}


void CoinsOnShelf::naiveSpecialCase()
{
    /* For each permutation, display it, calcluate it's span
     * and update minSpan if it's smaller */
    double minSpan = 99999;

    vector<unsigned> indexes;
    for(unsigned j = 0; j < _n; j++)
        indexes.push_back(j);

    do {
        AlgoritamBaza_updateCanvasAndBlock()
        _shelfNaive.resize(0);
        for(auto disc: _discs)
            disc->setFootprint(0);

        _shelfNaive.push_back( _discs[indexes[0]] );
        for(unsigned j = 1; j < _n; j++) {
            _shelfNaive.push_back( _discs[indexes[j]] );
            updateFootprintAB( _discs[indexes[j-1]], _discs[indexes[j]], true);
            //AlgoritamBaza_updateCanvasAndBlock();

            if(_shelfNaive.size() == _n) {
                double currentSpan = calculateSpan();
                if(minSpan > currentSpan) {
                    minSpan = currentSpan;
                    _shelfNaiveFinal.resize(0);
                    for(auto disc : _shelfNaive) {
                        Disk* discTmp = new Disk();
                        discTmp->setRadius(disc->radius());
                        discTmp->setFootprint(disc->footprint());
                        _shelfNaiveFinal.push_back(discTmp);
                    }
                }
            }
        }

    } while( next_permutation(indexes.begin(), indexes.end()));

    _ended = true;
    _naiveMinSpan = minSpan;
}

void CoinsOnShelf::naiveGeneralCase()
{
    /* The same as previous, only thats different that now
     * discs could be moved apart to fill new disc in new permutation
     * If we used previous method, some discs could overlap
     */
    double minSpan = 99999;

    vector<unsigned> indexes;
    for(unsigned j = 0; j < _n; j++)
        indexes.push_back(j);

    do {
        _shelfNaive.resize(0);
        for(auto disc: _discs)
            disc->setFootprint(0);

        _shelfNaive.push_back( _discs[indexes[0]] );
        for(unsigned j = 1; j < _n; j++) {
            _shelfNaive.push_back( _discs[indexes[j]] );
            updateFootprintAB( _discs[indexes[j-1]], _discs[indexes[j]], true);

            for(int k = static_cast<int>(j)-1; k >= 0; k--) {
                Disk* current = _discs[indexes[j]];
                Disk* prev = _discs[indexes[static_cast<unsigned>(k)]];

                double radiusDiff = current->radius() - prev->radius();
                radiusDiff = radiusDiff*radiusDiff;

                double footDiff = current->footprint() - prev->footprint();
                footDiff = footDiff*footDiff;

                double centerdist = footDiff + radiusDiff;
                centerdist = sqrt(centerdist);

                double diff = centerdist - current->radius() - prev->radius();

                if(diff < -0.05) {
                    double radiusDist = current->radius() + prev->radius();
                    radiusDist = radiusDist * radiusDist;
                    double x = radiusDist - radiusDiff;
                    x = sqrt(x);
                    x = x + prev->footprint();
                    current->setFootprint(x);
                }
            }

            if(_shelfNaive.size() == _n) {
                double currentSpan = calculateSpan();
                if(minSpan > currentSpan) {
                    minSpan = currentSpan;
                    _shelfNaiveFinal.resize(0);
                    for(auto disc : _shelfNaive) {
                        Disk* discTmp = new Disk();
                        discTmp->setRadius(disc->radius());
                        discTmp->setFootprint(disc->footprint());
                        _shelfNaiveFinal.push_back(discTmp);
                    }
                }
            }
        }

        AlgoritamBaza_updateCanvasAndBlock()
    } while( next_permutation(indexes.begin(), indexes.end()));

    _ended = true;
    _naiveMinSpan = minSpan;
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
    double max = _discs[0]->radius();
    double min = _discs[_n-1]->radius();

    if(max/min < 2)
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


    _ended = true;
    AlgoritamBaza_updateCanvasAndBlock()
    //qDebug() << getSpan();
    emit animacijaZavrsila();
}

void CoinsOnShelf::crtajAlgoritam(QPainter *painter) const
{
    if(!painter) return;
    painter->viewport();

    QPen pen = painter->pen();
    pen.setWidth(1);
    pen.setColor(Qt::black);
    painter->setPen(pen);

    // Draw horizontal line - shelf
    painter->drawLine(0, 70, 1100, 70);

    // Draw all discs
    for(Disk* disc: _shelf) {
        double radius = disc->radius();
        double x = disc->footprint() + 450;
        double y = disc->radius() + 70;
        QPointF center(x,y);
        painter->drawEllipse(center, radius, radius);
    }

    // And print span and red vertical lines if we have more than 1 disc
    if(_shelf.size() >= 2) {
        printSpan(painter,false);

        QPen redPen = painter->pen();
        redPen.setWidth(2);
        redPen.setColor(Qt::red);
        painter->setPen(redPen);

        double x = _shelf.front()->footprint() + 450 - _shelf.front()->radius() - 2;
        QPointF leftDown(x, 70);
        QPointF leftUp(x, 70 + _shelf.front()->radius() * 2.5);
        painter->drawLine(leftDown, leftUp);

        x = _shelf.back()->footprint() + 450 + _shelf.back()->radius() + 2;
        QPointF rightDown(x, 70);
        QPointF rightUp(x, 70 + _shelf.back()->radius() * 2.5);
        painter->drawLine(rightDown, rightUp);
    }
}

void CoinsOnShelf::pokreniNaivniAlgoritam()
{
    if(_algorithm == SPECIAL) {
        naiveSpecialCase();
    }

    if(_algorithm == GENERAL) {
        naiveGeneralCase();
    }

    _ended = true;
    AlgoritamBaza_updateCanvasAndBlock()
    //qDebug() << getSpanNaive();
    emit animacijaZavrsila();
}

void CoinsOnShelf::crtajNaivniAlgoritam(QPainter *painter) const
{

    if(!painter) return;
    painter->viewport();

    QPen pen = painter->pen();
    pen.setWidth(1);
    pen.setColor(Qt::black);
    painter->setPen(pen);

    // Draw horizontal line
    painter->drawLine(0, 70, 1100, 70);

    if(_ended == false) {

        // Draw disks
        for(Disk* disc: _shelfNaive) {
            double radius = disc->radius();
            double x = disc->footprint() + 120;
            double y = disc->radius() + 70;
            QPointF center(x,y);
            painter->drawEllipse(center, radius, radius);
        }

        // Draw vertical red lines and text
        if(_shelfNaive.size() >= 2) {
            printSpan(painter,true);

            QPen redPen = painter->pen();
            redPen.setWidth(2);
            redPen.setColor(Qt::red);
            painter->setPen(redPen);

            double y = 99999;
            for(auto disc: _shelfNaive) {
                if (disc->footprint() - disc->radius() < y)
                    y = disc->footprint() - disc->radius();
            }
            QPointF leftDown(y+120, 70);
            QPointF leftUp(y+120, 150);
            painter->drawLine(leftDown, leftUp);

            double x = 0;
            for(auto disc: _shelfNaive)
                if (disc->footprint() + disc->radius() > x)
                    x = disc->footprint() + disc->radius();
            QPointF rightDown(x+120, 70);
            QPointF rightUp(x+120, 150);
            painter->drawLine(rightDown, rightUp);
        }
    }
    else {
        printSpanFinal(painter);

        for(Disk* disc: _shelfNaiveFinal) {
            double radius = disc->radius();
            double x = disc->footprint() + 120;
            double y = disc->radius() + 70;
            QPointF center(x,y);
            painter->drawEllipse(center, radius, radius);
        }

        QPen redPen = painter->pen();
        redPen.setWidth(2);
        redPen.setColor(Qt::red);
        painter->setPen(redPen);

        double y = 99999;
        for(auto disc: _shelfNaiveFinal) {
            if (disc->footprint() - disc->radius() < y)
                y = disc->footprint() - disc->radius();
        }
        QPointF leftDown(y+120, 70);
        QPointF leftUp(y+120, 150);
        painter->drawLine(leftDown, leftUp);

        double x = 0;
        for(auto disc: _shelfNaiveFinal)
            if (disc->footprint() + disc->radius() > x)
                x = disc->footprint() + disc->radius();
        QPointF rightDown(x+120, 70);
        QPointF rightUp(x+120, 150);
        painter->drawLine(rightDown, rightUp);

    }
}

unsigned long CoinsOnShelf::getShelfSize()
{
    return _shelf.size();
}

unsigned long CoinsOnShelf::getNaiveShelfSize()
{
    return _shelfNaiveFinal.size();
}

list<Disk *> CoinsOnShelf::getShelfDisks()
{
    return _shelf;
}

vector<Disk *> CoinsOnShelf::getShelfNaiveDisks()
{
    return _shelfNaiveFinal;
}

double CoinsOnShelf::getSpan()
{
    double leftEdge = _shelf.front()->footprint() - _shelf.front()->radius();
    double rightEdge = _shelf.back()->footprint() + _shelf.back()->radius();
    return rightEdge - leftEdge;
}

double CoinsOnShelf::getSpanNaive()
{
    double leftEdge = _shelfNaiveFinal.front()->footprint() - _shelfNaiveFinal.front()->radius();
    double rightEdge = _shelfNaiveFinal.back()->footprint() + _shelfNaiveFinal.back()->radius();
    return rightEdge - leftEdge;
}

void CoinsOnShelf::specialCaseEvenDiscs()
{
    // This method follow schema in documentation
    _shelf.push_back(_discs[0]);
    unsigned j = 1;
    AlgoritamBaza_updateCanvasAndBlock()

    while(true) {
        if(pomocneFunkcije::bliski(_shelf.back()->radius(), _discs[_n - j]->radius()))
            break;
        updateFootprintAB(_shelf.back(), _discs[_n - j], true);
        _shelf.push_back(_discs[_n - j]);
        AlgoritamBaza_updateCanvasAndBlock()
        if(pomocneFunkcije::bliski(_shelf.back()->radius(), _discs[j]->radius()))
            break;
        updateFootprintAB(_shelf.back(), _discs[j], true);
        _shelf.push_back(_discs[j]);
        AlgoritamBaza_updateCanvasAndBlock()
        j += 2;
    }

    j = 2;
    while(true) {
        if(pomocneFunkcije::bliski(_shelf.front()->radius(), _discs[_n - j]->radius()))
            break;
        updateFootprintAB(_shelf.front(), _discs[_n - j], false);
        _shelf.push_front(_discs[_n - j]);
        AlgoritamBaza_updateCanvasAndBlock()
        if(pomocneFunkcije::bliski(_shelf.front()->radius(), _discs[j]->radius()))
            break;
        updateFootprintAB(_shelf.front(), _discs[j], false);
        _shelf.push_front(_discs[j]);
        AlgoritamBaza_updateCanvasAndBlock()
        j += 2;
    }
}

void CoinsOnShelf::specialCaseOddDiscs()
{
    // This method follows schema in documentation
    _shelf.push_back(_discs[0]);
    unsigned j = 2;
    unsigned i = 0;
    AlgoritamBaza_updateCanvasAndBlock()

    while(true) {
        if(i < (_n/2 - 1)) {
            updateFootprintAB(_shelf.front(), _discs[_n - j], false);
            _shelf.push_front(_discs[_n - j]);
            ++i;
            AlgoritamBaza_updateCanvasAndBlock()
        }
        else break;
        if(i < (_n/2 - 1)) {
            updateFootprintAB(_shelf.front(), _discs[j], false);
            _shelf.push_front(_discs[j]);
            ++i;
            AlgoritamBaza_updateCanvasAndBlock()
        }
        else break;
        j += 2;
    }

    updateFootprintAB(_discs[0], _discs[1], true);
    _shelf.push_back(_discs[1]);
    AlgoritamBaza_updateCanvasAndBlock()
    j = 3;
    i = 0;
    while(true) {
        if(i < (_n/2 - 1)) {
            updateFootprintAB(_shelf.back(), _discs[_n - j], true);
            _shelf.push_back(_discs[_n - j]);
            ++i;
            AlgoritamBaza_updateCanvasAndBlock()
        }
        else break;
        if(i < (_n/2 - 1)) {
            updateFootprintAB(_shelf.back(), _discs[j], true);
            _shelf.push_back(_discs[j]);
            ++i;
            AlgoritamBaza_updateCanvasAndBlock()
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
    AlgoritamBaza_updateCanvasAndBlock()
}

void CoinsOnShelf::generalCase()
{
    // Put first two discs, adjust footprint for smaller, add gap to queue
    _shelf.push_back(_discs[0]);
    AlgoritamBaza_updateCanvasAndBlock()
    _shelf.push_back(_discs[1]);
    updateFootprintAB(_discs[0], _discs[1], true);
    MaxGap* newEntry = new MaxGap(_discs[0], _discs[1]);
    _queue.push(newEntry);
    AlgoritamBaza_updateCanvasAndBlock()

    for(unsigned i = 2; i < _n; ++i) {
        if(_queue.top()->maxGapRadius() >= _discs[i]->radius()) {
            // Disc can fit into gap, add it to gap, readjust gap size
            Disk *left = _queue.top()->leftDisk();
            Disk *right = _queue.top()->rightDisk();

            // We insert disc between left and right on shelf
            list<Disk*>::iterator it = _shelf.begin();
            while(!pomocneFunkcije::bliski((*it)->radius(), right->radius()))
                it++;
            _shelf.insert(it, _discs[i]);

            // Now we update footprint distance, that this newly added disc touch smaller
            // and also update two new gaps
            if(left->radius() < right->radius()) {
                updateFootprintAB(left, _discs[i], true);

                newEntry = new MaxGap(left, _discs[i]);
                _queue.push(newEntry);

                MaxGap *newEntry2 = new MaxGap(_discs[i], right);

                                                // Code bellow will calculate gap size //
                double z = pow( (left->radius() + _discs[i]->radius()) , 2) - pow( (left->radius() - _discs[i]->radius()) ,2);
                z = sqrt(z);
                z = fabs(right->footprint() - left->footprint()) - z;
                z = z / (2 * (sqrt(_discs[i]->radius()) + sqrt(right->radius())));
                z = pow(z,2);
                                                //                 End                 //

                newEntry2->setGapRadiusManual(z);
                _queue.push(newEntry2);

            }
            else {
                // Right disc iz smaller, so this disk will touch right disc on its left side
                updateFootprintAB(right, _discs[i], false);

                newEntry = new MaxGap(_discs[i], right);
                _queue.push(newEntry);

                MaxGap *newEntry2 = new MaxGap(left, _discs[i]);

                                                // Code bellow will calculate gap size //
                double z = pow( (right->radius() + _discs[i]->radius()) , 2) - pow( (right->radius() - _discs[i]->radius()) ,2);
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
            AlgoritamBaza_updateCanvasAndBlock()
        }
        else {
            // Disc cannot be fitted into gap, so we must add it to either side of shelf
            Disk* leftmost = _shelf.front();
            Disk* rightmost = _shelf.back();

            double leftFootprintCandidate = 2 * sqrt( leftmost->radius() * _discs[i]->radius());
            double rightFootprintCandidate = 2 * sqrt( rightmost->radius() * _discs[i]->radius());

            if(leftFootprintCandidate <= leftmost->radius()) {
                // We can fit disk I at leftmost point on shelf, without increasing span
                _shelf.push_front(_discs[i]);
                updateFootprintAB(leftmost,_discs[i], false);
                newEntry = new MaxGap(_discs[i], leftmost);
                _queue.push(newEntry);
            }
            else if(rightFootprintCandidate <= rightmost->radius()) {
                // We can fit disk I at rightmost point on shelf, without increasing span
                _shelf.push_back(_discs[i]);
                updateFootprintAB(rightmost, _discs[i], true);
                newEntry = new MaxGap(rightmost, _discs[i]);
                _queue.push(newEntry);
            }
            else {
                // We must increase span with adding disk I, so we add it to larger one
                if(leftmost->radius() > rightmost->radius()) {
                    _shelf.push_front(_discs[i]);
                    updateFootprintAB(leftmost,_discs[i], false);
                    newEntry = new MaxGap(_discs[i], leftmost);
                    _queue.push(newEntry);
                }
                else {
                    _shelf.push_back(_discs[i]);
                    updateFootprintAB(rightmost, _discs[i], true);
                    newEntry = new MaxGap(rightmost, _discs[i]);
                    _queue.push(newEntry);
                }
            }
            AlgoritamBaza_updateCanvasAndBlock()
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
    double increment = 2 * sqrt(A->radius() * B->radius());
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
    double divider = sqrt(_A->radius()) + sqrt(_B->radius());
    divider = pow(divider, 2);
    _maxGapRadius = (_A->radius() * _B->radius()) / divider;
}

double MaxGap::maxGapRadius()
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

void MaxGap::setGapRadiusManual(double newRadius)
{
    _maxGapRadius = newRadius;
}
