#ifndef LEC_H
#define LEC_H

#include <vector>
#include <fstream>

#include "utility.h"
#include "algoritambaza.h"

class lec : public AlgoritamBaza {
public:
    lec(QWidget *,
        int,
        const bool & = false,
        std::string = "",
        int = BROJ_SLUCAJNIH_OBJEKATA);

    void pokreniAlgoritam() final;
    void crtajAlgoritam(QPainter *) const final;
    void pokreniNaivniAlgoritam() final;
    void crtajNaivniAlgoritam(QPainter *) const final;

private:
    void generate_random_points(int points_count);
    void read_points(const std::string& file_name);

    std::vector<util::point> m_points;
    std::vector<util::line_segment> m_convex_hull_edges;
    util::circle m_largest_circle;
};

#endif // LEC_H
