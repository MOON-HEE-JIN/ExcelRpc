#include <afxwin.h>
#include <afxdlgs.h>
#include "CSharpRpcGenerator.h"
#include "MsvcRpcGenerator.h"
#include "RpcSchemaManager.h"
#include "resource.h"
#include <filesystem>
#include <exception>

class WorkbookDialog : public CDialog
{
public:
    WorkbookDialog() : CDialog(IDD_WORKBOOK) {}

protected:
    BOOL OnInitDialog() override
    {
        CDialog::OnInitDialog();
        UpdateButtons();
        return TRUE;
    }

    void OnOK() override {}

    afx_msg void OnSelectFile()
    {
        CFileDialog dialog(TRUE, L"xlsx", nullptr,
            OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR,
            L"Excel Workbook (*.xlsx)|*.xlsx||", this);
        if (dialog.DoModal() != IDOK)
            return;
        workbookPath_ = dialog.GetPathName();
        ready_ = false;
        SetDlgItemText(IDC_WORKBOOK_PATH, workbookPath_);
        SetDlgItemText(IDC_STATUS, L"파일 선택 완료. Read 버튼을 눌러 주세요.");
        UpdateButtons();
    }

    afx_msg void OnRead()
    {
        ready_ = false;
        UpdateButtons();
        if (workbookPath_.IsEmpty())
            return;
        CWaitCursor wait;
        try
        {
            const std::filesystem::path path(workbookPath_.GetString());
            // Resolve CallerAndCallee.ini and output directories beside the workbook.
            std::filesystem::current_path(path.parent_path());
            ready_ = g_rpcSchemaManager.LoadSchema(path.u8string());
            SetDlgItemText(IDC_STATUS, ready_
                ? L"준비 완료. MSVC 또는 CSharp 버튼을 눌러 주세요."
                : L"읽기 실패. 파일 및 필수 시트를 확인해 주세요.");
        }
        catch (const std::exception&)
        {
            SetDlgItemText(IDC_STATUS, L"읽기 실패. 파일 경로와 접근 권한을 확인해 주세요.");
        }
        UpdateButtons();
    }

    afx_msg void OnMsvc() { Generate(false); }
    afx_msg void OnCSharp() { Generate(true); }
    DECLARE_MESSAGE_MAP()

private:
    void UpdateButtons()
    {
        GetDlgItem(IDC_READ)->EnableWindow(!workbookPath_.IsEmpty());
        GetDlgItem(IDC_MSVC)->EnableWindow(ready_);
        GetDlgItem(IDC_CSHARP)->EnableWindow(ready_);
    }

    void Generate(bool csharp)
    {
        if (!ready_)
            return;
        CWaitCursor wait;
        try
        {
            const auto parent = std::filesystem::path(workbookPath_.GetString()).parent_path();
            std::filesystem::current_path(parent);
            const auto output = parent / (csharp ? L"CSHAP_FILE" : L"MSVC_FILE");
            std::filesystem::create_directories(output);
            const bool success = csharp
                ? CSharpRpcGenerator().GenerateAll() : MsvcRpcGenerator().GenerateAll();
            CString message;
            if (success)
                message.Format(L"생성 완료: %s", output.c_str());
            else
                message = L"생성 실패. 출력 폴더의 파일 및 쓰기 권한을 확인해 주세요.";
            SetDlgItemText(IDC_STATUS, message);
        }
        catch (const std::exception&)
        {
            SetDlgItemText(IDC_STATUS, L"생성 실패. 출력 폴더와 접근 권한을 확인해 주세요.");
        }
    }

    CString workbookPath_;
    bool ready_ = false;
};

BEGIN_MESSAGE_MAP(WorkbookDialog, CDialog)
    ON_BN_CLICKED(IDC_SELECT_FILE, &WorkbookDialog::OnSelectFile)
    ON_BN_CLICKED(IDC_READ, &WorkbookDialog::OnRead)
    ON_BN_CLICKED(IDC_MSVC, &WorkbookDialog::OnMsvc)
    ON_BN_CLICKED(IDC_CSHARP, &WorkbookDialog::OnCSharp)
END_MESSAGE_MAP()

class WorkbookApplication : public CWinApp
{
public:
    BOOL InitInstance() override
    {
        CWinApp::InitInstance();
        WorkbookDialog dialog;
        m_pMainWnd = &dialog;
        dialog.DoModal();
        m_pMainWnd = nullptr;
        return FALSE;
    }
};

WorkbookApplication theApp;
