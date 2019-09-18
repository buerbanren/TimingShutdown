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
	setWindowTitle(QString::fromLocal8Bit("��ʱ�ػ�"));//���ô��ڱ���
	setMaximumSize(320, 180);//���ƴ��ڴ�С
	setMinimumSize(320, 180);

	//���̲˵�����
	QMenu *trayMenu=new QMenu(this);
	QAction *widgetShow =new QAction(this);
	QAction *exit =new QAction(this);
	QAction *about = new QAction(this);
	about->setText(QString::fromLocal8Bit("����"));
	widgetShow->setText(QString::fromLocal8Bit("������"));
	exit->setText(QString::fromLocal8Bit("�˳�"));
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
			QMessageBox::information(this, QString::fromLocal8Bit("����"), QString::fromLocal8Bit("��С����Ʒ\n��ӭ��λָ����\nemail��moyuyx@outlook.com"));
		});

	//��������ͼ�꼰�˵�
	QSystemTrayIcon *trayIcon=new QSystemTrayIcon(this);
	trayIcon->setIcon(ico);
	trayIcon->setToolTip(QString::fromLocal8Bit("LCD�ػ�С����"));
	trayIcon->setContextMenu(trayMenu);
	trayIcon->show();

	ts->setText(QStringLiteral("����ʱ"));//�����ı���ǩ
	ts->move(15, 38);

	xh->setText(QStringLiteral("Сʱ"));
	xh->move(108, 38);

	fz->setText(QStringLiteral("����"));
	fz->move(187, 38);

	hour->setParent(this);//����Сʱ΢����
	hour->setMinimum(0);
	hour->setMaximum(23);
	hour->setValue(1);
	hour->move(60, 35);
	hour->setSingleStep(1);

	minute->setParent(this);//���÷���΢����
	minute->setMinimum(0);
	minute->setMaximum(59);
	minute->setValue(0);
	minute->move(140, 35);
	minute->setSingleStep(5);

	mks->setParent(this);//ȷ����ť
	mks->setText(QString::fromLocal8Bit("ȷ��"));
	mks->move(220, 35);

	cancel->setText(QString::fromLocal8Bit("ȡ��"));//ȡ������ʱ
	cancel->move(220, 61);

	connect(mks, &QPushButton::released, this, &TimingShutdown::timing);//��ť�ۺ�������
	connect(cancel, &QPushButton::clicked,
		[=]()
		{
			timer->stop();
		});//ȡ�����˳�

	lcdCount->setDigitCount(8);//lcd���������
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
		QMessageBox::warning(this, QString::fromLocal8Bit("����"), QStringLiteral("��ʱ����Ϊ��ֵ"), QMessageBox::Ok, QMessageBox::NoButton);
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
		if (h == 0 && m == 0)//����ʱ����
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