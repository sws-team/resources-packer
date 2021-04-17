#include "dialogresource.h"
#include "ui_dialogresource.h"

#include <QFileDialog>
#include <QFile>
#include <QFileInfo>

const QStringList DialogResource::RESOURCE_TYPES = QStringList()
		<< tr("Texture")
		<< tr("Audio")
		<< tr("Music")
		<< tr("Font")
		<< tr("Other");

DialogResource::DialogResource(const bool change, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DialogResource)
	,m_change(change)
{
	ui->setupUi(this);
	this->setWindowTitle(tr("Resources dialog"));
	ui->cmb_type->addItems(RESOURCE_TYPES);
}

DialogResource::~DialogResource()
{
	delete ui;
}

void DialogResource::setPath(const QString &path)
{
	ui->edit_file->setText(path);
}

QString DialogResource::currentPath() const
{
	return ui->edit_file->text();
}

void DialogResource::setResource(const ModelResource &resource)
{
	resourceData = resource.data;
	ui->edit_name->setText(resource.name);
	ui->cmb_type->setCurrentIndex(resource.type - 1);
	ui->edit_description->setText(resource.description);
}

ModelResource DialogResource::resource() const
{
	const QString fileName = ui->edit_file->text();
	ModelResource res;
	res.type = ui->cmb_type->currentIndex() + 1;
	res.data = resourceData;
	res.name = ui->edit_name->text();
	res.description = ui->edit_description->toPlainText();
	return res;
}

void DialogResource::on_button_browse_clicked()
{
	const QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), ui->edit_file->text());
	if (fileName.isEmpty())
		return;
	ui->edit_file->setText(fileName);
	if (m_change)
	{
		const QString baseName = QFileInfo(fileName).baseName();
		ui->edit_name->setText(baseName);
		ui->edit_description->setText(baseName);
	}
	QFile file(fileName);
	if (file.open(QIODevice::ReadOnly))
	{
		const QByteArray bytes = file.readAll();
		resourceData = bytes.toBase64();
	}
	file.close();
}
