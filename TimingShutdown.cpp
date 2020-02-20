#include "TimingShutdown.h"
#include <Qdebug>
#include <ctime>
#include <QMessageBox>
#include <QTextCodec>
#include <QString>
#include <qaction.h>
#include <QApplication>

TimingShutdown::TimingShutdown(QWidget *parent)
	: QWidget(parent)
{
	//��Ȩ����
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hd);
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid);
	p.PrivilegeCount = 1;
	p.Privileges[0].Luid = luid;
	p.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hd, false, &p, NULL, NULL, NULL);

	ico.addFile(":/TimingShutdown/Resources/icon.png/");
	setWindowIcon(ico);
	setWindowTitle(QString::fromLocal8Bit("��ʱ�ػ� - by:��С�� �ᰮ�ƽ�"));//���ô��ڱ���
	setMaximumSize(320, 180);//���ƴ��ڴ�С
	setMinimumSize(320, 180);

	//���̲˵�����
	widgetShow->setText(QString::fromLocal8Bit("������"));
	about->setText(QString::fromLocal8Bit("����"));
	exit->setText(QString::fromLocal8Bit("�˳�"));
	trayMenu->addAction(widgetShow);
	trayMenu->addAction(about);
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
			if (timer->isActive())
			{
				timer->stop();
			}
			trayIcon->hide();
			trayMenu->close();
			this->close();
			QApplication::exit(0);
		});
	connect(about, &QAction::triggered,
		[=]()
		{
			MessageBox(NULL, TEXT("designed by ��С�� from �ᰮ�ƽ���̳"), TEXT("����"), MB_OK);
		});

	//��������ͼ�꼰�˵�
	trayIcon->setIcon(ico);
	trayIcon->setToolTip(QString::fromLocal8Bit("LCD�ػ�С����"));
	trayIcon->setContextMenu(trayMenu);
	trayIcon->show();

	ts->setText(QStringLiteral("����ʱ"));//�����ı���ǩ
	ts->move(15, 23);
	xh->setText(QStringLiteral("Сʱ"));
	xh->move(108, 23);
	fz->setText(QStringLiteral("����"));
	fz->move(187, 23);

	hour->setParent(this);//����Сʱ΢����
	hour->setMinimum(0);
	hour->setMaximum(23);
	hour->setValue(0);
	hour->move(60, 20);
	hour->setSingleStep(1);

	minute->setParent(this);//���÷���΢����
	minute->setMinimum(0);
	minute->setMaximum(59);
	minute->setValue(1);
	minute->move(140, 20);
	minute->setSingleStep(5);

	mks->setParent(this);//ȷ����ť
	mks->setText(QString::fromLocal8Bit("ȷ��"));
	mks->move(220, 20);

	cztxt->setText(QString::fromLocal8Bit("����"));
	cztxt->move(15, 58);
	typegj->setChecked(true);
	typegj->setText(QString::fromLocal8Bit("�ػ�"));
	typegj->move(55, 58);
	typecq->setText(QString::fromLocal8Bit("����"));
	typecq->move(105, 58);
	typezx->setText(QString::fromLocal8Bit("ע��"));
	typezx->move(160, 58);

	cancel->setText(QString::fromLocal8Bit("ȡ��"));//ȡ������ʱ
	cancel->move(220, 51);

	connect(mks, &QPushButton::released, this, &TimingShutdown::timing);//��ť�ۺ�������
	connect(cancel, &QPushButton::clicked,//ȡ�����˳�
		[=]()
		{
			if (timer->isActive())
			{
				timer->stop();
				//system("shutdown -a");
				QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"),
					QString::fromLocal8Bit("��ֹͣ����ʱ��"), QMessageBox::StandardButton::Ok);
			}
			else
			{
				QMessageBox::information(this, QString::fromLocal8Bit("��ʾ"),
					QString::fromLocal8Bit("δ��ʼ����ʱ��"), QMessageBox::StandardButton::Ok);
			}
		});

	lcdCount->setDigitCount(8);//lcd���������
	lcdCount->setParent(this);
	lcdCount->resize(240, 60);
	lcdCount->move(39, 90);
	lcdCount->setStyleSheet("color:#f00;text-align:auto;background:#fff;");

	timer->setInterval(1000);//���ö�ʱ��
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
		s = 00;
	}
}

void TimingShutdown::timeEvent()
{
	tmstr=tmstr.setNum(h,10)+':';
	tmm = QString::number(m, 10);
	tmstr = tmstr.append(tmm + ':');
	tmstr = tmstr.append(QString::number(s, 10));
	qDebug() << s<<endl;
	lcdCount->display(tmstr);
	if (s == 0)
	{
		if (m != 0)
			m--;
		else
		{
			if (h != 0)
			{
				h--;
				m = 59;
			}
			else
			{
				timer->stop();
				if (typegj->isChecked())
				{
					//system("shutdown /s /t 0");
					ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0);
				}
				else if (typecq->isChecked())
				{
					//system("shutdown /r /t 0");
					ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);
				}
				else
				{
					//system("shutdown -l");
					ExitWindowsEx(EWX_LOGOFF | EWX_FORCE, 0);
				}
				return;
			}
		}
		s = 59;
	}
	else
		s--;
}

void TimingShutdown::closeEvent(QCloseEvent *e)
{
	e->ignore();
	this->hide();
}
