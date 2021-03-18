#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameresource.h"
#include "modelresource.h"
#include "resourcesmodel.h"
#include "dialogresource.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QDebug>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QStyleFactory>

const QString MainWindow::FILE_FORMAT = QObject::tr("Resources file (*.dat)");

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	this->setWindowTitle(tr("Resources Packer"));
	qApp->setStyle(QStyleFactory::create("fusion"));

	settings = new QSettings(qApp->applicationDirPath() + "/settings.ini", QSettings::IniFormat, this);
	const QString lastFileName = settings->value("lastFileName").toString();

	model = new ResourcesModel(this);
	ui->tableView->setModel(model);
	ui->tableView->horizontalHeader()->setStretchLastSection(true);

	ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

	connect(ui->tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::updateSelection);
	connect(ui->actionClose, &QAction::triggered, this, &MainWindow::closeFile);
	connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);

	if (!lastFileName.isEmpty())
	{
		currentFileName = lastFileName;
		openFile(lastFileName);
	}
	updateSelection();
}

MainWindow::~MainWindow()
{
	settings->setValue("lastFileName", currentFileName);
	delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
	const QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), currentFileName, FILE_FORMAT);
	if (fileName.isEmpty())
		return;
	closeFile();
	openFile(fileName);
	currentFileName = fileName;
}

void MainWindow::closeFile()
{
	model->clear();
	currentFileName.clear();
}

void MainWindow::updateSelection()
{
	const bool enabled = !ui->tableView->selectionModel()->selectedRows(0).isEmpty();
	ui->actionRemove->setEnabled(enabled);
	ui->actionEdit->setEnabled(enabled);
}

void MainWindow::on_actionSave_triggered()
{
	if (currentFileName.isEmpty())
	{
		on_actionSave_as_triggered();
		return;
	}
	saveFile(currentFileName);
}

void MainWindow::openFile(const QString &fileName)
{
	QList<ModelResource> modelResources;
	const std::vector<GameResource::Resource> resources = GameResource::Resource::loadResources(fileName.toStdString());
	for(const GameResource::Resource &resource : resources)
	{
		ModelResource res;
		res.type = resource.type;
		res.data = QByteArray::fromStdString(resource.data);
		res.name = QString::fromStdString(resource.name);
		res.description = QString::fromStdString(resource.description);
		modelResources.push_back(res);
	}
	model->setResources(modelResources);
}

void MainWindow::saveFile(const QString &fileName)
{
	std::vector<GameResource::Resource> resources;
	const QList<ModelResource> modelResources = model->resources();
	foreach(const ModelResource& res, modelResources)
	{
		GameResource::Resource resource;
		resource.type = static_cast<GameResource::RESOURCE_TYPE>(res.type);
		resource.data = res.data.toStdString();
		resource.name = res.name.toStdString();
		resource.description = res.description.toStdString();
		resources.push_back(resource);
	}
	const bool result = GameResource::Resource::saveResources(fileName.toStdString(), resources);
	if (!result)
		QMessageBox::critical(this, tr("Error"), tr("Error to save file"));
}

void MainWindow::on_actionAdd_triggered()
{
	DialogResource dlgResource(true, this);
	dlgResource.setPath(lastResourceFileName);
	if (dlgResource.exec() == QDialog::Accepted)
	{
		model->addResource(dlgResource.resource());
		lastResourceFileName = dlgResource.currentPath();
	}
}

void MainWindow::on_actionRemove_triggered()
{
	const QModelIndex index = ui->tableView->currentIndex();
	if (!index.isValid())
		return;

	const int row = index.row();
	model->removeResource(row);
}

void MainWindow::on_actionEdit_triggered()
{
	const QModelIndex index = ui->tableView->currentIndex();
	if (!index.isValid())
		return;

	const int row = index.row();
	DialogResource dlgResource(false, this);
	dlgResource.setResource(model->getResource(row));
	if (dlgResource.exec() == QDialog::Accepted)
	{
		model->editResource(row, dlgResource.resource());
	}
}

void MainWindow::on_actionSave_as_triggered()
{
	const QString fileName = QFileDialog::getSaveFileName(this, tr("Save"), currentFileName, FILE_FORMAT);
	if (fileName.isEmpty())
		return;
	saveFile(fileName);
}

void MainWindow::on_actionAbout_triggered()
{
	QMessageBox::information(this, tr("About"), this->windowTitle() + tr("\nCreated by SWS TEAM\n2020"));
}

void MainWindow::on_actionAdd_list_triggered()
{
	QDialog dlgType(this);
	QVBoxLayout layout(&dlgType);
	dlgType.setLayout(&layout);
	QComboBox cmbType(&dlgType);
	cmbType.addItems(DialogResource::RESOURCE_TYPES);
	QDialogButtonBox buttonnBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlgType);
	layout.addWidget(&cmbType);
	layout.addWidget(&buttonnBox);
	connect(&buttonnBox, &QDialogButtonBox::accepted, &dlgType, &QDialog::accept);
	connect(&buttonnBox, &QDialogButtonBox::rejected, &dlgType, &QDialog::reject);

	if (dlgType.exec() != QDialog::Accepted)
		return;

	const int type = cmbType.currentIndex() + 1;
	const QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open"), currentFileName);
	foreach(const QString& fileName, fileNames)
	{
		ModelResource resource;
		QFile file(fileName);
		if (file.open(QIODevice::ReadOnly))
		{
			const QByteArray bytes = file.readAll();
			resource.data = bytes.toBase64();
		}
		file.close();
		resource.name = QFileInfo(fileName).baseName();
		resource.description = resource.name;
		resource.type = type;

		model->addResource(resource);
	}
}
