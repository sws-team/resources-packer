#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_actionOpen_triggered();
	void closeFile();
	void updateSelection();

	void on_actionSave_triggered();

	void on_actionAdd_triggered();

	void on_actionRemove_triggered();

	void on_actionEdit_triggered();

	void on_actionSave_as_triggered();

	void on_actionAbout_triggered();

	void on_actionAdd_list_triggered();

private:
	Ui::MainWindow *ui;
	class QSettings *settings;

	class ResourcesModel* model;
	void openFile(const QString& fileName);
	void saveFile(const QString& fileName);

	static const QString FILE_FORMAT;
	QString currentFileName;
	QString lastResourceFileName;
};
#endif // MAINWINDOW_H
