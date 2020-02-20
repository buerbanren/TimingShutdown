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
#include <Windows.h>
#include <qsystemtrayicon.h>
#include <qmenu.h>
#include <QPainter>
#include <QPen>
#include <QDialog>
#include <QEvent>
#include <QFocusEvent>
#include <QRadioButton>
#include <Windows.h>

class TimingShutdown : public QWidget
{
	Q_OBJECT
public:
	TimingShutdown(QWidget *parent = Q_NULLPTR);
	void timing();
	unsigned int timerId;
	unsigned int h,m,s=00;//时分秒
	QString tmm;
	QString tmstr;//lcd显示字符串
	QString boxTitle = QStringLiteral("提示");
	QString boxMessage = QStringLiteral("倒计时结束");
	void TimingShutdown::closeEvent(QCloseEvent *e);

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
	QIcon ico;
	QMenu *trayMenu = new QMenu(this);
	QAction *widgetShow = new QAction(this);
	QAction *exit = new QAction(this);
	QAction *about = new QAction(this);
	QSystemTrayIcon *trayIcon = new QSystemTrayIcon();
	QLabel *cztxt = new QLabel(this);
	QRadioButton *typegj = new QRadioButton(this);
	QRadioButton *typecq = new QRadioButton(this);
	QRadioButton *typezx = new QRadioButton(this);
	HANDLE hd;
	LUID luid;
	TOKEN_PRIVILEGES p;

signals:
	void exitPro();
private slots:
	void timeEvent();

};
