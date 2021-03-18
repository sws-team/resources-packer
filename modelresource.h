#ifndef MODELRESOURCE_H
#define MODELRESOURCE_H

#include <QByteArray>
#include <QString>

struct ModelResource
{
	int type;
	QString name;
	QByteArray data;
	QString description;
};

#endif // MODELRESOURCE_H
