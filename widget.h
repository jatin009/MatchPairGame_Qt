#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Label;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    const int m_rows;
    const int m_cols;
    Label** m_labels;

    Label* m_prevBox;
    Label* m_currentBox;

    QTimer* m_timer;
    QStringList m_imageNames;

    void mousePressEvent(QMouseEvent *);

private slots:
    void timerExpired();
};

#endif // WIDGET_H
