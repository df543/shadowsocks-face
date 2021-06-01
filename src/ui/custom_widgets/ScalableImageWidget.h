#ifndef SCALABLE_IMAGE_WIDGET_H
#define SCALABLE_IMAGE_WIDGET_H

class ScalableImageWidget: public QWidget
{
    Q_OBJECT

public:
    explicit ScalableImageWidget(QWidget *parent = nullptr):
        QWidget(parent) {}

    void setImage(const QImage &_image)
    {
        this->image = _image;
        repaint();
    }

private:
    QImage image;

protected:
    void paintEvent(QPaintEvent *e) override
    {
        QWidget::paintEvent(e);
        QPainter painter(this);
        QSizeF size = image.size().scaled(width(), height(), Qt::KeepAspectRatio);
        painter.translate((width() - size.width()) / 2, (height() - size.height()) / 2);
        painter.scale(size.width() / image.width(), size.height() / image.height());
        painter.drawImage(0, 0, image);
    }
};

#endif // SCALABLE_IMAGE_WIDGET_H
