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

MyToolFrame::MyToolFrame():wxFrame((wxFrame *)NULL, wxID_ANY, _("�ҵĹ�����"), wxDefaultPosition, wxSize(300, 300))
{
	wxMenu *fileopt = new wxMenu();
	fileopt->Append(wxID_CLEANTRASHFILE, _("���������ļ�(&T)"), _("���������ļ�"));

	wxMenu *regopt = new wxMenu();
	regopt->Append(wxID_SHOWHIDEFILE, _("�����ļ�(&H)"), _("�����ļ�"));
	regopt->Append(wxID_SHELLCONTEXTMENU, _("�Ҽ�������(&S)"), _("�Ҽ�������"));

	wxMenu *miscopt = new wxMenu();
	miscopt->Append(wxID_SHOWSYSINFO, _("ϵͳ��Ϣ(&S)"), _("ϵͳ��Ϣ"));
	miscopt->Append(wxID_CLEANOUTPUT, _("����(&C)\tCtrl-E"), _("����"));

	m_pMenuBar = new wxMenuBar();
	m_pMenuBar->Append(fileopt, "�ļ�(&F)");
	m_pMenuBar->Append(regopt, "ע���(&R)");
	m_pMenuBar->Append(miscopt, "����(&M)");

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
	MyTmpOutput("Recent�ļ���������ɣ�\n", wxDEBUGLEVEL_PROGRAM);

	tmpenv = "TEMP";
	wxGetEnv(tmpenv, &tmpath);
	MyDeleteAllFile(tmpath);
	MyTmpOutput("TEMP�ļ���������ɣ�\n", wxDEBUGLEVEL_PROGRAM);

	tmpenv = "TMP";
	wxGetEnv(tmpenv, &tmpath);
	MyDeleteAllFile(tmpath);
	MyTmpOutput("TMP�ļ���������ɣ�\n", wxDEBUGLEVEL_PROGRAM);

	tmpenv = "APPDATA";
	wxGetEnv(tmpenv, &tmpath);
	tmpath.Append("\\Opera\\Opera\\global_history.dat");
	MyDeleteAllFile(tmpath);
	tmpenv = "APPDATA";
	wxGetEnv(tmpenv, &tmpath);
	tmpath.Append("\\Opera\\Opera\\typed_history.xml");
	MyDeleteAllFile(tmpath);
	MyTmpOutput("Opera��ʷ������ɣ�\n", wxDEBUGLEVEL_PROGRAM);
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

	tmpstr.Append("��������");
	tmpstr.Append(::wxGetFullHostName());
	tmpstr.Append("\n");

	wxIPV4address addr;
	tmpstr.Append("IP��");
	addr.Hostname(::wxGetFullHostName());
	tmpstr.Append(addr.IPAddress());
	tmpstr.Append("\n");

	tmpstr.Append("�û���");
	tmpstr.Append(::wxGetUserId());
	tmpstr.Append("\n");

	tmpstr.Append("����Ŀ¼��");
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
	MyTmpOutput("�˹��ܻ�δʵ�֣�\n\n", wxDEBUGLEVEL_PROGRAM);
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
					filename.Append("��ɾ���ɹ���\n");
				else
					filename.Append("��ɾ��ʧ�ܣ�\n");
			}
			else
				filename.Append("�������ڣ�\n");
			filename.Prepend("��");
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
				filename.Append("��ɾ���ɹ���\n");
			else
				filename.Append("��ɾ��ʧ�ܣ�\n");
		}
		else
			filename.Append("�������ڣ�\n");
		filename.Prepend("��");
		MyTmpOutput(filename, wxDEBUGLEVEL_INFO);
	}
}