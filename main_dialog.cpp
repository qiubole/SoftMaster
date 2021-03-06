#include "stdafx.h"
#include "main_dialog.h"
#include "core\soft_strip_factory.h"
#include "core\abstract_soft_strip_view.h"
#include "main_tab_button.h"
#include "soft_house_tree_view_adapter.h"
#include "soft_list_adapter.h"
#include "soft_upgrade_adapter.h"
#include "soft_upgrade_history_tree_view_adapter.h"
#include "soft_unstall_adapter.h"

	
MainDialog::MainDialog() : SHostWnd(_T("LAYOUT:XML_MAINWND")) 
, abstract_soft_strip_view_(NULL)
, wnd_softhouse_(NULL)
, wnd_download_(NULL)
, wnd_unstall_(NULL)
, wnd_upgrade_(NULL)
, wnd_upgrade_list_(NULL)
, wnd_upgrade_history_(NULL)
{
	layout_inited_ = FALSE;
}

MainDialog::~MainDialog()
{
	if (abstract_soft_strip_view_) {
		delete abstract_soft_strip_view_;
	}
}

int MainDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	#ifdef DWMBLUR	//win7毛玻璃开关
	MARGINS mar = {5,5,30,5};
	DwmExtendFrameIntoClientArea ( m_hWnd, &mar );
	#endif

	SetMsgHandled(FALSE);
	return 0;
}

BOOL MainDialog::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	layout_inited_ = TRUE;
	Init();
	return 0;
}

void MainDialog::Init()
{
	abstract_soft_strip_view_ = CreateSoftStrip(this);
	if (abstract_soft_strip_view_) {
		abstract_soft_strip_view_->LoadStripView();
	}
	wnd_softhouse_ =static_cast<MainTabButton*>(FindChildByName(L"wnd_softhouse"));
	wnd_download_ = static_cast<MainTabButton*>(FindChildByName(L"wnd_download"));
	wnd_unstall_ = static_cast<MainTabButton*>(FindChildByName(L"wnd_unstall"));
	wnd_upgrade_ = static_cast<MainTabButton*>(FindChildByName(L"wnd_upgrade"));

	//更新位置界面
	wnd_upgrade_list_ = static_cast<SWindow*>(FindChildByName(L"wnd_upgrade_list"));
	if (wnd_upgrade_list_) {
		wnd_upgrade_list_->SetVisible(TRUE);
	}
	btn_upgrade_list = static_cast<SButton*>(FindChildByName(L"btn_select_update"));
	if (btn_upgrade_list) {
		btn_upgrade_list->SetAttribute(L"skin", L"_skin.btn.color.selected");
		btn_upgrade_list->SetAttribute(L"colorText", L"#FFFFFF");
	}
	wnd_upgrade_history_ = static_cast<SWindow*>(FindChildByName(L"wnd_upgrade_history"));
	if (wnd_upgrade_history_) {
		wnd_upgrade_history_->SetVisible(FALSE);
	}
	btn_upgrade_history = static_cast<SButton*>(FindChildByName(L"btn_select_updatehistory"));
	if (btn_upgrade_history) {
		btn_upgrade_history->SetAttribute(L"skin", L"_skin.btn.color.normal");
	}
	wnd_upgrade_cap1 = static_cast<SWindow*>(FindChildByName(L"upgrade_cap1"));
	wnd_upgrade_cap2 = static_cast<SWindow*>(FindChildByName(L"upgrade_cap2"));
	if (wnd_upgrade_cap1 && wnd_upgrade_cap2) {
		wnd_upgrade_cap1->SetVisible(TRUE, 1);
		wnd_upgrade_cap2->SetVisible(FALSE);
	}
	//////////////////////////////////////////////////////////////////////////
	tree_view_soft_list_ = static_cast<STreeView*>(FindChildByName(L"tree_view_soft_list"));
	if (tree_view_soft_list_)
	{
		SoftHouseTreeViewAdapter * pTreeViewAdapter = new SoftHouseTreeViewAdapter;
		tree_view_soft_list_->SetAdapter(pTreeViewAdapter);
		pTreeViewAdapter->Release();
	}

	//多列listview
	SMCListView * pMcListView = FindChildByName2<SMCListView>("mclv_test");
	if (pMcListView)
	{
		IMcAdapter *pAdapter = new SoftListAdapter;
		pMcListView->SetAdapter(pAdapter);
		pAdapter->Release();
	}

	//行高固定的列表
	SListView *pLstViewFix = FindChildByName2<SListView>("lv_test_fix");
	if (pLstViewFix)
	{
		ILvAdapter *pAdapter = new SoftUpgradeAdapter;
		pLstViewFix->SetAdapter(pAdapter);
		pAdapter->Release();
	}

	tree_view_upgrade_history_ = static_cast<STreeView*>(FindChildByName(L"tree_view_upgrade_history"));
	if (tree_view_upgrade_history_)
	{
		SoftUpgradeHistoryTreeViewAdapter * pTreeViewAdapter = new SoftUpgradeHistoryTreeViewAdapter;
		tree_view_upgrade_history_->SetAdapter(pTreeViewAdapter);
		pTreeViewAdapter->Release();
	}

	SMCListView * pUnstallMcListView = FindChildByName2<SMCListView>("mclv_unstall");
	if (pUnstallMcListView)
	{
		IMcAdapter *pAdapter = new SoftUnstallAdapter;
		pUnstallMcListView->SetAdapter(pAdapter);
		pAdapter->Release();
	}

}
void MainDialog::UpdatePageStatus() {
}
AbstractSoftStripView * MainDialog::softstip() {
	return static_cast<AbstractSoftStripView*>(abstract_soft_strip_view_);
}

HWND MainDialog::GetNativeHwnd() const
{
	return m_hWnd;
}

//TODO:消息映射
void MainDialog::OnClose()
{
	CSimpleWnd::DestroyWindow();
}

void MainDialog::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void MainDialog::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void MainDialog::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void MainDialog::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	if (!layout_inited_) return;
	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}
void MainDialog::OnBtnSelectSoftHouse() {
	wnd_softhouse_->SetVisible(TRUE, TRUE);
	wnd_download_->SetVisible(FALSE, FALSE);
	wnd_unstall_->SetVisible(FALSE, FALSE);
	wnd_upgrade_->SetVisible(FALSE, FALSE);
}
void MainDialog::OnBtnSelectDownload() {
	wnd_softhouse_->SetVisible(FALSE, FALSE);
	wnd_download_->SetVisible(TRUE, TRUE);
	wnd_unstall_->SetVisible(FALSE, FALSE);
	wnd_upgrade_->SetVisible(FALSE, FALSE);
}
void MainDialog::OnBtnSelectUnstall() {
	wnd_softhouse_->SetVisible(FALSE, FALSE);
	wnd_download_->SetVisible(FALSE, FALSE);
	wnd_unstall_->SetVisible(TRUE, TRUE);
	wnd_upgrade_->SetVisible(FALSE, FALSE);
}
void MainDialog::OnBtnSelectUpgrade() {
	wnd_softhouse_->SetVisible(FALSE, FALSE);
	wnd_download_->SetVisible(FALSE, FALSE);
	wnd_unstall_->SetVisible(FALSE, FALSE);
	wnd_upgrade_->SetVisible(TRUE, TRUE);
}
void MainDialog::OnBtnSelectPageUpgrade() {
	if (wnd_upgrade_list_) {
		wnd_upgrade_list_->SetVisible(TRUE,TRUE);
	}
	if (wnd_upgrade_history_) {
		wnd_upgrade_history_->SetVisible(FALSE);
	}
	//
	if (btn_upgrade_list) {
		btn_upgrade_list->SetAttribute(L"skin", L"_skin.btn.color.selected");
		btn_upgrade_list->SetAttribute(L"colorText", L"#FFFFFF");
	}
	if (btn_upgrade_history) {
		btn_upgrade_history->SetAttribute(L"skin", L"_skin.btn.color.normal");
		btn_upgrade_history->SetAttribute(L"colorText", L"#778899");
	}

	if (wnd_upgrade_cap1 && wnd_upgrade_cap2) {
		wnd_upgrade_cap1->SetVisible(TRUE, 1);
		wnd_upgrade_cap2->SetVisible(FALSE);
	}
}
void MainDialog::OnBtnSelectPageUpgradeHistory() {

	if (btn_upgrade_history) {
		btn_upgrade_history->SetAttribute(L"skin", L"_skin.btn.color.selected");
		btn_upgrade_history->SetAttribute(L"colorText", L"#FFFFFF");
	}
	if (btn_upgrade_list) {
		btn_upgrade_list->SetAttribute(L"skin", L"_skin.btn.color.normal");
		btn_upgrade_list->SetAttribute(L"colorText", L"#778899");
	}

	if (wnd_upgrade_list_) {
		wnd_upgrade_list_->SetVisible(false,TRUE);
	}
	if (wnd_upgrade_history_) {
		wnd_upgrade_history_->SetVisible(true);
	}

	if (wnd_upgrade_cap1 && wnd_upgrade_cap2) {
		wnd_upgrade_cap2->SetVisible(TRUE, 1);
		wnd_upgrade_cap1->SetVisible(FALSE);
	}
}