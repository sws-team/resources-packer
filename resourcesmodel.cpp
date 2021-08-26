#include "resourcesmodel.h"
#include "arcresource.h"

#include <QDebug>
#include <QPixmap>

ResourcesModel::ResourcesModel(QObject *parent)
	: QAbstractTableModel(parent)
{

}

QVariant ResourcesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		switch (section)
		{
		case COLUMN_TYPE:
			return tr("Type");
			break;
		case COLUMN_NAME:
			return tr("Name");
			break;
		case COLUMN_DESCRIPTION:
			return tr("Description");
			break;
		case COLUMN_DATA:
			return tr("Data");
			break;
		default:
			break;
		}
	}
	return QVariant();
}

int ResourcesModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return m_resources.size();
}

int ResourcesModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return COLUMNS_COUNT;
}

QVariant ResourcesModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		const ModelResource res = m_resources.value(index.row());
		switch (index.column())
		{
		case COLUMN_TYPE:
		{
			switch (res.type)
			{
			case ArcEngine::UNKNOWN:
				return tr("Unknown");
			case ArcEngine::TEXTURE:
				return tr("Texture");
			case ArcEngine::SOUND:
				return tr("Sound");
			case ArcEngine::MUSIC:
				return tr("Music");
			case ArcEngine::FONT:
				return tr("Font");
			case ArcEngine::OTHER:
				return tr("Other");
			default:
				break;
			}
		}
			break;
		case COLUMN_NAME:
			return res.name;
			break;
		case COLUMN_DESCRIPTION:
			return res.description;
			break;
		default:
			break;
		}
	}
	else if (role == Qt::DecorationRole)
	{
		const ModelResource res = m_resources.value(index.row());
		switch (index.column())
		{
		case COLUMN_DATA:
		{
			switch (res.type)
			{
			case ArcEngine::TEXTURE:
			{
				QByteArray bytes;
				bytes = QByteArray::fromBase64(res.data);

				QPixmap p;
				p.loadFromData(bytes,"png");
				p = p.scaled(128, 128, Qt::KeepAspectRatio);
				return p;
			}
			default:
				break;
			}
		}
			break;
		default:
			break;
		}
	}
	return QVariant();
}

ModelResource ResourcesModel::getResource(int row) const
{
	return m_resources.value(row);
}

void ResourcesModel::setResources(const QList<ModelResource> &resources)
{
	beginResetModel();
	m_resources = resources;
	endResetModel();
}

QList<ModelResource> ResourcesModel::resources() const
{
	return m_resources;
}

void ResourcesModel::editResource(int row, const ModelResource &resource)
{
	m_resources[row] = resource;
}

void ResourcesModel::addResource(const ModelResource &resource)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	m_resources.push_back(resource);
	endInsertRows();
}

void ResourcesModel::removeResource(int row)
{
	beginRemoveRows(QModelIndex(), row, row);
	m_resources.removeAt(row);
	endRemoveRows();
}

void ResourcesModel::clear()
{
	beginResetModel();
	m_resources.clear();
	endResetModel();
}
