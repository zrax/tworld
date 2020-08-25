/* Copyright (C) 2001-2010 by Madhav Shanbhag,
 * under the GNU General Public License. No warranty. See COPYING for details.
 */

#ifndef TWPROGRESSBAR_H
#define TWPROGRESSBAR_H


#include <QtWidgets/QProgressBar>


// QProgressBar's setValue is slow enough to cause glitchy movement
//  during gameplay; so replace it with a simple implementation...

class TWProgressBar : public QProgressBar
{
public:
	TWProgressBar(QWidget* pParent = nullptr);
	
	// These aren't virtual, but we can still get by...
	void setValue(int nValue);
	int value() const
		{return m_nValue;}
		
	void setPar(int nPar);
	int par() const
		{return m_nPar;}

	void setParBad(bool bParBad);
	int isParBad() const
		{return m_bParBad;}

	void setFullBar(bool bFullBar);
	int isFullBar() const
		{return m_bFullBar;}

	QString text() const override;

protected:
	void paintEvent(QPaintEvent* pPaintEvent) override;

	int m_nValue, m_nPar;
	bool m_bParBad;
	bool m_bFullBar;
};


#endif
