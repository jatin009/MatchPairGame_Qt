#include "widget.h"
#include "label.h"
#include <QGridLayout>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDirIterator>
#include <QMessageBox>
#include <QtDebug>
#include <QTime>
#include <QTimer>

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent), m_rows(4), m_cols(2), m_labels(new Label*[m_rows*m_cols]),
      m_prevBox(NULL), m_currentBox(NULL), m_timer(new QTimer(this))
{
    if (m_rows%2 != 0 && m_cols%2 != 0)
    {
        QMessageBox::information(this, "Must be even", QString("Game requires only even number of boxes."), QMessageBox::Ok);
        exit(0);
    }

    if( m_rows < 2 || m_cols < 2)
    {
        QMessageBox::information(this, "Insufficient Boxes", QString("At least 2 rows and 2 columns needed."), QMessageBox::Ok);
        exit(0);
    }

    //get all images in .qrc file
    QDirIterator it(":/images", QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QString imgName = it.next();
        if ( imgName == ":/images/GeoPoint.png" || imgName == ":/images/images.qrc")
            continue;
        m_imageNames << imgName;
    }

    int size = m_rows*m_cols/2;

    //check if minimum number of images are present in .qrc file
    if ( m_imageNames.size() > size )
    {
        while (m_imageNames.size() != size ) {
            m_imageNames.removeLast();
        }
    }
    else
    {
        QMessageBox::information(this, "Insufficient Images", QString("Minimum %1 images are required.").arg(m_rows*2), QMessageBox::Ok);
        exit(0);
    }

    m_imageNames.append(m_imageNames);
    std::random_shuffle(m_imageNames.begin(), m_imageNames.end());

    QGridLayout* grid = new QGridLayout(this);

    for (int i=0;i<m_rows*m_cols;i++)
    {
        m_labels[i] = new Label( QPixmap(m_imageNames.at(i)), this);
        m_labels[i]->setMaximumSize(100,100);

        m_labels[i]->setScaledContents(true);
        m_labels[i]->setFrameShape(QFrame::Box);
        grid->addWidget(m_labels[i], i/m_cols, i%m_cols);
    }

    QLabel *scoreLabel = new QLabel(this);
    scoreLabel->setFont(QFont("PT Sans", 18));
    scoreLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    grid->addWidget(scoreLabel, m_rows, 0, 1, m_cols);

    m_timer->setSingleShot(true);
    m_timer->setInterval(1000);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timerExpired()));

    setGeometry(10, 10, 1000, 800);
}

Widget::~Widget()
{
    
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if ( m_timer->isActive())
        return;

    Label* obj = dynamic_cast<Label*>(childAt(event->pos()));

    if ( !obj)
        return;

    int state = obj->boxState();

    if ( state == Label::Hide )
    {
        if ( m_prevBox == NULL && m_currentBox == NULL )
        {
            obj->setBoxState(Label::Show);
            m_currentBox = obj;            
        }
        else if ( m_currentBox != NULL && m_prevBox == NULL )
        {
            obj->setBoxState(Label::Show);
            m_prevBox = m_currentBox;
            m_currentBox = obj;

            m_timer->start();            
        }
        return;
    }
    else if ( state == Label::Show || state == Label::Matched )
        return;
}

void Widget::timerExpired()
{    
    static int scoreCount = 0;
    static int moves = 0;
    moves++;

    //match images; if match change state to matched else change to hide
    if( m_prevBox->pixmap()->toImage() == m_currentBox->pixmap()->toImage() )
    {
        m_prevBox->setBoxState(Label::Matched);
        m_currentBox->setBoxState(Label::Matched);
        scoreCount += 2;        

        if ( scoreCount == m_rows*m_cols )
        {
                static_cast<QLabel*>(static_cast<QGridLayout*>(this->layout())->itemAtPosition(m_rows, 1)->widget())->setText(QString("Game Score: %1 | Moves: %2").arg(scoreCount).arg(moves));

            QMessageBox::information(this, "Game Over", QString("Game Over!\nYour score is %1").arg(scoreCount*100/moves), QMessageBox::Ok);
            exit(0);
        }
    }
    else
    {
        m_prevBox->setBoxState(Label::Hide);
        m_currentBox->setBoxState(Label::Hide);
    }

    static_cast<QLabel*>(static_cast<QGridLayout*>(this->layout())->itemAtPosition(m_rows, 1)->widget())->setText(QString("Game Score: %1 | Moves: %2").arg(scoreCount).arg(moves));

    m_prevBox = NULL;
    m_currentBox = NULL;
    return;
}
