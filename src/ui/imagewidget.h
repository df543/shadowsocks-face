#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include "pch.hpp"

class ImageWidget: public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr): QWidget(parent) {}
    void setImage(const QImage &image)
    {
        _image = image;
    }

private:
    QImage _image;

protected:
    void paintEvent(QPaintEvent *e)
    {
        QWidget::paintEvent(e);
        QPainter painter(this);
        QSizeF sz = _image.size().scaled(width(), height(), Qt::KeepAspectRatio);
        painter.translate((width() - sz.width()) / 2, (height() - sz.height()) / 2);
        painter.scale(sz.width() / _image.width(), sz.height() / _image.height());
        painter.drawImage(0, 0, _image);
    }
};

#endif // IMAGEWIDGET_H
