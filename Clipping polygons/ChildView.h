
// ChildView.h : interface of the CChildView class
//


#pragma once


// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildView();

	// Generated message map functions
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:
	enum class Mode 
	{
		UNDEFINED,
		MOVING,
		ROTATING,
		RESIZING,
	};

	Mode _mode = Mode::UNDEFINED;
	bool _firstDown = false;
	bool _wasRotated = false;
	bool _horzResizing = false;
	CRect _resizeWindow;
	HCURSOR _cursor;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
