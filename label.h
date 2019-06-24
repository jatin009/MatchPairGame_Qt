#ifndef LABEL_H
#define LABEL_H

#include <QLabel>

class Label: public QLabel
{
    Q_OBJECT

public:
    Label(const QPixmap& image, QWidget* parent=0);

    enum BoxState {
        Show,
        Hide,
        Matched
    };

    BoxState boxState();
    void setBoxState(BoxState);

private:
    BoxState m_state;
    QPixmap m_image;
};

#endif // LABEL_H
