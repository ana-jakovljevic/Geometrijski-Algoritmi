#include "lec.h"
#include "delaunay.h"
#include "voronoi.h"
#include "convex_hull.h"
#include "largest_empty_circle.h"
#include <iostream>
#include <set>

lec::lec(QWidget *drawing,
         int pause,
         const bool& brute_force,
         std::string file_name,
         int points_count)
    :AlgoritamBaza(drawing, pause, brute_force)
{
    if (!file_name.empty()) {
        read_points(file_name);
    }
    else {
        generate_random_points(std::max(3,points_count));
    }

    delaunay del{m_points};
    voronoi vor{del.triangulation()};
    convex_hull ch{del.triangulation()};
    m_convex_hull_edges = ch.edges();

    largest_empty_circle lec(del.triangulation(), vor.graph());
    m_largest_circle = lec.get_largest_circle();
}

void lec::read_points(const std::string& file_name)
{
    std::ifstream input_file(file_name);

    if (input_file) {
        double x,y;
        char comma;
        while (input_file >> x >> comma >> y) {
            m_points.emplace_back(x,y);
        }
    }
}

void lec::generate_random_points(int points_count)
{
    srand(static_cast<unsigned>(time(nullptr)));
    int x_max = (_pCrtanje) ? _pCrtanje->width() - 10 : CANVAS_WIDTH;
    int y_max = (_pCrtanje) ? _pCrtanje->height() - 10 : CANVAS_HEIGHT;

    int x_min = 10;
    int y_min = 10;

    int x_diff = x_max-x_min;
    int y_diff = y_max-y_min;

    std::set<std::pair<int,int>> s;

    for(int i = 0; i < points_count; ++i) {
        s.insert({x_min + rand()%x_diff, y_min + rand()%y_diff});
    }

    for (auto p : s) {
        m_points.emplace_back(p.first, p.second);
    }
}

void lec::pokreniAlgoritam()
{
    emit animacijaZavrsila();
}

void lec::crtajAlgoritam(QPainter * painter) const
{
    if (!painter) {
        return;
    }

    QPen p = painter->pen();

    p.setWidth(2);
    p.setColor(Qt::green);
    painter->setPen(p);

    painter->drawEllipse(QPointF(m_largest_circle.center().x(), m_largest_circle.center().y()),
                         m_largest_circle.r(),
                         m_largest_circle.r());

    p.setWidth(5);
    p.setColor(Qt::green);
    painter->setPen(p);
    painter->drawPoint(m_largest_circle.center().x(), m_largest_circle.center().y());

    p.setWidth(1);
    p.setColor(Qt::blue);
    painter->setPen(p);

    for (const auto& e : m_convex_hull_edges) {
        painter->drawLine(e.origin().x(), e.origin().y(), e.destination().x(), e.destination().y());
    }

    p.setWidth(5);
    p.setColor(Qt::red);
    painter->setPen(p);

    for (const auto &pt : m_points) {
        painter->drawPoint(pt.x(), pt.y());
    }
}

void lec::pokreniNaivniAlgoritam()
{
    emit animacijaZavrsila();
}

void lec::crtajNaivniAlgoritam(QPainter *) const
{

}
