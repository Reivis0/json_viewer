#include "jsonhighlighter.h"

JsonHighlighter::JsonHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
  m_keyFormat.setForeground(Qt::darkBlue);
  m_keyFormat.setFontWeight(QFont::Bold);
  m_stringFormat.setForeground(Qt::darkGreen);
  m_numberFormat.setForeground(Qt::darkRed);
  m_booleanFormat.setForeground(Qt::darkYellow);
}

void JsonHighlighter::highlightBlock(const QString &text)
{
  highlightNumbers(text);
  highlightBooleans(text);
  highlightStrings(text);
  highlightStringsKeys(text);
}

void JsonHighlighter::highlightStrings(const QString &text)
{
  QRegularExpression reg(QStringLiteral("\"(.*?)\""));
  QRegularExpressionMatchIterator matchIterator = reg.globalMatch(text);
    while (matchIterator.hasNext())
    {
      QRegularExpressionMatch match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), m_stringFormat);
    }
}

void JsonHighlighter::highlightStringsKeys(const QString &text)
{
  QRegularExpression reg(QStringLiteral("\"[^\"]+\"\\s*:"));
  QRegularExpressionMatchIterator matchIterator = reg.globalMatch(text);
  while (matchIterator.hasNext())
  {
    QRegularExpressionMatch match = matchIterator.next();
    setFormat(match.capturedStart(), match.capturedLength(), m_keyFormat);
  }
}

void JsonHighlighter::highlightNumbers(const QString &text)
{
  QRegularExpression reg(QStringLiteral("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?"));
  QRegularExpressionMatchIterator matchIterator = reg.globalMatch(text);
  while (matchIterator.hasNext())
  {
    QRegularExpressionMatch match = matchIterator.next();
    setFormat(match.capturedStart(), match.capturedLength(), m_numberFormat);
  }
}

void JsonHighlighter::highlightBooleans(const QString &text)
{
  QRegularExpression reg(QStringLiteral("(true|false)"));
  QRegularExpressionMatchIterator matchIterator = reg.globalMatch(text);
  while (matchIterator.hasNext())
  {
    QRegularExpressionMatch match = matchIterator.next();
    setFormat(match.capturedStart(), match.capturedLength(), m_booleanFormat);
  }
}
