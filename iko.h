#ifndef IKO_H
#define IKO_H

#include <QWidget>

class IKO : public QWidget
{
    Q_OBJECT
public:
    explicit IKO(QWidget *parent = 0);
protected:
 virtual void paintEvent(QPaintEvent*) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
////    void contextMenuEvent(QContextMenuEvent *event) override;
    void slotShowContextMenu(const QPoint& pos);
signals:
    void signalBearing(QString strb);
    void signalDistance(QString d);
    void sigXkm(QString sxkm);
    void sigYkm(QString sykm);
    void signalAddTras(float b,float dxykm);
 ////   void customContextMenuRequested(const QPoint&);
public slots:
    void OnDelTr();
    void OnRubeg();
    void OnDelTP();
    void OnProp();

private:
    QList<QPoint> mDrawBuffer;
    void GetDB(QPoint mp, float &distance_km, float &b);
    bool DragMode;
    QPoint dragPoint;
    int num_point;  // найденная точка трассы при нажатии мышью
    QPixmap pix;
    QSize IkoSize;  // размер ИКО в пикселях
    
};

#endif // IKO_H
