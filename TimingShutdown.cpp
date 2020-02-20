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
	//提权操作
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hd);
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid);
	p.PrivilegeCount = 1;
	p.Privileges[0].Luid = luid;
	p.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hd, false, &p, NULL, NULL, NULL);

	ico.addFile(":/TimingShutdown/Resources/icon.png/");
	setWindowIcon(ico);
	setWindowTitle(QString::fromLocal8Bit("定时关机 - by:川小波 吾爱破解"));//设置窗口标题
	setMaximumSize(320, 180);//限制窗口大小
	setMinimumSize(320, 180);

	//托盘菜单设置
	widgetShow->setText(QString::fromLocal8Bit("主界面"));
	about->setText(QString::fromLocal8Bit("关于"));
	exit->setText(QString::fromLocal8Bit("退出"));
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
			MessageBox(NULL, TEXT("designed by 川小波 from 吾爱破解论坛"), TEXT("关于"), MB_OK);
		});

	//设置托盘图标及菜单
	trayIcon->setIcon(ico);
	trayIcon->setToolTip(QString::fromLocal8Bit("LCD关机小程序"));
	trayIcon->setContextMenu(trayMenu);
	trayIcon->show();

	ts->setText(QStringLiteral("倒计时"));//设置文本标签
	ts->move(15, 23);
	xh->setText(QStringLiteral("小时"));
	xh->move(108, 23);
	fz->setText(QStringLiteral("分钟"));
	fz->move(187, 23);

	hour->setParent(this);//设置小时微调栏
	hour->setMinimum(0);
	hour->setMaximum(23);
	hour->setValue(0);
	hour->move(60, 20);
	hour->setSingleStep(1);

	minute->setParent(this);//设置分钟微调栏
	minute->setMinimum(0);
	minute->setMaximum(59);
	minute->setValue(1);
	minute->move(140, 20);
	minute->setSingleStep(5);

	mks->setParent(this);//确定按钮
	mks->setText(QString::fromLocal8Bit("确定"));
	mks->move(220, 20);

	cztxt->setText(QString::fromLocal8Bit("操作"));
	cztxt->move(15, 58);
	typegj->setChecked(true);
	typegj->setText(QString::fromLocal8Bit("关机"));
	typegj->move(55, 58);
	typecq->setText(QString::fromLocal8Bit("重启"));
	typecq->move(105, 58);
	typezx->setText(QString::fromLocal8Bit("注销"));
	typezx->move(160, 58);

	cancel->setText(QString::fromLocal8Bit("取消"));//取消倒计时
	cancel->move(220, 51);

	connect(mks, &QPushButton::released, this, &TimingShutdown::timing);//按钮槽函数连接
	connect(cancel, &QPushButton::clicked,//取消并退出
		[=]()
		{
			if (timer->isActive())
			{
				timer->stop();
				//system("shutdown -a");
				QMessageBox::information(this, QString::fromLocal8Bit("提示"),
					QString::fromLocal8Bit("已停止倒计时！"), QMessageBox::StandardButton::Ok);
			}
			else
			{
				QMessageBox::information(this, QString::fromLocal8Bit("提示"),
					QString::fromLocal8Bit("未开始倒计时！"), QMessageBox::StandardButton::Ok);
			}
		});

	lcdCount->setDigitCount(8);//lcd数码框设置
	lcdCount->setParent(this);
	lcdCount->resize(240, 60);
	lcdCount->move(39, 90);
	lcdCount->setStyleSheet("color:#f00;text-align:auto;background:#fff;");

	timer->setInterval(1000);//设置定时器
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
