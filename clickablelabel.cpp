#include "clickablelabel.h"

ClickableLabel::ClickableLabel(const QPixmap& pixmap, QWidget* parent)
    : QLabel(parent)
{
    setPixmap(pixmap);
}

ClickableLabel::~ClickableLabel()
{
}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    emit clicked();
}
