#ifndef RESOURCESMODEL_H
#define RESOURCESMODEL_H

#include <QAbstractTableModel>
#include "modelresource.h"

class ResourcesModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit ResourcesModel(QObject *parent = nullptr);

	// Header:
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	// Basic functionality:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	ModelResource getResource(int row) const;
	void setResources(const QList<ModelResource> &resources);

	QList<ModelResource> resources() const;

	void editResource(int row, const ModelResource& resource);
	void addResource(const ModelResource& resource);
	void removeResource(int row);

	void clear();

private:

	enum COLUMNS
	{
		COLUMN_TYPE,
		COLUMN_NAME,
		COLUMN_DESCRIPTION,
		COLUMN_DATA,

		COLUMNS_COUNT
	};
	QList <ModelResource> m_resources;
};

#endif // RESOURCESMODEL_H
