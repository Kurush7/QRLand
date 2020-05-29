#include <cmath>

#include <QtGui>
#include "Presenter.h"
#include "MainWindow.h"

void Presenter::backChangeColor() {
    window.canvas->setBgColor(window.backColorEdit->getColor());
    window.canvas->repaint();
}


void Presenter::loadModel() {
    const char* filename = window.fileName->text().toUtf8().constData();
    // todo
}
void Presenter::openFile() {
    auto fileName = QFileDialog::getOpenFileName(&window, tr("Open File"), "~", tr("*"));
    QFile file(fileName);

    if (!file.exists()) {
        window.fileName->setText("файл не найден!");
    } else {
        //errorLabel->setVisible(false);
        //fileName->setText(file.fileName());
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