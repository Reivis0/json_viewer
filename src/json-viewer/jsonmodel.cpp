#include "jsonmodel.h"

JsonModel::JsonModel(QObject *parent) : QAbstractItemModel(parent)
{
  m_root = nullptr;
}

JsonModel::~JsonModel()
{
  delete m_root;
}

void JsonModel::addItem(const QString &text, const QModelIndex &parent)
{
  auto node = new Node{text};
  if (parent.isValid())
  {
    auto parentNode = getNode(parent);
    parentNode->m_children.append(node);
    node->m_parent = parentNode;
  }
  else
  {
    m_root = node;
    m_root->m_parent = nullptr;
  }

  beginInsertRows(parent, rowCount(parent), rowCount(parent));
  endInsertRows();

  if (parent.isValid())
  {
    emit dataChanged(parent, parent);
  }
  else
  {
    emit dataChanged(index(0, 0), index(rowCount(), 0));
  }
}

bool JsonModel::loadJson(const QByteArray &jsonBytes)
{
  clear();
  QString json = QString::fromUtf8(jsonBytes);
  int pos = 0;
  pos = skipWhitespace(json, pos);
    
  if (pos >= json.length())
  {
    return false;
  }

  beginResetModel();
  parseValue(json, pos, QModelIndex());
  endResetModel();
    
  return true;
}


bool JsonModel::hasElement(const QModelIndex &parent, const QString &text) const
{
  int rows = rowCount(parent);
  for (int i = 0; i < rows; ++i)
  {
    QModelIndex idx = index(i, 0, parent);
    QString dataText = data(idx, Qt::DisplayRole).toString();
    if (dataText == text) return true;
  }
  return false;
}


int JsonModel::skipWhitespace(const QString &json, int pos)
{
  while (pos < json.length() && (json[pos].isSpace() || json[pos] == '\n' || json[pos] == '\r' || json[pos] == '\t'))
  {
    pos++;
  }
  return pos;
}

QString JsonModel::parseString(const QString &json, int &pos)
{
  QString res;
  if (json[pos] != '"')
  {
    return res;
  }
  pos++;
    
  while (pos < json.length())
  {
    QChar c = json[pos];
    if (c == '"')
    {
      pos++;
      return res;
    }
    if (c == '\\')
    {
      pos++;
      if (pos >= json.length())
      {
        break;
      }
      res.append(json[pos]); 
    }
    else
    {
      res.append(c);
    }
    pos++;
  }
  return res;
}

QString JsonModel::parseNumber(const QString &json, int &pos)
{
  int start = pos;
  while (pos < json.length() && (json[pos].isDigit() || json[pos] == '.' || json[pos] == '-' || json[pos] == 'e' || json[pos] == 'E' || json[pos] == '+'))
  {
    pos++;
  }
  return json.mid(start, pos - start);
}

QString JsonModel::parseBoolNull(const QString &json, int &pos)
{
  if (json.midRef(pos).startsWith("true"))
  { 
    pos += 4; 
    return "true";
  }
  if (json.midRef(pos).startsWith("false"))
  {
    pos += 5;
    return "false";
  }
  if (json.midRef(pos).startsWith("null"))
  {
    pos += 4;
    return "null";
  }
    return QString();
}

int JsonModel::parseValue(const QString &json, int pos, const QModelIndex &parent, const QString &key)
{
  pos = skipWhitespace(json, pos);
  if (pos >= json.length())
  {
    return pos;
  }

  QChar c = json[pos];

  if (c == '{')
  {
    return parseObject(json, pos, parent, key);
  }
  else if (c == '[')
  {
    return parseArray(json, pos, parent, key);
  }
  else
  {
    QString valStr;
    if (c == '"') 
    {
      valStr = "\"" + parseString(json, pos) + "\"";
    }
    else if (c.isDigit() || c == '-')
    {
        valStr = parseNumber(json, pos);
    }
    else
    {
      valStr = parseBoolNull(json, pos);
    }

    QString displayText;
    if (!key.isEmpty())
    {
      displayText = key + " : " + valStr;
    }
    else 
    {
      displayText = valStr;
    }

    addItem(displayText, parent);
  }
  return pos;
}

int JsonModel::parseObject(const QString &json, int pos, const QModelIndex &parent, const QString &key)
{
  QString header = key.isEmpty() ? "object" : key;
  addItem(header, parent); 
    
  QModelIndex objIndex = index(rowCount(parent) - 1, 0, parent);
  Node* objNode = getNode(objIndex);

  pos++;
  int count = 0;

  while (pos < json.length())
  {
    pos = skipWhitespace(json, pos);
    if (json[pos] == '}')
    {
      pos++;
      break;
    }
        
    if (count > 0)
    {
      if (json[pos] == ',')
      {
        pos++;
      }
      pos = skipWhitespace(json, pos);
    }

    QString itemKey = parseString(json, pos);
    pos = skipWhitespace(json, pos);
    if (json[pos] == ':')
    {
      pos++;
    }
        
    pos = parseValue(json, pos, objIndex, itemKey);
    count++;
  }

  QString finalHeader;
  if (key.isEmpty())
  {
    finalHeader = "object {" + QString::number(count) + "}";
  }
  else
  {
    finalHeader = key + " {" + QString::number(count) + "}";
  }
    
  objNode->m_text = finalHeader;
   
  return pos;
}

int JsonModel::parseArray(const QString &json, int pos, const QModelIndex &parent, const QString &key)
{
  QString header;
  if (key.isEmpty())
  {
    header = "array";
  }
  else
  {
    header = key;
  }
    
  addItem(header, parent);
  QModelIndex arrIndex = index(rowCount(parent) - 1, 0, parent);
  Node* arrNode = getNode(arrIndex);

  pos++;
  int count = 0;

  while (pos < json.length())
  {
    pos = skipWhitespace(json, pos);
    if (json[pos] == ']')
    {
      pos++;
      break;
    }

    if (count > 0)
    {
      if (json[pos] == ',')
      {
        pos++;
      }
      pos = skipWhitespace(json, pos);
    }

    QString indexKey = QString::number(count);
    pos = parseValue(json, pos, arrIndex, indexKey);
    count++;
  }

  QString finalHeader;
  if (key.isEmpty())
  {
    finalHeader = "array [" + QString::number(count) + "]";
  }
  else
  {
    finalHeader = key + " [" + QString::number(count) + "]";
  }

  arrNode->m_text = finalHeader;
    
  return pos;
}

QModelIndex JsonModel::index(int row, int column, const QModelIndex &parent) const
{
  if (column != 0)
  {
    return QModelIndex();
  }

  if (!parent.isValid())
  {
    if (row == 0 && m_root != nullptr) return createIndex(row, 0, m_root);
    {
        return QModelIndex();
    }
  }
  else
  {
    auto parentNode = getNode(parent);
    if (row >= 0 && row < parentNode->m_children.size())
    {
        return createIndex(row, 0, parentNode->m_children.at(row));
    }
  }
  return QModelIndex();
}

QModelIndex JsonModel::parent(const QModelIndex &child) const
{
  if (!child.isValid())
  {
    return QModelIndex();
  }
  auto node = getNode(child);
  if (!node || !node->m_parent)
  {
    return QModelIndex();
  }
    
  Node* parentNode = node->m_parent;
  if (!parentNode->m_parent)
  {
    return createIndex(0, 0, parentNode);
  }

  int row = parentNode->m_parent->m_children.indexOf(parentNode);
  return createIndex(row, 0, parentNode);
}

int JsonModel::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid())
  {
    return m_root ? 1 : 0;
  }
  auto parentNode = getNode(parent);
  return parentNode ? parentNode->m_children.size() : 0;
}

int JsonModel::columnCount(const QModelIndex &) const
{
  return 1;
}

QVariant JsonModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
  {
    return QVariant();
  }
  auto node = getNode(index);
  if (role == Qt::DisplayRole)
  {
    return node->m_text;
  }
  return QVariant();
}

Qt::ItemFlags JsonModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
  {
    return Qt::ItemIsEnabled;
  }
  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex JsonModel::rootIndex() const
{
  return index(0, 0);
}

Node* JsonModel::getNode(const QModelIndex &index) const
{
  if (index.isValid())
  {
    return static_cast<Node*>(index.internalPointer());
  }
  return m_root;
}

void JsonModel::clear()
{
  beginResetModel();
  delete m_root;
  m_root = nullptr;
  endResetModel();
}
