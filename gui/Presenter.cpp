#include <cmath>

#include <QtGui>
#include "Presenter.h"
#include "MainWindow.h"

Presenter::Presenter(MainWindow &w): window(w) {
        painter = std::shared_ptr<Painter>(new QTPainter(window.canvas));
}

void Presenter::backChangeColor() {
    window.canvas->setBgColor(window.backColorEdit->getColor());
    window.canvas->repaint();
}


void Presenter::loadModel() {
    auto filename = window.fileName->text().toUtf8().constData();
    facade.addModel(filename);
    facade.draw(painter);
}
void Presenter::openFile() {
    auto fileName = QFileDialog::getOpenFileName(&window, tr("Open File"), "~", tr("*"));
    window.fileName->setText(fileName);
    QFile file(fileName);

    if (!file.exists()) {
        window.fileName->setText("файл не найден!");
    }
}

void Presenter::select(double x, double y) {
    cout << x << ' ' << y << '\n';
}

void Presenter::selBind() {
    // todo
}
void Presenter::selChangeColor() {
    // todo
}
void Presenter::selDelete() {
    // todo
}

void Presenter::transform(TransformStateDir) {
    // todo
}