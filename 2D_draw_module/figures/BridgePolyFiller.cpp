//
// Created by kurush on 05.04.2020.
//

#include "BridgePolyFiller.h"

using namespace chrono;

bool operator <(const SimplePoint &a, const SimplePoint &b) {
    return b.x < a.x || (b.x == a.x && b.y < a.y);
}



timeResults PolyFiller::measureTime(Transformer *t) {
    vector<Edge> edges;
    for (auto line: lines) {
        auto pa = t->fit(line->a);
        auto pb = t->fit(line->b);
        edges.push_back(Edge(pa, pb));
    }

    timeResults res;
    statRead = 0, statWrite = 0;
    int cnt = 50;
    QPainter p(image); Point aa, bb;
    p.setPen(QPen(lineColor, 1));
    for (int i = 0; i < cnt; ++i) {
        system_clock::time_point start = system_clock::now();
        fillFigure(edges);
        system_clock::time_point end = system_clock::now();
        image->fill(backColor);
        for (auto line: lines) {
            aa = t->fit(line->a);
            bb = t->fit(line->b);
            p.drawLine(aa.x, aa.y, bb.x, bb.y);
        }
        duration<double> sec = end - start;
        res.time += sec.count();
    }
    res.time = res.time * 1000 / cnt;

    usedMap.clear();
    statUsed = 0;
    statRead = 0, statWrite = 0;
    fillFigureStats(edges);
    res.write = statWrite;
    res.read = statRead;
    res.used = statUsed;
    res.square = 0;
    fillFigure(edges);
    for (int i = 0; i < image->width(); ++i) {
        for (int j = 0; j < image->height(); ++j) {
            if (image->pixelColor(i, j) == fillColor)
                res.square++;
        }
    }

    return res;
}
void PolyFiller::draw(QPainter &p, Transformer *t, bool setStyle) {
    if (debugging || lines.empty())
        return;
    image->fill(backColor);
    if (fill) {
        vector<Edge> edges;
        for (auto line: lines) {
            auto pa = t->fit(line->a);
            auto pb = t->fit(line->b);
            edges.push_back(Edge(pa, pb));
        }
        fillFigure(edges);

        //p.setOpacity(0.5);
    }
    p.setPen(lineColor);
    for (auto line: lines)
        line->draw(p, t, false);
}
pair<double,double> PolyFiller::getXBorders() {
    double l = lines[0]->a.x;
    double r = l;
    for (auto line: lines) {
        l = min(l, line->a.x);
        l = min(l, line->b.x);
        r = max(r, line->a.x);
        r = max(r, line->b.x);
    }
    return make_pair(l,r);
}


inline void BridgePolyFiller::fillLine(int x, int y, int mid) {
    if (x < mid)
        for (int j = x+1; j < mid; ++j) {
            auto color = image->pixelColor(j,y) == fillColor? backColor : fillColor;
            image->setPixelColor(j, y, color);
        }
    else
        for (int j = mid; j <= x-1; ++j) {
            auto color = image->pixelColor(j,y) == fillColor? backColor : fillColor;
            image->setPixelColor(j, y, color);
        }
}

inline void BridgePolyFiller::fillVertexes(std::vector<Edge>& edges) {
    SimplePoint a, b, a0, b0;
    map<SimplePoint, char> vertices;  // хеш-таблица состояний вершин
    int mid = bridge;
    int flat_edge_num = -1;  // счетчик горизонтальных ребер
    int n = edges.size();
    const char NOT_VISITED = 0, UP = 1, DOWN = 2;

    for (int j = 0; j < n; ++j) {  // перебор всех ребер
        a = edges[j].a;
        b = edges[j].b;

        int x = Dround(a.x), y = Dround(a.y);
        int bx = Dround(b.x), by = Dround(b.y);
        int dy = by - y;

        if (dy == 0) {      // обработка горизотнальных ребер
            int typeA = vertices[a];
            int typeB = vertices[b];
            if (typeA != NOT_VISITED && typeB != NOT_VISITED) { // обе вершины уже посещались
                if (typeA != typeB) fillLine(x, y, mid);
                continue;
            }
            vertices[a] = flat_edge_num;
            vertices[b] = flat_edge_num;
            a = SimplePoint(flat_edge_num, flat_edge_num);
            vertices[a] = typeA ? typeA : typeB;   // фиктивная вершина
            flat_edge_num--;
            continue;
        }

        int type = vertices[a];     // обработка начальной точки
        a0 = a;
        if (type < 0) {     // нужно перейти к фиктивной вершине
            a0 = SimplePoint(type, type);
            type = vertices[a0];
        }
        if (type == NOT_VISITED) {
            vertices[a0] = dy < 0 ? DOWN : UP;
        } else if ((type == DOWN && dy > 0) || (type == UP && dy < 0)) {
            fillLine(x, y, mid);
        }

        type = vertices[b];     // обработка конечной точки
        b0 = b;
        if (type < 0) {
            b0 = SimplePoint(type, type);
            type = vertices[b0];
        }
        if (type == NOT_VISITED) {
            vertices[b0] = dy > 0 ? DOWN : UP;
        } else if ((type == DOWN && dy < 0) || (type == UP && dy > 0)) {
            fillLine(bx, by, mid);
        }
    }
}

int BridgePolyFiller::fillFigure(vector<Edge> &edges) {
    SimplePoint a, b;
    int n = edges.size();
    if (autoBridging) {     // определение координаты перегородки
        int x1 = edges[0].a.x;
        int x2 = edges[n-1].a.x;
        int x3 = edges[n/2].a.x;
        if (x1 > x2) swap(x1, x2);
        if (x1 > x3) swap(x1, x3);
        if (x2 > x3) swap(x2, x3);
        bridge = x2;
    }
    int mid = bridge;

    fillVertexes(edges);   // заполнение вершин
    for (int j = 0; j < n; ++j) {
        a = edges[j].a;
        b = edges[j].b;

        int x = Dround(a.x), y = Dround(a.y);
        int dx = Dround(b.x) - x, dy = Dround(b.y) - y;
        char sx = signInt(dx), sy = signInt(dy);
        dx = fabs(dx), dy = fabs(dy);

        if (dy == 0)
            continue;

        bool yDirection = false;
        if (dx < dy) {
            yDirection = true;
            swap(dx, dy);
        }

        int y0 = y;
        int e = 2 * dy - dx;   // алгоритм Брезенхема
        for (int i=0; i < dx; ++i) {  // не обрабатывается последняя итерация (i < dx вместо i <= dx)
            if (e >= 0) {
                if (y != y0)    // не обрабатывается строка начальной точки
                    fillLine(x, y, mid);
                y = y + sy;
                x = x + sx;
                e += 2 * (dy - dx);
            }
            else {
                if (yDirection) {
                    if (y != y0)    // не обрабатывается строка начальной точки
                        fillLine(x, y, mid);
                    y = y + sy;
                } else
                    x = x + sx;
                e += 2 * dy;
            }
        }
    }
}


void BridgePolyFiller::initDebug(Transformer *t) {
    debugging = true;
    debug.edges.clear();
    debug.vertices.clear();

    vector<Edge> edges;
    for (auto line: lines) {
        auto pa = t->fit(line->a);
        auto pb = t->fit(line->b);
        debug.edges.push_back(Edge(pa, pb));
    }

    if (autoBridging) {
        int n = debug.edges.size();
        int x1 = debug.edges[0].a.x;
        int x2 = debug.edges[n-1].a.x;
        int x3 = debug.edges[n/2].a.x;
        if (x1 > x2) swap(x1, x2);
        if (x1 > x3) swap(x1, x3);
        if (x2 > x3) swap(x2, x3);
        debug.mid = x2;
    }
    else
        debug.mid = bridge;

    initDebugEdge(0);
}

void BridgePolyFiller::initDebugEdge(int i) {
    if (i >= debug.edges.size()) {
        fillVertexes(debug.edges);
        debugging = false;
        return;
    }

    debug.cur_edge = i;
    auto a = debug.edges[i].a;
    auto b = debug.edges[i].b;
    if (a.x > b.x)
        swap(a,b);

    debug.x = Dround(a.x), debug.y = Dround(a.y);
    int bx = Dround(b.x), by = Dround(b.y);
    debug.dx = bx - debug.x, debug.dy = by - debug.y;

    debug.sx = signInt(debug.dx), debug.sy = signInt(debug.dy);
    debug.dx = fabs(debug.dx), debug.dy = fabs(debug.dy);

    debug.yDirection = false;
    if (debug.dx < debug.dy) {
        debug.yDirection = true;
       swap(debug.dx, debug.dy);
    }

    debug.e = 2 * debug.dy - debug.dx;
    debug.i = 0;
    debug.y0 = debug.y;
}

void BridgePolyFiller::debugLine() {
    int line = debug.cur_edge;
    while (debug.cur_edge == line && debugging)
        debugNext();
}

void BridgePolyFiller::debugNext() {
    if (debug.i < debug.dx) {
        if (debug.e >= 0) {
            if (debug.y != debug.y0)    // не обрабатывается первая линия
                fillLine(debug.x, debug.y, debug.mid);
            debug.y = debug.y + debug.sy;
            debug.x = debug.x + debug.sx;
            debug.e += 2 * (debug.dy - debug.dx);
        } else {
            if (debug.yDirection) {
                if (debug.y != debug.y0)    // не обрабатывается первая линия
                    fillLine(debug.x, debug.y, debug.mid);
                debug.y = debug.y + debug.sy;
            } else
                debug.x = debug.x + debug.sx;
            debug.e += 2 * debug.dy;
        }
    debug.i++;
    }
    else {
        initDebugEdge(debug.cur_edge+1);
    }
}


inline void BridgePolyFiller::fillLineStats(int x, int y, int mid) {
    if (x < mid)
        for (int j = x+1; j < mid; ++j) {
            auto color = image->pixelColor(j,y) == fillColor? backColor : fillColor;
            image->setPixelColor(j, y, color);
            statRead++;
            statWrite++;
            if (usedMap[SimplePoint(j, y)] == false) {
                statUsed++;
                usedMap[SimplePoint(j, y)] = true;
            }
        }
    else
        for (int j = mid; j <= x-1; ++j) {
            auto color = image->pixelColor(j,y) == fillColor? backColor : fillColor;
            image->setPixelColor(j, y, color);
            statRead++;
            statWrite++;
            if (usedMap[SimplePoint(j, y)] == false) {
                statUsed++;
                usedMap[SimplePoint(j, y)] = true;
            }
        }
}

int BridgePolyFiller::fillFigureStats(vector<Edge> edges) {
    SimplePoint a, b;
    map<SimplePoint, VertexType> vertices;  // хеш-таблица
    int n = edges.size();   // количество ребер

    // выбор положения перегородки
    if (autoBridging) {  // среднее из трех значений
        int x1 = edges[0].a.x;
        int x2 = edges[n-1].a.x;
        int x3 = edges[n/2].a.x;
        if (x1 > x2) swap(x1, x2);
        if (x1 > x3) swap(x1, x3);
        if (x2 > x3) swap(x2, x3);
        bridge = x2;
    }
    int mid = bridge;

    for (int j = 0; j < n; ++j) {   // перебор ребер
        a = edges[j].a;
        b = edges[j].b;
        if (a.x > b.x)
            swap(a,b);

        int x = Dround(a.x), y = Dround(a.y);
        int bx = Dround(b.x), by = Dround(b.y);
        int dx = bx - x, dy = by - y;

        VertexType type = vertices[a];
        if (type == NOT_VISITED || (type == DOWN && dy < 0) || (type == UP && dy > 0)) {
            fillLine(x, y, mid);
            vertices[a] = dy < 0? DOWN : UP;
        }
        type = vertices[b];
        if (type == NOT_VISITED || (type == DOWN && -dy < 0) || (type == UP && -dy > 0)) {
            fillLine(bx, by, mid);
            vertices[b] = -dy < 0? DOWN : UP;
        }

        char sx = signInt(dx), sy = signInt(dy);
        dx = fabs(dx), dy = fabs(dy);

        bool yDirection = false;
        if (dx < dy) {
            yDirection = true;
            swap(dx, dy);
        }

        int y0 = y;
        int e = 2 * dy - dx;   // алгоритм Брезенхема
        for (int i=0; i < dx; ++i) {  // не обрабатывается последняя итерация
            if (e >= 0) {
                if (y != y0)    // не обрабатывается первая линия
                    fillLineStats(x, y, mid);
                y = y + sy;
                x = x + sx;
                e += 2 * (dy - dx);
            }
            else {
                if (yDirection) {
                    if (y != y0)    // не обрабатывается первая линия
                        fillLineStats(x, y, mid);
                    y = y + sy;
                } else
                    x = x + sx;
                e += 2 * dy;
            }
        }
    }
}

int BridgePolyFiller::getAutoBridge() {
    int n = lines.size();
    if (!n)
        return 0;
    int x1 = lines[0]->a.x;
    int x2 = lines[n-1]->a.x;
    int x3 = lines[n/2]->a.x;
    if (x1 > x2) swap(x1, x2);
    if (x1 > x3) swap(x1, x3);
    if (x2 > x3) swap(x2, x3);
    return x2;
}