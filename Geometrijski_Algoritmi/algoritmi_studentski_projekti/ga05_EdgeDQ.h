#ifndef EDGEDQ_H
#define EDGEDQ_H
#include "algoritambaza.h"
#include <iostream>
class EdgeDQ{
public:
    QPointF _org;
    QPointF _dest;
    EdgeDQ* onext=nullptr;
    EdgeDQ* oprev=nullptr;
    EdgeDQ* sym=nullptr;
    bool data=false;
    EdgeDQ(QPointF org, QPointF dest) : _org(org), _dest(dest){}
    bool operator==(const EdgeDQ &a){
        return a._org == this->_org && a._dest == this->_dest && a.onext == this->onext && a.oprev == this->oprev && a.sym == this->sym;
    }


    friend std::ostream& operator<<(std::ostream& out, const EdgeDQ& edge){
        out << edge._org.x() << " " << edge._org.y()  << " " << edge._dest.x() << " " << edge._dest.y() << std::endl;
        return out;
    }
};


#endif // EDGEDQ_H
