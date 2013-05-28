
class MyToolApp: public wxApp
{
public:
	virtual bool OnInit();
private:
	wxFrame* m_pMainFrame;
};

class MyToolFrame: public wxFrame
{
public:
	MyToolFrame();
	~MyToolFrame();

	//file opt
	void OnClearTrashFile(wxCommandEvent& event);

	//reg opt
	void OnShowHideFile(wxCommandEvent& event);
	void OnShellContextMenu(wxCommandEvent& event);

	//misc opt
	void OnShowSysInfo(wxCommandEvent& event);
	void OnCleanOutput(wxCommandEvent& event);

	//default opt
	void OnDefaultProc(wxCommandEvent& event);

private:
	void MyTmpOutput(wxString output, wxInt32 level);
	void MyDeleteAllFile(wxString tmpath);

	wxMenuBar*	m_pMenuBar;
	wxTextCtrl*	m_pTextCtrl;
};

enum
{
	//file opt
	wxID_CLEANTRASHFILE,

	//reg opt
	wxID_SHOWHIDEFILE,
	wxID_SHELLCONTEXTMENU,

	//misc opt
	wxID_SHOWSYSINFO,
	wxID_CLEANOUTPUT,
};

enum
{
	wxDEBUGLEVEL_INFO,
	wxDEBUGLEVEL_WARN,
	wxDEBUGLEVEL_ERROR,
	wxDEBUGLEVEL_PROGRAM,
};