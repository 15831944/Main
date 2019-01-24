//11/22/2018  4:37:13 PM
#ifndef _CHECKCOMBOBOX_GRIDCONTROL_H_
#define _CHECKCOMBOBOX_GRIDCONTROL_H_
#pragma  once

_HM_GridControl_BEGIN

class HM_GridControl_EXT HMCheckComboBox : public CComboBox
{
public:
	HMCheckComboBox();
	virtual ~HMCheckComboBox();

	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	// Selects all/unselects the specified item
	INT SetCheck(INT nIndex, BOOL bFlag);

	// Returns checked state
	BOOL GetCheck(INT nIndex);

	void SetMe();

	// Selects all/unselects all
	void SelectAll(BOOL bCheck = TRUE);

protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(HMCheckComboBox)
	protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(HMCheckComboBox)
	afx_msg LRESULT OnCtlColorListBox(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetTextLength(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDropDown();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	// Routine to update the text
	void RecalcText();

	// The subclassed COMBOLBOX window (notice the 'L')
	HWND m_hListBox;

	// The string containing the text to display
	CString m_strText;
	BOOL m_bTextUpdated;
	// A flag used in MeasureItem, see comments there
	BOOL m_bItemHeightSet;
	//��ѡ����Ŀ
	vector<size_t>m_vecSelect;//
	CString GetSeperatorStr()const;
	void InitSelec(const CString& str);
	vector<CString>GetOptionTitles() const;
};

_HM_GridControl_END
#endif
