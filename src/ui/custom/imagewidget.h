#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include "common.hpp"

class ImageWidget: public QWidget {
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr): QWidget(parent) {}
    void setImage(const QImage &image) {
        _image = image;
    }

private:
    QImage _image;

protected:
    void paintEvent(QPaintEvent *e) {
        QWidget::paintEvent(e);
        QPainter painter(this);
        QSizeF size = _image.size().scaled(width(), height(), Qt::KeepAspectRatio);
        painter.translate((width() - size.width()) / 2, (height() - size.height()) / 2);
        painter.scale(size.width() / _image.width(), size.height() / _image.height());
        painter.drawImage(0, 0, _image);
    }
};

#endif // IMAGEWIDGET_H
