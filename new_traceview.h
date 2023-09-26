#ifndef NEW_TRACEVIEW_H
#define NEW_TRACEVIEW_H
#include <qsplitter.h>
#include <QStandardItem>

class RightForm;
class IKO;
class SI_XH;
class DocTras;

class New_traceView : public QSplitter
{

public:
 New_traceView(QWidget* pwgt=0);
    ~New_traceView();
    IKO* pIKO;
    SI_XH*  psi_xh;
    inline int GetCurSel(){return m_ntr_val;}
    void SetCurSel(int cur_sel);

    inline DocTras* GetDocument()
       { return (DocTras*)m_pDocument; }

    void CorNTras(int num_pos);
    void FillElList(int i);
    void FillList();
    void AddTras(float B, float D);
    int GetDefH();
    unsigned int GetDefA();
    unsigned int GetDefV();

    RightForm* pright1;

private:
   int		m_ntr_val;  // номер трассы, выбранной в комбо
   DocTras* m_pDocument;
   QStandardItemModel* modelTrPoints; // модель точек трассы

};

#endif // NEW_TRACEVIEW_H
