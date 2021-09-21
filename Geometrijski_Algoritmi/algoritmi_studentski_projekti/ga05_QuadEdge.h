#ifndef GA05_QUADEDGE_H
#define GA05_QUADEDGE_H

/*
class QuadEdge{
public:
    EdgeDQ edges[4];
    QuadEdge(){
        // Make sure the edges know their own indices for memory magic
            edges[0].setIndex(0);
            edges[1].setIndex(1);
            edges[2].setIndex(2);
            edges[3].setIndex(3);

            // By default, don't render them
            edges[0].draw = false;
            edges[1].draw = false;
            edges[2].draw = false;
            edges[3].draw = false;

            // Set them up to point to each other
            edges[0].setNext((edges + 0));
            edges[1].setNext((edges + 3));
            edges[2].setNext((edges + 2));
            edges[3].setNext((edges + 1));
    }

};
*/

/*EdgeDQ* EdgeDQ::Make(std::vector<QuadEdge*>& list){
    list.push_back(new QuadEdge());
    return list.back()->edges;
}*/
#endif // GA05_QUADEDGE_H
