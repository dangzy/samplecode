#include "wx/wx.h"
#include "wx/socket.h"
#include "wx/dir.h"
#include "wx/filename.h"
#include "wx/file.h"
#include "mytool.h"

const wxInt32 g_DebugInfoLevel = 0;

IMPLEMENT_APP(MyToolApp)

bool MyToolApp::OnInit()
{
	m_pMainFrame = new MyToolFrame();
	m_pMainFrame->Show(true);
	SetTopWindow(m_pMainFrame);
	return true;
}

MyToolFrame::MyToolFrame():wxFrame((wxFrame *)NULL, wxID_ANY, _("我的工具箱"), wxDefaultPosition, wxSize(300, 300))
{
	wxMenu *fileopt = new wxMenu();
	fileopt->Append(wxID_CLEANTRASHFILE, _("清理垃圾文件(&T)"), _("清理垃圾文件"));

	wxMenu *regopt = new wxMenu();
	regopt->Append(wxID_SHOWHIDEFILE, _("隐藏文件(&H)"), _("隐藏文件"));
	regopt->Append(wxID_SHELLCONTEXTMENU, _("右键命令行(&S)"), _("右键命令行"));

	wxMenu *miscopt = new wxMenu();
	miscopt->Append(wxID_SHOWSYSINFO, _("系统信息(&S)"), _("系统信息"));
	miscopt->Append(wxID_CLEANOUTPUT, _("清屏(&C)\tCtrl-E"), _("清屏"));

	m_pMenuBar = new wxMenuBar();
	m_pMenuBar->Append(fileopt, "文件(&F)");
	m_pMenuBar->Append(regopt, "注册表(&R)");
	m_pMenuBar->Append(miscopt, "其他(&M)");

	SetMenuBar(m_pMenuBar);

	Connect(wxID_CLEANTRASHFILE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyToolFrame::OnClearTrashFile));

	Connect(wxID_SHOWHIDEFILE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyToolFrame::OnShowHideFile));
	Connect(wxID_SHELLCONTEXTMENU, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyToolFrame::OnShellContextMenu));

	Connect(wxID_SHOWSYSINFO, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyToolFrame::OnShowSysInfo));
	Connect(wxID_CLEANOUTPUT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MyToolFrame::OnCleanOutput));

	m_pTextCtrl  = new wxTextCtrl(this, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
}

MyToolFrame::~MyToolFrame()
{
}

void MyToolFrame::OnClearTrashFile(wxCommandEvent& event)
{
	wxString tmpenv = "USERPROFILE";
	wxString tmpath;
	wxGetEnv(tmpenv, &tmpath);
	tmpath.Append("\\Recent");
	MyDeleteAllFile(tmpath);
	MyTmpOutput("Recent文件夹清理完成！\n", wxDEBUGLEVEL_PROGRAM);

	tmpenv = "TEMP";
	wxGetEnv(tmpenv, &tmpath);
	MyDeleteAllFile(tmpath);
	MyTmpOutput("TEMP文件夹清理完成！\n", wxDEBUGLEVEL_PROGRAM);

	tmpenv = "TMP";
	wxGetEnv(tmpenv, &tmpath);
	MyDeleteAllFile(tmpath);
	MyTmpOutput("TMP文件夹清理完成！\n", wxDEBUGLEVEL_PROGRAM);

	tmpenv = "APPDATA";
	wxGetEnv(tmpenv, &tmpath);
	tmpath.Append("\\Opera\\Opera\\global_history.dat");
	MyDeleteAllFile(tmpath);
	tmpenv = "APPDATA";
	wxGetEnv(tmpenv, &tmpath);
	tmpath.Append("\\Opera\\Opera\\typed_history.xml");
	MyDeleteAllFile(tmpath);
	MyTmpOutput("Opera历史清理完成！\n", wxDEBUGLEVEL_PROGRAM);
}

void MyToolFrame::OnShowHideFile(wxCommandEvent& event)
{
	OnDefaultProc(event);
}

void MyToolFrame::OnShellContextMenu(wxCommandEvent& event)
{
	OnDefaultProc(event);
}

void MyToolFrame::OnShowSysInfo(wxCommandEvent& event)
{
	wxString tmpstr;

	tmpstr.Append("主机名：");
	tmpstr.Append(::wxGetFullHostName());
	tmpstr.Append("\n");

	wxIPV4address addr;
	tmpstr.Append("IP：");
	addr.Hostname(::wxGetFullHostName());
	tmpstr.Append(addr.IPAddress());
	tmpstr.Append("\n");

	tmpstr.Append("用户：");
	tmpstr.Append(::wxGetUserId());
	tmpstr.Append("\n");

	tmpstr.Append("个人目录：");
	tmpstr.Append(::wxGetHomeDir());
	tmpstr.Append("\n");

	MyTmpOutput(tmpstr, wxDEBUGLEVEL_PROGRAM);
}

void MyToolFrame::OnCleanOutput(wxCommandEvent& event)
{
	m_pTextCtrl->Clear();
}

void MyToolFrame::OnDefaultProc(wxCommandEvent& event)
{
	MyTmpOutput("此功能还未实现！\n\n", wxDEBUGLEVEL_PROGRAM);
}

void MyToolFrame::MyTmpOutput(wxString output, wxInt32 level)
{
	if (level >= g_DebugInfoLevel)
	{
		m_pTextCtrl->AppendText(_(output));
	}
}

void MyToolFrame::MyDeleteAllFile(wxString tmpath)
{
	wxFileName file(tmpath);
	wxString filename;

	if (wxDir::Exists(tmpath))
	{
		wxDir dir(tmpath);

		wxString fullname;

		bool cont = dir.GetFirst(&filename);
		while ( cont )
		{
			fullname = tmpath;
			fullname.Append("\\").Append(filename);
			if (wxFile::Exists(fullname))
			{
				if (::wxRemoveFile(fullname))
					filename.Append("”删除成功！\n");
				else
					filename.Append("”删除失败！\n");
			}
			else
				filename.Append("”不存在！\n");
			filename.Prepend("“");
			MyTmpOutput(filename, wxDEBUGLEVEL_INFO);
			cont = dir.GetNext(&filename);
		}
	}
	else
	{
		filename = file.GetFullName();
		if (file.FileExists())
		{
			if (::wxRemoveFile(tmpath))
				filename.Append("”删除成功！\n");
			else
				filename.Append("”删除失败！\n");
		}
		else
			filename.Append("”不存在！\n");
		filename.Prepend("“");
		MyTmpOutput(filename, wxDEBUGLEVEL_INFO);
	}
}