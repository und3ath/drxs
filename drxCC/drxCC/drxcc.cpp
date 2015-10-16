#include "drxcc.h"

WCHAR *drxCC::m_DeviceName = L"\\\\.\\drx";




drxCC::drxCC(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	if (!OpenDevice())
	{
		QMessageBox m;
		m.setText("Drx Driver can't be opened .");
	}

	connect(ui.pushButton_getNotifyRoutine, SIGNAL(clicked()), this, SLOT(on_GetCallBacksClicked()));
	connect(ui.pushButton_derefCallback, SIGNAL(clicked()), this, SLOT(on_DerefCallbackClicked()));
	connect(ui.pushButton_godShell, SIGNAL(clicked()), this, SLOT(on_ImpersonateKernelClicked()));
	connect(ui.pushButton_setPProcess, SIGNAL(clicked()), this, SLOT(on_ProtectProcessClicked()));
}

drxCC::~drxCC()
{

}

bool drxCC::OpenDevice()
{
	m_DeviceHandle = CreateFile(L"\\\\.\\drx", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_DeviceHandle == INVALID_HANDLE_VALUE)
	{		
		return false;
	}
	return true;
}

void drxCC::on_GetCallBacksClicked()
{
	QString selectedAction = ui.comboBox->currentText();
	ui.treeWidget_callbackList->clear();

	if (selectedAction == "ProcessNotifyRoutine")
	{
		GetNotifyRoutine(IOCTL_LIST_PROCESS_NOTIF);
	}
	else if (selectedAction == "ThreadNotifyRoutine")
	{
		GetNotifyRoutine(IOCTL_LIST_THREAD_NOTIF);
	}
	else if (selectedAction == "ImageNotifyRoutine")
	{
		GetNotifyRoutine(IOCTL_LIST_IMAGE_NOTIF);
	}
	else if (selectedAction == "RegistryNotifyRoutine")
	{
		GetNotifyRoutine(IOCTL_LIST_REG_NOTIF);
	}

}

void drxCC::GetNotifyRoutine(DWORD ioctl)
{
	wchar_t buffer[1024];
	DWORD dwret;
	if (!DeviceIoControl(m_DeviceHandle, ioctl, NULL, 0, &buffer, 1024, &dwret, NULL))
	{
		//std::cout << "Error processing ioctl." << std::endl;
		CloseHandle(m_DeviceHandle);
		return;
	}


	QString buf;
	buf = QString::fromWCharArray(buffer);

	QList<QString> itemj = buf.split("|");
	itemj.removeLast();

	for (int i = 0; i < itemj.count(); i++)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem();
		QList<QString> ll = itemj[i].split("(*)");
		if (ll.count() == 2)
		{
			item->setText(0, ll[0]);
			item->setText(1, ll[1]);
		}
		else
		{
			item->setText(0, itemj[0]);
		}
			
		ui.treeWidget_callbackList->addTopLevelItem(item);
	}


}

void drxCC::on_DerefCallbackClicked()
{
	QString selectedAction = ui.comboBox->currentText();
	ui.treeWidget_callbackList->clear();

	wchar_t data[20];
	
	ui.lineEdit_callback->text().toWCharArray(data);

	PVOID ptrToDeref = (PVOID)_wcstoui64(data, NULL, 0);


	if (selectedAction == "ProcessNotifyRoutine")
	{
		DerefCallback(IOCTL_DEREF_PROCESS_NOTIF, ptrToDeref);
	}
	else if (selectedAction == "ThreadNotifyRoutine")
	{
		DerefCallback(IOCTL_DEREF_THREAD_NOTIF, ptrToDeref);
	}
	else if (selectedAction == "ImageNotifyRoutine")
	{
		DerefCallback(IOCTL_DEREF_IMAGE_NOTIF, ptrToDeref);
	}
}

void drxCC::DerefCallback(DWORD ioctl, PVOID addr)
{
	wchar_t buffer[1024];
	DWORD dwret;
	if (!DeviceIoControl(m_DeviceHandle, ioctl, addr, sizeof(PVOID), &buffer, 1024, &dwret, NULL))
	{
		//std::cout << "Error processing ioctl." << std::endl;
		CloseHandle(m_DeviceHandle);
		return;
	}

	QString buf;
	buf = QString::fromWCharArray(buffer);

	QList<QString> itemj = buf.split("|");

	for (int i = 0; i < itemj.count(); i++)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem();
		item->setText(0, itemj[i]);
		ui.treeWidget_callbackList->addTopLevelItem(item);
	}

}

void drxCC::on_ImpersonateKernelClicked()
{
	ULONG pid = (ULONG)ui.spinBox_pid->value();
	wchar_t buffer[1024];
	DWORD dwret;

	if (!DeviceIoControl(m_DeviceHandle, IOCTL_PROC_DUP_TOKEN, (PVOID)pid, sizeof(ULONG), &buffer, 1024, &dwret, NULL))
	{
		CloseHandle(m_DeviceHandle);
		return;
	}
	QString buf;
	buf = QString::fromWCharArray(buffer);

	QMessageBox msgBox;
	msgBox.setText(buf);
	msgBox.exec();
}

void drxCC::on_ProtectProcessClicked()
{
	DRXDRV_PROCESS_PROTECT_INFORMATION protectInfo = { 0, { 0, 0, {0, 0, 0} } };
	protectInfo.processId = (ULONG)ui.spinBox_pid_pProcess->value();

	BOOL isUnprotect = ui.radioButton_disablePProcess->isChecked() ? true : false;

	if (!isUnprotect)
	{
		protectInfo.SignatureProtection.SignatureLevel = 0x0f;
		protectInfo.SignatureProtection.SectionSignatureLevel = 0x0f;
	}

	wchar_t buffer[1024];
	DWORD dwret;

	if (!DeviceIoControl(m_DeviceHandle, IOCTL_PROC_PROTECT, (PVOID)&protectInfo, sizeof(DRXDRV_PROCESS_PROTECT_INFORMATION), &buffer, 1024, &dwret, NULL))
	{
		CloseHandle(m_DeviceHandle);
		return;
	}

	QString buf;
	buf = QString::fromWCharArray(buffer);

	QMessageBox msgBox;
	msgBox.setText(buf);
	msgBox.exec();


}