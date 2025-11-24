#ifndef JSONHIGHLIGHTER_H
#define JSONHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStringList>
#include <QList>

class JsonHighlighter : public QSyntaxHighlighter
{
  Q_OBJECT

public:
  explicit JsonHighlighter(QTextDocument *parent = nullptr);

private:
  void highlightBlock(const QString &text) override;

  void highlightStrings(const QString &text);
  void highlightStringsKeys(const QString &text);
  void highlightNumbers(const QString &text);
  void highlightBooleans(const QString &text);

  QTextCharFormat m_keyFormat;
  QTextCharFormat m_stringFormat;
  QTextCharFormat m_numberFormat;
  QTextCharFormat m_booleanFormat;
};

#endif // JSONHIGHLIGHTER_H
