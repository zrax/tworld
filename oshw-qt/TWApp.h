/* Copyright (C) 2001-2010 by Madhav Shanbhag,
 * under the GNU General Public License. No warranty. See COPYING for details.
 */

#ifndef TWAPP_H
#define TWAPP_H

class TileWorldMainWnd;

#include <QApplication>

class TileWorldApp : public QApplication
{
public:
	static const QString s_sTitle;

	TileWorldApp(int& argc, char** argv);
	~TileWorldApp();
	
	int RunTWorld();
	void ExitTWorld();
	
	bool Initialize(bool bSilence, int nSoundBufSize, bool bShowHistogram, bool bFullScreen);
	
	
private:
	bool m_bSilence, m_bShowHistogram, m_bFullScreen;
	int& m_argc;
	char** m_argv;
};


extern TileWorldApp* g_pApp;
extern TileWorldMainWnd* g_pMainWnd;


#endif
