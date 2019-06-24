#include "label.h"

Label::Label(const QPixmap& image, QWidget *parent) : QLabel(parent), m_image(image)
{
    setPixmap(image);
    setBoxState(Label::Hide);
}

Label::BoxState Label::boxState()
{
    return m_state;
}

void Label::setBoxState(Label::BoxState state)
{
    m_state = state;

    if ( state == Label::Show)
        setPixmap(m_image);
    else if ( state == Label::Hide)
        setPixmap(QPixmap(":/images/GeoPoint.png"));
    else if (state == Label::Matched)
        setEnabled(false);
}
