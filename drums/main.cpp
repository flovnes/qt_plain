#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QTimer>

class Drum : public QWidget {
  public:
    Drum(QWidget *parent = nullptr) : QWidget(parent) {}

  protected:
    void paintEvent(QPaintEvent* event) {
      Q_UNUSED(event);
      QPainter painter(this);
      drawDrum(painter);
    }

  private:
    void drawDrum(QPainter& painter) {
      painter.setRenderHint(QPainter::Antialiasing);

      int drumHeadRadius = width()* 0.4;
      int drumHeadX = (width() - drumHeadRadius * 2) / 2;
      int drumHeadY1 = height() * 0.1;
      int drumHeadY2 = height() * 0.5;

      painter.setPen(QColor("#0f0f0f"));
      painter.setBrush(QColor("#0f0f0f"));
      painter.drawEllipse(drumHeadX, drumHeadY2, drumHeadRadius * 2, drumHeadRadius);

      int drumBodyWidth = width() * 0.8;
      int drumBodyHeight = height() * 0.4;
      int drumBodyX = (width() - drumBodyWidth) / 2;
      int drumBodyY = height() * 0.3;

      painter.drawRect(drumBodyX, drumBodyY, drumBodyWidth, drumBodyHeight);

      painter.setPen(QColor("#948e8e"));
      painter.setBrush(QColor("#948e8e"));
      painter.drawEllipse(drumHeadX, drumHeadY1, drumHeadRadius * 2, drumHeadRadius);
    }
};

class Stick : public QWidget {
  public:
    Stick(QWidget *parent = nullptr, bool isStick1 = true) : QWidget(parent), rotation(0), stick1(isStick1) {
      setAttribute(Qt::WA_TranslucentBackground);
    }

    void setRotation(int angle) {
      rotation = angle;
      update();
    }

  protected:
    void paintEvent(QPaintEvent *event) override {
      Q_UNUSED(event);

      QPainter painter(this);
      painter.setRenderHint(QPainter::Antialiasing);

      painter.setBrush(QColor("#cc671f"));
      painter.setPen(QColor("#120602"));

      painter.translate(width() / 2.0, height() / 2.0);
      painter.rotate(rotation);

      QPolygonF stick;
      stick << QPointF(-width() / 2.0, -height() / 16.0)
            << QPointF(width() / 2.0, -height() / 16.0)
            << QPointF(width() / 2.0, height() / 16.0)
            << QPointF(-width() / 2.0, height() / 16.0);
      painter.drawPolygon(stick);

      painter.setBrush(QColor("#cc671f"));
      if (stick1) {
        painter.drawEllipse(QPointF(width() / 2.0 -3.5, 0), height() / 8.0, height() / 8.0);
      } else {
        painter.drawEllipse(QPointF(-width() / 2.0 +3.5, 0), height() / 8.0, height() / 8.0);
      }
    }

  private:
    int rotation;
    bool stick1;
};

class Window : public QWidget {
  public:
    Window(QSize size,QWidget* parent = nullptr);
    void updateRotation();
    void delaySwitch();

  private:
    Stick* stick1;
    Stick* stick2;
    Drum* drum;
    QTimer* timer;
    QTimer* delayTimer;
    int rotationAngle;
    bool increasing;
    bool delayActive;

  };

  Window::Window(QSize size, QWidget* parent) : QWidget(parent), rotationAngle(0) {
    this->setStyleSheet("background-color: #171717;");
    int middle_x = size.width() / 2;
    int middle_y = size.height() / 2;

    drum = new Drum(this);
    stick1 = new Stick(this, true);
    stick2 = new Stick(this, false);

    drum->move(middle_x, middle_y);
    drum->resize(230, 200);

    delayActive = false;
    stick1->move(middle_x -10, middle_y +20);
    stick2->move(middle_x +140, middle_y +20);

    timer = new QTimer(this);
    delayTimer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &Window::updateRotation);
    connect(delayTimer, &QTimer::timeout, this, &Window::delaySwitch);

    timer->start(3);
  }

void Window::updateRotation() {
  if (delayActive) return;
  if (increasing) {
    rotationAngle++;
    if (rotationAngle >= 8) {
      increasing = false;
      delayActive = true;
      delayTimer->start(60);
    }
  } else {
    rotationAngle--;
    if (rotationAngle <= -8) {
      increasing = true;
      delayActive = true;
      delayTimer->start(300);
    }
  }

  stick1->setRotation(rotationAngle);
  stick2->setRotation(rotationAngle);
}

void Window::delaySwitch() { delayActive = false; }

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  Window task_four(QSize(400, 300));
  task_four.show();
  return a.exec();
}
