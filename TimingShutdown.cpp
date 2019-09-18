#include "TimingShutdown.h"
#include <Qdebug>
#include <ctime>
#include <QMessageBox>
#include <QTextCodec>
#include <QString>
#include <qmenubar.h>
#include <qmenu.h>
#include <qaction.h>
#include <qsystemtrayicon.h>
#include <QApplication>

TimingShutdown::TimingShutdown(QWidget *parent)
	: QWidget(parent)
{
	QIcon ico;
	ico.addFile(":/TimingShutdown/Resources/icon.png/");
	setWindowIcon(ico);
	setWindowTitle(QString::fromLocal8Bit("定时关机"));//设置窗口标题
	setMaximumSize(320, 180);//限制窗口大小
	setMinimumSize(320, 180);

	//托盘菜单设置
	QMenu *trayMenu=new QMenu(this);
	QAction *widgetShow =new QAction(this);
	QAction *exit =new QAction(this);
	QAction *about = new QAction(this);
	about->setText(QString::fromLocal8Bit("关于"));
	widgetShow->setText(QString::fromLocal8Bit("主界面"));
	exit->setText(QString::fromLocal8Bit("退出"));
	trayMenu->addAction(about);
	trayMenu->addAction(widgetShow);
	trayMenu->addSeparator();
	trayMenu->addAction(exit);
	connect(widgetShow, &QAction::triggered,
		[=]()
		{
			this->show();
		});
	connect(exit, &QAction::triggered,
		[=]()
		{
			timer->stop();
			trayMenu->close();
			this->close();
			QApplication::exit(0);
		});
	connect(about, &QAction::triggered,
		[=]()
		{
			QMessageBox::information(this, QString::fromLocal8Bit("关于"), QString::fromLocal8Bit("川小波出品\n欢迎各位指导！\nemail：moyuyx@outlook.com"));
		});

	//设置托盘图标及菜单
	QSystemTrayIcon *trayIcon=new QSystemTrayIcon(this);
	trayIcon->setIcon(ico);
	trayIcon->setToolTip(QString::fromLocal8Bit("LCD关机小程序"));
	trayIcon->setContextMenu(trayMenu);
	trayIcon->show();

	ts->setText(QStringLiteral("倒计时"));//设置文本标签
	ts->move(15, 38);

	xh->setText(QStringLiteral("小时"));
	xh->move(108, 38);

	fz->setText(QStringLiteral("分钟"));
	fz->move(187, 38);

	hour->setParent(this);//设置小时微调栏
	hour->setMinimum(0);
	hour->setMaximum(23);
	hour->setValue(1);
	hour->move(60, 35);
	hour->setSingleStep(1);

	minute->setParent(this);//设置分钟微调栏
	minute->setMinimum(0);
	minute->setMaximum(59);
	minute->setValue(0);
	minute->move(140, 35);
	minute->setSingleStep(5);

	mks->setParent(this);//确定按钮
	mks->setText(QString::fromLocal8Bit("确定"));
	mks->move(220, 35);

	cancel->setText(QString::fromLocal8Bit("取消"));//取消倒计时
	cancel->move(220, 61);

	connect(mks, &QPushButton::released, this, &TimingShutdown::timing);//按钮槽函数连接
	connect(cancel, &QPushButton::clicked,
		[=]()
		{
			timer->stop();
		});//取消并退出

	lcdCount->setDigitCount(8);//lcd数码框设置
	lcdCount->setParent(this);
	lcdCount->resize(240, 60);
	lcdCount->move(39, 90);
	lcdCount->setStyleSheet("color:#f00;text-align:auto;background:#0e0;borer-style:none;");

	timer->setInterval(1000);
	timer->stop();
	connect(timer, SIGNAL(timeout()), this, SLOT(timeEvent()));


}

void TimingShutdown::timing()
{
	h = hour->value();
	m = minute->value();
	if (h == 0 && m == 0)
	{
		QMessageBox::warning(this, QString::fromLocal8Bit("警告"), QStringLiteral("计时不可为空值"), QMessageBox::Ok, QMessageBox::NoButton);
	}
	else
	{
		timer->start();
		m--;

	}

}

void TimingShutdown::timeEvent()
{
	tmstr=tmstr.setNum(h,10)+':';
	tmm = QString::number(m, 10);
	tmstr = tmstr.append(tmm + ':');
	tmstr = tmstr.append(QString::number(s, 10));
	
	lcdCount->display(tmstr);
	if (s--==0)
	{
		if (h == 0 && m == 0)//倒计时结束
				{
					timer->stop();
					system("shutdown -s -t 0");
				}
		if (m-- == 0 && h != 0)
		{
			h--, m = 59;
		}
		if (h == 0)
		{
			
		}
		
		s = 59;
	}
}

void TimingShutdown::closeEvent(QCloseEvent *e)
{
	e->ignore();
	this->hide();

}