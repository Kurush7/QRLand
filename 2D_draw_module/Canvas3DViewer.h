//
// Created by kurush on 23.05.2020.
//

#ifndef KG_CANVAS3DVIEWER_H
#define KG_CANVAS3DVIEWER_H

#include "Canvas.h"
#include <QTimer>


enum TransformStateDir {
    None, Xinc, Xdec,
    Yinc, Ydec,
    Zinc, Zdec
};
const int REPAINT_TIME = 100;



class Canvas3DViewer: public Canvas {
    Q_OBJECT
public:
    explicit Canvas3DViewer(int w, int h, QWidget *parent = nullptr,
                    QColor fill = Qt::white): Canvas(w, h, parent, fill) {
        state = None;
        cur_key = -1;
        connect(&timer, SIGNAL(timeout()), this, SLOT(timerAlarm()));
        setFocusPolicy(Qt::StrongFocus);
    }

signals:
    void sendModify(TransformStateDir);
private slots:
    void timerAlarm();

private:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *) override;

    int cur_key;
    TransformStateDir state;
    QTimer timer;
};


#endif //KG_CANVAS3DVIEWER_H
