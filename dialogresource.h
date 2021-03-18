#ifndef DIALOGRESOURCE_H
#define DIALOGRESOURCE_H

#include <QDialog>

#include "modelresource.h"

namespace Ui {
class DialogResource;
}

class DialogResource : public QDialog
{
	Q_OBJECT

public:
	explicit DialogResource(const bool change, QWidget *parent = nullptr);
	~DialogResource();

	void setPath(const QString& path);
	QString currentPath() const;

	void setResource(const ModelResource& resource);
	ModelResource resource() const;

	static const QStringList RESOURCE_TYPES;

private slots:
	void on_button_browse_clicked();

private:
	Ui::DialogResource *ui;
	QByteArray resourceData;
	bool m_change;
};

#endif // DIALOGRESOURCE_H
