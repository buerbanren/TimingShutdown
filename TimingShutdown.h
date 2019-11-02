#pragma once

#include <QtWidgets/QWidget>
#include <QSpinBox>
#include <QDate>
#include <QPushButton>
#include <QLcdNumber>
#include <QTimer>
#include <QLabel>
#include <QIcon>
#include <QEvent>
#include <QMessageBox>
#include <QAction>
#include <QCloseEvent>

class TimingShutdown : public QWidget
{
	Q_OBJECT
public:
	TimingShutdown(QWidget *parent = Q_NULLPTR);
	void timing();
	unsigned int timerId;
	unsigned int h,m,s=00;//ʱ����
	QString tmm;
	QString tmstr;//lcd��ʾ�ַ���
	QString boxTitle = QStringLiteral("��ʾ");
	QString boxMessage = QStringLiteral("����ʱ����");
	void closeEvent(QCloseEvent *e);

private:
	QSpinBox *hour= new QSpinBox(this);
	QSpinBox *minute = new QSpinBox(this);
	QLCDNumber *lcdCount=new QLCDNumber(this);
	QTimer *timer = new QTimer(this);
	QPushButton *mks = new QPushButton(this);
	QPushButton *cancel = new QPushButton(this);
	tm ltm;
	QLabel *ts = new QLabel(this);
	QLabel *xh = new QLabel(this);
	QLabel *fz = new QLabel(this);

signals:
	void exitPro();
private slots:
	void timeEvent();

};
