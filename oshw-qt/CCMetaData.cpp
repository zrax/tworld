/* Copyright (C) 2001-2010 by Madhav Shanbhag,
 * under the GNU General Public License. No warranty. See COPYING for details.
 */

#include "CCMetaData.h"

#include <QFile>
#include <QDomDocument>
#include <QDomElement>


namespace CCX
{


template <typename T>
static bool ReadElmAttr(const QDomElement& elm, const QString& sAttr, T (*pf)(const QString&), T& rValue)
{
	if (!elm.hasAttribute(sAttr))
		return false;
	rValue = (*pf)(elm.attribute(sAttr));
	return true;
}


inline static QString ParseString(const QString& s)
{
	return s;
}

inline static int ParseInt(const QString& s)
{
	return s.toInt();
}

inline static QColor ParseColor(const QString& s)
{
	return QColor(s);
}

static Qt::AlignmentFlag ParseHAlign(const QString& s)
{
	return (s == QStringLiteral("right")) ? Qt::AlignRight
		: (s == QStringLiteral("center")) ? Qt::AlignHCenter
		: Qt::AlignLeft;
}

static Qt::AlignmentFlag ParseVAlign(const QString& s)
{
	return (s == QStringLiteral("bottom")) ? Qt::AlignBottom
		: (s == QStringLiteral("middle")) ? Qt::AlignVCenter
		: Qt::AlignTop;
}

static Compatibility ParseCompat(const QString& s)
{
	return (s == QStringLiteral("yes")) ? COMPAT_YES
		: (s == QStringLiteral("no")) ? COMPAT_NO
		: COMPAT_UNKNOWN;
}

static TextFormat ParseFormat(const QString& s)
{
	return (s == QStringLiteral("html")) ? TEXT_HTML : TEXT_PLAIN;
}


void RulesetCompatibility::ReadXML(const QDomElement& elm)
{
	ReadElmAttr(elm, QStringLiteral("ms"),       &ParseCompat, eMS);
	ReadElmAttr(elm, QStringLiteral("lynx"),     &ParseCompat, eLynx);
	ReadElmAttr(elm, QStringLiteral("pedantic"), &ParseCompat, ePedantic);
}


void PageProperties::ReadXML(const QDomElement& elm)
{
	ReadElmAttr(elm, QStringLiteral("format"),  &ParseFormat, eFormat);
	ReadElmAttr(elm, QStringLiteral("align"),   &ParseHAlign, align);
	ReadElmAttr(elm, QStringLiteral("valign"),  &ParseVAlign, valign);
	ReadElmAttr(elm, QStringLiteral("color"),   &ParseColor , color);
	ReadElmAttr(elm, QStringLiteral("bgcolor"), &ParseColor , bgcolor);
}


void Page::ReadXML(const QDomElement& elm, const Levelset& levelset)
{
	sText = elm.text();

	pageProps = levelset.pageProps;
	pageProps.ReadXML(elm);
}


void Text::ReadXML(const QDomElement& elm, const Levelset& levelset)
{
	vecPages.clear();
	QDomNodeList lstElmPages = elm.elementsByTagName(QStringLiteral("page"));
	for (int i = 0; i < int(lstElmPages.length()); ++i)
	{
		QDomElement elmPage = lstElmPages.item(i).toElement();
		Page page;
		page.ReadXML(elmPage, levelset);
		vecPages.push_back(page);
	}
}


void Level::ReadXML(const QDomElement& elm, const Levelset& levelset)
{
	sAuthor = levelset.sAuthor;
	ReadElmAttr(elm, QStringLiteral("author"), &ParseString, sAuthor);
	
	ruleCompat = levelset.ruleCompat;
	ruleCompat.ReadXML(elm);
	
	QDomNodeList lstElm;
	lstElm = elm.elementsByTagName(QStringLiteral("prologue"));
	if (lstElm.length() != 0)
		txtPrologue.ReadXML(lstElm.item(0).toElement(), levelset);
	lstElm = elm.elementsByTagName(QStringLiteral("epilogue"));
	if (lstElm.length() != 0)
		txtEpilogue.ReadXML(lstElm.item(0).toElement(), levelset);
}


void Levelset::ReadXML(const QDomElement& elm)
{
	ReadElmAttr(elm, QStringLiteral("description"), &ParseString, sDescription);
	ReadElmAttr(elm, QStringLiteral("copyright"),   &ParseString, sCopyright);
	ReadElmAttr(elm, QStringLiteral("author"),      &ParseString, sAuthor);

	ruleCompat.ReadXML(elm);
	pageProps.ReadXML(elm);
	
	for (int i = 0; i < int(vecLevels.size()); ++i)
	{
		Level& rLevel = vecLevels[i];
		rLevel.sAuthor = sAuthor;
		rLevel.ruleCompat = ruleCompat;
	}

	// vecLevels.clear();
	QDomNodeList lstElmLevels = elm.elementsByTagName(QStringLiteral("level"));
	for (int i = 0; i < int(lstElmLevels.length()); ++i)
	{
		QDomElement elmLevel = lstElmLevels.item(i).toElement();
		int nNumber = 0;
		if (!ReadElmAttr(elmLevel, QStringLiteral("number"), &ParseInt, nNumber))
			continue;
		if ( ! (nNumber >= 1  &&  nNumber < int(vecLevels.size())) )
			continue;
		Level& rLevel = vecLevels[nNumber];
		rLevel.ReadXML(elmLevel, *this);
	}
	
	QDomNodeList lstElmStyle = elm.elementsByTagName(QStringLiteral("style"));
	if (lstElmStyle.length() != 0)
	{
		QDomElement elmStyle = lstElmStyle.item(0).toElement();
		if (elmStyle.parentNode() == elm)
			sStyleSheet = elmStyle.text();
	}
}


bool Levelset::ReadFile(const QString& sFilePath, int nLevels)
{
	Clear();
	
	vecLevels.resize(1+nLevels);
	
	QFile file(sFilePath);
	if (!file.exists())
		return true;
	if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
		return false;
	
	QDomDocument doc;
	if (!doc.setContent(&file))
		return false;
		
	QDomElement elmRoot = doc.documentElement();
	if (elmRoot.tagName() != QStringLiteral("levelset"))
		return false;
		
	ReadXML(elmRoot);
	
	file.close();
	
	return true;
}


void Levelset::Clear()
{
	*this = Levelset();
}


}
