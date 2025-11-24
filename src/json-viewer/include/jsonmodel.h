#ifndef JSONMODEL_H
#define JSONMODEL_H

#include <QAbstractItemModel>
#include <QVector>
#include <QString>

struct Node
{
  QString m_text;
  Node *m_parent = nullptr;
  QVector<Node*> m_children;

  ~Node()
  {
    qDeleteAll(m_children);
  }
};

class JsonModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  explicit JsonModel(QObject *parent = nullptr);
  ~JsonModel();

  bool loadJson(const QByteArray &json);
    
  bool hasElement(const QModelIndex &parent, const QString &text) const;

  QVariant data(const QModelIndex &index, int role) const override;
  Qt::ItemFlags flags(const QModelIndex &index) const override;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex &index) const override;
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QModelIndex rootIndex() const;
  void clear();

private:
  Node *m_root;
  Node* getNode(const QModelIndex &index) const;
  void addItem(const QString &text, const QModelIndex &parent);

  int parseValue(const QString &json, int pos, const QModelIndex &parent, const QString &key = QString());
  int parseObject(const QString &json, int pos, const QModelIndex &parent, const QString &key = QString());
  int parseArray(const QString &json, int pos, const QModelIndex &parent, const QString &key = QString());
  int skipWhitespace(const QString &json, int pos);
  QString parseString(const QString &json, int &pos);
  QString parseNumber(const QString &json, int &pos);
  QString parseBoolNull(const QString &json, int &pos);
};

#endif // JSONMODEL_H
