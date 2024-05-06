#include <QApplication>
#include <QWidget>
#include <QPropertyAnimation>
#include <QPainter>

class TriangleWidget : public QWidget {
  public:
    TriangleWidget(QWidget *parent = nullptr) : QWidget(parent) {}

  protected:
    void paintEvent(QPaintEvent *event) override {
      Q_UNUSED(event);

      QPainter painter(this);
      painter.setRenderHint(QPainter::Antialiasing);

      QString Qs = "#32a852";
      QColor brush(Qs);

      painter.setBrush(brush);

      Qs = "#19612d";
      QPen pen(Qs);
      painter.setPen(pen);

      QPolygonF triangle;
      triangle << QPointF(width() / 2, 1) << QPointF(0, height()) << QPointF(width()-1, height());

      painter.drawPolygon(triangle);
  }
};

class TaskThree : public QWidget {
  public:
    TaskThree(QWidget *parent = nullptr);
    void animationLoop();
    void initSizeAnim(int &anim_time, int &tr_end_wdh, int &tr_height, int &tr_start_wdh);
    void sizeForward(int &anim_time, int &tr_start_wdh, int &tr_height, int &tr_end_wdh);
    void initPositionAnim(int &anim_time, int &middle_x, int &start_y, int &left_x);
    void positionForward(int &anim_time, int &left_x, int &start_y, int &middle_x);
  private slots:
    void callPosForward();
    void callPosReverse();
    void callSizeForward();
    void callSizeReverse();
  private:
    TriangleWidget *triangleWidget;
    QPropertyAnimation *animPosForward;
    QPropertyAnimation *animPosReverse;
    QPropertyAnimation *animSizeForward;
    QPropertyAnimation *animSizeReverse;
};

TaskThree::TaskThree(QWidget *parent) : QWidget(parent) {
  this->setStyleSheet("background-color: #171717;");
  int tr_start_wdh = 120;
  int tr_end_wdh = 0;
  int tr_height = 60;
  int middle_x = this->size().rwidth()/2;
  int middle_y =this->size().rheight()/2;
  int start_y = middle_y - tr_height/2;
  int left_x = middle_x - tr_start_wdh;
  int anim_time = 500;

  triangleWidget = new TriangleWidget(this);

  initPositionAnim(anim_time, middle_x, start_y, left_x);
  initSizeAnim(anim_time, tr_end_wdh, tr_height, tr_start_wdh);

  animPosForward->start();
  animSizeForward->start();

  animationLoop();
}

void TaskThree::animationLoop()
{
  connect(animPosForward, &QPropertyAnimation::finished, this, &TaskThree::callSizeReverse);
  connect(animSizeReverse, &QPropertyAnimation::finished, this, &TaskThree::callSizeForward);
  connect(animSizeForward, &QPropertyAnimation::finished, this, &TaskThree::callPosReverse);
  connect(animPosReverse, &QPropertyAnimation::finished, this, &TaskThree::callPosForward);
}

void TaskThree::initSizeAnim(int &anim_time, int &tr_end_wdh, int &tr_height, int &tr_start_wdh)
{
  animSizeReverse = new QPropertyAnimation(triangleWidget, "size", this);
  animSizeReverse->setDuration(anim_time);
  animSizeReverse->setStartValue(QSize(tr_end_wdh, tr_height));
  animSizeReverse->setEndValue(QSize(tr_start_wdh, tr_height));

  animSizeForward = new QPropertyAnimation(triangleWidget, "size", this);
  animSizeForward->setDuration(anim_time);
  animSizeForward->setStartValue(QSize(tr_start_wdh, tr_height));
  animSizeForward->setEndValue(QSize(tr_end_wdh, tr_height));
}

void TaskThree::initPositionAnim(int &anim_time, int &middle_x, int &start_y, int &left_x)
{
  animPosReverse = new QPropertyAnimation(triangleWidget, "pos", this);
  animPosReverse->setDuration(anim_time);
  animPosReverse->setStartValue(QPoint(middle_x, start_y));
  animPosReverse->setEndValue(QPoint(left_x, start_y));

  animPosForward = new QPropertyAnimation(triangleWidget, "pos", this);
  animPosForward->setDuration(anim_time);
  animPosForward->setStartValue(QPoint(left_x, start_y));
  animPosForward->setEndValue(QPoint(middle_x, start_y));
}

void TaskThree::callPosForward() {
  animPosForward->start();
  animSizeForward->start();
}

void TaskThree::callPosReverse() {
  animPosReverse->start();
  animSizeReverse->start();
}

void TaskThree::callSizeForward() {
  animSizeForward->start();
}

void TaskThree::callSizeReverse() {
  animSizeReverse->start();
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  TaskThree buttonAnimWidget;
  buttonAnimWidget.resize(QSize(640, 500));
  buttonAnimWidget.show();
  return a.exec();
}
