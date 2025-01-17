#include "asemanabstractquicklistmodelhint.h"

#include <QRegExp>

AsemanAbstractQuickListModelHint::AsemanAbstractQuickListModelHint(QObject *parent) :
    QObject(parent)
{

}

QVariant AsemanAbstractQuickListModelHint::getPathValue(QVariant data, const QString &path)
{
    if (path.isEmpty())
        return data;

    QStringList pathList = path.split(QStringLiteral("->"));
    for (const QString &pt: pathList)
    {
        QRegExp rx(QStringLiteral("(\\w+)\\s*\\[\\s*(\\d+)\\s*\\]$"));
        if (rx.indexIn(pt) >= 0)
        {
            QString key = rx.cap(1);
            qint32 idx = rx.cap(2).toInt();

            QVariantList list = data.toMap().value(key).toList();
            if (idx < list.count())
                data = list.at(idx);
            else
            {
                data = QVariant();
                break;
            }
        }
        else
            data = data.toMap().value(pt);
    }

    return data;
}

QVariant AsemanAbstractQuickListModelHint::setPathValue(const QVariant &data, const QString &path, const QVariant &value)
{
    if (path.isEmpty())
        return value;

    QStringList pathList = path.split(QStringLiteral("->"));
    return setPathValue(data, pathList, value);
}

QVariant AsemanAbstractQuickListModelHint::setPathValue(const QVariant &data, QStringList pathList, const QVariant &value)
{
    if (pathList.isEmpty())
        return value;

    QString pt = pathList.takeFirst();

    QVariantMap map = data.toMap();

    QRegExp rx(QStringLiteral("(\\w+)\\s*\\[\\s*(\\d+)\\s*\\]$"));
    if (rx.indexIn(pt) >= 0)
    {
        QString key = rx.cap(1);
        qint32 idx = rx.cap(2).toInt();

        QVariantList list = map.value(key).toList();
        if (idx < list.count())
            list[idx] = setPathValue(list.at(idx), pathList, value);
        else
            list << setPathValue(QVariant(), pathList, value);

        map[key] = list;
    }
    else
        map[pt] = setPathValue(map.value(pt), pathList, value);

    return map;
}

QVariant AsemanAbstractQuickListModelHint::deletePath(const QVariant &data, const QString &path)
{
    if (path.isEmpty())
        return QVariantMap();

    QStringList pathList = path.split(QStringLiteral("->"));
    return deletePath(data, pathList);
}

QVariant AsemanAbstractQuickListModelHint::deletePath(const QVariant &data, QStringList pathList)
{
    if (pathList.isEmpty())
        return QVariant();

    QString pt = pathList.takeFirst();

    QVariantMap map = data.toMap();

    QRegExp rx(QStringLiteral("(\\w+)\\s*\\[\\s*(\\d+)\\s*\\]$"));
    if (rx.indexIn(pt) >= 0)
    {
        QString key = rx.cap(1);
        qint32 idx = rx.cap(2).toInt();

        QVariantList list = map.value(key).toList();
        if (idx < list.count())
            list[idx] = deletePath(list.at(idx), pathList);

        map[key] = list;
    }
    else
    {
        if (pathList.isEmpty())
            map.remove(pt);
        else
            map[pt] = deletePath(map.value(pt), pathList);
    }

    return map;
}

AsemanAbstractQuickListModelHint::~AsemanAbstractQuickListModelHint()
{

}
