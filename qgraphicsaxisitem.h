#ifndef QGRAPHICSAXISITEM_H
#define QGRAPHICSAXISITEM_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>
#include <QString>

class QGraphicsAxisItem : public QGraphicsItem
{
public:
    // Constructor.
    QGraphicsAxisItem(char mode, qreal pix_min, qreal pix_max, int num_of_ticks, qreal tick_increment, qreal tick_start, QColor colour);
    // Required becuase of the inherited QGraphicsItem.
    QRectF boundingRect() const override;
    // Paints the item, in this case the axis.
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    // Used for comverting the pixel values into data.
    qreal pixel_to_data();
private:
    QRectF boundingRectx, boundingRecty;
    QColor axis_colour;
    // Maximum and minimum pixel values.
    qreal pix_min, pix_max;
    // Number of pixels in a given interval.
    qreal n_pix;
    // Defines either x or y for axis.
    char mode;
    // Number of ticks along the axis.
    int num_of_ticks;
    // Tick length.
    int tick_length;
    // The increments along each axis.
    qreal tick_increment;
    // The starting tick value.
    qreal tick_start;
    // The margin that surrounds the axis.
    int margin_x;
    int margin_y;
    int height_rect;
    int length_offset;
    // Sets the arrow pixel offset.
    int arrow_pix_offset;
    // Number of decimal places value label has.
    int significant_digit;
};

#endif // QGRAPHICSAXISITEM_H
