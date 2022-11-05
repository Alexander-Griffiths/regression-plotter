#include "qgraphicsaxisitem.h"

QGraphicsAxisItem::QGraphicsAxisItem(char mode, qreal pix_min, qreal pix_max, int num_of_ticks, qreal tick_increment, qreal tick_start, QColor colour)
{
    // Assigns values to private variables.
    this->pix_min = pix_min;
    this->pix_max = pix_max;
    this->mode = mode;
    this->num_of_ticks = num_of_ticks;
    this->tick_increment = tick_increment;
    this->tick_start = tick_start;
    axis_colour = colour;
    n_pix = (pix_max-pix_min)/(double)num_of_ticks;
    margin_x = 30;
    margin_y = 50;
    height_rect = 100;
    length_offset = 0;
    arrow_pix_offset = 15;
    tick_length = 5;
    significant_digit = 2;
}

QRectF QGraphicsAxisItem::boundingRect() const
{

    QRectF boundingRect;
    if (mode == 'x')
    {
        qreal length_box = pix_max - pix_min;
        boundingRect.setRect(pix_max - margin_x, - margin_y, length_box + length_offset, height_rect);
    }
    else
    {
        qreal height_box = pix_max - pix_min;
        boundingRect.setRect(- height_rect, - (pix_max+length_offset), height_rect, height_box + length_offset);
    }

    return boundingRect;

}

void QGraphicsAxisItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Used to draw the axis onto the screen.
    qreal current_value = pix_min;
    qreal current_tick = tick_start;
    QString text;
    // Number of pixels between tick and value.
    int label_offset = 20;
    painter->setPen(axis_colour);
    if (mode == 'x')
    {
        // 0 specifies the coordinates that the line is drawn from in this case (pix_min, 0), (pix_max, 0).
        painter->drawLine(pix_min , 0, pix_max, 0);
        // Arrow at end of axis.
        painter->drawLine(pix_max - arrow_pix_offset, arrow_pix_offset, pix_max, 0);
        painter->drawLine(pix_max - arrow_pix_offset, -arrow_pix_offset, pix_max, 0);
        // For ticks.
        for (int i = 0; i < num_of_ticks; i++)
        {
            painter->drawLine(current_value, 0, current_value, -tick_length);
            // Text item of type float with decimal places of significant_digit.
            text=QString::number(current_tick, 'f', significant_digit);
            painter->drawText(current_value, label_offset, text);
            current_value += n_pix;
            current_tick += tick_increment;
        }

    }
    else if(mode == 'y')
    {
        // Y axis rotate for ease.
        painter->rotate(180);
        // 0 specifies the coordinates that the line is drawn from in this case (0, pix_min), (0, pix_max).
        painter->drawLine(0, pix_min, 0, pix_max);
        // Arrow at end of axis.
        painter->drawLine(arrow_pix_offset, pix_max - arrow_pix_offset, 0, pix_max);
        painter->drawLine(-arrow_pix_offset, pix_max - arrow_pix_offset, 0, pix_max);
        // For ticks.
        for (int i = 0; i < num_of_ticks; i++)
        {
            painter->drawLine(0, current_value, tick_length, current_value);
            // Text item of type float with decimal places of significant_digit.
            text=QString::number(current_tick, 'f', significant_digit);
            painter->rotate(-180);
            painter->drawText(-label_offset-10, -current_value, text);
            painter->rotate(180);
            current_value += n_pix;
            current_tick += tick_increment;
        }

    }



}

qreal QGraphicsAxisItem::pixel_to_data()
{
    return n_pix/tick_increment;
}



